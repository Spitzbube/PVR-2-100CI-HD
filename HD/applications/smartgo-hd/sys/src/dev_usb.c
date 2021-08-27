

#include <string.h>
#include <fapi/sys_services.h>
#include <fapi/drv_ata.h>
#include <fapi/drv_usb.h>
#include <fapi/drv_usb_volume.h>
#include <fapi/drv_usb_func_msc.h>
#include <fapi/drv_dma.h>
#if 0
#include "rtos.h"
#endif
#include "sys.h"
#include "sys_api.h"
#include "sys_priv.h"
#include "device.h"
#if 0
#include "fapi_adapter.h"
#include "fapi_usb.h"
#endif

#include "fatfs/api_f.h"
#include "fatfs/usbdrv.h"

#include "event_queue.h"
#include "musb/list_49cd8c.h"
#include "musb/usb_ch9.h"
#include "musb/musbdefs.h"
#include "musb/musb_dma.h"
#include "musb/musb.h"
//#include "usb_storage.h"
#include "dev_usb.h"



typedef struct
{
   FAPI_SYS_HandleT usbHandle; //0
   F_DRIVER* usbFsHandle; //4
   int running; //8
#if 1
   USB_CONNECT_STATUS_S  currStatus;     //!< Current status.
#else
   int Data_12; //12
   int Data_16; //16
   int Data_20; //20
#endif
   FAPI_SYS_HandleT ataHandle; //24
   uint32_t connectFlag; //28
   int mscLocked; //32
   //36
} USB_DATA_S;


static USB_DATA_S* usbDat = NULL; //21f0b048
int Data_21f0b04c; //21f0b04c
int Data_21f0b050; //21f0b050

char Data_21f7e12c[512]; //21f7e12c



void USB_MediaNotify(FAPI_USB_VolumeInfoT* infoPtr, uint32_t insertFlag);
void USB_Wakeup(void);
void USB_Shutdown(void);
int32_t dev_usb_init_msc_function_client(USB_DATA_S*);
int USB_Start(USB_DATA_S*);



/* 21bc3aa4 - complete */
int32_t dev_usb_init(void)
{
   /*struct deviceParams*/SYS_DEV_CFG_S devCfg;
   FAPI_USB_OpenParamsT usbParams;
   FAPI_USB_VolumeOpenParamsT volumeParams;
   struct Struct_21b8818c hddStatus; //sp56;
   
   int32_t errorCode = 0;
   
   SYS_PrintString("Initializing USB...");
   
   if (usbDat != 0)
   {
      FAPI_SYS_PRINT_MSG("ASSERTION FAILED at %s, l.%i\n",
            "sys/src/dev_usb.c", 137);
      rtos_halt(-1);
   }

   do
   {
      usbDat = SYS_MemoryAllocate(sizeof(USB_DATA_S));
      
      if (usbDat == 0)
      {
         errorCode = -10000002;
         break;
      }

      memset(usbDat, 0, sizeof(USB_DATA_S));

      memset(&usbParams.desireHostRoleFlag, 0, 4);

      usbParams.version = FAPI_USB_VERSION;
      usbParams.ctrlId = 0;
      usbParams.desireHostRoleFlag = 1;
      usbParams.otgStateNotifyFct = 0;
      usbParams.otgErrorNotifyFct = 0;

      usbDat->usbHandle = FAPI_USB_Open(&usbParams, &errorCode);

      if (usbDat->usbHandle == 0)
      {
         break;
      }

      volumeParams.version = FAPI_USB_VOLUME_VERSION;
      volumeParams.volumeId = 0;
      volumeParams.useDMA = 1;
      volumeParams.useRTOS = 1;

      errorCode = f_createdriver(&usbDat->usbFsHandle,
            USBDRV_Init, (unsigned long)&volumeParams);

      if (errorCode != 0)
      {
         errorCode = -10003002;
         break;
      }

      memset(&devCfg, 0, sizeof(devCfg));

      devCfg.devName = "USB";
      devCfg.wakeup = USB_Wakeup;
      devCfg.shutdown = USB_Shutdown;
      //devCfg.Data_12 = 0;
      //devCfg.Data_16 = 0;

      errorCode = device_add(1, &devCfg);

      if (errorCode != 0)
      {
         break;
      }

      usbDat->currStatus.connected = 0;
      usbDat->currStatus.devType = -1;
      usbDat->currStatus.accessStatus = 0;

#if 1//def APPL_USB_FUNC_MSC_ENABLED

      usbDat->ataHandle = NULL;
      usbDat->connectFlag = 0;
      usbDat->mscLocked = 0;

#if 0
      /* Setup USB function mode if HDD device is available */
      if (0 != SYS_DeviceGetStatus(0, &hddStatus))
      {
         break;
      }

      if (hddStatus.Data_0 != 0)
#else
          USB_Wakeup();
#endif
      {
         errorCode = dev_usb_init_msc_function_client(usbDat);
      }

#endif /* APPL_USB_FUNC_MSC_ENABLED */

   }
   while (0);

   return errorCode;
}


