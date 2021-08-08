

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



struct Struct_21bc3690
{
   FAPI_SYS_HandleT usbHandle; //0
   F_DRIVER* Data_4; //4
   int Data_8; //8
   int Data_12; //12
   int Data_16; //16
   int Data_20; //20
   FAPI_SYS_HandleT ataHandle; //24
   uint32_t connectFlag; //28
   int mscLocked; //32
   //36
};


struct Struct_21bc3690* Data_21f0b048; //21f0b048
int Data_21f0b04c; //21f0b04c
int Data_21f0b050; //21f0b050

char Data_21f7e12c[512]; //21f7e12c



void dev_usb_volume_notify(FAPI_USB_VolumeInfoT* infoPtr, uint32_t insertFlag);
void func_21bc374c(void);
void func_21bc3678(void);
int32_t dev_usb_init_msc_function_client(struct Struct_21bc3690*);
int dev_usb_start_driver(struct Struct_21bc3690*);



/* 21bc3aa4 - complete */
int32_t dev_usb_init(void)
{
   struct deviceParams sp20;
   FAPI_USB_OpenParamsT usbParams;
   FAPI_USB_VolumeOpenParamsT volumeParams;
   struct Struct_21b8818c sp56;
   
   int32_t errorCode = 0;
   
   SYS_PrintString("Initializing USB...");
   
   if (Data_21f0b048 != 0)
   {
      FAPI_SYS_PRINT_MSG("ASSERTION FAILED at %s, l.%i\n",
            "sys/src/dev_usb.c", 137);
      rtos_halt(-1);
   }

   do
   {
      Data_21f0b048 = SYS_MemoryAllocate(sizeof(struct Struct_21bc3690));
      
      if (Data_21f0b048 == 0)
      {
         errorCode = -10000002;
         break;
      }

      memset(Data_21f0b048, 0, sizeof(struct Struct_21bc3690));

      memset(&usbParams.desireHostRoleFlag, 0, 4);

      usbParams.version = FAPI_USB_VERSION;
      usbParams.ctrlId = 0;
      usbParams.desireHostRoleFlag = 1;
      usbParams.otgStateNotifyFct = 0;
      usbParams.otgErrorNotifyFct = 0;

      Data_21f0b048->usbHandle = FAPI_USB_Open(&usbParams, &errorCode);

      if (Data_21f0b048->usbHandle == 0)
      {
         break;
      }

      volumeParams.version = FAPI_USB_VOLUME_VERSION;
      volumeParams.volumeId = 0;
      volumeParams.useDMA = 1;
      volumeParams.useRTOS = 1;

      errorCode = f_createdriver(&Data_21f0b048->Data_4,
            USBDRV_Init, (unsigned long)&volumeParams);

      if (errorCode != 0)
      {
         errorCode = -10003002;
         break;
      }

      memset(&sp20, 0, sizeof(sp20));

      sp20.Data_0 = "USB";
      sp20.Data_4 = func_21bc374c;
      sp20.Data_8 = func_21bc3678;
      //sp20.Data_12 = 0;
      //sp20.Data_16 = 0;

      errorCode = device_add(1, &sp20);

      if (errorCode != 0)
      {
         break;
      }

      Data_21f0b048->Data_12 = 0;
      Data_21f0b048->Data_16 = -1;
      Data_21f0b048->Data_20 = 0;
      Data_21f0b048->ataHandle = NULL;
      Data_21f0b048->connectFlag = 0;
      Data_21f0b048->mscLocked = 0;

      if (0 != func_21b8818c(0, &sp56))
      {
         break;
      }

      if (sp56.Data_0 != 0)
      {
         errorCode = dev_usb_init_msc_function_client(Data_21f0b048);
      }
   }
   while (0);

   return errorCode;
}


