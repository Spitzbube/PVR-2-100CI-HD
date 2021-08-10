

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



static int usbUtilGetTimeStamp(void);
int usbCheckHandle(struct fapi_usb_handle* a);
//int FAPI_USB_Stop(struct fapi_usb_handle* a);
//char* usbServiceSystemToBusAddress(void*, char*);
uint32_t usbServiceSystemToBusAddress(void* a, uint32_t b);
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
void usbOtgStateNotifier(void* a, struct MGC* b, int c);
void usbOtgErrorNotifier(void* a, void* b, int c);
static struct fapi_usb_handle* fapi_usb_get_handle(uint32_t);


#define USB_IS_USED          (uint32_t)0x1
#define USB_IS_RUNNING       (uint32_t)0x2


typedef struct usbHostDriver
{
   MUSB_DeviceDriver musbDriver; //0
   char peripheralList[52]; //28
   unsigned listLength; //80
   struct usbHostDriver* nextPtr; //84
} usbHostDriverT;

typedef struct {
    FAPI_SYS_SemaphoreT*  buffer; //0
    uint32_t              count; //4
    //8
} usbLockT;

typedef struct {
    uint8_t* buffer; //0
    uint32_t itemLength; //4
    uint32_t itemCount; //8
    uint32_t head; //12
    uint32_t tail; //16
    uint32_t bsrFinished; /*20*/ // indicates when BSR has completely finished and needs to be re-activated
    //24
} usbQueueT;

typedef struct {
    FAPI_SYS_HandleT   hSoftTimer; //0
    uint32_t           resolution; //4
    MGC_Timer/*usbTimerWrapperT*/*  buffer; //8
    uint32_t           count; //12
    //16
} usbTimerT;

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
int baseAddress[1] = {0xc7000800}; //21efc67c
MUSB_SystemUtils systemUtils = //21efc680
{
      1, 0, 0, 0
};


int Data_21f6652c; //21f6652c
static FAPI_SYS_SemaphoreT usbSemaphore = 0; //21f66530
static int usbInitialized = 0; //21f66534
int Data_21f66538; //21f66538
FAPI_SYS_SemaphoreT usbBsrLock[1]; //21f6653c
typedef struct fapi_usb_handle
{
   int magic; //0
   unsigned inUse; //4
   uint32_t index; //8
   char hostRoleFlag; //12
   FAPI_USB_OtgStateFuncT otgStateNotifyFct; //16
   FAPI_USB_OtgErrorFuncT otgErrorNotifyFct; //20
   MUSB_SystemServices sysServices; //24
   MUSB_Controller* controllerPtr; //76
   MUSB_Port* portPtr; //80
   MUSB_OtgClient otgClient; //84 +16
   MUSB_FunctionClient functionClient; //100 +44
   MUSB_HostClient hostClient; //144 +16
   void* busHandle; //160
   usbHostDriverT* hostDriverListPtr; //164
   unsigned char hostDriverCount; //168
   usbLockT lock; //172
   usbQueueT queue; //180
   usbTimerT timer; //204
   //220
} usbHandleT;

usbHandleT usbHandleArray[1]; //21f66540 ->21F6661C

/*static*/ uint32_t usbCheckRunning(const usbHandleT* handlePtr);


extern int Data_21f7a064; //21f7a064


extern char Data_2207f32c[]; //2207f32c
extern char Data_2207f42c[]; //2207f42c


static void usbHandleDelete( usbHandleT* handlePtr )
{

    if ( handlePtr->hostDriverCount ) {
        usbHostDriverT* driver_ptr, *next_ptr;

        driver_ptr = handlePtr->hostDriverListPtr;
        while( driver_ptr ) {
            next_ptr = driver_ptr->nextPtr;
            FAPI_SYS_FREE( driver_ptr );
            driver_ptr = next_ptr;
        }
    }
    memset( handlePtr, 0, sizeof(usbHandleT));
}


/* 21c3f8a0 - todo */
void usbUtilAppendMessageNumber(void)
{
   printf("usbUtilAppendMessageNumber: TODO\n");
}