/* 21bc382c - todo */
void USB_ConnectStatusHandling(void* a)
{
    SYS_DEVICE_PARAMS_S devParams; //sp;

    FAPI_USB_VolumeInfoT* pInfoUSB;

    SYS_EVT_USB_CONNECT_S ctrlEvent; //sp80;

    SYS_EVT_USB_CONNECT_S* pEvent = a;

   //   printf("USB_ConnectStatusHandling\n");

   if (usbDat == 0)
   {
       return;
   }

    //21bc384c
    switch (pEvent->status.devType)
    {
    case 0: //USB_DEVTYPE_HOST
        //21bc3858
        if (pEvent->status.connected != 0)
        {
            //21bc3864
            if ((usbDat->currStatus.connected != 0) &&
                  (usbDat->currStatus.devType == USB_DEVTYPE_FUNCTION/*1*/))
            {
               //21bc3a5c
               SYS_DeviceSetAvailability(1, 0, 0, 0);
            }
            //21bc387c
            if (pEvent->status.accessStatus == 0)
            {
               //21bc3888
               ctrlEvent.hdr.type = EVT_USB_CONNECT_STATUS; //59;
               ctrlEvent.hdr.length = sizeof(ctrlEvent);
               ctrlEvent.status.connected = 1;
               ctrlEvent.status.devType = USB_DEVTYPE_HOST; //0;
               ctrlEvent.status.accessStatus = USB_MSC_ACCESS_DENIED; //1;

               if (0 == SYS_DeviceSetExternalAccess(0, 1))
               {
                  //21bc38bc
                  if (usbDat->mscLocked != 0)
                  {
                     //21bc3a6c
                     FAPI_USB_FUNC_MSC_Unlock();

                     usbDat->mscLocked = 0;
                  }
                  else
                  {
                     //21bc38cc
                     FAPI_SYS_PRINT_MSG("[USB] USB-MSC: access granted but was not locked!\n");
                  }
                  //21bc38e4
                  ctrlEvent.status.accessStatus = USB_MSC_ACCESS_GRANTED; //2;
               }
               //21bc38ec
               EVT_Send(&ctrlEvent, "USB MSC");
            }
            //->21bc3904
        } //if (pEvent->Data_8 != 0)
        else
        {
            //21bc3a4c
            SYS_DeviceSetExternalAccess(0, 0);
            //->21bc3904
        }
        break;

    case 1: //USB_DEVTYPE_FUNCTION
        //21bc3920
        if (pEvent->status.connected != 0)
        {
            //21bc392c
            pInfoUSB = FAPI_USB_GetVolumeInfo(usbDat->usbFsHandle->user_ptr);

            if (pInfoUSB != 0)
            {
               //21bc3940
               func_21ba53c0(0xd980, 1);

               (usbDat->usbFsHandle->readsector)(usbDat->usbFsHandle,
                     Data_21f7e12c, 0);

               unsigned r2 = (Data_21f7e12c[455] << 8) |
                  (Data_21f7e12c[456] << 16) |
                  (Data_21f7e12c[457] << 24) |
                  Data_21f7e12c[454];

               (usbDat->usbFsHandle->readsector)(usbDat->usbFsHandle,
                     Data_21f7e12c, r2);

               Data_21f0b04c = Data_21f7e12c[13];
               Data_21f0b050 = (Data_21f7e12c[12] << 8) + Data_21f7e12c[11];

               FAPI_SYS_PRINT_MSG("\n SectorPerCluster_usb = %d / %d ",
                     Data_21f0b04c,
                     Data_21f0b050);
               //21bc39e8
               memset(&devParams, 0, sizeof(devParams));

               devParams.sectorCount = pInfoUSB->sectorCount;
               devParams.sectorSize = pInfoUSB->sectorSize;

               SYS_StrNCpy(devParams.serialNumber,
                       (char*) pInfoUSB->serialNumber,
                       SYS_DEV_SERIALNUM_LEN/*21*/);

               SYS_DeviceSetAvailability(1, 1, usbDat->usbFsHandle, &devParams);
            } //if (sl != 0)
            //21bc3904
        } //if (pEvent->Data_8 != 0)
        else
        {
            //21bc3a38
            SYS_DeviceSetAvailability(1, 0, 0, 0);
            //->21bc3904
        }
        break;

    default:
        break;
    }

    //21bc3904
    memcpy(&usbDat->currStatus, &pEvent->status, 12);
}


