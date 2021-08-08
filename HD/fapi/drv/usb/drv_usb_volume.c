

#include <string.h>
#include <fapi/sys_services.h>
#include <fapi/drv_timer.h>
#include <fapi/drv_usb_volume.h>
#include <musb/mu_hfi.h>


struct VolumeHandle
{
   int magic; //0
   char inUse: 1; //4
   char Data_4_1: 1; //4
   char Data_4_2: 1; //4
   char useDMA: 1; //4
   char useRTOS: 1; //4
   struct
   {
      int Data_0; //0
      int fill_4[3]; //4
      int (*Data_16)(int,int,int,unsigned short,int,int,int,int);
   }* Data_8; //8
#if 1
   int volumeId; //12
   int isConnected; //16
   int isRemovable; //20
   int busAddress; //24
   int sectorSize; //28
   int sectorCount; //32
   char serialNumber[32]; //36
   char diskVendor[16]; //68
   char diskProduct[32]; //84
   char diskRevision[8]; //116
#else
   FAPI_USB_VolumeInfoT Data_12; //12
#endif
   FAPI_SYS_SemaphoreT Data_124; //124
   FAPI_SYS_SemaphoreT Data_128; //128
   int Data_132; //132
   struct VolumeHandle* Data_136; //136
   //140
};



int FAPI_USB_VolumeInit(void);



/*static*/ int usbVolumeInitialized = 0; //21f6661c
/*static*/ FAPI_SYS_SemaphoreT usbVolumeSemaphore = 0; //21f66620
FAPI_USB_VolumeNotifyFuncT usbVolumeNotifyFunc; //21f66624
struct VolumeHandle* Data_21f66628; //21f66628
/*static*/ struct VolumeHandle usbVolumeHandleArray[8]; //21f6662c


static inline int usbVolumeCheckHandle(struct VolumeHandle* a)
{
   int res = 0;
   
   if ((a != 0) &&
         (a->magic == 0x75766f6c) &&
         (a->inUse != 0))
   {
      res = 1;
   }
   
   return res;
}


/* 21c3fb34 - complete */
void FAPI_USB_SetVolumeNotifyFct(FAPI_USB_VolumeNotifyFuncT notifyFct)
{
   usbVolumeNotifyFunc = notifyFct;
}


/* 21c402a8 - nearly complete */
/* v3.8: 3c3130 - todo */
int func_21c402a8(void** r8, struct Struct_21c402a8* r5, void* r7)
{
//   FAPI_SYS_PRINT_MSG("func_21c402a8\n");
   
   unsigned i;
   unsigned short* ip;
   struct VolumeHandle* h = 0;
   
   *r8 = 0;
   
   if (usbVolumeInitialized == 0)
   {
      if (0 != FAPI_USB_VolumeInit())
      {
         return 8; 
      }
   }
   
   for (i = 0; i < 8; i++)
   {
      if (usbVolumeHandleArray[i].Data_8 == 0)
      {
         h = &usbVolumeHandleArray[i];
         break;
      }
   }
   
   if (h == 0)
   {
      return 8;
   }
   
   while (1)
   {
      if (FAPI_SYS_GET_SEMAPHORE(h->Data_124, 0) < 0) 
      {
         break;
      }
   }
   
   h->Data_8 = r7;
   h->isRemovable = (r5->Data_0 == 2)? 1: 0;
   h->isConnected = 1;
   h->busAddress = r5->bDeviceAddress;
   h->sectorSize = r5->dwBlockLength;
   
   ip = r5->arData_92;
   
   for (i = 0; (*ip != 0) && (i < 31); i++)
   {
      h->serialNumber[i] = *ip++;
   }

   h->serialNumber[i] = 0;
  
   ip = r5->vendor;
   
   for (i = 0; (*ip != 0) && (i < 15); i++)
   {
      h->diskVendor[i] = *ip++;
   }

   h->diskVendor[i] = 0;
   
   ip = r5->product;

   for (i = 0; (*ip != 0) && (i < 31); i++)
   {
      h->diskProduct[i] = *ip++;
   }

   h->diskProduct[i] = 0;
   
   ip = r5->revision;
   
   for (i = 0; (*ip != 0) && (i < 7); i++)
   {
      h->diskRevision[i] = *ip++;
   }

   h->diskRevision[i] = 0;

#if 1
   FAPI_SYS_PRINT_MSG("func_21c402a8: serialNumber='%s' diskVendor='%s' diskProduct='%s' diskRevision='%s'\n",
         h->serialNumber, 
         h->diskVendor, 
         h->diskProduct, 
         h->diskRevision);
#endif
   
   if ((Data_21f66628 != 0) &&
         (Data_21f66628->busAddress == h->busAddress))
   {
      h->Data_136 = Data_21f66628;
   }
   else
   {
      Data_21f66628 = h;
   }

   *r8 = h;

#if 1
   FAPI_SYS_PRINT_MSG("func_21c402a8: r5->Data_0=%d r5->Data_8=%d r5->Data_12=%d r5->Data_16=%d\n",
         r5->Data_0, 
         r5->Data_8, 
         r5->Data_12, 
         r5->Data_16);
#endif
   
   if ((r5->Data_0 == 1) &&
         (r5->Data_8 != 0))
   {
      if ((r5->Data_12 != 0) ||
            (r5->Data_16 != 0))
      {
         h->sectorSize = r5->Data_8;
               
         if (r5->Data_16 != 0)
         {
            h->sectorCount = -1;
         }
         else
         {
            h->sectorCount = r5->Data_12;
         }
         
         h->Data_4_1 = 1;
         h->Data_4_2 = 0;
         
         if (usbVolumeNotifyFunc != 0)
         {
            (usbVolumeNotifyFunc)(&h->volumeId, 1);
         }
      }
   }

   return 0;
}


