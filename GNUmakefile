# Ut Video Codec Suite  POSIX/MinGW Makefile
#
# Originally written by: Derek Buitenhuis <derek.buitenhuis AT gmail.com>
#
# Usage: make
#        make install prefix=/some/dir/here
#        make uninstall prefix=/some/dir/here

DESTDIR=

prefix=/usr/local
libdir=$(DESTDIR)$(prefix)/lib
includedir=$(DESTDIR)$(prefix)/include

# Default to no assembly
ARCH=
OS=linux

CROSS_PREFIX=
ASM=nasm
CXX=$(CROSS_PREFIX)g++
AR=$(CROSS_PREFIX)ar
RANLIB=$(CROSS_PREFIX)ranlib

UTV_CORE_DIR=utv_core

CXXFLAGS=-g -O2 -Wall -Wextra -Wno-multichar -Wno-unused-parameter -Wno-sign-compare
ASMFLAGS=-g -Xgnu

# on MinGW env, uncomment following lines and set proper value to WINSDK_ROOT.
#WINSDK_ROOT="/c/Program Files/Microsoft SDKs/Windows/v6.1"
#CPPFLAGS=-I/usr/include/w32api -I$(WINSDK_ROOT)/Include

# Pretty-ify Building
ifndef V
$(foreach VAR,CXX ASM AR RANLIB,\
    $(eval override $(VAR) = @printf " %s\t%s\n" $(VAR) "$$@"; $($(VAR))))
endif

# Set the proper nasm output format
ifeq ($(ARCH),x86)
  ifeq ($(OS),windows)
    ASMFORMAT=win32
  else
    ASMFORMAT=elf32
  endif
else
  ifeq ($(ARCH),x64)
    ifeq ($(OS),windows)
      ASMFORMAT=win64
    else
      ASMFORMAT=elf64
    endif
  endif
endif

# Only build if we want to build asm
ifneq ($(ARCH),)
CXXFLAGS += -DSTATIC_LIB_WITH_ASM
ASM_OBJECTS = $(UTV_CORE_DIR)/Convert_asm_$(ARCH).o \
              $(UTV_CORE_DIR)/HuffmanCode_asm_$(ARCH).o \
              $(UTV_CORE_DIR)/Predict_asm_$(ARCH).o
else
ASM_OBJECTS=
endif

%.o: %.asm
	$(ASM) $(ASMFLAGS) -f $(ASMFORMAT) -I$(UTV_CORE_DIR)/ -o $@ $^

%.a:
	$(AR) rcu $@ $^
	$(RANLIB) $@

OBJ = $(UTV_CORE_DIR)/Codec.o \
      $(UTV_CORE_DIR)/Convert.o \
      $(UTV_CORE_DIR)/DummyCodec.o \
      $(UTV_CORE_DIR)/Format.o \
      $(UTV_CORE_DIR)/FrameBuffer.o \
      $(UTV_CORE_DIR)/GlobalConfig.o \
      $(UTV_CORE_DIR)/HuffmanCode.o \
      $(UTV_CORE_DIR)/Predict.o \
      $(UTV_CORE_DIR)/Thread.o \
      $(UTV_CORE_DIR)/TunedFunc.o \
      $(UTV_CORE_DIR)/UL00Codec.o \
      $(UTV_CORE_DIR)/ULRACodec.o \
      $(UTV_CORE_DIR)/ULRGCodec.o \
      $(UTV_CORE_DIR)/ULY0Codec.o \
      $(UTV_CORE_DIR)/ULY2Codec.o \
      $(UTV_CORE_DIR)/utv_core.o \
      $(ASM_OBJECTS)

ifneq ($(ARCH),)
  ifeq ($(ASMFORMAT),)
all:
	@echo "Invalid ARCH specified. Use x86 or x64, or none at all."
  endif
else
all: static-lib
endif

$(UTV_CORE_DIR)/libutvideo.a: $(OBJ)

static-lib: $(UTV_CORE_DIR)/libutvideo.a

clean:
	@printf " RM\t$(UTV_CORE_DIR)/*.o\n";
	@rm -f $(UTV_CORE_DIR)/*.o
	@printf " RM\t$(UTV_CORE_DIR)/libutvideo.a\n";
	@rm -f $(UTV_CORE_DIR)/libutvideo.a

install: all
	@mkdir -p $(libdir) # in case of custom install dir
	@printf " MKDIR\t\t$(includedir)/utvideo\n";
	@mkdir -p $(includedir)/utvideo
	@printf " INSTALL\t$(includedir)/utvideo/Codec.h\n";
	@cp -f $(UTV_CORE_DIR)/Codec.h $(includedir)/utvideo/Codec.h
	@printf " INSTALL\t$(includedir)/utvideo/utvideo.h\n";
	@cp -f $(UTV_CORE_DIR)/utvideo.h $(includedir)/utvideo/utvideo.h
	@printf " INSTALL\t$(libdir)/libutvideo.a\n";
	@cp -f $(UTV_CORE_DIR)/libutvideo.a $(libdir)/libutvideo.a

uninstall:
	@printf " RM\t$(includedir)/utvideo/*.h\n";
	@rm -f $(includedir)/utvideo/*.h
	@printf " RMDIR\t$(includedir)/utvideo\n";
	@-rmdir $(includedir)/utvideo 2> /dev/null || \
	  if [ -d $(includedir)/utvideo ]; then \
	    printf " NOTE: Not removing $(includedir)/utvideo since it is not empty.\n"; \
	  fi
	@printf " RM\t$(libdir)/libutvideo.a\n";
	@rm -f $(libdir)/libutvideo.a

.PHONY: all static-lib clean install uninstall