/* 21bc374c - complete */
void USB_Wakeup(void)
{
//    FAPI_SYS_PRINT_DEBUG(4,"[USB] Wakeup...\n");

   USB_Start(usbDat);
   
   usbDat->running = 1;
}


/* 21bc3690 - complete */
int USB_Start(USB_DATA_S* a)
{
   int res;
   uint8_t peripheralList[50];
   
   if (a->usbHandle == 0)
   {
      return 0xff676596;
   }

   int numBytes = MGC_FillStorageClassPeripheralList(peripheralList, sizeof(peripheralList));
      
   res = FAPI_USB_RegHostDriver(a->usbHandle,
         MUSB_GetStorageClassDriver(), peripheralList, numBytes);

#if 0
   FAPI_SYS_PRINT_MSG("USB_Start(1): res = %d\n", res);
#endif
   
   FAPI_USB_SetVolumeNotifyFct(USB_MediaNotify);
   
   if (res == 0)
   {
      res = FAPI_USB_Start(a->usbHandle);
      
#if 0
      FAPI_SYS_PRINT_MSG("USB_Start(2): res = %d\n", res);
#endif
      
      if ((res != 0) && (res != 0xffff9621))
      {
         FAPI_SYS_PRINT_MSG("[USB] FAPI_USB_Start() failed: %d\n", res);
      }
   }
   
   return 0;
}


/* 21bc3678 - todo */
void USB_Shutdown(void)
{
    usbDat->running = 0;

//    FAPI_SYS_PRINT_DEBUG(4,"[USB] Shutdown...\n");

#if 0 //TODO
    (void)USB_Stop(usbDat); //21bc364c
#endif
}


/* 21bc33a8 - complete */
void dev_usb_msc_notify(uint32_t connectFlag)
{
   struct Event_59 sp;

   if (connectFlag != 0) connectFlag = 1;

   if (usbDat->connectFlag != connectFlag)
   {
      usbDat->connectFlag = connectFlag;

      if (connectFlag == 0)
      {
         if (usbDat->mscLocked == 0)
         {
            if (FAPI_USB_FUNC_MSC_Lock())
            {
               usbDat->mscLocked = 1;
            }
            else
            {
               FAPI_SYS_PRINT_MSG("[USB] FAPI_USB_FUNC_MSC_Lock failed!\n");
            }
         }
      }

      sp.header.tag = EVT_USB_CONNECT_STATUS; //59;
      sp.header.size = 20;
      sp.status.connected = connectFlag;
      sp.status.devType = 0;
      sp.status.accessStatus = 0;

      EVT_Send(&sp, "USB MSC");
   }
}