/* 21c401a4 - nearly complete */
FAPI_SYS_HandleT FAPI_USB_OpenVolume(
                  const FAPI_USB_VolumeOpenParamsT* paramsPtr,
                  int32_t* errorCodePtr)
{
   struct VolumeHandle* h;
   int res = 0;

   if (usbVolumeInitialized == 0)
   {
      res = FAPI_USB_VolumeInit();
   }

   if (paramsPtr == 0)
   {
      res = -27001; //0xffff9687;
   }
   else
   {
      if (paramsPtr->version > FAPI_USB_VOLUME_VERSION)
      {
         res = -27010; //0xffff967e;
      }

      if (paramsPtr->volumeId > FAPI_USB_MAX_VOLUME_ID)
      {
         res = -27100; //0xffff9624;
      }
   }
   if (res != 0)
   {
      if (errorCodePtr != 0)
      {
         *errorCodePtr = res;
      }
      return 0;
   }

   h = &usbVolumeHandleArray[paramsPtr->volumeId];

   if (h->inUse == 0)
   {
      h->magic = 0x75766f6c; //uvol
      h->inUse = 1;
      h->useDMA = paramsPtr->useDMA;
      h->useRTOS = paramsPtr->useRTOS;
   }

   if (errorCodePtr != 0)
   {
      *errorCodePtr = res;
   }

   return h;
}


/* 21c3fb44 - todo */
int32_t FAPI_USB_CloseVolume (FAPI_SYS_HandleT handle)
{   
   //printf("FAPI_USB_CloseVolume\n");
   
   int res;
   struct VolumeHandle* h = handle;
   
   if (0 != usbVolumeCheckHandle(h))
   {
      res = 0;

      h->inUse = 0;
      h->useDMA = 0;
      h->useRTOS = 0;
      h->magic = 0;      
   }
   else
   {
      res = -27008;
   }
   
   return res;
}


/* 21c400dc - complete */
int FAPI_USB_VolumeInit(void)
{
   unsigned i;

   if (usbVolumeSemaphore == 0)
   {
      usbVolumeSemaphore = FAPI_SYS_CREATE_SEMAPHORE(1);
   }

   for (i = 0; i < 8; i++)
   {
      memset(&usbVolumeHandleArray[i], 0, sizeof(struct VolumeHandle));

      usbVolumeHandleArray[i].volumeId = i;
      usbVolumeHandleArray[i].Data_124 = FAPI_SYS_CREATE_SEMAPHORE(0);
      usbVolumeHandleArray[i].Data_128 = usbVolumeSemaphore;

      if ((usbVolumeHandleArray[i].Data_124 == 0) ||
            (usbVolumeHandleArray[i].Data_128 == 0))
      {
         return -27009; 
      }
   }

   usbVolumeInitialized = 1;

   return 0;
}


/* 21c3fec8 - todo */
uint32_t FAPI_USB_ReadVolume(FAPI_SYS_HandleT handle,
                             void*            dataPtr,
                             uint32_t         lba,
                             uint32_t         lbaCnt)
{
   printf("FAPI_USB_ReadVolume\n");
   
   
   return 0;
}


