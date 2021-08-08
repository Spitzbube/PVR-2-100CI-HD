

#include <string.h>
#include <fapi/sys_services.h>
#include <fapi/reg_dma.h>
#include <fapi/drv_timer.h>
#include <fapi/drv_usb.h>
#include <fapi/drv_dma.h>

#if 0
#include "fapi_usb.h"
#endif
#include "musb/list_49cd8c.h"
#if 0
#include "usb_ch9.h"
#endif
#include "musb/musbdefs.h"
#include "musb/musb_dma.h"
#include "musb/musb.h"

struct fapi_usb_handle;



static int func_21c3d2cc(void);
int fapi_usb_check_handle(struct fapi_usb_handle* a);
int func_21c3e31c(struct fapi_usb_handle* a);
int func_21c3d22c(struct fapi_usb_handle* a);
//char* func_21c3d2d4(void*, char*);
uint32_t func_21c3d2d4(void* a, uint32_t b);
int fapi_usb_dequeue_message_item(void*, void*);
int fapi_usb_queue_message_item(void*, void*);
void fapi_usb_clear_message_queue(void*);
int fapi_usb_create_timer(void*, unsigned short, int, unsigned char, 
      void (*)(Struct_49d2fc*, unsigned short, MGC_Timer*));
int fapi_usb_delete_timer(void*, unsigned short);
int fapi_usb_lock(void*, unsigned short);
int fapi_usb_unlock(void*, unsigned short);
int fapi_usb_print(void*, char*);
static void usbTimerFunction(uint64_t, uint32_t);
void func_21c3d3ac(void* a, struct MGC* b, int c);
void func_21c3d3ec(void* a, void* b, int c);
static struct fapi_usb_handle* fapi_usb_get_handle(uint32_t);


FAPI_SYS_DriverT FAPI_USB_Driver0 = //21efc658 
{
      "USB0",
      21,
      1,
      FAPI_USB_Init,
      FAPI_USB_Exit,
      FAPI_USB_Isr0,
      FAPI_USB_Bsr0,
      0x1000,
      0x100,
};
int Data_21efc67c[1] = {0xc7000800}; //21efc67c
MUSB_SystemUtils Data_21efc680 = //21efc680
{
      1, 0, 0, 0
};


int Data_21f6652c; //21f6652c
static FAPI_SYS_SemaphoreT usbSemaphore = 0; //21f66530
static int usbInitialized = 0; //21f66534
int Data_21f66538; //21f66538
FAPI_SYS_SemaphoreT Data_21f6653c[1]; //21f6653c
struct fapi_usb_handle
{
   int magic; //0
   unsigned Data_4; //4
   uint32_t index; //8
   char bData_12; //12
   FAPI_USB_OtgStateFuncT otgStateNotifyFct; //16
   FAPI_USB_OtgErrorFuncT otgErrorNotifyFct; //20
   MUSB_SystemServices Data_24; //24
   MUSB_Controller* Data_76; //76
   MUSB_Port* Data_80; //80
   MUSB_OtgClient otgClient; //84 +16
   MUSB_FunctionClient functionDriver; //100 +44
   MUSB_HostClient deviceDriverList; //144 +16
   void* Data_160; //160
   struct MUSB_DeviceDriverCandidate* deviceDriverCandidates; //164
   unsigned char numDeviceDriverCandidates; //168
   FAPI_SYS_SemaphoreT* locks; //172
   unsigned numLocks; //176
   char* queue; //180
   int queueItemSize; //184
   int queueLen; //188
   unsigned queueWritePtr; //192
   unsigned queueReadPtr; //196
   int Data_200; //200
   FAPI_SYS_HandleT Data_204; //204
   unsigned Data_208; //208
   MGC_Timer* timers; //212
   unsigned numTimer; //216
   //220
} usbHandleArray[1]; //21f66540 ->21F6661C



extern int Data_21f7a064; //21f7a064


extern char Data_2207f32c[]; //2207f32c
extern char Data_2207f42c[]; //2207f42c



/* 21c3f8a0 - todo */
void func_21c3f8a0(void)
{
   printf("func_21c3f8a0\n");
}


/* 21c3f858 - todo */
void func_21c3f858(void)
{
   printf("func_21c3f858\n");
}