/* 21bc382c - todo */
void func_21bc382c(void* a)
{
//   printf("func_21bc382c\n");
   
   struct Event_59* r5 = a;
   
   if (Data_21f0b048 != 0)
   {
      //21bc384c
      if (r5->Data_12 == 0)
      {
         //21bc3858
         if (r5->insertFlag != 0)
         {
            //21bc3864
            if ((Data_21f0b048->Data_12 != 0) && 
                  (Data_21f0b048->Data_16 == 1))
            {
               //21bc3a5c
               func_21b88d20(1, 0, 0, 0);
            }
            //21bc387c
            if (r5->Data_16 == 0)
            {
               //21bc3888
               struct Event_59 sp80;
               
               sp80.header.tag = 59;
               sp80.header.size = sizeof(struct Event_59);
               sp80.insertFlag = 1;
               sp80.Data_12 = 0;
               sp80.Data_16 = 1;
                              
               if (0 == func_21b88b2c(0, 1))
               {
                  //21bc38bc
                  if (Data_21f0b048->mscLocked != 0)
                  {
                     //21bc3a6c
                     FAPI_USB_FUNC_MSC_Unlock();
                     
                     Data_21f0b048->mscLocked = 0;
                  }
                  else
                  {
                     //21bc38cc
                     FAPI_SYS_PRINT_MSG("[USB] USB-MSC: access granted but was not locked!\n");
                  }
                  //21bc38e4
                  sp80.Data_16 = 2;
               }
               //21bc38ec
               EVT_Send(&sp80, "USB MSC");
            }
            //->21bc3904
         } //if (r5->Data_8 != 0)
         else
         {
            //21bc3a4c
            func_21b88b2c(0, 0);
            //->21bc3904
         }
      } //if (r5->Data_12 == 0)
      //21bc38fc
      else if (r5->Data_12 == 1)
      {
         //21bc3920
         if (r5->insertFlag != 0)
         {
            //21bc392c
            FAPI_USB_VolumeInfoT* sl;
            
            sl = FAPI_USB_GetVolumeInfo(Data_21f0b048->Data_4->user_ptr);
            
            if (sl != 0)
            {
               //21bc3940
               func_21ba53c0(0xd980, 1);
               
               (Data_21f0b048->Data_4->readsector)(Data_21f0b048->Data_4,
                     Data_21f7e12c, 0);
               
               unsigned r2 = (Data_21f7e12c[457] << 24) |                  
                  (Data_21f7e12c[456] << 16) | 
                  (Data_21f7e12c[455] << 8) | 
                  Data_21f7e12c[454];
               
               (Data_21f0b048->Data_4->readsector)(Data_21f0b048->Data_4,
                     Data_21f7e12c, r2);
               
               Data_21f0b04c = Data_21f7e12c[13];
               Data_21f0b050 = (Data_21f7e12c[12] << 8) | Data_21f7e12c[11];
               
               FAPI_SYS_PRINT_MSG("\n SectorPerCluster_usb = %d / %d ",
                     Data_21f0b04c, 
                     Data_21f0b050);
               //21bc39e8
               
               struct 
               {
                  uint32_t Data_0; //0
                  uint32_t Data_4; //4
                  char Data_8[72]; //8
                  //80
               } sp;
               
               memset(&sp, 0, sizeof(sp));
               
               sp.Data_0 = sl->sectorCount;
               sp.Data_4 = sl->sectorSize;
               
               SYS_StrNCpy(sp.Data_8, (char*) sl->serialNumber, 21);
               
               func_21b88d20(1, 1, Data_21f0b048->Data_4, &sp);
            } //if (sl != 0)
            //21bc3904
         } //if (r5->Data_8 != 0)
         else
         {
            //21bc3a38
            func_21b88d20(1, 0, 0, 0);
            //->21bc3904
         }
      }
      //21bc3904
      memcpy(&Data_21f0b048->Data_12, &r5->insertFlag, 12);
   }
   //21bc3918
}


/* 21bc374c - complete */
void func_21bc374c(void)
{
   dev_usb_start_driver(Data_21f0b048);
   
   Data_21f0b048->Data_8 = 1;
}


/* 21bc3690 - complete */
int dev_usb_start_driver(struct Struct_21bc3690* a)
{
   int res;
   uint8_t peripheralList[50];
   
   if (a->usbHandle == 0)
   {
      return 0xff676596;
   }

   int numBytes = usb_storage_get_filter(peripheralList, sizeof(peripheralList));
      
   res = FAPI_USB_RegHostDriver(a->usbHandle,
         usb_storage_get_driver(), peripheralList, numBytes);

#if 0
   FAPI_SYS_PRINT_MSG("dev_usb_start_driver(1): res = %d\n", res);
#endif
   
   FAPI_USB_SetVolumeNotifyFct(dev_usb_volume_notify);
   
   if (res == 0)
   {
      res = FAPI_USB_Start(a->usbHandle);
      
#if 0
      FAPI_SYS_PRINT_MSG("dev_usb_start_driver(2): res = %d\n", res);
#endif
      
      if ((res != 0) && (res != 0xffff9621))
      {
         FAPI_SYS_PRINT_MSG("[USB] FAPI_USB_Start() failed: %d\n", res);
      }
   }
   
   return 0;
}


/* 21bc3678 - todo */
void func_21bc3678(void)
{
   
}


/* 21bc33a8 - complete */
void dev_usb_msc_notify(uint32_t connectFlag)
{
   struct Event_59 sp;

   if (connectFlag != 0) connectFlag = 1;

   if (Data_21f0b048->connectFlag != connectFlag)
   {
      Data_21f0b048->connectFlag = connectFlag;

      if (connectFlag == 0)
      {
         if (Data_21f0b048->mscLocked == 0)
         {
            if (FAPI_USB_FUNC_MSC_Lock())
            {
               Data_21f0b048->mscLocked = 1;
            }
            else
            {
               FAPI_SYS_PRINT_MSG("[USB] FAPI_USB_FUNC_MSC_Lock failed!\n");
            }
         }
      }

      sp.header.tag = 59;
      sp.header.size = 20;
      sp.insertFlag = connectFlag;
      sp.Data_12 = 0;
      sp.Data_16 = 0;

      EVT_Send(&sp, "USB MSC");
   }
}


/* 21bc34a0 - complete */
int32_t dev_usb_init_msc_function_client(struct Struct_21bc3690* a)
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

      Data_21f0b048->mscLocked = 1;

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
void dev_usb_volume_notify(FAPI_USB_VolumeInfoT* infoPtr, uint32_t insertFlag)
{
//   FAPI_SYS_PRINT_MSG("dev_usb_volume_notify\n");
   
   struct Event_59 sp;
   
   sp.header.tag = 59;
   sp.header.size = sizeof(struct Event_59);
   sp.insertFlag = insertFlag;
   sp.Data_12 = 1;
   sp.Data_16 = 0;
   
   EVT_Send(&sp, "USB");
}


/* 21bc311c - todo */
void func_21bc311c(void)
{
   
}





