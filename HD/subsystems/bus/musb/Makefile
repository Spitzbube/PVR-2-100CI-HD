################################################################################
################################################################################
##
## Makefile -- project specific makefile to build the Mentor USB library
##
## (C) Copyright 2006-2010 by Fujitsu Microelectronics Europe GmbH
## (C) Copyright 2010 by Fujitsu Semiconductor Europe GmbH
##
## Mandatory settings:
##
## o TOPDIR    = the toplevel directory (using slashes as path separator)
## o TARGET    = the machine qualifier to generate code for
## o SOURCES   = the list of source files to compile
## o STARTUP   = the name of the (assembler) startup file to use
## o HEADERS   = the list of local header files to install
## o PROGRAM   = the name of the executable to generate (without extension)
## o LIBRARIES = the list of library files to be linked
##
################################################################################
################################################################################

  TOPDIR = ../../..
  SUBDIR = subsystems/bus/musb

include $(TOPDIR)/env/make/Makefile.config

################################################################################
# project specific settings
################################################################################


  HEADERS  = $(wildcard include/*.h)
# HEADERS += $(wildcard include/c8051)
  HEADERS += $(wildcard include/class)
  HEADERS += $(wildcard src/systems/FAPI4/plat_*.h)
  HEADERS += $(wildcard src/systems/FAPI4/fapi_*.h)

  SOURCES  = src/list_49cd8c.c
  SOURCES += src/musb.c
  SOURCES += src/musb_dma_dummy.c
  SOURCES += src/musb_todo.c
  SOURCES += src/usb_storage.c

  LIBRARY = libmusb
  HEADER_SUBDIR = musb

  LINTFLAGS_USER += -e923

  MISRA = 0

ifdef MUSB_DIR
  DIR :=
else
# MUSB_DIR=$(shell cygpath -d -m "$$PWD")
  MUSB_DIR = ../..
  DIR := MUSB_DIR=$(MUSB_DIR)
endif

###############################################################################
#
# Set all Mentor specific compiler defines
#
###############################################################################
  TARGET_OS = FAPI4
  TARGET_ARCH = $(DECODER)
  PLATFORM = $(subst BOARD_FUJITSU,,$(BOARD))
  TARGET_TYPE = $(TARGET_ARCH)$(PLATFORM)

  MUSB_ARGS = TARGET_OS=$(TARGET_OS)
  MUSB_ARGS += TARGET_TYPE=$(TARGET_TYPE)
  MUSB_ARGS += MHDRC=1
  MUSB_ARGS += DMA=1
  MUSB_ARGS += HUB=1
  MUSB_ARGS += ISO=1
  MUSB_ARGS += HDRC_CFG=$(MUSB_DIR)/fme/$(TARGET_ARCH)_cfg.v

###############################################################################
#
# Helper macros to specify all Mentor build libraries
#
###############################################################################

  MU_LIB_C = bin/$(TARGET_OS)/$(TARGET_ARCH)/mu_libc$(LIBRARY_EXT)
  MU_LIB_UCD = bin/$(TARGET_OS)/$(TARGET_TYPE)/mu_ucd$(LIBRARY_EXT)
  MU_LIB_MSD = bin/$(TARGET_OS)/$(TARGET_ARCH)/dv_msd$(LIBRARY_EXT)
  MU_LIB_HUB = bin/$(TARGET_OS)/$(TARGET_ARCH)/dv_hub$(LIBRARY_EXT)

  MU_LIB_C_D = bin/$(TARGET_OS)/$(TARGET_ARCH)/mu_libc$(LIBRARY_DEBUG_EXT)
  MU_LIB_UCD_D = bin/$(TARGET_OS)/$(TARGET_TYPE)/mu_ucd$(LIBRARY_DEBUG_EXT)
  MU_LIB_MSD_D = bin/$(TARGET_OS)/$(TARGET_ARCH)/dv_msd$(LIBRARY_DEBUG_EXT)
  MU_LIB_HUB_D = bin/$(TARGET_OS)/$(TARGET_ARCH)/dv_hub$(LIBRARY_DEBUG_EXT)

###############################################################################
#
# Helper macros to specify all FAPI based libraries
#
###############################################################################

  MUSB_LIB = $(BUILD_LIBRARY_SUBDIR)/libmusb$(LIBRARY_EXT)
  MUSB_MSD = $(BUILD_LIBRARY_SUBDIR)/libmusb_msd$(LIBRARY_EXT)
  MUSB_HUB = $(BUILD_LIBRARY_SUBDIR)/libmusb_hub$(LIBRARY_EXT)
  MUSB_LIB_D = $(BUILD_LIBRARY_SUBDIR)/libmusb$(LIBRARY_DEBUG_EXT)
  MUSB_MSD_D = $(BUILD_LIBRARY_SUBDIR)/libmusb_msd$(LIBRARY_DEBUG_EXT)
  MUSB_HUB_D = $(BUILD_LIBRARY_SUBDIR)/libmusb_hub$(LIBRARY_DEBUG_EXT)

################################################################################
# define user targets
################################################################################

default: build

clean: clean-musb-files

build: clean build-musb-files

headers: install-headers

#install: build install-musb-files
install: build install-library

all: clean build install

################################################################################
# include internal definitions and rules
################################################################################

  RELEASE_SUBDIR = $(SUBDIR)
  RELEASE_FILES += $(HEADERS)
  RELEASE_FILES += $(wildcard build/$(DECODER_SUBDIR)/M-arm11-realview/libmusb*.a)
  RELEASE_FILES += $(wildcard build/$(DECODER_SUBDIR)/M-arm11-gnuarm/libmusb*.a)
  RELEASE_FILES += Makefile

release: doc release-files

doc: force
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Copy documentation [$(TOPDIR)/docs/online/$(DECODER)/general/musb]
	@$(DELETE) $(DELETEFLAGS) $(TOPDIR)/docs/online/$(DECODER)/general/musb/*/.svn
	@$(DELETE) $(DELETEFLAGS) $(TOPDIR)/docs/online/$(DECODER)/general/musb/*/*/.svn
	@$(DELETE) $(DELETEFLAGS) $(TOPDIR)/docs/online/$(DECODER)/general/musb/*/*/*/.svn
	$(ATSIGN)$(MKDIR) $(MKDIRFLAGS) $(TOPDIR)/docs/online/$(DECODER)/general/musb/
	$(ATSIGN)$(COPY) -r $(COPYFLAGS) docs/* $(TOPDIR)/docs/online/$(DECODER)/general/musb/
	$(ATSIGN)$(DELETE) $(DELETEFLAGS) $(TOPDIR)/docs/online/$(DECODER)/general/musb/musb_main.dox
	$(ATSIGN)$(DELETE) $(DELETEFLAGS) $(TOPDIR)/docs/online/$(DECODER)/general/musb/musb_overview.*
	@$(DELETE) $(DELETEFLAGS) $(TOPDIR)/docs/online/$(DECODER)/general/musb/*/.svn
	@$(DELETE) $(DELETEFLAGS) $(TOPDIR)/docs/online/$(DECODER)/general/musb/*/*/.svn
	@$(DELETE) $(DELETEFLAGS) $(TOPDIR)/docs/online/$(DECODER)/general/musb/*/*/*/.svn

include $(TOPDIR)/env/make/Makefile.rules

################################################################################
# include optional dependencies
################################################################################

-include $(DEPENDENCY_FILENAME)

################################################################################
# include musb library specific targets (at FME only!)
################################################################################

-include ./fme/Makefile.${USERNAME}

clean-musb-files: clean-objects clean-library

build-musb-files: build-objects build-library

install-musb-files:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Install Libraries [$(INSTALL_LIBRARY_DIR)]
	$(ATSIGN)$(MKDIR) $(MKDIRFLAGS) $(INSTALL_LIBRARY_DIR)
	$(ATSIGN)$(COPY) -r $(COPYFLAGS) $(BUILD_LIBRARY_SUBDIR)/lib*$(LIBRARY_EXT) $(INSTALL_LIBRARY_DIR)
