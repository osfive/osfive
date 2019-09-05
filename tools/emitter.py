from flags import collect_all_user_flags
from flags import collect_flags, print_flags
from parser import proc0
import sys
import os

def obj_set_flags(resobj, context, root, key, l):
	if not 'objects' in context:
		return
	for obj in context['objects']:
		o = os.path.join(root, obj)
		if not o in resobj:
			resobj[o] = {}
		if not key in resobj[o]:
			resobj[o][key] = []
		for el in l:
			resobj[o][key].append(el)

def proc1(resobj, flags, root, context, context_str):
	obj_set_flags(resobj, context, root, 'incs', [])

	if not 'module' in context:
		return
	for m in context['module']:
		options = []
		if m in context:
			node = context[m]
			proc1(resobj, flags, context_str, node, m)
			if 'options' in node:
				options += node['options']

		p = os.path.join(root, context_str, m, "mdepx.conf")
		if not os.path.exists(p):
			continue
		f = open(p, "r")
		data = f.read()
		f.close()
		cfg = {}
		proc0(cfg, data)

		if not m in cfg:
			continue

		context1 = cfg[m]
		collect_flags(flags, m, context1, False)

		incs = []
		cflags = []

		if 'incs' in context1:
			incs += context1['incs']
		if 'cflags' in context1:
			cflags += context1['cflags']

		obj_set_flags(resobj, context1, root, 'incs', incs)
		obj_set_flags(resobj, context1, root, 'cflags', cflags)

		for opt in options:
			incs1 = []
			cflags1 = []

			if not opt in context1:
				continue
			node = context1[opt]
			collect_flags(flags, "%s_%s" % (m, opt), node, False)

			if 'incs' in node:
				incs1 += node['incs']
			if 'cflags' in node:
				cflags1 += node['cflags']

			obj_set_flags(resobj, node, root, 'incs', incs1 + incs)
			obj_set_flags(resobj, node, root, 'cflags', cflags1 + cflags)

	return resobj

def emit_objects_flags(resobj):
	for obj in resobj:
		print("OBJECTS+=%s" % obj)

	for obj in resobj:
		if 'incs' in resobj[obj]:
			for inc in resobj[obj]['incs']:
				print("CFLAGS_%s/%s+=-I${OSDIR}/%s" % \
					(objdir, obj, inc))

		if 'cflags' in resobj[obj]:
			for cflag in resobj[obj]['cflags']:
				print("CFLAGS_%s/%s+=%s" % \
					(objdir, obj, cflag))

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 3:
		sys.exit(1)

	config_file = args[1]
	objdir = args[2]

	if not os.path.exists(config_file):
		sys.exit(2)
	f = open(config_file, "r")
	config_str = f.read()
	f.close()

	config = {}
	proc0(config, config_str)

	flags = {}
	resobj = {}
	proc1(resobj, flags, '', config, '')
	emit_objects_flags(resobj)

	if 'mdepx' in config:
		if 'kernel' in config['mdepx']:
			kernel = config['mdepx']['kernel']
			collect_all_user_flags(flags, kernel)

	print_flags(flags)
