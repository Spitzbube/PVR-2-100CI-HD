################################################################################
################################################################################
##
## Makefile -- Makefile for ADI library
##
## (C) Copyright 2012-2015 by Goke Microelectronics Co.,Ltd
##
## Version: $Id$
##
## Mandatory settings:
##
## o TOPDIR      = the toplevel directory (using slashes as path separator)
## o TARGET      = the machine qualifier to generate code for
##
################################################################################
################################################################################


TOPDIR = ..
SUBDIR = fapi

include $(TOPDIR)/env/make/Makefile.config

################################################################################
# project specific settings
################################################################################

  NODEBUG = 1

  SOURCES += drv/boot/drv_boot.c
  SOURCES += drv/clkpwr/drv_clkpwr.c
  SOURCES += drv/ata/drv_ata.c
  SOURCES += drv/bm/drv_bm.c
  SOURCES += drv/bm/fapi_bm.c   #obsolete
  SOURCES += drv/dma/drv_dma.c
  SOURCES += drv/dma/drv_dma_sr.c
  SOURCES += drv/dma/drv_dma_ccir656.c
  SOURCES += drv/flash/drv_flash.c
  SOURCES += drv/flash/drv_sflash.c
  SOURCES += drv/flash/drv_sflash_devices.c
  SOURCES += drv/flash/fapi_flash.c   #obsolete
  SOURCES += drv/gpio/drv_gpio.c
  SOURCES += drv/gpreg/drv_gpreg.c
  SOURCES += drv/hdmi/drv_hdmi.c
  SOURCES += drv/i2c/drv_i2c.c
  SOURCES += drv/intr/drv_intr.c
  SOURCES += drv/mmu/bget.c
  SOURCES += drv/mmu/drv_mmu.c
  SOURCES += drv/mmu/drv_mmu_heap.c
  SOURCES += drv/sync/drv_sync.c
  SOURCES += drv/sync/fapi_sync.c   #obsolete
  SOURCES += drv/timer/drv_timer.c
  SOURCES += drv/tsd/drv_tsd.c
  SOURCES += drv/uart/drv_uart.c
  SOURCES += drv/upi/drv_upi.c
  SOURCES += drv/usb/drv_usb.c
  SOURCES += drv/usb/drv_usb_volume.c
  SOURCES += drv/usb/fm_usb_msc.c
  SOURCES += drv/videc/drv_videc.c
  SOURCES += drv/videc/drv_videc_decoder0.c
  SOURCES += drv/vienc/drv_vienc.c
  SOURCES += drv/vienc/drv_vienc_hd.c
  SOURCES += drv/vienc/drv_vienc_sd.c
  SOURCES += drv/vienc/drv_vienc_priv.c
  SOURCES += drv/viscale/drv_viscale.c
  SOURCES += drv/viscale/drv_viscale_cp.c
  SOURCES += drv/viscale/drv_viscale_osd1.c
  SOURCES += drv/viscale/drv_viscale_osd2.c
  SOURCES += drv/viscale/drv_viscale_sd.c
  SOURCES += drv/viscale/drv_viscale_vo.c
  
  SOURCES += drv/obsolete/21cb40c8.c  #obsolete

  SOURCES += reg/$(DECODER)/reg_bm.c
  SOURCES += reg/$(DECODER)/reg_clkpwr.c
  SOURCES += reg/$(DECODER)/reg_ddrramif.c
  SOURCES += reg/$(DECODER)/reg_dma.c
  SOURCES += reg/$(DECODER)/reg_gpio.c
  SOURCES += reg/$(DECODER)/reg_i2c.c
  SOURCES += reg/$(DECODER)/reg_irq_ctrl.c
  SOURCES += reg/$(DECODER)/reg_sflash.c
  SOURCES += reg/$(DECODER)/reg_tsd.c
  SOURCES += reg/$(DECODER)/reg_uart.c
  SOURCES += reg/$(DECODER)/reg_upi.c
  SOURCES += reg/$(DECODER)/reg_vd.c
  SOURCES += reg/$(DECODER)/reg_vd_ext.c
  SOURCES += reg/$(DECODER)/reg_ve.c
  SOURCES += reg/$(DECODER)/reg_ve_hd.c
  SOURCES += reg/$(DECODER)/reg_vic.c
  SOURCES += reg/$(DECODER)/reg_vo.c

  SOURCES += sys/sys_services.c

#  CCFLAGS_USER += -DINLINE=__inline
  
  HEADERS = $(SOURCES:%.c=%.h)
  HEADERS += sys/sys_driver.h
  HEADERS += sys/sys_error.h
  HEADERS += sys/sys_init.h
  HEADERS += sys/customer/sys_init_customer.h
  SOURCES += drv/bm/drv_bm_priv.h
  SOURCES += drv/dma/drv_dma_priv.h
  SOURCES += drv/flash/drv_flash_priv.h
  SOURCES += drv/tsd/drv_tsd_priv.h
  SOURCES += drv/usb/drv_usb_func_msc.h
  SOURCES += drv/usb/drv_usb_func_vendor.h
  SOURCES += drv/usb/drv_usb_volume.h
  SOURCES += drv/videc/drv_videc_priv.h
  SOURCES += drv/vienc/drv_vienc_priv.h
  SOURCES += drv/viscale/drv_viscale_priv.h
  HEADERS += reg/$(DECODER)/reg_efuse.h
  HEADERS += reg/$(DECODER)/reg_sdram.h

  HEADERS += reg/$(DECODER)/reg_bm_old.h

  LIBRARY = libfapi
  HEADER_SUBDIR = fapi
  
  DIRECTORIES = board   

################################################################################
# define user targets
################################################################################

default: all

clean: $(DIRECTORIES:%=subdir-clean-%) clean-objects clean-library

build: $(DIRECTORIES:%=subdir-build-%) build-objects build-library

headers: $(DIRECTORIES:%=subdir-headers-%) install-headers

install: build $(DIRECTORIES:%=subdir-install-%) install-library

all: clean build install


include $(TOPDIR)/env/make/Makefile.rules


subdir-clean-%:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Making [$(SUBDIR)/$(@:subdir-clean-%=%)] for target [$(TARGET)]
	@$(SUBMAKE) -C $(@:subdir-clean-%=%) clean TARGET=$(TARGET)

subdir-headers-%:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Making [$(SUBDIR)/$(@:subdir-headers-%=%)] for target [$(TARGET)]
	@$(SUBMAKE) -C $(@:subdir-headers-%=%) headers TARGET=$(TARGET)

subdir-build-%:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Making [$(SUBDIR)/$(@:subdir-build-%=%)] for target [$(TARGET)]
	@$(SUBMAKE) -C $(@:subdir-build-%=%) build TARGET=$(TARGET)

subdir-install-%:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Making [$(SUBDIR)/$(@:subdir-install-%=%)] for target [$(TARGET)]
	@$(SUBMAKE) -C $(@:subdir-install-%=%) install TARGET=$(TARGET)
	