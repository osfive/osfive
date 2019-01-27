LIBFLAC =	osfive/lib/libFLAC/libFLAC/bitmath.o		\
		osfive/lib/libFLAC/libFLAC/bitreader.o		\
		osfive/lib/libFLAC/libFLAC/crc.o		\
		osfive/lib/libFLAC/libFLAC/format.o		\
		osfive/lib/libFLAC/libFLAC/lpc.o		\
		osfive/lib/libFLAC/libFLAC/cpu.o		\
		osfive/lib/libFLAC/libFLAC/md5.o		\
		osfive/lib/libFLAC/libFLAC/float.o		\
		osfive/lib/libFLAC/libFLAC/fixed.o		\
		osfive/lib/libFLAC/libFLAC/memory.o		\
		osfive/lib/libFLAC/libFLAC/stream_decoder.o

LIBFLAC_FLAGS = -I${CURDIR}/osfive/lib/libFLAC/include		\
		-I${CURDIR}/osfive/lib/libFLAC/libFLAC/include	\
		-DHAVE_CONFIG_H

$(foreach obj,${LIBFLAC},	\
	${eval CFLAGS_${OBJDIR}/$(obj) = ${LIBFLAC_FLAGS}})

OBJECTS +=	${LIBFLAC}