/* 21c3f7a4 - complete */
uint32_t FAPI_USB_Isr0(void)
{
   int res;
   
   if (0 != FREG_DMA_GetUsbIntStatus_UsbIrq())
   {
      if (Data_21f7a064 != 0)
      {
         func_21cc8328("ISR0 start");         
      }

      res = (usbHandleArray[0].Data_76->pfIsr)(usbHandleArray[0].Data_76->pIsrParam);
      
      if (Data_21f7a064 != 0)
      {
         func_21cc8328("ISR0 end");               
      }

      FREG_DMA_SetUsbIntClear_ClearUsbIrq(1);
      
      if ((res > 0) && (usbHandleArray[0].Data_200 != 0))
      {
         return 1;
      }      
   }
   else
   {
      FREG_DMA_SetUsbIntClear(FREG_DMA_GetUsbIntStatus());      
   }

   return 0;
}


static inline int func_21c3f4cc(struct fapi_usb_handle* r7)
{   
   unsigned char i, j;
   MUSB_FunctionClient* pFunctionDriver = 0;
   
   if (r7->functionDriver.pStandardDescriptors != 0)
   {
      pFunctionDriver = &r7->functionDriver;
   }
   
   r7->otgClient.pPrivateData = r7;
   r7->otgClient.pbDesireHostRole = &r7->bData_12;
   r7->otgClient.pfOtgState = func_21c3d3ac;
   r7->otgClient.pfOtgError = func_21c3d3ec;
   
   if (r7->numDeviceDriverCandidates != 0)
   {
      //21c3f504
      struct MUSB_DeviceDriverCandidate* r2_;
      int r4_ = 0; 
      for (r2_ = r7->deviceDriverCandidates; r2_ != 0; r2_ = r2_->next)
      {
         //21c3f518
         r4_ += (r2_->listLength + 1);      
      }
      //21c3f530
      r7->deviceDriverList.aDeviceDriverList =
         FAPI_SYS_MALLOC(r7->numDeviceDriverCandidates * sizeof(MUSB_DeviceDriver));

      if (r7->deviceDriverList.aDeviceDriverList == 0)
      {
         //21c3f2d4
         return 0;
      }
      //21c3f554
      r7->deviceDriverList.bDeviceDriverListLength = r7->numDeviceDriverCandidates;
      
      r7->deviceDriverList.pPeripheralList = FAPI_SYS_MALLOC(r4_);
            
      if (r7->deviceDriverList.pPeripheralList == 0)
      {
         //21c3f730
         FAPI_SYS_FREE(r7->deviceDriverList.aDeviceDriverList);
         //21c3f2d4
         return 0;
      }
      //21c3f580
      r7->deviceDriverList.wPeripheralListLength = r4_;
      
      char* pString = r7->deviceDriverList.pPeripheralList;
      struct MUSB_DeviceDriverCandidate* drivers = r7->deviceDriverCandidates; 
      
      for (i = 0; (i < r7->numDeviceDriverCandidates) && (drivers != 0); drivers = drivers->next, i++)
      {
         //21c3f5a0
         memcpy(&r7->deviceDriverList.aDeviceDriverList[i],
               &drivers->deviceDriver, 
               sizeof(MUSB_DeviceDriver));
         //21c3f5c8
         for (j = 0; j < drivers->listLength; j++)
         {
            *pString++ = drivers->peripheralList[j];
         }
         //21c3f5f8
         *pString++ = i;
      }
   } //if (r7->numDeviceDriverCandidates != 0)
   //21c3f6b0
   if (0 != MGC_HdrcUlpiVbusControl(r7->Data_80->pPrivateData, 1, 0))
   {
      r7->Data_160 = func_21cc6c24(r7->Data_80, 
            pFunctionDriver, &r7->deviceDriverList, &r7->otgClient);
   }
   
   return 1;
}