/* 21c3f858 - todo */
void usbUtilAppendMessageString(void)
{
   printf("usbUtilAppendMessageString: TODO\n");
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

      res = (usbHandleArray[0].controllerPtr->pfIsr)(usbHandleArray[0].controllerPtr->pIsrParam);
      
      if (Data_21f7a064 != 0)
      {
         func_21cc8328("ISR0 end");               
      }

      FREG_DMA_SetUsbIntClear_ClearUsbIrq(1);
      
      if ((res > 0) && (usbHandleArray[0].queue.bsrFinished != 0))
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


static inline int usbRegisterClient(struct fapi_usb_handle* r7)
{   
   unsigned char i, j;
   MUSB_FunctionClient* pFunctionDriver = 0;
   
   if (r7->functionClient.pStandardDescriptors != 0)
   {
      pFunctionDriver = &r7->functionClient;
   }
   
   r7->otgClient.pPrivateData = r7;
   r7->otgClient.pbDesireHostRole = &r7->hostRoleFlag;
   r7->otgClient.pfOtgState = usbOtgStateNotifier;
   r7->otgClient.pfOtgError = usbOtgErrorNotifier;
   
   if (r7->hostDriverCount != 0)
   {
      //21c3f504
      struct usbHostDriver* r2_;
      int r4_ = 0; 
      for (r2_ = r7->hostDriverListPtr; r2_ != 0; r2_ = r2_->nextPtr)
      {
         //21c3f518
         r4_ += (r2_->listLength + 1);      
      }
      //21c3f530
      r7->hostClient.aDeviceDriverList =
         FAPI_SYS_MALLOC(r7->hostDriverCount * sizeof(MUSB_DeviceDriver));

      if (r7->hostClient.aDeviceDriverList == 0)
      {
         //21c3f2d4
         return 1;
      }
      //21c3f554
      r7->hostClient.bDeviceDriverListLength = r7->hostDriverCount;
      
      r7->hostClient.pPeripheralList = FAPI_SYS_MALLOC(r4_);
            
      if (r7->hostClient.pPeripheralList == 0)
      {
         //21c3f730
         FAPI_SYS_FREE(r7->hostClient.aDeviceDriverList);
         //21c3f2d4
         return 1;
      }
      //21c3f580
      r7->hostClient.wPeripheralListLength = r4_;
      
      char* pString = r7->hostClient.pPeripheralList;
      struct usbHostDriver* drivers = r7->hostDriverListPtr;
      
      for (i = 0; (i < r7->hostDriverCount) && (drivers != 0); drivers = drivers->nextPtr, i++)
      {
         //21c3f5a0
         memcpy(&r7->hostClient.aDeviceDriverList[i],
               &drivers->musbDriver,
               sizeof(MUSB_DeviceDriver));
         //21c3f5c8
         for (j = 0; j < drivers->listLength; j++)
         {
            *pString++ = drivers->peripheralList[j];
         }
         //21c3f5f8
         *pString++ = i;
      }
   } //if (r7->hostDriverCount != 0)
   //21c3f6b0
   if (0 != MGC_HdrcUlpiVbusControl(r7->portPtr->pPrivateData, 1, 0))
   {
      r7->busHandle = MUSB_RegisterOtgClient(r7->portPtr,
            pFunctionDriver, &r7->hostClient, &r7->otgClient);
   }
   
   return r7->busHandle ? 0 : 1;
}

static uint32_t usbInitController( usbHandleT* handlePtr/*r7*/ )
{
    MUSB_SystemServices*   service_ptr;
    MUSB_Controller*       r8; //ctrl_ptr;
    unsigned r2;
    FAPI_SYS_HandleT r4;
    unsigned cpu_sr;
    FAPI_TIMER_OpenParamsT timerParams; //sp;

    if (0 == MUSB_FAPI_InitDma(baseAddress[0]))
    {
        //21c3f2d4
        return 0;
    }

    service_ptr = &handlePtr->sysServices;

    service_ptr->wVersion = 2;
    service_ptr->pPrivateData = handlePtr;
    service_ptr->pfSystemToBusAddress = usbServiceSystemToBusAddress;
    service_ptr->pfQueueBackgroundItem = fapi_usb_queue_message_item;
    service_ptr->pfDequeueBackgroundItem = fapi_usb_dequeue_message_item;
    service_ptr->pfFlushBackgroundQueue = fapi_usb_clear_message_queue;
    service_ptr->pfArmTimer = fapi_usb_create_timer;
    service_ptr->pfCancelTimer = fapi_usb_delete_timer;
    service_ptr->pfLock = fapi_usb_lock;
    service_ptr->pfUnlock = fapi_usb_unlock;
    service_ptr->pfPrintDiag = fapi_usb_print;
    service_ptr->pfNewPowerLoad = NULL;
    service_ptr->pfRemovePowerLoad = NULL;

    //v3.8: MUSB_Controller* p = func_49d2fc(a, 3, 0xa080800, 0xa080800);
    r8 = MUSB_NewController(&systemUtils, 3,
          baseAddress[handlePtr->index],
          baseAddress[handlePtr->index]);

    if (r8 == 0)
    {
       //goto label_21c3f2d4;
        return 0;
    }

    handlePtr->controllerPtr = r8;
    handlePtr->portPtr = MUSB_GetPort(/*ctrlStartCount*/Data_21f66538++);

    if (handlePtr->portPtr == 0)
    {
       //goto label_21c3f2d4;
        return 0;
    }

    /* Create the required number of locks */

    handlePtr->lock.buffer = FAPI_SYS_MALLOC(r8->wLockCount * 4);

    if (handlePtr->lock.buffer == NULL)
    {
       //21c3f2d4
       return 0;
    }

    handlePtr->lock.count = r8->wLockCount;

//    if (handlePtr->lock.count != 0)
    {
       //21c3f288
       unsigned r6;
       for (r6 = 0; r6 < handlePtr->lock.count; r6++)
       {
          //21c3f2a0
           handlePtr->lock.buffer[r6] = FAPI_SYS_CREATE_SEMAPHORE(1);

          if (handlePtr->lock.buffer[r6] == 0)
          {
             //21c3f2d4
              return 0;
          }
       }
       //21c3f318
    }
    //21c3f318

    /* Create the queue */

    handlePtr->queue.buffer = FAPI_SYS_MALLOC(r8->wQueueLength * r8->wQueueItemSize);

    if (handlePtr->queue.buffer == NULL)
    {
       //21c3f2d4
        return 0;
    }

    memset(handlePtr->queue.buffer, 0, r8->wQueueLength * r8->wQueueItemSize);
    //21c3f354
    handlePtr->queue.itemCount = r8->wQueueLength;
    handlePtr->queue.itemLength = r8->wQueueItemSize;
    handlePtr->queue.head = 0;
    handlePtr->queue.tail = 0;
    handlePtr->queue.bsrFinished = 1;

    /* Create the required number of timer */

    handlePtr->timer.buffer = FAPI_SYS_MALLOC(r8->wTimerCount * sizeof(MGC_Timer));

    if (handlePtr->timer.buffer == 0)
    {
       //21c3f2d4
        return 0;
    }

    memset(handlePtr->timer.buffer, 0, r8->wTimerCount * sizeof(MGC_Timer));
    //21c3f3b8
    handlePtr->timer.resolution = r8->adwTimerResolutions[0];

    for (r2 = 1; r2 < r8->wTimerCount; r2++)
    {
       if (r8->adwTimerResolutions[r2] != handlePtr->timer.resolution)
       {
          //->21c3f2d4
           return 0;
       }
    }
    //21c3f3fc
    memset(&timerParams, 0, sizeof(timerParams));

    timerParams.version = 0x20001;
    timerParams.type_ = 2;
    timerParams.mode = 2;
    timerParams.resolution = handlePtr->timer.resolution * 1000;
    timerParams.multiplier = 1;
    timerParams.callback1 = usbTimerFunction;
    timerParams.optData1 = (uint32_t) handlePtr;
    timerParams.exec1InIsr = 0;

    r4 = FAPI_TIMER_Open(&timerParams, 0);

    if (r4 == 0)
    {
       //->21c3f2d4
        return 0;
    }

    handlePtr->timer.hSoftTimer = r4;
    handlePtr->timer.count = r8->wTimerCount;

    cpu_sr = FAPI_SYS_DISABLE_IRQ;;

    FREG_DMA_SetUsbIntMask(0x7E);
    FREG_DMA_SetUsbMode(0);

    FAPI_SYS_ENABLE_IRQ(cpu_sr);

    if (0 != FAPI_TIMER_Start(r4))
    {
       //21c3f2d4
        return 0;
    }

    /* Create a DMA Controller and enable Interrupts */

    if (0 != MUSB_StartController(r8, service_ptr/*&handlePtr->sysServices*/))
    {
       //21c3f2d4
        return 0;
    }

    return 1;
}


/* 21c3f140 - todo */
int32_t FAPI_USB_Start(FAPI_SYS_HandleT handle)
{
   unsigned char i; 
   struct fapi_usb_handle* r7 = handle;
   
   FAPI_SYS_GET_SEMAPHORE(usbSemaphore, -1);
   
   if (usbInitialized == 0)
   {
      //->21c3f620
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      return 0xffff9684;
   }
   
   if (0 == usbCheckHandle(r7))
   {
      //->21c3f634
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      return 0xffff9680;
   }
   
   if (0 != usbCheckRunning(r7))
   {
      //->21c3f648
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      return 0xffff9621;
   }
   
   if (0 == usbInitController(r7))
   {
       FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
       FAPI_USB_Stop(r7);
       return 0xffff961f;
   }

   //21c3f4cc - Create the function driver and the device drivers

   if (0 != usbRegisterClient(r7))
   {
       //label_21c3f2d4:
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      FAPI_USB_Stop(r7);
      return 0xffff961f;
   }
   //21c3f6f0   
   r7->inUse |= 2;
   
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
   
   if ((this->queue.itemCount == 0) ||
         (((this->queue.head + 1) % this->queue.itemCount) == this->queue.tail))
   {
      FAPI_SYS_ENABLE_IRQ(cpu_sr);
      
      if (Data_21f7a064 != 0)
      {
         func_21cc8328("Queue overflow!");
      }

      return 0;
   }

   msgSlot = this->queue.buffer + this->queue.head * this->queue.itemLength;
   
   memcpy(msgSlot, msg, this->queue.itemLength);
   
   this->queue.head = (this->queue.head + 1) % this->queue.itemCount;
   
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

   if ((this->queue.tail == this->queue.head) ||
         (this->queue.itemCount == 0))
   {
      this->queue.bsrFinished = 1;
      
      FAPI_SYS_ENABLE_IRQ(cpu_sr);
      
      return 0;
   }

   msgSlot = this->queue.buffer + this->queue.tail * this->queue.itemLength;
   
   memcpy(msg, msgSlot, this->queue.itemLength);
   
   this->queue.tail = (this->queue.tail + 1) % this->queue.itemCount;

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
   struct usbHostDriver* r4 = 0;
   struct fapi_usb_handle* h = handle;
   
   FAPI_SYS_GET_SEMAPHORE(usbSemaphore, -1);
   
   if (usbInitialized == 0)
   {
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      
      return -27004; //0xffff9684;
   }

   if (0 == usbCheckHandle(h))
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

   if (h->hostDriverCount == 0xFF)
   {
      FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
      
      return FAPI_USB_ERR_TOO_MANY_DRIVERS;
   }

   r4 = FAPI_SYS_MALLOC(sizeof(struct usbHostDriver));
         
   if (r4 != 0)
   {
      memcpy(&r4->musbDriver, driverPtr, sizeof(MUSB_DeviceDriver));
      memcpy(r4->peripheralList, peripheralListPtr, listLength);
      
      r4->listLength = listLength;
      r4->nextPtr = h->hostDriverListPtr;
      h->hostDriverListPtr = r4;
      h->hostDriverCount++;
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
   FAPI_SYS_PRINT_MSG("fapi_usb_create_timer: this->timer.count=%d, this->timer.resolution=%d\n",
         this->timer.count, this->timer.resolution);
#endif
   
   if ((b < this->timer.count) &&
         (this->timer.resolution != 0))
   {
      MGC_Timer* pTimer = &this->timer.buffer[b];
      
      pTimer->count = c / this->timer.resolution; //func_3fe31c();
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
   
   if (b < this->timer.count)
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
      
      MGC_Timer* pTimer = &this->timer.buffer[b];
      
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
   
   if (index >= this->lock.count)
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
   
   FAPI_SYS_GET_SEMAPHORE(this->lock.buffer[index], -1);
   
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
   
   if (index >= this->lock.count)
   {
      return 0;
   }

   FAPI_SYS_SET_SEMAPHORE(this->lock.buffer[index], 0);
   
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
   
   for (i = 0; i < r6->timer.count; i++)
   {
      MGC_Timer* t = &r6->timer.buffer[i];
      
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

               int r4 = MGC_Read8(baseAddress[r6->index], 0x0E);
               
               (t->func)(r6->controllerPtr->pPrivateData, i);
               
               MGC_Write8(baseAddress[r6->index], 0x0E, r4);
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
      if (0 != usbCheckHandle(&usbHandleArray[0]))
      {
         FAPI_USB_Close(&usbHandleArray[0]);
      }
      
      memset(&usbHandleArray, 0, sizeof(struct fapi_usb_handle));
      
      usbInitialized = 0;
      Data_21f66538 = 0;
      
      systemUtils.wVersion = 1;
      systemUtils.pfMessageString = 0;
      systemUtils.pfMessageNumber = 0;
      systemUtils.pfGetTime = 0;
      
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
   
   if (0 == usbCheckHandle(a))
   {
      return 0xffff9680;
   }
   
   FAPI_USB_Stop(a);

   FAPI_SYS_GET_SEMAPHORE(usbSemaphore, -1);
   
   usbHandleDelete(a);
   
   FAPI_SYS_SET_SEMAPHORE(usbSemaphore, 0);
   
   return 0;
}


/* 21c3e31c - todo */
void FAPI_USB_Stop(/*struct fapi_usb_handle* a*/FAPI_SYS_HandleT handle)
{
   printf("FAPI_USB_Stop: TODO\n");
//   return 0;
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
   
   if (baseAddress[0] == 0)
   {
      return 0xffff9687;
   }

   systemUtils.wVersion = 1;
   systemUtils.pfMessageString = usbUtilAppendMessageString;
   systemUtils.pfMessageNumber = usbUtilAppendMessageNumber;
   systemUtils.pfGetTime = usbUtilGetTimeStamp;
   
   usbSemaphore = FAPI_SYS_CREATE_SEMAPHORE(1);
         
   if (usbSemaphore == 0)
   {
      return 0xffff967f;
   }
   
   for (i = 0; i < 1; i++)
   {
      usbBsrLock[i] = FAPI_SYS_CREATE_SEMAPHORE(1);
   
      if (usbBsrLock[i] == 0)
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

      if (usbHandleArray[paramsPtr->ctrlId].inUse != 0)
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

      h->queue.bsrFinished = 1;
      h->hostRoleFlag = (paramsPtr->desireHostRoleFlag == 0)? 0: 1;
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
      FAPI_SYS_GET_SEMAPHORE(usbBsrLock[usbHandleArray[0].index], -1);
   }
   
   if (usbHandleArray[0].queue.head !=
      usbHandleArray[0].queue.tail)
   {
      if (usbHandleArray[0].queue.bsrFinished != 0)
      {
         usbHandleArray[0].queue.bsrFinished = 0;
      }
      
      (usbHandleArray[0].controllerPtr->pfBsr)(usbHandleArray[0].controllerPtr->pBsrParam);
   }
   
   if (privData != 0)
   {
      FAPI_SYS_SET_SEMAPHORE(usbBsrLock[usbHandleArray[0].index], 0);
   }
}


/* 21c3d3ec - complete */
/* v3.8: func_49ef08 */
void usbOtgErrorNotifier(void* a, void* b, int c)
{
   struct fapi_usb_handle* r5 = a;
   
   if (0 != usbCheckHandle(r5))
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
void usbOtgStateNotifier(void* a, struct MGC* b, int c)
{
   struct fapi_usb_handle* r5 = a;
   
   if (0 != usbCheckHandle(r5))
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
   
   this->queue.head = 0;
   this->queue.tail = 0;
   this->queue.bsrFinished = 1;
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);
}


/* 21c3d2d4 - todo */
//char* usbServiceSystemToBusAddress(void* a, char* b)
uint32_t usbServiceSystemToBusAddress(void* a, uint32_t b)
{
   uint32_t res = 0;
   
   if (a != 0)
   {
      res = FAPI_SYS_GET_PHYSICAL_ADDRESS(b);
   }
   
   return res;
}


/* 21c3d2cc - complete */
int usbUtilGetTimeStamp(void)
{
//   printf("usbUtilGetTimeStamp\n");
   
   return 0;
}


/* 21c3d240 - complete */
int usbCheckHandle(struct fapi_usb_handle* a)
{
   if ((a != 0) && 
         (a->magic == 0x75736220) && //usb
         (a->inUse != 0))
   {
      return 1;
   }

   return 0;
}


/* 21c3d22c - complete */
/*static*/ uint32_t usbCheckRunning(const usbHandleT* handlePtr)
{
    return( handlePtr && (handlePtr->inUse & USB_IS_RUNNING));
}


struct fapi_usb_handle* fapi_usb_get_handle(uint32_t a)
{
   usbHandleArray[a].inUse = 1;
   usbHandleArray[a].magic = 0x75736220; //usb
   usbHandleArray[a].index = a;

   return &usbHandleArray[a];
}

