INCLUDES = -DLOCALE_DIR=\"${datadir}/locale/\" \
	-I$(top_srcdir)/include

noinst_LIBRARIES = libpcsxcore.a

libpcsxcore_a_SOURCES = \
	$(top_builddir)/libpcsxcore/psxbios.cpp	\
	$(top_builddir)/libpcsxcore/cdrom.cpp	\
	$(top_builddir)/libpcsxcore/psxcounters.cpp	\
	$(top_builddir)/libpcsxcore/psxdma.cpp	\
	$(top_builddir)/libpcsxcore/disr3000a.cpp	\
	$(top_builddir)/libpcsxcore/spu.cpp	\
	$(top_builddir)/libpcsxcore/sio.cpp	\
	$(top_builddir)/libpcsxcore/psxhw.cpp	\
	$(top_builddir)/libpcsxcore/mdec.cpp	\
	$(top_builddir)/libpcsxcore/psxmem.cpp	\
	$(top_builddir)/libpcsxcore/misc.cpp	\
	$(top_builddir)/libpcsxcore/plugins.cpp	\
	$(top_builddir)/libpcsxcore/decode_xa.cpp	\
	$(top_builddir)/libpcsxcore/psxinterpreter.cpp	\
	$(top_builddir)/libpcsxcore/gte.cpp	\
	$(top_builddir)/libpcsxcore/psxhle.cpp	\
	$(top_builddir)/libpcsxcore/cdrom.h \
	$(top_builddir)/libpcsxcore/coff.h \
	$(top_builddir)/libpcsxcore/debug.cpp \
	$(top_builddir)/libpcsxcore/debug.h \
	$(top_builddir)/libpcsxcore/decode_xa.h \
	$(top_builddir)/libpcsxcore/gte.h \
	$(top_builddir)/libpcsxcore/mdec.h \
	$(top_builddir)/libpcsxcore/misc.h \
	$(top_builddir)/libpcsxcore/plugins.h \
	$(top_builddir)/libpcsxcore/psemu_plugin_defs.h \
	$(top_builddir)/libpcsxcore/psxbios.h \
	$(top_builddir)/libpcsxcore/psxcommon.h \
	$(top_builddir)/libpcsxcore/psxcounters.h \
	$(top_builddir)/libpcsxcore/psxdma.h \
	$(top_builddir)/libpcsxcore/psxhle.h \
	$(top_builddir)/libpcsxcore/psxhw.h \
	$(top_builddir)/libpcsxcore/psxmem.h \
	$(top_builddir)/libpcsxcore/r3000a.cpp	\
	$(top_builddir)/libpcsxcore/r3000a.h \
	$(top_builddir)/libpcsxcore/psxevents.cpp	\
	$(top_builddir)/libpcsxcore/psxevents.h \
	$(top_builddir)/libpcsxcore/sio.h \
	$(top_builddir)/libpcsxcore/spu.h \
	$(top_builddir)/libpcsxcore/system.h \
	$(top_builddir)/libpcsxcore/cdriso.c \
	$(top_builddir)/libpcsxcore/cdriso.h \
	$(top_builddir)/libpcsxcore/cheat.cpp \
	$(top_builddir)/libpcsxcore/cheat.h \
	$(top_builddir)/libpcsxcore/socket.cpp \
	$(top_builddir)/libpcsxcore/socket.h

if ARCH_X86_64
libpcsxcore_a_SOURCES += \
	$(top_builddir)/libpcsxcore/ix86_64/iR3000A-64.cpp	\
	$(top_builddir)/libpcsxcore/ix86_64/ix86-64.cpp	\
	$(top_builddir)/libpcsxcore/ix86_64/ix86_cpudetect.cpp	\
	$(top_builddir)/libpcsxcore/ix86_64/ix86_fpu.cpp	\
	$(top_builddir)/libpcsxcore/ix86_64/ix86_3dnow.cpp	\
	$(top_builddir)/libpcsxcore/ix86_64/ix86_mmx.cpp	\
	$(top_builddir)/libpcsxcore/ix86_64/ix86_sse.cpp	\
	$(top_builddir)/libpcsxcore/
else		
if ARCH_X86
libpcsxcore_a_SOURCES += \
	$(top_builddir)/libpcsxcore/ix86/iR3000A.cpp	\
	$(top_builddir)/libpcsxcore/ix86/ix86.cpp	\
	$(top_builddir)/libpcsxcore/
endif
endif

if ARCH_PPC
libpcsxcore_a_SOURCES += \
	$(top_builddir)/libpcsxcore/ppc/pR3000A.cpp	\
	$(top_builddir)/libpcsxcore/ppc/ppc.cpp	\
	$(top_builddir)/libpcsxcore/ppc/reguse.cpp	\
	$(top_builddir)/libpcsxcore/ppc/pasm.s	\
	$(top_builddir)/libpcsxcore/
libpcsxcore_a_CCASFLAGS = -x assembler-with-cpp -mregnames
endif