/* 21c3fcb8 - todo */
uint32_t FAPI_USB_WriteVolume(FAPI_SYS_HandleT handle,
                              void*            dataPtr,
                              uint32_t         lba,
                              uint32_t         lbaCnt)
{
   int r4 = 0;
   struct VolumeHandle* r5 = handle;

#if 0
   if (r5 == 0) return 0;
   if (r5->magic != 0x75766f6c) return 0; //uvol
   if (r5->inUse == 0) return 0;
#else
   if (0 == usbVolumeCheckHandle(r5))
   {
      return 0;
   }
#endif

   FAPI_SYS_GET_SEMAPHORE((r5->Data_136 != 0)?
         r5->Data_136->Data_128:
         r5->Data_128, -1);

   r5->Data_132 = 0;

   if ((r5->Data_8 == 0) ||
         (r5->Data_4_1 == 0))
   {
      //21c3fd48
      //r4 = 0;

      FAPI_SYS_SET_SEMAPHORE((r5->Data_136 != 0)?
            r5->Data_136->Data_128:
            r5->Data_128, 0);

      return r4;
   }
   //21c3fd74
   r5->Data_4_2 = 0;

   r4 = (r5->Data_8->Data_16)(r5->Data_8->Data_0,
         lba, 0, lbaCnt, dataPtr, 0, 0x21c3f97c, r5->useDMA);
   if (r4 != 0)
   {
      r4 = 0;

      FAPI_SYS_SET_SEMAPHORE((r5->Data_136 != 0)?
            r5->Data_136->Data_128:
            r5->Data_128, 0);

      return r4;
   }

   if (r5->useRTOS != 0)
   {
      if (FAPI_SYS_Services.getSemaphoreFunc != 0)
      {
         if (-1 != (FAPI_SYS_Services.getSemaphoreFunc)(r5->Data_124, 8000))
         {
            r4 = r5->Data_132;
            //->21c3fd4c
            FAPI_SYS_SET_SEMAPHORE((r5->Data_136 != 0)?
                  r5->Data_136->Data_128:
                  r5->Data_128, 0);

            return r4;
         }
      }

      r5->Data_132 = 0;

      r5->Data_4_2 = 1;

      r4 = r5->Data_132;
      //->21c3fd4c
      FAPI_SYS_SET_SEMAPHORE((r5->Data_136 != 0)?
            r5->Data_136->Data_128:
            r5->Data_128, 0);

      return r4;
   }
   //21c3fe00
   unsigned long long r6_r7 = FAPI_TIMER_GetTimeStamp(1000);
   //21c3fe10
   while (r5->Data_4_1 != 0)
   {
      if (FAPI_SYS_Services.getSemaphoreFunc != 0)
      {
         if (0 <= (FAPI_SYS_Services.getSemaphoreFunc)(r5->Data_124, 0))
         {
            //->21c3fe88
            break;
         }
      }
      //21c3fe3c
      FAPI_USB_Bsr0(0);
      FAPI_TIMER_Bsr0(1);
      FAPI_TIMER_Bsr1(1);
      FAPI_TIMER_Bsr2(1);

      if ((FAPI_TIMER_GetTimeStamp(1000) - r6_r7) > 8000)
      {
         r5->Data_4_2 = 1;
         r5->Data_132 = 0;
         break;
      }
   }
   //21c3fe88
   if (r5->Data_4_1 == 0)
   {
      //21c3fe94
      r5->Data_4_2 = 1;
      r5->Data_132 = 0;
   }
   //->21c3fde0
   r4 = r5->Data_132;

   FAPI_SYS_SET_SEMAPHORE((r5->Data_136 != 0)?
         r5->Data_136->Data_128:
         r5->Data_128, 0);

   return r4;
}


/* 21c3fa04 - complete */
/* v3.8: 3c32f8 - complete */
void func_21c3fa04(void/*Struct_b75bb8*/* a, MUSB_HfiMediumInfo* r5)
{
//   FAPI_SYS_PRINT_MSG("func_21c3fa04\n", a);
   
   struct VolumeHandle* h = a;
   
   if (h != 0)
   {
      while (1)
      {
         if (FAPI_SYS_GET_SEMAPHORE(h->Data_124, 0) < 0)
         {
            break;
         }
      }

      h->sectorSize = r5->dwBlockSize;
      
      if (r5->dwBlockCountHi != 0)
      {
         h->sectorCount = -1;
      }
      else
      {
         h->sectorCount = r5->dwBlockCountLo;
      }
      
      h->Data_4_1 = 1;
      h->Data_4_2 = 0;
      
      if (usbVolumeNotifyFunc != 0)
      {
         (usbVolumeNotifyFunc)(&h->volumeId, 1);
      }
   }
}


/* 21c3fa94 - todo */
/* v3.8: 3c3388 - todo */
int func_21c3fa94(/*Struct_b75bb8* a*/)
{
   FAPI_SYS_PRINT_MSG("func_21c3fa94\n");

   return 0;
}


/* 21c3fba4 - todo */
FAPI_USB_VolumeInfoT* FAPI_USB_GetVolumeInfo(FAPI_SYS_HandleT handle)
{
//   FAPI_SYS_PRINT_MSG("FAPI_USB_GetVolumeInfo\n");
   
   struct VolumeHandle* h = handle;
   FAPI_USB_VolumeInfoT* r0 = 0;
   
   if (0 != usbVolumeCheckHandle(h))
   {
      r0 = &h->volumeId;
   }

   return r0;
}