/* 21c3f140 - todo */
int32_t FAPI_USB_Start(FAPI_SYS_HandleT handle)
{
   unsigned char i; 
   FAPI_TIMER_OpenParamsT sp;
   struct fapi_usb_handle* r7 = handle;
   
   FAPI_SYS_GET_SEMAPHORE(usbSemaphore, -1);
   
   if (usbInitialized == 0)
   {
      //->21c3f620
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      return 0xffff9684;
   }
   
   if (0 == fapi_usb_check_handle(r7))
   {
      //->21c3f634
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      return 0xffff9680;
   }
   
   if (0 != func_21c3d22c(r7))
   {
      //->21c3f648
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      return 0xffff9621;
   }
   
   if (0 == MGC_SetDmaControllerFactory(Data_21efc67c[0]))
   {
      //21c3f2d4
#if 1
//label_21c3f2d4:
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      func_21c3e31c(r7);      
      return 0xffff961f;
#else
      goto label_21c3f2d4;
#endif
   }
   
   r7->Data_24.wVersion = 2;
   r7->Data_24.pPrivateData = r7;
   r7->Data_24.pfSystemToBusAddress = func_21c3d2d4;
   r7->Data_24.pfQueueBackgroundItem = fapi_usb_queue_message_item;
   r7->Data_24.pfDequeueBackgroundItem = fapi_usb_dequeue_message_item;
   r7->Data_24.pfFlushBackgroundQueue = fapi_usb_clear_message_queue;
   r7->Data_24.pfArmTimer = fapi_usb_create_timer;
   r7->Data_24.pfCancelTimer = fapi_usb_delete_timer;
   r7->Data_24.pfLock = fapi_usb_lock;
   r7->Data_24.pfUnlock = fapi_usb_unlock;
   r7->Data_24.pfPrintDiag = fapi_usb_print;
   r7->Data_24.pfNewPowerLoad = NULL;
   r7->Data_24.pfRemovePowerLoad = NULL;
   
   //v3.8: MUSB_Controller* p = func_49d2fc(a, 3, 0xa080800, 0xa080800);
   MUSB_Controller* r8 = func_21cc4eac(&Data_21efc680, 3,
         Data_21efc67c[r7->index],
         Data_21efc67c[r7->index]);
         
   if (r8 == 0)
   {     
#if 1
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      func_21c3e31c(r7);      
      return 0xffff961f;
#endif
      goto label_21c3f2d4;
   }
   
   r7->Data_76 = r8;
   r7->Data_80 = func_21cc4dc8(Data_21f66538++);

   if (r7->Data_80 == 0)
   {
#if 1
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      func_21c3e31c(r7);      
      return 0xffff961f;
#endif
      goto label_21c3f2d4;
   }
      
   /* Create the required number of locks */
   
   r7->locks = FAPI_SYS_MALLOC(r8->wLockCount * 4);
   
   if (r7->locks == 0)
   {
      //21c3f2d4
#if 1
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      func_21c3e31c(r7);      
      return 0xffff961f;
#endif
      goto label_21c3f2d4;
   }
      
   r7->numLocks = r8->wLockCount;
   
   if (r7->numLocks != 0)
   {
      //21c3f288
      unsigned r6;
      for (r6 = 0; r6 < r7->numLocks; r6++)
      {
         //21c3f2a0
         r7->locks[r6] = FAPI_SYS_CREATE_SEMAPHORE(1);
               
         if (r7->locks[r6] == 0)
         {
            //21c3f2d4
#if 1
//label_21c3f2d4:
            FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
            func_21c3e31c(r7);            
            return -27105; //0xffff961f;
#else
            goto label_21c3f2d4;
#endif
         }
      }
      //21c3f318
   }
   //21c3f318
   /* Create the queue */
   
   r7->queue = FAPI_SYS_MALLOC(r8->wQueueLength * r8->wQueueItemSize);
         
   if (r7->queue == 0)
   {
      //21c3f2d4
#if 1
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      func_21c3e31c(r7);      
      return 0xffff961f;
#endif
      goto label_21c3f2d4;
   }
      
   memset(r7->queue, 0, r8->wQueueLength * r8->wQueueItemSize);
   //21c3f354
   r7->queueLen = r8->wQueueLength;
   r7->queueItemSize = r8->wQueueItemSize;
   r7->queueWritePtr = 0;
   r7->queueReadPtr = 0;
   r7->Data_200 = 1;
   
   /* Create the required number of timer */
   
   r7->timers = FAPI_SYS_MALLOC(r8->wTimerCount * sizeof(MGC_Timer));
         
   if (r7->timers == 0)
   {
      //21c3f2d4
#if 1
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      func_21c3e31c(r7);      
      return 0xffff961f;
#endif
      goto label_21c3f2d4;
   }

   memset(r7->timers, 0, r8->wTimerCount * sizeof(MGC_Timer));
   //21c3f3b8
   r7->Data_208 = r8->adwTimerResolutions[0];

   unsigned r2;
   for (r2 = 1; r2 < r8->wTimerCount; r2++)
   {
      if (r8->adwTimerResolutions[r2] != r7->Data_208)
      {
         //->21c3f2d4
#if 1
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      func_21c3e31c(r7);      
      return 0xffff961f;
#endif
      goto label_21c3f2d4;
      }
   }   
   //21c3f3fc   
   sp.version = 0x20001;
   sp.type_ = 2;
   sp.mode = 2;
   sp.resolution = r7->Data_208 * 1000;
   sp.multiplier = 1;
   sp.callback1 = usbTimerFunction;
   sp.optData1 = (uint32_t) r7;
   sp.callback2 = 0;
   sp.optData2 = 0;
   sp.exec1InIsr = 0;
   sp.exec2InIsr = 0;
   
   FAPI_SYS_HandleT r4 = FAPI_TIMER_Open(&sp, 0);
   
   if (r4 == 0)
   {
      //->21c3f2d4
#if 1
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      func_21c3e31c(r7);      
      return 0xffff961f;
#endif
      goto label_21c3f2d4;
   }
   
   r7->Data_204 = r4;
   r7->numTimer = r8->wTimerCount;
   
   unsigned cpu_sr;
   
   cpu_sr = FAPI_SYS_DISABLE_IRQ;;
   
   FREG_DMA_SetUsbIntMask(0x7E);
   FREG_DMA_SetUsbMode(0);
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);
   
   if (0 != FAPI_TIMER_Start(r4))
   {
      //21c3f2d4
#if 1
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      func_21c3e31c(r7);      
      return 0xffff961f;
#endif
      goto label_21c3f2d4;
   }
   
   /* Create a DMA Controller and enable Interrupts */
   
   if (0 != func_21cc41c4(r8, &r7->Data_24))
   {
      //21c3f2d4
#if 1
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      func_21c3e31c(r7);      
      return 0xffff961f;
#endif
      goto label_21c3f2d4;
   }
   //21c3f4cc - Create the function driver and the device drivers
   if ((0 == func_21c3f4cc(r7)) ||
         (r7->Data_160 == 0))
   {
label_21c3f2d4:
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      func_21c3e31c(r7);      
      return 0xffff961f;
   }
   //21c3f6f0   
   r7->Data_4 |= 2;
   
   FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);

   return 0;
}