/* 21bc34a0 - complete */
int32_t dev_usb_init_msc_function_client(USB_DATA_S* a)
{
   int32_t errorCode;
   FAPI_USB_FUNC_MSC_InitParamsT mscInitParams;
   FAPI_ATA_OpenParamsT ataParams;
   FAPI_USB_FUNC_MSC_LunT mscLun;
   FAPI_ATA_DeviceInfoT ataDeviceInfo;

   if (a->ataHandle != NULL)
   {
      return -3;
   }

   errorCode = 0;

   memset(&ataParams, 0, sizeof(FAPI_ATA_OpenParamsT));

   ataParams.version = FAPI_ATA_VERSION;
   ataParams.type = 0;
   ataParams.channel = 0;
   ataParams.sectorBytes = FAPI_ATA_SECTOR_BYTES;
   ataParams.use48BitLba = 1;
   ataParams.waitTimeout = 15000;

   do
   {
      a->ataHandle = FAPI_ATA_Open(&ataParams, &errorCode);

      if (a->ataHandle == NULL)
      {
         break;
      }

      memset(&ataDeviceInfo, 0, sizeof(ataDeviceInfo));

      errorCode = FAPI_ATA_GetDeviceInfo(a->ataHandle, &ataDeviceInfo);

      if (errorCode != 0)
      {
         break;
      }

      if (ataDeviceInfo.lbaCapacity == 0)
      {
         errorCode = -10001009;
         break;
      }

      mscLun.type = FAPI_USB_FUNC_MSC_LUN_HDD;
      mscLun.namePtr = "MB86H60 HDD 1";
      mscLun.startAddrOrLba = 0;
      mscLun.lbaCapacity = ataDeviceInfo.lbaCapacity;
      mscLun.blockLength = FAPI_ATA_SECTOR_BYTES;
      mscLun.readOnly = 0;
      mscLun.ataHandle = a->ataHandle;

      memset(&mscInitParams, 0, sizeof(mscInitParams));

#if 0
      mscInitParams.vendorID = 0;
      mscInitParams.productID = 0;
      mscInitParams.vendorNamePtr = 0;
      mscInitParams.productNamePtr = 0;
      mscInitParams.serialNumberPtr = 0;
#endif
      mscInitParams.lunCount = 1;
      mscInitParams.lunList = &mscLun;

      errorCode = FAPI_USB_FUNC_MSC_Init(&mscInitParams);

      if (errorCode != 0)
      {
         break;
      }

      errorCode = FAPI_USB_RegFuncDriver(a->usbHandle,
            FAPI_USB_FUNC_MSC_GetFunctionClient());

      if (errorCode != 0)
      {
         break;
      }

      if (1 != FAPI_USB_FUNC_MSC_Lock())
      {
         errorCode = -10001002;
         break;
      }

      usbDat->mscLocked = 1;

      FAPI_USB_FUNC_MSC_SetHostNotifyFct(dev_usb_msc_notify);
   }
   while (0);

   if (errorCode != 0)
   {
      if (a->ataHandle != NULL)
      {
         FAPI_ATA_Close(a->ataHandle);

         a->ataHandle = NULL;
      }
   }

   return errorCode;
}


/* 21bc345c - complete */
void USB_MediaNotify(FAPI_USB_VolumeInfoT* infoPtr, uint32_t insertFlag)
{
//   FAPI_SYS_PRINT_MSG("USB_MediaNotify\n");
   
   struct Event_59 ctrlEvent;
   
   ctrlEvent.header.tag = EVT_USB_CONNECT_STATUS; //59;
   ctrlEvent.header.size = sizeof(struct Event_59);
   ctrlEvent.status.connected = insertFlag;
   ctrlEvent.status.devType = 1;
   ctrlEvent.status.accessStatus = 0;
   
   EVT_Send(&ctrlEvent, "USB");
}


/* 21bc311c - todo */
void func_21bc311c(void)
{
   
}





