LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

# This is the target being built.
LOCAL_MODULE := libpsx

# All of the source files that we will compile.

LOCAL_SRC_FILES := \
  libpcsxcore/cdriso.c \
	libpcsxcore/cdrom.c \
	libpcsxcore/cheat.c \
	libpcsxcore/debug.c \
	libpcsxcore/decode_xa.c \
	libpcsxcore/disr3000a.c \
	libpcsxcore/gte.c \
	libpcsxcore/LnxMain.c \
	libpcsxcore/mdec.c \
	libpcsxcore/misc.c \
	libpcsxcore/plugins.c \
	libpcsxcore/ppf.c \
	libpcsxcore/psxbios.c \
	libpcsxcore/psxcommon.c \
	libpcsxcore/psxcounters.c \
	libpcsxcore/psxdma.c \
	libpcsxcore/psxhle.c \
	libpcsxcore/psxhw.c \
	libpcsxcore/psxinterpreter.c \
	libpcsxcore/psxmem.c \
	libpcsxcore/r3000a.c \
	libpcsxcore/sio.c \
	libpcsxcore/socket.c \
	libpcsxcore/spu.c \
  libpcsxcore/sound/spu.c \
	libpcsxcore/sound/cfg.c \
	libpcsxcore/sound/dma.c \
	libpcsxcore/sound/freeze.c \
	libpcsxcore/sound/registers.c \
	libpcsxcore/sound/android.c \
  libpcsxcore/gpuAPI/gpuAPI.cpp \
  libpcsxcore/gpuAPI/newGPU/newGPU.cpp 	      \
  libpcsxcore/gpuAPI/newGPU/fixed.cpp         \
  libpcsxcore/gpuAPI/newGPU/core_Command.cpp  \
  libpcsxcore/gpuAPI/newGPU/core_Dma.cpp      \
  libpcsxcore/gpuAPI/newGPU/core_Draw.cpp     \
  libpcsxcore/gpuAPI/newGPU/core_Misc.cpp     \
  libpcsxcore/gpuAPI/newGPU/raster_Sprite.cpp \
  libpcsxcore/gpuAPI/newGPU/raster_Poly.cpp   \
  libpcsxcore/gpuAPI/newGPU/raster_Line.cpp   \
  libpcsxcore/gpuAPI/newGPU/raster_Image.cpp  \
  libpcsxcore/gpuAPI/newGPU/inner.cpp         \
  libpcsxcore/gpuAPI/newGPU/ARM_asm.S         \
  libpcsxcore/new_dynarec/new_dynarec.c \
  libpcsxcore/new_dynarec/linkage_arm.S \
  libpcsxcore/new_dynarec/pcsxmem.c \
  libpcsxcore/new_dynarec/emu_if.c \
  libpcsxcore/android/minimal.c

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_SRC_FILES += libpcsxcore/gte_neon.S
endif

LOCAL_SRC_FILES += \
	libpcsxcore/android/emumedia.cpp \
	libpcsxcore/android/emulator.cpp

# Static libraries.
LOCAL_STATIC_LIBRARIES += cpufeatures

# All of the shared libraries we link against.
LOCAL_LDLIBS := \
	-ldl \
	-llog \
	-lz \
	-lEGL -lGLESv1_CM \

# Also need the JNI headers.
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/android \
	$(NDK_ROOT)/sources/cpufeatures
  

# Compiler flags.
LOCAL_CFLAGS += -O2 -fomit-frame-pointer -DNDEBUG -DANDROID -DANDROID_ARMV7 -DGP2X -DARM_ARCH -DINLINE="inline" -DDYNAREC -DPSXREC
LOCAL_CFLAGS += -ffast-math
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -mfpu=neon
endif
ifeq ($(TARGET_ARCH_ABI),armeabi)
LOCAL_CFLAGS += -DARMv5_ONLY
endif

include $(BUILD_SHARED_LIBRARY)
include $(NDK_ROOT)/sources/cpufeatures/Android.mk