/* 21c3eff0 - todo */
/* MGC_QueueMessageItem */
int fapi_usb_queue_message_item(void* a, void* msg)
{
   void* msgSlot;
   unsigned cpu_sr;
   struct fapi_usb_handle* this = a;
   
   cpu_sr = FAPI_SYS_DISABLE_IRQ;;
   
   if ((this->queueLen == 0) ||
         (((this->queueWritePtr + 1) % this->queueLen) == this->queueReadPtr))
   {
      FAPI_SYS_ENABLE_IRQ(cpu_sr);
      
      if (Data_21f7a064 != 0)
      {
         func_21cc8328("Queue overflow!");
      }

      return 0;
   }

   msgSlot = this->queue + this->queueWritePtr * this->queueItemSize;
   
   memcpy(msgSlot, msg, this->queueItemSize);
   
   this->queueWritePtr = (this->queueWritePtr + 1) % this->queueLen;
   
   if (Data_21f7a064 != 0)
   {
      Data_2207f32c[0] = 0;
      Data_2207f42c[0] = 0;
      
      func_21cc3b54(Data_2207f32c, 256, "Queue message item: ");            
      func_21cc3e28(Data_2207f42c, 33, (int)msgSlot, 16, 8);      
      func_21cc3b54(Data_2207f32c, 256, Data_2207f42c);      
      func_21cc8328(Data_2207f32c);
   }

   FAPI_SYS_ENABLE_IRQ(cpu_sr);
   
   return 1;
}


