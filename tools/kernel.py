import sys
import os
from files import process

def proc(d, key, val):
	k = key.strip()
	v = val.strip()

	#print('proc: key "' + k + '" val "' + v + '"')

	if v.startswith('{'):
		f = {}
		proc0(f, v.strip("{}"))
		if k in d:
			# Merge dicts
			for e in f:
				if e in d[k]:
					if type(d[k][e]) != list:
						d[k][e] = [d[k][e]]
					d[k][e].append(f[e])
				else:
					d[k][e] = f[e]
		else:
			d[k] = f
	else:
		if k in d:
			if type(d[k]) != list:
				d[k] = [d[k]]
			d[k].append(v)
		else:
			d[k] = v

def proc0(d, data):
	i = 0
	tmp = ''
	key = ''
	val = ''
	depth = 0

	#print("proc0 %s\n" % data)

	while i < len(data):
		c = data[i]

		if ((c == ';' or i == len(data) - 1) and depth == 0):
			if val == '':
				val = tmp
				tmp = ''
			if (key and val):
				proc(d, key, val)
				key = ''
				val = ''
			i += 1
			continue

		elif (c == '='):
			i += 1
			continue

		elif (c == ' '):
			if (tmp.strip() == '' or depth > 0):
				tmp += c
				i += 1
				continue

			if (key == ''):
				key = tmp
				tmp = ''
			else:
				val = tmp
				tmp = ''
			i += 1
			continue

		elif (c == '{'):
			depth += 1
		elif (c == '}'):
			depth -= 1

		tmp += c
		i += 1

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 2:
		sys.exit(1)

	osdir = args[1]
	data = args[2]

	modules = {}
	proc0(modules, data)

	result = []
	for m in modules['module']:
		l = []
		if m in modules:
			d = modules[m]
			if type(d) != dict:
				sys.exit(4)

			# Process directives only
			if 'include' in d:
				inc = d['include']
				if type(inc) == list:
					for file in inc:
						l.append(file)
				else:
					l.append(inc)
		result += process(osdir, "kernel/%s" % m, l)

	print(" ".join(result))
