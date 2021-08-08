

#include <fapi/sys_services.h>
#include <fapi/drv_dma.h>
#include <fapi/drv_usb.h>
#include "musb/list_49cd8c.h"
#include "musb/musb_dma.h"
#include "musb/usb_ch9.h"
#include "musb/musbdefs.h"
#include "musb/musb.h"


/* 21ccbb1c - complete */
void musb_dma_dummy_destroy(MGC_DmaController* a)
{
   return;   
}


/* 21ccbb14 - complete */
MGC_DmaController* musb_dma_dummy_create(int (*a)(void* pPrivateData, char bLocalEnd, char bTransmit),
              struct MGC* b,
              char* (*c)(int, char* b),
              int d,
              int addr1,
              int addr2) 
{
   return 0;
}


MGC_DmaControllerFactory MGC_HdrcDmaControllerFactory = //21f02190 
{
   0x0300,
   musb_dma_dummy_create,
   musb_dma_dummy_destroy,   
};