/* 21c3eeb4 - todo */
/* MGC_DequeueMessageItem */
int fapi_usb_dequeue_message_item(void* a, void* msg)
{
   void* msgSlot;
   int cpu_sr;
   struct fapi_usb_handle* this = a;
   
   cpu_sr = FAPI_SYS_DISABLE_IRQ;;

   if ((this->queueReadPtr == this->queueWritePtr) ||
         (this->queueLen == 0))
   {
      this->Data_200 = 1;
      
      FAPI_SYS_ENABLE_IRQ(cpu_sr);
      
      return 0;
   }

   msgSlot = this->queue + this->queueReadPtr * this->queueItemSize;
   
   memcpy(msg, msgSlot, this->queueItemSize);
   
   this->queueReadPtr = (this->queueReadPtr + 1) % this->queueLen;

   if (Data_21f7a064 != 0)
   {
      Data_2207f32c[0] = 0;
      Data_2207f42c[0] = 0;
      
      func_21cc3b54(Data_2207f32c, 256, "Dequeue message item: ");            
      func_21cc3e28(Data_2207f42c, 33, (int)msgSlot, 16, 8);      
      func_21cc3b54(Data_2207f32c, 256, Data_2207f42c);      
      func_21cc8328(Data_2207f32c);
   }
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);
   
   return 1;
}


/* 21c3ecac - complete */
int32_t FAPI_USB_RegHostDriver(FAPI_SYS_HandleT handle,
               const MUSB_DeviceDriver* driverPtr,
               const uint8_t* peripheralListPtr,
               uint32_t listLength)
{
   struct MUSB_DeviceDriverCandidate* r4 = 0;
   struct fapi_usb_handle* h = handle;
   
   FAPI_SYS_GET_SEMAPHORE(usbSemaphore, -1);
   
   if (usbInitialized == 0)
   {
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      
      return -27004; //0xffff9684;
   }

   if (0 == fapi_usb_check_handle(h))
   {
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      
      return -27008; //0xffff9680;
   }

   if ((driverPtr == 0) || (peripheralListPtr == 0) ||
         (listLength == 0) || (listLength > 50) || 
         (peripheralListPtr[listLength-1] != 9/*end tag*/))
   {
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      
      return -27001; //0xffff9687;
   }

   if (h->numDeviceDriverCandidates == 0xFF)
   {
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      
      return FAPI_USB_ERR_TOO_MANY_DRIVERS;
   }

   r4 = FAPI_SYS_MALLOC(sizeof(struct MUSB_DeviceDriverCandidate));
         
   if (r4 != 0)
   {
      memcpy(&r4->deviceDriver, driverPtr, sizeof(MUSB_DeviceDriver));
      memcpy(r4->peripheralList, peripheralListPtr, listLength);
      
      r4->listLength = listLength;
      r4->next = h->deviceDriverCandidates;
      h->deviceDriverCandidates = r4;
      h->numDeviceDriverCandidates++;      
   }
   else
   {
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      
      return -27002; //0xffff9686;
   }

   FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
   
   return 0;
}


/* 21c3eb7c - todo */
int fapi_usb_create_timer(void* a, 
      unsigned short b, 
      int c, 
      unsigned char d, 
      void (*func)(Struct_49d2fc* a, unsigned short b, MGC_Timer* c))
{
   struct fapi_usb_handle* this = a;
   
   if (Data_21f7a064 != 0) 
   {
      Data_2207f32c[0] = 0;
      Data_2207f42c[0] = 0;  
   
      func_21cc3b54(Data_2207f32c, 256, "TIMER create no ");
      func_21cc3e28(Data_2207f42c, 33, b, 16, 8); 
      func_21cc3b54(Data_2207f32c, 256, Data_2207f42c);      
      func_21cc3b54(Data_2207f32c, 256, ", time (ms): ");      
      Data_2207f42c[0] = 0;        
      func_21cc3e28(Data_2207f42c, 33, c, 16, 8); 
      func_21cc3b54(Data_2207f32c, 256, Data_2207f42c);
      func_21cc8328(Data_2207f32c);
   }
   
#if 0
   FAPI_SYS_PRINT_MSG("fapi_usb_create_timer: this->numTimer=%d, this->Data_208=%d\n", 
         this->numTimer, this->Data_208);
#endif
   
   if ((b < this->numTimer) &&
         (this->Data_208 != 0))
   {
      MGC_Timer* pTimer = &this->timers[b];   
      
      pTimer->count = c / this->Data_208; //func_3fe31c();      
      pTimer->recount = (d != 0)? pTimer->count: 0;
      pTimer->func = func;
      
      return 1;
   }
   
   return 0;
}


