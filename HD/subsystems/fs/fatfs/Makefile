################################################################################
################################################################################
##
## Makefile -- project specific makefile to build an object library
##
## (C) Copyright 2006-2010 by Fujitsu Microelectronics Europe GmbH
## (C) Copyright 2010 by Fujitsu Semiconductor Europe GmbH
##
## Mandatory settings:
##
## o TOPDIR    = the toplevel directory (using slashes as path separator)
## o TARGET   = the machine qualifier to generate code for
## o SOURCES   = the list of source files to compile
## o STARTUP   = the name of the (assembler) startup file to use
## o HEADERS   = the list of local header files to install
## o PROGRAM   = the name of the executable to generate (without extension)
## o LIBRARIES = the list of library files to be linked
##
################################################################################
################################################################################

TOPDIR           = ../../..
SUBDIR           = subsystems/fs/fatfs

include $(TOPDIR)/env/make/Makefile.config

#TARGET           = arm11-realview

SOURCES         += src/fatfs.c
SOURCES         += src/api_f.c
SOURCES         += src/usbdrv.c

HEADERS         += src/fatfs.h 
HEADERS         += src/api_f.h
HEADERS         += src/fwerr.h
HEADERS         += src/usbdrv.h
  
LIBRARY          = libfatfs
HEADER_SUBDIR    = fatfs

################################################################################
# define user targets
################################################################################

default: build

clean: clean-objects clean-library

build: headers build-objects build-library

headers: install-headers

install: build install-library

all: clean build install

################################################################################
# include internal definitions and rules
################################################################################

RELEASE_SUBDIR = $(SUBDIR)
RELEASE_FILES += $(SOURCES)
RELEASE_FILES += $(HEADERS)
RELEASE_FILES += Makefile

release: doc release-files

doc: $(DIRECTORIES:%=subdir-doc-%)
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Copy flashfs documentation [$(TOPDIR)/docs/online/$(DECODER)/general/flashfs/]
	$(ATSIGN)$(MKDIR) $(MKDIRFLAGS) $(TOPDIR)/docs/online/$(DECODER)/general/flashfs/
#    $(ATSIGN)$(COPY) $(COPYFLAGS) docs/pdf/*.pdf $(TOPDIR)/docs/online/$(DECODER)/general/flashfs/

include $(TOPDIR)/env/make/Makefile.rules

################################################################################
# include optional dependencies
################################################################################

-include $(DEPENDENCY_FILENAME)
