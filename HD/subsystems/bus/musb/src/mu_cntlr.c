
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
  1 1   0   0   mu_cntlr.c
  2 2   0   0   stddef.h
  3 3   0   0   stdint.h
  4 4   0   0   mu_dsi.h
  5 4   0   0   mu_dev.h
  6 4   0   0   mu_cdi.h
  7 4   0   0   mu_list.h
  8 4   0   0   mu_hset.h
  9 4   0   0   mu_dci.h
  10    1   0   0   mu_impl.h
  11    5   0   0   sys_driver.h
  12    5   0   0   sys_services.h
#endif

#if 0
  Offset  Name
  14192       MUSB_SetHsDmaControllerFactory
  15791       MUSB_SetDiagnosticLevel
  16022       MUSB_NewController
  16238       MUSB_SetControllerHostPower
  16359       MUSB_StartController
  16452       MUSB_StopController
  16529       MUSB_DestroyController
  16624       MUSB_CountPorts
  16662       MUSB_GetPort
  16749       MUSB_DeactivateClient
  16835       MUSB_GetBusFrame
  16910       MUSB_SuspendBus
  17079       MUSB_ResumeBus
  17148       MUSB_AllocDmaBuffer
  17348       MUSB_FreeDmaBuffer
  17504       MGC_AllocateDynamicFifo
  18527       MGC_DiagMsg
  18569       MGC_DiagNumber
  18598       MGC_bDiagLevel
#endif

typedef struct
{
#if 1
    MGC_Controller ControllerImpl; //0
#else
   int addr1; //0
   int addr2; //4
   MUSB_SystemUtils* pSystemUtils; //8
   MUSB_SystemServices* pOS; //12
   int fill_16; //16
   struct MGC* Data_20; //20
   struct MGC_DmaControllerFactory* pDmaControllerFactory; //24
   int (*pfEnableInterrupts)(struct Struct_49d2fc_t*); //28
   void (*pfDisableInterrupts)(struct Struct_49d2fc_t*); //32
   void (*Data_36)(void); //36
   void (*Data_40)(void); //40
   int fill_44; //44
#endif

#if 1
   MUSB_Controller Controller; //48
#else
   MUSB_Controller Data_48; //48 +40
#endif

#if 1
   MGC_Port PortImpl; //88
#else
   struct MGC Data_88; //88 +616
#endif

   MUSB_Port Port;
   //704
} MGC_ControllerWrapper; /*Line 79*/



extern int Data_21f02130[]; // = {10}; //21f02130
extern unsigned short wData_21f7a040; //21f7a040
extern MUSB_LinkedList Data_21f7a044; //21f7a044



/* 48bd78 - todo */
static void MGC_ControllerInit(MGC_ControllerWrapper* pWrapper, /*116*/
        void* pControllerAddressIsr, /*117*/
        void* pControllerAddressIst) /*118*/
{
   pWrapper->ControllerImpl.pControllerAddressIsr = pControllerAddressIsr;
   pWrapper->ControllerImpl.pControllerAddressIst = pControllerAddressIst;
   pWrapper->ControllerImpl.pPort = &pWrapper->PortImpl;

   MUSB_ListInit(&pWrapper->PortImpl.ConnectedDeviceList);
   MUSB_ListInit(&pWrapper->PortImpl.Schedule.ScheduleSlots);

   pWrapper->PortImpl.pInterfacePort = &pWrapper->Port;
   pWrapper->Controller.wVersion = 1;
   pWrapper->Controller.pPrivateData = &pWrapper->ControllerImpl;
   pWrapper->Port.pPrivateData = &pWrapper->PortImpl;
   pWrapper->Port.bcdCdiRev = 0x100;
   pWrapper->PortImpl.pController = &pWrapper->ControllerImpl;
}


/* 48c1c0 - complete */
static uint16_t MGC_DiscoverController(void* pBase) /*142*/
{
    uint16_t wType = 0; /*147*/

    /* Empty */

    return wType;
}