/* 21c3eab0 - todo */
int fapi_usb_delete_timer(void* a, unsigned short b)
{
   struct fapi_usb_handle* this = a;
   
   if (b < this->numTimer) 
   {
      if (Data_21f7a064)
      {
         Data_2207f32c[0] = 0;
         Data_2207f42c[0] = 0;  
      
         func_21cc3b54(Data_2207f32c, 256, "TIMER cancel no ");
         func_21cc3e28(Data_2207f42c, 33, b, 16, 8); 
         func_21cc3b54(Data_2207f32c, 256, Data_2207f42c);      
         func_21cc8328(Data_2207f32c);
      }
      
      MGC_Timer* pTimer = &this->timers[b];
      
      pTimer->count = 0;
      pTimer->recount = 0;
      pTimer->func = 0;
      
      return 1;   
   }
   
   return 0;
}


/* 21c3e968 - todo */
int fapi_usb_lock(void* a, unsigned short index)
{
   struct fapi_usb_handle* this = a;
   
   if (index >= this->numLocks)
   {
      return 0;
   }

   if (Data_21f7a064 != 0) 
   {
      Data_2207f32c[0] = 0;
      Data_2207f42c[0] = 0;  
   
      func_21cc3b54(Data_2207f32c, 256, "Waiting for lock index ");
      func_21cc3e28(Data_2207f42c, 33, index, 16, 4); 
      func_21cc3b54(Data_2207f32c, 256, Data_2207f42c);
      func_21cc8328(Data_2207f32c);
   }
   
   FAPI_SYS_GET_SEMAPHORE(this->locks[index], -1);
   
   if (Data_21f7a064 != 0) 
   {
      Data_2207f32c[0] = 0;
      Data_2207f42c[0] = 0;  
   
      func_21cc3b54(Data_2207f32c, 256, "Got lock index ");
      func_21cc3e28(Data_2207f42c, 33, index, 16, 4); 
      func_21cc3b54(Data_2207f32c, 256, Data_2207f42c);
      func_21cc8328(Data_2207f32c);
   }
   
   return 1;
}


/* 21c3e8a0 - todo */
int fapi_usb_unlock(void* a, unsigned short index)
{
   struct fapi_usb_handle* this = a;
   
   if (index >= this->numLocks)
   {
      return 0;
   }

   FAPI_SYS_SET_SEMAPHORE(this->locks[index], 0);
   
   if (Data_21f7a064 != 0) 
   {
      Data_2207f32c[0] = 0;
      Data_2207f42c[0] = 0;  
   
      func_21cc3b54(Data_2207f32c, 256, "Released lock index ");
      func_21cc3e28(Data_2207f42c, 33, index, 16, 4); 
      func_21cc3b54(Data_2207f32c, 256, Data_2207f42c);
      func_21cc8328(Data_2207f32c);
   }
            
   return 1;   
}


/* 21c3e768 - complete */
void usbTimerFunction(uint64_t value, uint32_t optData)
{
   unsigned i;
   
   struct fapi_usb_handle* r6 = (void*) optData;
   
   for (i = 0; i < r6->numTimer; i++)
   {
      MGC_Timer* t = &r6->timers[i];
      
      if (t->func)
      {         
         if (t->count != 0)
         {
            t->count--;
            
            if (t->count == 0)
            {
               if (t->recount != 0)
               {
                  t->count = t->recount;
               }
               
               if (Data_21f7a064 != 0)
               {
                  Data_2207f32c[0] = 0;
                  Data_2207f42c[0] = 0;
                  
                  func_21cc3b54(Data_2207f32c, 256, "TIMER expired no ");            
                  func_21cc3e28(Data_2207f42c, 33, (int)i, 16, 8);      
                  func_21cc3b54(Data_2207f32c, 256, Data_2207f42c);      
                  func_21cc8328(Data_2207f32c);
               }

               int r4 = MGC_Read8(Data_21efc67c[r6->index], 0x0E);
               
               (t->func)(r6->Data_76->pPrivateData, i);
               
               MGC_Write8(Data_21efc67c[r6->index], 0x0E, r4);
            }
         }
      }
   }   
}


