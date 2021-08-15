
#include <string.h>
#include <fapi/sys_services.h>
#include <fapi/reg_dma.h>
#include <fapi/drv_dma.h>
#include <fapi/drv_usb.h>
#include "musb/usb_ch9.h"
#include "musb/musbhdrc.h"
#include "musb/musbdefs.h"
#include "musb/list_49cd8c.h"
#include "musb/musb_dma.h"
#include "musb/musb.h"


#if 0
 The File Name Table:
  Entry Dir Time    Size    Name
  1 1   0   0   mu_mdr.c
  2 2   0   0   stdint.h
  3 3   0   0   mu_dsi.h
  4 3   0   0   mu_dev.h
  5 3   0   0   mu_cdi.h
  6 3   0   0   mu_list.h
  7 3   0   0   mu_hset.h
  8 3   0   0   mu_dci.h
  9 1   0   0   mu_impl.h
#endif

#if 0
  Offset  Name
  14316       MGC_MhdrcInit
  14470       MGC_MhdrcBindEndpoint
  14757       MGC_MhdrcStartRx
  15269       MGC_MhdrcStartTx
  15703       MGC_MhdrcDumpState
  15764       MGC_MhdrcDumpEndpoint
#endif


extern uint16_t MGC_Read16(uint8_t* baseAddr, uint16_t offset);
extern uint8_t MGC_HdrcInit(MGC_Port*);


/* 21ccef50 - complete */
/* v3.8: func_49754c */
uint8_t MGC_MhdrcInit(MGC_Port* pPort) /*Line 39*/
{
    uint8_t bResult; /*fp26*/
    uint8_t bMajor; /*fp25*/
    uint16_t wMinor; /*fp24*/
   uint16_t wVersion; /*fp22*/
   MGC_Controller* pController = pPort->pController; /*fp20*/ /*45*/
   uint8_t* pBase = pController->pControllerAddressIst; /*fp16*/ /*46*/

   bResult = MGC_HdrcInit(pPort);

   /* log release info by reading the HWVERS register */
   wVersion = MGC_Read16(pBase, 0x6c);
   bMajor = ((wVersion & 0x7c00) >> 10) & 0xFF;
   wMinor = wVersion & 0x3ff;

   if ((bMajor > 1) || ((bMajor == 1) && (wMinor >= 400)))
   {
      pPort->bHasDisablePing = 1;
   }

   if ((bMajor > 1) || ((bMajor == 1) && (wMinor >= 600)))
   {
      pPort->bHasRqPktCount = 1;
   }

   return bResult;
}