/* 21cc4eac - todo */
/* v3.8: func_49d2fc */
MUSB_Controller* MUSB_NewController(MUSB_SystemUtils* pUtils/*fp36*/, /*197*/
        uint16_t wControllerType/*fp40*/,
      void* pControllerAddressIsr/*fp44*/, /*199*/
      void* pControllerAddressBsr/*fp48*/) /*201*/
{
    uint8_t bEnd; /*207  Type <178>*/
    MGC_EndpointResource/*MGC_Endpoint*/* pEnd; /*type <2491>*/
    uint16_t wType = wControllerType; /*fp24*/ /*type <187>*/
    uint8_t bOK = 0; /*fp21*/
    MUSB_Controller* pResult = NULL; /*fp20*/ /*211 type <3f68>   */
    MGC_ControllerWrapper* pWrapper = NULL; /*fp16*/ /*212 type <3e40>*/

   pWrapper = FAPI_SYS_MALLOC(sizeof(MGC_ControllerWrapper));

   if (pWrapper != NULL)
   {
      MGC_FAPI_MemSet(pWrapper, 0, sizeof(MGC_ControllerWrapper));

      /* Add an element in the list and set its data */
      if (0 != MUSB_ListAppendItem(&Data_21f7a044, pWrapper, 0))
      {
         wData_21f7a040++;
      }
      else
      {
         //21cc4f70
         (FAPI_SYS_Services.freeFunc)(pWrapper);
         pWrapper = NULL;
      }
   }

   if (pWrapper != NULL)
   {
       pWrapper->ControllerImpl.pUtils = pUtils;

      MGC_ControllerInit(pWrapper, pControllerAddressIsr, pControllerAddressBsr);

      if (wType == 0)
      {
          wType = MGC_DiscoverController(pControllerAddressBsr);
      }

      if (wType == 3)
      {
          extern int MGC_HdrcIsr(MUSB_Controller*);
          extern void MGC_DrcBsr(MUSB_Port*);
          extern MGC_DmaControllerFactory* mgc_pDmaControllerFactory;
          extern uint32_t MGC_HdrcStart(struct _MGC_Controller*);
          extern uint32_t MGC_HdrcStop(struct _MGC_Controller*);
          extern uint32_t MGC_HdrcDestroy(struct _MGC_Controller*);
          extern uint32_t MGC_DrcSetHostPower(struct _MGC_Controller*);
          extern uint32_t MGC_HdrcReadBusState(struct _MGC_Port*);
          extern uint32_t func_21cc96e8(struct _MGC_Port*);
          extern uint32_t func_21cc67d8(struct _MGC_Port*);
          extern MGC_EndpointResource* func_21cceaac(struct _MGC_Port*,
                  const MUSB_DeviceEndpoint*,
                  MUSB_EndpointResource*,
                  uint8_t);
          extern uint32_t mgc_receive_on_endpoint(struct _MGC_Port*, //<2d55>
                  MGC_EndpointResource*, //<2491>
                  uint8_t*, //<f1a>
                  uint32_t, //<1a6>
                  void*, //<f3>
                  uint8_t //<178>
                  );
          extern uint32_t mgc_send_on_endpoint(struct _MGC_Port*, //<2d55>
                  MGC_EndpointResource*, //<2491>
                  const uint8_t*, //<659>
                  uint32_t, //<1a6>
                  void* //<f3>
                  );
          extern uint32_t func_21cc993c(struct _MGC_Port*, //<2d55>
                  MGC_EndpointResource*, //<2491>
                  uint8_t, //<178>
                  uint8_t //<178>
                  );
          extern uint32_t func_21ccb504(struct _MGC_Port*, //<2d55>
                  MGC_EndpointResource*, //<2491>
                  uint8_t, //<178>
                  uint8_t //<178>
                  );
          extern uint32_t func_21ccabf0(struct _MGC_Port*, //<2d55>
                  uint8_t //<178>
                  );
          extern uint8_t MGC_HdrcServiceDefaultEnd(struct _MGC_Port*, //<2d55>
                  MGC_BsrItem* //<3411>
                  );
          extern uint8_t MGC_HdrcServiceTxAvail(struct _MGC_Port*, //<2d55>
                  uint16_t, //<187>
                  MGC_BsrItem* //<3411>
                  );
          extern uint8_t MGC_HdrcServiceRxReady(struct _MGC_Port*, //<2d55>
                  uint16_t, //<187>
                  MGC_BsrItem* //<3411>
                  );
          extern uint8_t MGC_HdrcLoadFifo(struct _MGC_Port*, //<2d55>
                  uint8_t, //<178>
                  uint32_t, //<1a6>
                  const uint8_t* //<659>
                  );
          extern uint8_t MGC_HdrcUnloadFifo(struct _MGC_Port*, //<2d55>
                  uint8_t, //<178>
                  uint32_t, //<1a6>
                  uint8_t* //<f1a>
                  );
          extern uint8_t func_21cc6d64(struct _MGC_Port*, //<2d55>
                  MUSB_Device*, //<1486>
                  MUSB_DeviceDriver* //<169a>
                  );
          extern uint8_t func_21ccb018(void*, uint8_t, uint8_t);
          extern void func_21cca940(struct _MGC_Port*, //<2d55>
                  MUSB_HsetPortMode //<1a14>
                  );
          extern void func_21cc9530(struct _MGC_Port*, //<2d55>
                  uint8_t, //<178>
                  uint8_t, //<178>
                  uint8_t, //<178>
                  uint32_t*, //<119c>
                  uint8_t*, //<f1a>
                  uint8_t* //<f1a>
                  );
          extern uint8_t MGC_MhdrcInit(MGC_Port*);

          bOK = MGC_MhdrcInit(&pWrapper->PortImpl);

          pWrapper->PortImpl.bWantHighSpeed = 1;
          pWrapper->PortImpl.bIsMultipoint = 1;

          pWrapper->Controller.wQueueLength = 80;
          pWrapper->Controller.wQueueItemSize = 8;
          pWrapper->Controller.wTimerCount = 1;
          pWrapper->Controller.adwTimerResolutions = Data_21f02130;
          pWrapper->Controller.wLockCount = 17;
          pWrapper->Controller.pfIsr = MGC_HdrcIsr;
          pWrapper->Controller.pIsrParam = &pWrapper->Controller;
          pWrapper->Controller.pfBsr = MGC_DrcBsr;
          pWrapper->Controller.pBsrParam = &pWrapper->Port;

          pWrapper->ControllerImpl.pDmaControllerFactory = mgc_pDmaControllerFactory;
          pWrapper->ControllerImpl.pfProgramStartController = MGC_HdrcStart; //7ec
          pWrapper->ControllerImpl.pfProgramStopController = MGC_HdrcStop;
          pWrapper->ControllerImpl.pfDestroyController = MGC_HdrcDestroy;
          pWrapper->ControllerImpl.pfSetControllerHostPower = MGC_DrcSetHostPower;

          pWrapper->PortImpl.pfReadBusState = MGC_HdrcReadBusState;
          pWrapper->PortImpl.pfProgramBusState = func_21cc96e8;
          pWrapper->PortImpl.pfResetPort = func_21cc67d8;
          pWrapper->PortImpl.pfBindEndpoint = func_21cceaac;
          pWrapper->PortImpl.pfProgramStartReceive = mgc_receive_on_endpoint;
          pWrapper->PortImpl.pfProgramStartTransmit = mgc_send_on_endpoint;
          pWrapper->PortImpl.pfProgramFlushEndpoint = func_21cc993c;
          pWrapper->PortImpl.pfProgramHaltEndpoint = func_21ccb504;
          pWrapper->PortImpl.pfDefaultEndResponse = func_21ccabf0;
          pWrapper->PortImpl.pfServiceDefaultEnd = MGC_HdrcServiceDefaultEnd;
          pWrapper->PortImpl.pfServiceTransmitAvail = MGC_HdrcServiceTxAvail;
          pWrapper->PortImpl.pfServiceReceiveReady = MGC_HdrcServiceRxReady;
          pWrapper->PortImpl.pfLoadFifo = MGC_HdrcLoadFifo;
          pWrapper->PortImpl.pfUnloadFifo = MGC_HdrcUnloadFifo;
          pWrapper->PortImpl.pfAcceptDevice = func_21cc6d64;
          pWrapper->PortImpl.pfDmaChannelStatusChanged = func_21ccb018;
          pWrapper->PortImpl.pfSetPortTestMode = func_21cca940;
          pWrapper->PortImpl.pfDynamicFifoLocation = func_21cc9530;

         pWrapper->Port.Type = MUSB_PORT_TYPE_OTG; //r9;
         pWrapper->Port.Speed = MUSB_PORT_SPEED_HIGH; //r9;
      }
   }

   if (bOK != 0)
   {
      for (bEnd = 0; bEnd < pWrapper->PortImpl.bEndCount; bEnd++)
      {
         pEnd = MUSB_ArrayFetch(&pWrapper->PortImpl.LocalEnds, bEnd);

         MUSB_ListInit(&pEnd->TxIrpList);
         MUSB_ListInit(&pEnd->RxIrpList);
      }
      pResult = &pWrapper->Controller;
   }

   return pResult;
}