/* 21c3e6c0 - complete */
void FAPI_USB_Exit(void)
{
   if (usbInitialized != 0)
   {
      if (0 != fapi_usb_check_handle(&usbHandleArray[0]))
      {
         FAPI_USB_Close(&usbHandleArray[0]);
      }
      
      memset(&usbHandleArray, 0, sizeof(struct fapi_usb_handle));
      
      usbInitialized = 0;
      Data_21f66538 = 0;
      
      Data_21efc680.wVersion = 1;
      Data_21efc680.pfMessageString = 0;
      Data_21efc680.pfMessageNumber = 0;
      Data_21efc680.pfGetTime = 0;
      
      FAPI_SYS_DESTROY_SEMAPHORE(usbSemaphore);
      
      usbSemaphore = 0;
   }
}


/* 21c3e5e8 - complete */
int32_t FAPI_USB_Close( FAPI_SYS_HandleT handle )
{
   struct fapi_usb_handle* a = handle;
   
   if (usbInitialized == 0)
   {
      return 0xffff9684;
   }
   
   if (0 == fapi_usb_check_handle(a))
   {
      return 0xffff9680;
   }
   
   func_21c3e31c(a);

   FAPI_SYS_GET_SEMAPHORE(usbSemaphore, -1);
   
   if (a->numDeviceDriverCandidates != 0)
   {
      struct MUSB_DeviceDriverCandidate* r3;
      void* r5;
      
      for (r3 = a->deviceDriverCandidates; r3 != 0; r3 = r5)
      {
         r5 = r3->next;
         
         FAPI_SYS_FREE(r3);
      }
   }

   memset(a, 0, sizeof(struct fapi_usb_handle));
   
   FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
   
   return 0;
}


/* 21c3e31c - todo */
int func_21c3e31c(struct fapi_usb_handle* a)
{
   printf("21c3e31c");
   return 0;
}


/* 21c3e20c - complete */
int32_t FAPI_USB_Init(void)
{
   unsigned i = 0;
   
   if (usbInitialized != 0)
   {
      return 0xffff9685;
   }
   
   memset(&usbHandleArray, 0, sizeof(struct fapi_usb_handle));
   
   if (Data_21efc67c[0] == 0)
   {
      return 0xffff9687;
   }

   Data_21efc680.wVersion = 1;
   Data_21efc680.pfMessageString = func_21c3f858;
   Data_21efc680.pfMessageNumber = func_21c3f8a0;
   Data_21efc680.pfGetTime = func_21c3d2cc;
   
   usbSemaphore = FAPI_SYS_CREATE_SEMAPHORE(1);
         
   if (usbSemaphore == 0)
   {
      return 0xffff967f;
   }
   
   for (i = 0; i < 1; i++)
   {
      Data_21f6653c[i] = FAPI_SYS_CREATE_SEMAPHORE(1);
   
      if (Data_21f6653c[i] == 0)
      {
         return 0xffff967f;
      }
   }
   
   usbInitialized = 1;
   
   return 0;
}


