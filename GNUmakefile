# Ut Video Codec Suite  POSIX/MinGW Makefile
#
# Originally written by: Derek Buitenhuis <derek.buitenhuis AT gmail.com>
#
# Usage: make
#        make install prefix=/some/dir/here
#        make uninstall prefix=/some/dir/here

include config.mak

# Pretty-ify Building
ifndef V
$(foreach VAR,CXX ASM AR RANLIB INSTALL,\
    $(eval override $(VAR) = @printf " %s\t%s\n" $(VAR) "$$@"; $($(VAR))))
endif

# Set the proper nasm output format
ifeq ($(ARCH),x86)
  ifeq ($(SYS),MINGW)
    ASMFORMAT=win32
  else
    ASMFORMAT=elf32
  endif
else
  ifeq ($(ARCH),x64)
    ifeq ($(SYS),MINGW)
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

$(SONAME):
	$(CXX) -shared -o $@ $^ $(SOFLAGS) $(SOFLAGS_USER) $(LDFLAGS)

ifneq ($(SYS),MINGW)
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
else
OBJ = $(UTV_CORE_DIR)/Codec.o \
      $(UTV_CORE_DIR)/Convert.o \
      $(UTV_CORE_DIR)/DummyCodec.o \
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
endif

ifneq ($(ARCH),)
  ifeq ($(ASMFORMAT),)
all:
	@echo "Invalid ARCH specified. Use x86 or x64, or none at all."
  endif
else
all: default
endif

$(STATICLIB): $(OBJ)
$(SONAME): $(OBJ)

static-lib: $(STATICLIB)
shared-lib: $(SONAME)

clean:
	@printf " RM\t$(UTV_CORE_DIR)/*.o\n";
	@rm -f $(UTV_CORE_DIR)/*.o
	@printf " RM\t$(UTV_CORE_DIR)/*.a\n";
	@rm -f $(UTV_CORE_DIR)/*.a
	@$(if $(IMPLIBNAME),\
	@printf " RM\t$(IMPLIBNAME)\n";${\n}\
	@rm -f $(IMPLIBNAME))
	@$(if $(SONAME),\
	@printf " RM\t$(SONAME)\n";${\n}\
	@rm -f $(SONAME))

install: all
	@install -d $(DESTDIR)$(includedir)
	@install -d $(DESTDIR)$(includedir)/utvideo
	@install -d $(DESTDIR)$(libdir)
	@printf " INSTALL\t$(includedir)/utvideo/Codec.h\n";
	@install -m 644 $(UTV_CORE_DIR)/Codec.h $(DESTDIR)$(includedir)/utvideo
	@printf " INSTALL\t$(includedir)/utvideo/utvideo.h\n";
	@install -m 644 $(UTV_CORE_DIR)/utvideo.h $(DESTDIR)$(includedir)/utvideo
	@$(if $(STATICLIB), \
	@printf " INSTALL\t$(libdir)/libutvideo.a\n";${\n}\
	@install -m 644 $(STATICLIB) $(DESTDIR)$(libdir)${\n}\
	@$(RANLIBX) $(DESTDIR)$(libdir)/libutvideo.a)
ifeq ($(SYS),MINGW)
	@$(if $(SONAME), \
	@printf " INSTALL\t$(bindir)/$(SONAME)\n";${\n}\
	@install -d $(DESTDIR)$(bindir)${\n}\
	@install -m 755 $(SONAME) $(DESTDIR)$(bindir))
else
	@$(if $(SONAME), \
	@printf " INSTALL\t$(libdir)/libutvideo.$(SOSUFFIX)\n";${\n}\
	@ln -f -s $(SONAME) $(DESTDIR)$(libdir)/libutvideo.$(SOSUFFIX))
	@$(if $(SONAME), \
	@printf " INSTALL\t$(libdir)/$(SONAME)\n";${\n}\
	@install -m 755 $(SONAME) $(DESTDIR)$(libdir))
endif
	@$(if $(IMPLIBNAME), \
	@printf " INSTALL\t$(libdir)/$(IMPLIBNAME)\n";${\n}\
	@install -m 644 $(IMPLIBNAME) $(DESTDIR)$(libdir))

define \n


endef

uninstall:
	@printf " RM\t$(includedir)/utvideo/*.h\n";
	@rm -f $(includedir)/utvideo/*.h
	@printf " RMDIR\t$(includedir)/utvideo\n";
	@-rmdir $(includedir)/utvideo 2> /dev/null || \
	  if [ -d $(includedir)/utvideo ]; then \
	    printf " NOTE: Not removing $(includedir)/utvideo since it is not empty.\n"; \
	  fi
	@$(if $(STATICLIB),\
	@printf " RM\t$(libdir)/libutvideo.a\n";${\n}\
	@rm -f $(libdir)/libutvideo.a)
	@$(if $(IMPLIBNAME),\
	@printf " RM\t$(libdir)/$(IMPLIBNAME)\n";${\n}\
	@rm -f $(libdir)/$(IMPLIBNAME))
ifeq ($(SYS),MINGW)
	@$(if $(SONAME),\
	@printf " RM\t$(bindir)/$(SONAME)\n";${\n}\
	@rm -f $(bindir)/$(SONAME))
else
	@$(if $(SONAME),\
	@printf " RM\t$(libdir)/libutvideo.$(SOSUFFIX)\n";${\n}\
	@rm -f $(libdir)/libutvideo.$(SOSUFFIX)${\n}\
	@printf " RM\t$(libdir)/$(SONAME)\n";${\n}\
	@rm -f $(libdir)/$(SONAME))
endif

distclean: clean
	@printf " RM\t*.log\n";
	@rm -f *.log
	@printf " RM\t*.mak\n";
	@rm -f *.mak

.PHONY: all static-lib shared-lib clean install uninstall distclean
