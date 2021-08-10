################################################################################
################################################################################
##
## Makefile -- makefile for smartgo-hd
##
## (C) Copyright 2012-2014 by Goke Microelectronics Co.,Ltd
##
## Mandatory settings:
##
## o TOPDIR      = the toplevel directory (using slashes as path separator)
## o TARGET      = the machine qualifier to generate code for
##
################################################################################
################################################################################

TOPDIR = ../..
SUBDIR = applications/smartgo-hd

include $(TOPDIR)/env/make/Makefile.config

################################################################################
# source files
################################################################################

SOURCES += sys.c
SOURCES += app_ref/src/app_main.c
SOURCES += app_ref/src/app_osd.c
SOURCES += app_ref/src/app_lang.c
SOURCES += app_ref/src/app_scaler.c
SOURCES += app_ref/src/app_items.c
SOURCES += app_ref/src/app_menustack.c
SOURCES += app_ref/src/app_menu_common.c
SOURCES += app_ref/src/app_menu_welcome.c
SOURCES += app_ref/src/app_menu_language.c
SOURCES += app_ref/src/app_menu_main.c

SOURCES += sys/src/av.c
SOURCES += sys/src/av_int.c
SOURCES += sys/src/av_api.c
SOURCES += sys/src/main.c
SOURCES += sys/src/device.c
SOURCES += sys/src/event_queue.c
SOURCES += sys/src/flash.c
SOURCES += sys/src/input.c
SOURCES += sys/src/settings.c
SOURCES += sys/src/stringop.c
SOURCES += sys/src/time.c
SOURCES += sys/src/vfd.c
SOURCES += sys/src/mcu.c
SOURCES += sys/src/pvr_sys.c
SOURCES += sys/src/dev_usb.c
SOURCES += osd_window.c
SOURCES += 21ba8d28.c
SOURCES += 21b902b8.c

SOURCES += test.c

SOURCES += reg_bm.c    #obsolete


SOURCES += fgs/src/fgs.c
SOURCES += fgs/src/fgs_box.c
#  SOURCES += fgs/src/fgs_item_char.c
#  SOURCES += fgs/src/fgs_item_check.c
#  SOURCES += fgs/src/fgs_item_number.c
SOURCES += fgs/src/fgs_item_select.c
SOURCES += fgs/src/fgs_item_text.c
SOURCES += fgs/src/fgs_list.c
SOURCES += fgs/src/fgs_navilist.c
SOURCES += fgs/src/fgs_panel.c
#  SOURCES += fgs/src/fgs_pininput.c


HEADERS += sys/inc/sys_priv.h
  
HEADERS += sys/src/event_queue.h
HEADERS += sys/src/device.h
HEADERS += sys/src/flash.h
HEADERS += sys/src/stringop.h
HEADERS += sys/src/sys_osd.h
HEADERS += sys/src/av.h
HEADERS += sys/src/sys_time.h
HEADERS += sys/src/av_api.h
HEADERS += sys.h
HEADERS += osd.h
HEADERS += osd_window.h
HEADERS += app_main.h
HEADERS += app_osd.h
HEADERS += 21ba8d28.h

HEADERS += app_ref/src/menustack.h
HEADERS += inc/smartgo_types.h 
HEADERS += inc/sys_api.h 
#HEADERS += app_ref/inc/app_osd.h 
HEADERS += app_ref/inc/app_lang.h 
HEADERS += app_ref/inc/app_scaler.h 
HEADERS += app_ref/inc/app_menu_items.h 

HEADERS += fgs/inc/fgs.h
HEADERS += fgs/inc/fgs_config.h
HEADERS += fgs/inc/fgs_keys.h

HEADERS += app_ref/res/lng/lng.h

-include app_ref/res/lng/Makefile.lng

################################################################################
# define define user cc flags
################################################################################

CCFLAGS_USER += -Ifgs/inc -Iinc

################################################################################
# define define user ld flags
################################################################################



################################################################################
# define library used
################################################################################

LIBRARIES  = fapi 
LIBRARIES += fapiboard 
LIBRARIES += famos 
LIBRARIES += val 
LIBRARIES += firmwarevideo 
LIBRARIES += firmwareaudio 
LIBRARIES += flashfs 
LIBRARIES += fatfs
LIBRARIES += fontserver
LIBRARIES += fontserver_engine_ft
LIBRARIES += freetype2
LIBRARIES += osdhandler
#LIBRARIES += picture
LIBRARIES += draw
LIBRARIES += png
LIBRARIES += z
LIBRARIES += musb


################################################################################
# define target name
################################################################################

PROGRAM = smartgo-hd

################################################################################
# define user targets
################################################################################

default: install

clean: clean-objects clean-all

build: headers build-objects build-program build-elfdump-program

install: build install-program

headers: install-headers

all: clean build install

  RELEASE_SUBDIR = $(SUBDIR)
  RELEASE_FILES += Makefile
  RELEASE_FILES += $(SOURCES)

release: release-files $(DIRECTORIES:%=subdir-release-%) info

doc: $(DIRECTORIES:%=subdir-doc-%)

info: force
	@echo $(DIRECTORIES)


include $(TOPDIR)/env/make/Makefile.rules

-include $(TOPDIR)/env/make/Makfile.dep