/* 21c3d528 - todo */
FAPI_SYS_HandleT FAPI_USB_Open(const FAPI_USB_OpenParamsT* paramsPtr,
                               int32_t* errorCodePtr)
{
   struct fapi_usb_handle* h = 0;
   int res = 0;
   
   if (usbInitialized == 0)
   {
      res = 0xffff9684;
   }
   
   if (paramsPtr == 0)
   {
      res = 0xffff9687;
   }
   else
   {
      if (paramsPtr->version > FAPI_USB_VERSION)
      {
         res = 0xffff967e;
      }
      
      if (paramsPtr->ctrlId != 0)      
      {
         res = 0xffff9624;
      }

      if (usbHandleArray[paramsPtr->ctrlId].Data_4 != 0)
      {
         res = 0xffff9623;
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

   if (res == 0)
   {
      FAPI_SYS_GET_SEMAPHORE(usbSemaphore, -1);
      
      h = fapi_usb_get_handle(paramsPtr->ctrlId);      

      h->Data_200 = 1;
      h->bData_12 = (paramsPtr->desireHostRoleFlag == 0)? 0: 1;
      h->otgStateNotifyFct = paramsPtr->otgStateNotifyFct;
      h->otgErrorNotifyFct = paramsPtr->otgErrorNotifyFct;
      
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
   }

   if (errorCodePtr != 0)
   {
      *errorCodePtr = res;
   }      
   
   return h;
}


/* 21c3d47c - complete */
void FAPI_USB_Bsr0(uint32_t privData)
{
   if (privData != 0)
   {
      FAPI_SYS_GET_SEMAPHORE(Data_21f6653c[usbHandleArray[0].index], -1);
   }
   
   if (usbHandleArray[0].queueWritePtr != 
      usbHandleArray[0].queueReadPtr)
   {
      if (usbHandleArray[0].Data_200 != 0)
      {
         usbHandleArray[0].Data_200 = 0;
      }
      
      (usbHandleArray[0].Data_76->pfBsr)(usbHandleArray[0].Data_76->pBsrParam);
   }
   
   if (privData != 0)
   {
      FAPI_SYS_SET_SEMAPHORE(Data_21f6653c[usbHandleArray[0].index], 0);
   }
}


/* 21c3d3ec - complete */
/* v3.8: func_49ef08 */
void func_21c3d3ec(void* a, void* b, int c)
{
   struct fapi_usb_handle* r5 = a;
   
   if (0 != fapi_usb_check_handle(r5))
   {
      if (b != 0)
      {
         if (r5->otgErrorNotifyFct != 0)
         {
            (r5->otgErrorNotifyFct)(r5->index, c);
         }
      }
   }
}


/* 21c3d3ac - complete */
/* v3.8: func_49ef18 */
void func_21c3d3ac(void* a, struct MGC* b, int c)
{
   struct fapi_usb_handle* r5 = a;
   
   if (0 != fapi_usb_check_handle(r5))
   {
      if (b != 0)
      {
         if (r5->otgStateNotifyFct != 0)
         {
            (r5->otgStateNotifyFct)(r5->index, c);
         }
      }
   }
}


/* 21c3d350 - complete */
int fapi_usb_print(void* a, char* b)
{
   if ((b != 0) && (a != 0))
   {
      FAPI_SYS_PRINT_MSG("%s\n", b);
   }
   else
   {
      FAPI_SYS_PRINT_MSG("NULL\n");
   }
   
   return 1;
}


/* 21c3d308 - complete */
void fapi_usb_clear_message_queue(void* a)
{
   unsigned cpu_sr;
   struct fapi_usb_handle* this = a;
   
   cpu_sr = FAPI_SYS_DISABLE_IRQ;;
   
   this->queueWritePtr = 0;
   this->queueReadPtr = 0;
   this->Data_200 = 1;
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);
}


/* 21c3d2d4 - todo */
//char* func_21c3d2d4(void* a, char* b)
uint32_t func_21c3d2d4(void* a, uint32_t b)
{
   uint32_t res = 0;
   
   if (a != 0)
   {
#if 0
      res = (FAPI_SYS_Services.getPhysAddressFunc != 0)?
            (FAPI_SYS_Services.getPhysAddressFunc)(b): b;
#else
      res = FAPI_SYS_GET_PHYSICAL_ADDRESS(b);
#endif
   }
   
   return res;
}


/* 21c3d2cc - complete */
int func_21c3d2cc(void)
{
//   printf("func_21c3d2cc\n");
   
   return 0;
}


/* 21c3d240 - complete */
int fapi_usb_check_handle(struct fapi_usb_handle* a)
{
   if ((a != 0) && 
         (a->magic == 0x75736220) && //usb
         (a->Data_4 != 0)) 
   {
      return 1;
   }

   return 0;
}


/* 21c3d22c - complete */
int func_21c3d22c(struct fapi_usb_handle* a)
{
   if (a == 0)
   {
      return 0;
   }
   
   return (a->Data_4 >> 1) & 1;
}


struct fapi_usb_handle* fapi_usb_get_handle(uint32_t a)
{
   usbHandleArray[a].Data_4 = 1;
   usbHandleArray[a].magic = 0x75736220; //usb
   usbHandleArray[a].index = a;

   return &usbHandleArray[a];
}