/* 21cc41c4 - complete */
uint32_t MUSB_StartController(MUSB_Controller* pController, /*553*/
        MUSB_SystemServices* pSystemServices) /*555*/
{
   MGC_Controller* pImpl = NULL; /*557*/

   if (pController != NULL)
   {
      pImpl = (MGC_Controller*) pController->pPrivateData;
   }

   if (pImpl != 0)
   {
      if (pImpl->pDmaControllerFactory != 0)
      {
         pImpl->pPort->pDmaController =
            (pImpl->pDmaControllerFactory->pfNewDmaController)(
               pImpl->pPort->pfDmaChannelStatusChanged,
               /*pDmaPrivate*/pImpl->pPort,
               pSystemServices->pfSystemToBusAddress,
               pSystemServices->pPrivateData,
               pImpl->pControllerAddressIsr,
               pImpl->pControllerAddressIst);
      }

      if (pImpl->pPort->pDmaController != 0)
      {
         (pImpl->pPort->pDmaController->pfDmaStartController)(
            pImpl->pPort->pDmaController->pPrivateData);
      }

      pImpl->pSystemServices = pSystemServices;

      return (pImpl->pfProgramStartController)(pImpl);
   }

   return 132;
}




/* 21cc4dc8 - complete */
/* v3.8: func_49c1d4 */
MUSB_Port* MUSB_GetPort(uint16_t index) /*653*/
{
   MUSB_Port* pResult = 0;
   MGC_ControllerWrapper* pWrapper; /*656*/

   if (index < wData_21f7a040)
   {
      pWrapper = MUSB_ListFindItem(&Data_21f7a044, index);
      if (pWrapper != 0)
      {
          pResult = &pWrapper->Port;
      }
   }

   return pResult;
}


