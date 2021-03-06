# Verbosity
V ?= 1

compiler_win = msvc

include rules/base.mk

ifeq ($(platform),win)
   ifeq ($(compiler),gcc)
      $(error mingw build not supported)
   else ifeq ($(findstring amd64,$(LIB)),)
      $(error x86 32-bit build not supported)
   endif
endif

TARGET_NAME    := dolphin
DEBUG           = 0
STATIC_LINKING  = 0
LTO             = 0

PERF_TEST        = 1
DEBUGFAST        = 0
LOGGING          = 1
HAVE_OPENGL_CORE = 1
HAVE_VULKAN      = 1

ifneq ($(filter osx ios,$(platform)),)
   HAVE_VULKAN = 0
endif

$(call add_defines, PERF_TEST LOGGING DEBUGFAST HAVE_OPENGL_CORE HAVE_VULKAN)

ROOT_DIR       = ../../..
BASE_DIR       = ..
LIBRETRO_DIR   = .
DEPS_DIR       = $(ROOT_DIR)/Externals

INCLUDES += -I$(LIBRETRO_DIR)


# msvc
FLAGS_msvc       += -nologo -MP -Zc:wchar_t -Z7 -Gm- -Zc:inline -fp:precise -Zp16 -Zc:forScope -GR- -Gd -Oi -volatile:iso
CFLAGS_msvc      += -TC
CXXFLAGS_msvc    += -TP
WARNINGS_msvc    += -wd4996 -wd4351 -WX -W3
CWARNINGS_msvc   += -wd4267 -wd4244
CXXWARNINGS_msvc += -wd4530 -wd4577
#FLAGS_msvc += -showIncludes
#CXXFLAGS_msvc += -Zo -Zc:rvalueCast
CXXPCH_msvc = $(ROOT_DIR)/Source/PCH/pch.pch

# gcc
FLAGS_gcc    += -msse2 -fno-strict-aliasing -fvisibility=hidden -fomit-frame-pointer
CXXFLAGS_gcc += -Wmissing-declarations -fvisibility-inlines-hidden
CXXFLAGS_gcc += -fno-exceptions -fno-rtti -std=c++14
WARNINGS_gcc += -Wall -Wtype-limits -Wsign-compare -Wignored-qualifiers -Wuninitialized -Wlogical-op -Wshadow -Winit-self
WARNINGS_gcc += -Werror=implicit-function-declaration
LIBS_gcc     += -lpthread

# clang
FLAGS_clang    += -msse2 -fno-strict-aliasing -fvisibility=hidden -fomit-frame-pointer
CXXFLAGS_clang += -Wmissing-declarations -fvisibility-inlines-hidden
CXXFLAGS_clang += -fno-exceptions -fno-rtti -std=c++14
WARNINGS_clang += -Wall -Wtype-limits -Wsign-compare -Wignored-qualifiers -Wuninitialized -Wshadow -Winit-self
WARNINGS_clang += -Werror=implicit-function-declaration
LIBS_clang     += -lpthread

LIBS_unix      += -lrt
LIBS_osx       += -liconv -framework IOKit -framework AppKit


INCLUDES += -I$(CORE_DIR)
INCLUDES += -I$(DEPS_DIR)/Bochs_disasm
INCLUDES += -I$(DEPS_DIR)/enet/include
INCLUDES += -I$(DEPS_DIR)/mbedtls/include
INCLUDES += -I$(DEPS_DIR)/LZO
INCLUDES += -I$(DEPS_DIR)/zlib
INCLUDES += -I$(DEPS_DIR)/libpng

CXXINCLUDES += -I$(BASE_DIR)
CXXINCLUDES += -I$(DEPS_DIR)/SFML/include
CXXINCLUDES += -I$(DEPS_DIR)/SFML/src/
CXXINCLUDES += -I$(DEPS_DIR)/xxhash
CXXINCLUDES += -I$(DEPS_DIR)/SOIL
ifeq ($(HAVE_VULKAN),1)
   CXXINCLUDES       += -I$(DEPS_DIR)/Vulkan/Include
   CXXINCLUDES_msvc  += -I$(DEPS_DIR)/glslang/glslang/Public
   CXXINCLUDES_msvc  += -I$(DEPS_DIR)/glslang/SPIRV
   CXXINCLUDES_gcc   += -isystem$(DEPS_DIR)/glslang/glslang/Public
   CXXINCLUDES_gcc   += -isystem$(DEPS_DIR)/glslang/SPIRV
   CXXINCLUDES_clang += -isystem$(DEPS_DIR)/glslang/glslang/Public
   CXXINCLUDES_clang += -isystem$(DEPS_DIR)/glslang/SPIRV
endif

include rules/defines.mk

OBJECTS :=
OBJECTS += main.o
OBJECTS += audio.o
OBJECTS += boot.o
OBJECTS += host.o
OBJECTS += input.o
OBJECTS += state.o
OBJECTS += stubs.o
OBJECTS += video.o
OBJECTS += libco/libco.o
OBJECTS += ../UICommon/UICommon.o

include rules/core.mk
include rules/common.mk
include rules/discio.mk
include rules/inputcommon.mk
include rules/videocommon.mk
include rules/videobackends.mk
include rules/externals.mk

include rules/libretro.mk
