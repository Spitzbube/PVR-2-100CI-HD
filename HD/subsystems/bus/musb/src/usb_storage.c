

#include <fapi/sys_services.h>
#include <fapi/drv_usb.h>
#include <fapi/drv_usb_volume.h>
#include <fapi/drv_dma.h>
#include "musb/list_49cd8c.h"
#include "musb/usb_ch9.h"
#include "musb/musbdefs.h"
#include "musb/musb_dma.h"
#include "musb/musb.h"
#include "musb/usb_storage.h"


/*
 *      SCSI opcodes
 */

#define INQUIRY               0x12


int usb_storage_init(void*, struct MGC*, MUSB_Device*, char*);
void usb_storage_deinit(void*, struct MGC*, MUSB_Device*);
void MUSB_StorageDriver_DeviceRequestSetConfigurationComplete(void* pContext, MUSB_ControlIrp* b);
void MUSB_StorageDriver_BulkStatusComplete(void* pContext, MUSB_Irp* b);
int MUSB_StorageDriver_DeviceRequestGetMaxLun(USB_StorageBulkOnly* a, 
      MUSB_Device* b);
int MUSB_StorageDriver_SendCBW(USB_StorageBulkOnly* a1, void* c, 
                  char lun, void* pCB, int bCBLength, 
                  void* pDataBuffer, int dwDataLength, int bHostTx, 
                  void (*pfBulkDataTransferComplete)(void* a, 
                            void*, int c,
                            int bUsbProtocolError, 
                            int bCSWStatus, 
                            int f, int g), 
                  char j, int k);
MGC_Pipe* func_21ccc5a8(struct MGC* a, 
      MUSB_DeviceEndpoint* b, void* c);
int func_21ccc110(MGC_Pipe* a);
void func_21ccc2b4(struct MGC* a, MUSB_Device* b);
void MUSB_StorageDriver_DeviceRequestComplete(void* pContext, MUSB_ControlIrp* b);
int MUSB_StartTransfer(MUSB_Irp* a);
int func_21ccf514(MGC_Pipe* a);
void MUSB_StorageDriver_BulkCommandComplete(void* pContext, MUSB_Irp* b);
void MUSB_StorageDriver_BulkDataTransferComplete(void* pContext, MUSB_Irp* b);
int MUSB_StorageDriver_ReadBlocks(void* a, 
               int lba, int r8, int numBlocks, void* pDataBuffer, 
               void (*complete)(FDRV_USB_VOLUME_S* a, unsigned short b),
               char r17); //Rx 
int func_4984A4(void* a1, void (*b)(FDRV_USB_VOLUME_S* a, int b));
int func_49843C(void* a1, void* b);
int func_4988B0(void* a1, int r9, int r8, int r13, 
               void* r16, int r5, 
               void (*complete)(FDRV_USB_VOLUME_S* a, unsigned short b), char r18); //Tx
int func_498300(void* a1);
int func_498380(void* a1, int b, char* c);
int func_49842C(void* a1);
int func_498138(void* a1);
int func_498744(void* a1);
int func_4981B4(void* a1);
int func_498238(void* a1, void (*b)(FDRV_USB_VOLUME_S*));
Struct_499b0c* func_21bf8e00(int a);
void func_21bf8178(Struct_499b0c* a);
void MUSB_StorageDriver_SendInquiryCommand(Struct_499b0c* a, USB_StorageBulkOnly* b, char c);
int func_21bf8c58(Struct_499b0c* a, 
       void (*b)(Struct_48b494_Inner1* a, int b), 
       Struct_48b494_Inner1* c, 
       USB_StorageBulkOnly* d, 
       char e);
int func_21bf8bcc(Struct_499b0c* a, USB_StorageBulkOnly* b, char c);
int MUSB_StorageDriver_CreateRead10CommandBlock(Struct_499b0c* a,
       char* CB,
       int,
       unsigned lba, 
       int,
       int length,
       char);
int func_21bf81e4(Struct_499b0c* a, 
       void* b,
       int c,
       int d, 
       int e,
       int f,
       char g);
int func_21bf8238(Struct_499b0c* a, USB_StorageBulkOnly* b, char d);
int func_21bf8240(Struct_499b0c*, USB_StorageBulkOnly*, int, char);
int func_21bf8248(Struct_499b0c* a, USB_StorageBulkOnly* b, char c, Struct_499b0c* d);
void func_21bf8250(int a);
int func_21bf8254(Struct_499b0c* a, USB_StorageBulkOnly* b, char d); 
void MUSB_StorageDriver_ReceiveInquiryResponse(
      void* a, 
      void* b, int c,
      int bUsbProtocolError, 
      int bCSWStatus, 
      int f, int g);
int func_498698(USB_StorageBulkOnly* a);



static const char usb_storage_filter_bytes[] = //21deec7c
{
   0x06, USB_CLASS_PER_INTERFACE,
   0x05, 0,
   0x06, USB_CLASS_MASS_STORAGE/*bInterfaceClass*/,
   0x07, 0x06/*bInterfaceSubClass = SCSI transparent command set*/,
   0x08, 0x50/*bInterfaceProtocol = Bulk-Only Transport*/,
   0x09, //end tag
};

static Struct_48b494_Inner3 Data_21efa330 = //21efa330
{
   func_21bf8e00, //499b0c,
   func_21bf8178, //499b7c,
   MUSB_StorageDriver_SendInquiryCommand, //21bf8d24
   func_21bf8c58, //499d64,
   func_21bf8bcc, //499e84,
   MUSB_StorageDriver_CreateRead10CommandBlock, //21bf8190
   func_21bf81e4, //499d1c,
   func_21bf8238, //499cc4,
   func_21bf8240, //499ccc,
   func_21bf8248, //499a0c,
   func_21bf8250, //499e80,
   func_21bf8254, //499a14,
};


static Struct_48b494_Inner3 Data_21efa360 = //21efa360
{
#if 0
      //TODO
   func_21bf8e00,
   func_21bf8178,
   MUSB_StorageDriver_SendInquiryCommand,
   func_21bf8c58,
   func_21bf8bcc,
   MUSB_StorageDriver_CreateRead10CommandBlock,
   func_21bf81e4,
   func_21bf8238,
   func_21bf8240,
   func_21bf8248,
   func_21bf8250,
   func_21bf8254,
#endif
};



// Mass Storage Class, Bulk-Only Transport (MSC BOT) Device Driver
static MUSB_DeviceDriver Data_601944 = //601944 
{
#if 0
      //TODO
   0,
   1,
   &Data_601940,
   MUSB_StorageDriver_Init,
   func_49D298,
   0,
   0,
#endif
};

static const MUSB_DeviceRequest MUSB_StorageDriver_rDeviceRequestGetMaxLun = //598A78 -256
{
   0xA1,
   USB_BULK_GET_MAX_LUN_REQUEST,
   0,
   0,
   1,   
};

static const MUSB_DeviceRequest MUSB_StorageDriver_rDeviceRequestGetStringDescriptor = //598a80
{
   USB_DIR_IN|USB_TYPE_STANDARD|USB_RECIP_DEVICE, //0x80
   USB_REQ_GET_DESCRIPTOR,
   USB_DT_STRING << 8,
   0,
   32,   
};

static const MUSB_DeviceRequest Data_598a88 = //598a88
{
   0x21,
   USB_BULK_RESET_REQUEST,
   0,
   0,
   1,   
};

static const MUSB_DeviceRequest Data_598a90 = //598a90 
{
   USB_DIR_OUT|USB_TYPE_STANDARD|USB_RECIP_ENDPOINT, //0x02
   USB_REQ_CLEAR_FEATURE,
   0,
   0,
   0,   
};





static uint32_t timerResolutions[1] = {10}; //21efa32c

// Mass Storage Class, Bulk-Only Transport (MSC BOT) Device Driver
static MUSB_DeviceDriver usb_storage_driver = //21efa310
{
   NULL, //void* pPrivateData; //0
   sizeof(timerResolutions)/sizeof(uint32_t), //uint8_t bTimerCount; //4
   timerResolutions, //uint32_t* adwTimerResolutions; //8
   usb_storage_init, //int (*pfDeviceConnected)(void* a,
   //          struct MGC* b, 
   //          MUSB_Device* c, char* d); //12
   usb_storage_deinit, //void (*pfDeviceDisconnected)(void* a,
   //          struct MGC* b, 
   //          MUSB_Device* c); //16
   NULL, //void (*pfBusSuspended)(void); //20
   NULL, //void (*pfBusResumed)(void); //24
   //28
};



/* 21bf6c00 - complete */
int usb_storage_get_filter(uint8_t* a, unsigned short b)
{
   int len = (b > 10)? 11: b;
   
   musb_memcpy(a, &usb_storage_filter_bytes, len);
   
   return len;
}


/* 21bf816c - complete */
/* v3.8: 491804 - todo */
Struct_48b494_Inner3* func_21bf816c(void)
{
   return &Data_21efa330;   
}


/* 21bf8e48 - complete */
Struct_48b494_Inner3* func_21bf8e48(void)
{
   return &Data_21efa360;   
}
  

/* 21bf6c28 - todo */
/* v3.8: 48B6D4 - todo */
void func_21bf6c28(void)
{
   FAPI_SYS_PRINT_MSG("TODO: usb_storage_driver.c: func_21bf6c28\n");
   
}


/* 21bf7288 - todo */
/* v3.8: 48b248 - todo */
int func_21bf7288(USB_StorageBulkOnly* a, char lun, void* c)
{
//   FAPI_SYS_PRINT_MSG("func_21bf7288\n");
   
   char r2;
   char r0;
   Struct_48b494_Inner1* r16 = &a->arLUN[lun];
   
   if ((a == 0) || (a != a->Data_32)) return 0;   

   musb_memcpy(&r16->Data_112, c, sizeof(MUSB_HfiDeviceInfo));

   r16->Data_112.idVendor = a->Data_120->DeviceDescriptor.idVendor;
   r16->Data_112.idProduct = a->Data_120->DeviceDescriptor.idProduct;
   r16->Data_112.bcdDevice = a->Data_120->DeviceDescriptor.bcdDevice;
   r16->Data_112.bDeviceAddress = a->Data_120->bBusAddress;
   
#if 0
   FAPI_SYS_PRINT_MSG("func_21bf7288: idVendor=0x%x idProduct=0x%x bcdDevice=0x%x bDeviceAddress=%d\n",
         r16->Data_112.idVendor, 
         r16->Data_112.idProduct, 
         r16->Data_112.bcdDevice, 
         r16->Data_112.bDeviceAddress);
#endif

   musb_memset(r16->Data_112.arData_92, 0, 31*sizeof(unsigned short));
//      sizeof(r16->Data_112.arData_92));

   r0 = ((a->arDeviceResponseBuffer[0] >> 1) > 31)? 
      31: 
      a->arDeviceResponseBuffer[0] >> 1;
      
   for (r2 = 0; r2 < r0; r2++)
   {
      r16->Data_112.arData_92[r2] = 
         (a->arDeviceResponseBuffer[3 + r2*2] << 8) | 
         a->arDeviceResponseBuffer[2 + r2*2];
   }
   r16->Data_112.arData_92[r2] = 0;

   if ((r16->bData_545 == 0) && (lun < (a->bNumLUNs - 1)))
   {
      (a->Data_140->SendInquiry)(r16->b, a, lun + 1);
   }

   return 1;
}


/* 21bf6c30 - todo */
/* v3.8: 48b3c8 - complete */
void func_21bf6c30(USB_StorageBulkOnly* a, uint8_t lun, uint8_t c)
{
//   FAPI_SYS_PRINT_MSG("TODO: usb_storage_driver.c: func_21bf6c30\n");
      
   Struct_48b494_Inner1* r3 = &a->arLUN[lun];

   if ((a != 0) && (a == a->Data_32))
   {
      r3->bData_551 = c;
   }
}


/* 21bf7078 - todo */
/* v3.8: 48b410 - complete  */
int func_21bf7078(USB_StorageBulkOnly* a, char lun, void* c)
{
//   FAPI_SYS_PRINT_MSG("TODO: usb_storage_driver.c: func_21bf7078\n");
   
   Struct_48b494_Inner1* r3 = &a->arLUN[lun];

   if ((a == 0) || (a != a->Data_32)) return 0;

   r3->bData_549 = 1;

   musb_memcpy(&r3->Data_392, c, sizeof(MUSB_HfiMediumInfo));

   return 1;
}


/* 21bf73c0 - todo */
/* v3.8: 48b494 - complete */
void func_21bf73c0(USB_StorageBulkOnly* a, char lun, char c)
{
//   FAPI_SYS_PRINT_MSG("usb_storage_driver.c: func_21bf73c0\n");

   Struct_48b494_Inner1* pLun = &a->arLUN[lun];
   
   if ((a == 0) || (a != a->Data_32)) return;

   pLun->bActive = c;

   if (c != 0)
   {
      //21bf73f8
      if (pLun->bData_550 == 0)
      {
         //21bf7404
         if (pLun->bData_545 != 0)
         {
            //21bf7410
            if ((pLun->bData_549 != 0) && (pLun->bData_552 == 0))
            {
               pLun->bData_552 = 1;
               
               func_21c3fa04(pLun->Data_532, &pLun->Data_392);
            }
            //->21bf743c
         }
         else
         {
            //21bf74ac
            pLun->bData_545 = 1;
            
            if (0 != func_21c402a8(&pLun->Data_532, 
               &pLun->Data_112, 
               &pLun->Data_472)) //->drvusb.c
            {
               pLun->bActive = 0;
            }

            if (pLun->bData_549 != 0)
            {
               pLun->bData_552 = 1;
               
               func_21c3fa04(pLun->Data_532, &pLun->Data_392);
            }
         }
      } //if (pLun->bData_550 == 0)
      else
      {
         //21bf7490
         pLun->bData_550 = 0;
         
         if (pLun->Data_536 != 0)
         {
            (pLun->Data_536)[0] = 1;
         }
      }
   } //if (c != 0)
   else
   {
      //21bf745c
      if ((pLun->bData_550 == 0) && (pLun->bData_545 != 0))
      {
         //21bf7474
         if (pLun->bData_552 != 0)
         {
            //21bf74f0
            func_21c3fa94(pLun->Data_532);
         }
         //21bf7480
         pLun->bData_549 = 0;
         pLun->bData_552 = 0;
      }
   }
   //21bf743c
   if (pLun->Data_528 != 0)
   {
      (pLun->Data_528)(pLun->Data_532);
      
      pLun->Data_528 = 0;
   }
}


/* 21bf70e8 - complete */
void* func_21bf70e8(MUSB_Device* a,
                struct MGC* b, 
                struct usb_interface_descriptor* pInterface, 
                struct usb_endpoint_descriptor* pBulkIn, 
                struct usb_endpoint_descriptor* pBulkOut, 
                Struct_48b494_Inner3* f) 
{
//   FAPI_SYS_PRINT_MSG("func_21bf70e8\n");

   USB_StorageBulkOnly* r13;

   MGC_Pipe* r20;
   MGC_Pipe* r21;
   
   MUSB_DeviceEndpoint fp16;
   MUSB_DeviceEndpoint fp32;
      
   fp16.pDevice = a;
   fp32.pDevice = a;
   
   if (a->ConnectionSpeed == 3)
   {
      fp16.wNakLimit = 16;
   }
   else
   {
      fp16.wNakLimit = 255;
   }
   if (a->ConnectionSpeed == 3)
   {
      fp32.wNakLimit = 16;
   }
   else
   {
      fp32.wNakLimit = 255;
   }
      
   musb_memcpy(&fp16.UsbDescriptor, (void*) pBulkIn, 8);
   musb_memcpy(&fp32.UsbDescriptor, (void*) pBulkOut, 8);
   
   r20 = func_21ccc5a8(b, &fp16, 0);
   if (r20 == 0)    
   {
      return 0;
   }

   r21 = func_21ccc5a8(b, &fp32, 0);
   if (r21 == 0) 
   {
      func_21ccc110(r20);
      return 0;   
   }

   r13 = FAPI_SYS_MALLOC(sizeof(USB_StorageBulkOnly));
   if (r13 == 0)
   {
      func_21ccc110(r20);
      func_21ccc110(r21);
      
      return 0;   
   }

   musb_memset(r13, 0, sizeof(USB_StorageBulkOnly));

   r13->Data_32 = r13;
   r13->Data_120 = a;
   r13->Data_124 = b;
   r13->pInterfaceDescriptor = pInterface;
   r13->Data_140 = f;
   r13->pBulkInEpDescriptor = pBulkIn;
   r13->pBulkOutEpDescriptor = pBulkOut;
   r13->pBulkIn = r20;
   r13->pBulkOut = r21;
   
   r13->BulkStatus.hPipe = r20;
   r13->BulkStatus.pBuffer = &r13->CSW;
   r13->BulkStatus.dwLength = 13; //sizeof(CSW)
   r13->BulkStatus.pfIrpComplete = MUSB_StorageDriver_BulkStatusComplete;
   r13->BulkStatus.pCompleteParam = r13;
   
   r13->Data_0 = r13;
   r13->pfGetMaxLun = MUSB_StorageDriver_DeviceRequestGetMaxLun;
   r13->Func_8 = func_21bf6c28;
   r13->pfSendCBW = MUSB_StorageDriver_SendCBW;
   r13->Func_16 = func_21bf7288;
   r13->Func_20 = func_21bf7078;
   r13->Func_24 = func_21bf73c0;
   r13->Func_28 = func_21bf6c30;
   
   return r13;
}


/* 498a94 - complete */
static Struct_498a94* func_498a94(MUSB_Device* a)
{
//   FAPI_SYS_PRINT_MSG("TODO: func_498a94\n");

   Struct_498a94* r13 = FAPI_SYS_MALLOC(sizeof(Struct_498a94));
   if (r13 == 0) 
   {
      return 0;   
   }
   
   musb_memset(r13, 0, sizeof(Struct_498a94));
   
   r13->pDeviceDriver = &Data_601944; //func_491810();
   
   return r13;
}


/* v3.8: 498a0c - complete */
static int MUSB_StorageDriver_DeviceRequestSetConfiguration(Struct_498a94* a)
{
   MUSB_DeviceRequest* pDeviceRequest = &a->device_request;
   MUSB_ControlIrp* pUrb = &a->control_urb;

   pDeviceRequest->bmRequestType = 0;
   pDeviceRequest->bRequest = USB_REQ_SET_CONFIGURATION;
   pDeviceRequest->wValue = //1;
      ((struct usb_config_descriptor**)a->Data_12->apConfigDescriptors)[0]->bConfigurationValue;
   pDeviceRequest->wIndex = 0;
   pDeviceRequest->wLength = 0;
   
   pUrb->pDevice = a->Data_12;
   pUrb->pOutBuffer = pDeviceRequest;
   pUrb->dwOutLength = sizeof(MUSB_DeviceRequest);
   pUrb->pInBuffer = 0;
   pUrb->dwInLength = 0;
   pUrb->dwStatus = 0;
   pUrb->dwActualOutLength = 0;
   pUrb->dwActualInLength = 0;
   pUrb->pfIrpComplete = MUSB_StorageDriver_DeviceRequestSetConfigurationComplete;
   pUrb->pCompleteParam = a;
   
   return MUSB_StartControlTransaction(a->Data_12->pPort, pUrb);
}


/* 21bf6b44 - complete */
/* v3.8: 498AF8 - complete */
void MUSB_StorageDriver_DeviceRequestSetConfigurationComplete(void* pContext, MUSB_ControlIrp* b)
{
   FAPI_SYS_PRINT_MSG("MUSB_StorageDriver_DeviceRequestSetConfigurationComplete\n");

   Struct_498a94* a = pContext;

   if (b->dwStatus == 0) 
   {
      a->Data_12->pCurrentConfiguration = 
         ((void**)a->Data_12->apConfigDescriptors)[0];
         
      USB_StorageBulkOnly* r3 = a->Data_92;
      
      (r3->pfGetMaxLun)(r3->Data_0, a->Data_12);  
   }
   else 
   {
      // Cleanup
      func_21ccc2b4(a->Data_8, a->Data_12);   
   }
}

  
/* 21bf6908 - todo */
int usb_storage_init(void* a, 
      struct MGC* b, 
      MUSB_Device* r9,
      char* d)
{
//   FAPI_SYS_PRINT_MSG("TODO: usb_storage_init\n");

   struct usb_endpoint_descriptor* pBulkOut;
   struct usb_endpoint_descriptor* pBulkIn;
   char iInterface, iEndpoint;
   USB_StorageBulkOnly* r17 = 0;   
   struct usb_config_descriptor* pConfig = ((void**)r9->apConfigDescriptors)[0];
   char res = 0;
   
   for (iInterface = 0; iInterface < pConfig->bNumInterfaces; iInterface++) 
   {
      struct usb_interface_descriptor* pInterface = 
         mgc_find_interface_descriptor((void*) pConfig, iInterface, 0);
      
      if ((pInterface != 0) && 
         (pInterface->bInterfaceClass == 8/*Mass Storage*/) && 
         (pInterface->bNumEndpoints >= 2))
      {
         char bInterfaceSubClass = pInterface->bInterfaceSubClass;
         char bInterfaceProtocol = pInterface->bInterfaceProtocol;         
         
         pBulkIn = pBulkOut = 0;
         
         if (bInterfaceProtocol == 0x50/*Bulk-Only Transport*/) 
         {
            for (iEndpoint = 0; iEndpoint < pInterface->bNumEndpoints; iEndpoint++) 
            {
               struct usb_endpoint_descriptor* pEndpoint =
                  mgc_find_endpoint_descriptor(pConfig, pInterface, iEndpoint);
               
               if ((pEndpoint != 0) &&
                  ((pEndpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == 
                     USB_ENDPOINT_XFER_BULK))
               {
                  if (pEndpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) 
                  {
                     pBulkIn = pEndpoint;   
                  }
                  else 
                  {
                     pBulkOut = pEndpoint;   
                  }
               }
            } //for (iEndpoint = 0; iEndpoint < pInterface->bNumEndpoints; iEndpoint++)

            if ((pBulkIn != 0) && (pBulkOut != 0))
            {
               //Struct_48b494_Inner3* ip;
               
               if (bInterfaceSubClass == 5)
               {
                  //21bf6b2c
                  //ip = func_21bf8e48();
                  //->21bf6a20
                  r17 = func_21bf70e8(r9, b, pInterface, pBulkIn, pBulkOut, func_21bf8e48());      
               }
               else if (bInterfaceSubClass == 6/*SCSI transparent command set*/)
               {
                  //ip = func_21bf816c();
                  r17 = func_21bf70e8(r9, b, pInterface, pBulkIn, pBulkOut, func_21bf816c());      
               }  
#if 0
               else
               {
                  continue;
               }
               
               r17 = func_21bf70e8(r9, b, pInterface, pBulkIn, pBulkOut, ip);
#endif
            }
         } //if (bInterfaceProtocol == 0x50/*Bulk-Only Transport*/) 
      }
   } //for (char iInterface = 0; iInterface < pConfig->bNumInterfaces; iInterface++)

   if (r17 != 0) 
   {
      Struct_498a94* r4 = func_498a94(r9);
      
      if (r4 != 0) 
      {
         r9->pDriverPrivateData = r4;
         r4->Data_92 = r17;
         r4->Data_12 = r9; 
         r4->Data_8 = b;
         
         res = MUSB_StorageDriver_DeviceRequestSetConfiguration(r4);
         if (res == 0) 
         {
            res = 1;   
         }
      }
   }

   return res;
}


/* 21bf6f94 - todo */
void func_21bf6f94(USB_StorageBulkOnly* a)
{
   FAPI_SYS_PRINT_MSG("func_21bf6f94\n");
}
  

/* 21bf68c0 - complete */
void usb_storage_deinit(void* a, struct MGC* b, MUSB_Device* c)
{
   Struct_498a94* r4 = c->pDriverPrivateData;

   if (r4->Data_8 == b)
   {
      if (r4->Data_12 == c)
      {
         func_21bf6f94(r4->Data_92);

         (FAPI_SYS_Services.freeFunc)(r4);

         c->pDriverPrivateData = NULL;
      }
   }
}


/* 21bf68b4 - complete */
void* usb_storage_get_driver(void)
{   
   return &usb_storage_driver;
}


/* 21bf7958 - todo */
/* v3.8: 48AE84 - complete */
/* Completition function when Bulk Status was received */
void MUSB_StorageDriver_BulkStatusComplete(void* pContext, 
                                           MUSB_Irp* pUrb)
{
   USB_StorageBulkOnly* pStorageBulkOnly = pContext;
   Struct_48b494_Inner1* r2 = pStorageBulkOnly->Data_204;
   
   #if 0
   FAPI_SYS_PRINT_MSG("MUSB_StorageDriver_BulkStatusComplete: pUrb->dwStatus=%d pUrb->dwSize=%d\n",
      pUrb->dwStatus, pUrb->dwSize);
   #endif
   
   if ((pStorageBulkOnly != 0) && (pStorageBulkOnly == pStorageBulkOnly->Data_32))
   {
      r2->Data_540 = 0;
      
      while (1) 
      {
         if (pUrb->dwStatus != 0) 
         {
            r2->bUsbProtocolError = 1;
         
            if ((pUrb->dwStatus == 0x80) && 
               (pStorageBulkOnly->bData_220 == 0))
            {
               pStorageBulkOnly->bData_220 = 1;
            
               // USB_REQ_CLEAR_FEATURE to USB_RECIP_ENDPOINT
               musb_memcpy(pStorageBulkOnly->arDeviceRequestBuffer, 
                  (void*) &Data_598a90, 
                  sizeof(MUSB_DeviceRequest));
   
               pStorageBulkOnly->arDeviceRequestBuffer[4] = pStorageBulkOnly->pBulkInEpDescriptor->bEndpointAddress;         
               pStorageBulkOnly->ControlUrb.pInBuffer = 0;
               pStorageBulkOnly->ControlUrb.dwInLength = 0;
            
               MUSB_StartControlTransaction(pStorageBulkOnly->ControlUrb.pDevice->pPort, &pStorageBulkOnly->ControlUrb);
            }
            else 
            {
               //48af34
               pStorageBulkOnly->bData_220 = 0;
               //->48afa8
               func_498698(pStorageBulkOnly);
            }
            //->48afe8 
         } //if (pUrb->dwStatus != 0)
         else if (pUrb->dwActualLength == 13/*sizeof(struct CSW)*/)
         {
            if (pStorageBulkOnly->CSW.dTag != r2->CBW.dTag) 
            {
               if (pStorageBulkOnly->bData_220 != 0) 
               {
                  r2->Data_540 = 4;
               
                  MUSB_StartTransfer(&pStorageBulkOnly->BulkStatus);
               }
               else 
               {
                  //48af78
                  r2->bUsbProtocolError = 1;
               
                  func_498698(pStorageBulkOnly);
               }
               //48af88
               pStorageBulkOnly->bData_220 = 0;
               //->48afe8
            } //if (pStorageBulkOnly->CSW.dTag != r2->CBW.dTag)
            else 
            {
               //48af90
               if (pStorageBulkOnly->CSW.bStatus == 2/*Phase Error*/) 
               {
                  //48af9c
                  pStorageBulkOnly->bData_220 = 0;
                  r2->bUsbProtocolError = 1;  
               
                  func_498698(pStorageBulkOnly);
                  //->48afe8 
               } //if (pStorageBulkOnly->CSW.bStatus == 2)
               else 
               {
                  //->48afb4 - success
                  break;
               }
            }
         } //else if (pUrb->dwSize == 13/*sizeof(struct CSW)*/)
         else 
         {
            //48af9c - CSW has incorrect length
            pStorageBulkOnly->bData_220 = 0;
            r2->bUsbProtocolError = 1;  
         
            func_498698(pStorageBulkOnly);
         }
         //->48afe8 
         return;
      } //while (1)
      //48afb4 - success
      pStorageBulkOnly->bData_220 = 0;
      r2->bData_547 = 0;
      
      if (r2->pfBulkDataTransferComplete != 0) 
      {
          (r2->pfBulkDataTransferComplete)(
            r2->Data_12, 
            r2->BulkDataTransfer.pBuffer, 
            r2->BulkDataTransfer.dwActualLength,
            r2->bUsbProtocolError, 
            pStorageBulkOnly->CSW.bStatus, 
            0, 0);                  
      }
   }
}


/* 21bf77cc - complete */
/* v3.8: 48B604 - complete */
/* usb_stor_Bulk_max_lun */
/* Determine what the maximum LUN supported is */
int MUSB_StorageDriver_DeviceRequestGetMaxLun(USB_StorageBulkOnly* a, 
      MUSB_Device* b)
{
   FAPI_SYS_PRINT_MSG("MUSB_StorageDriver_DeviceRequestGetMaxLun\n");

   if ((a != 0) && (a == a->Data_32))
   {
      musb_memcpy(&a->arDeviceRequestBuffer, 
         (void*) &MUSB_StorageDriver_rDeviceRequestGetMaxLun, 
         sizeof(MUSB_DeviceRequest));  
      
      a->arDeviceRequestBuffer[4] = a->pInterfaceDescriptor->bInterfaceNumber;
      
      a->ControlUrb.pDevice = b;
      a->ControlUrb.pOutBuffer = &a->arDeviceRequestBuffer;
      a->ControlUrb.dwOutLength = sizeof(MUSB_DeviceRequest);
      a->ControlUrb.pInBuffer = a->arDeviceResponseBuffer;
      a->ControlUrb.dwInLength = 1;
      a->ControlUrb.pfIrpComplete = MUSB_StorageDriver_DeviceRequestComplete;
      a->ControlUrb.pCompleteParam = a;
      
      a->bGetMaxLunActive = 1;
      
      if (0 == MUSB_StartControlTransaction(b->pPort, &a->ControlUrb))
      {
         return 1;
      }
   }

   return 0;
}


/* 21bf74fc - todo */
/* v3.8: 48b0d8 - complete */
int MUSB_StorageDriver_SendCBW(USB_StorageBulkOnly* a1, void* c, 
                  char lun, void* pCB, int bCBLength, 
                  void* pDataBuffer, int dwDataLength, int bHostTx, 
                  void (*pfBulkDataTransferComplete)(void* a, 
                            void*, int c,
                            int bUsbProtocolError, 
                            int bCSWStatus, 
                            int f, int g), 
                  char bDMA, int x)
{   
   FAPI_SYS_PRINT_MSG("MUSB_StorageDriver_SendCBW\n");

   int bCBLength_ = (bCBLength > 16)? 16: bCBLength;
   
   USB_StorageBulkOnly* a = a1;

   if ((a == 0) || (a != a->Data_32)) label1: return 0;

   Struct_48b494_Inner1* r19 = &a->arLUN[lun];
  
   if (r19->bData_547 != 0) goto label1;//return 0;

   r19->bData_547 = 1;
   r19->bUsbProtocolError = 0;
   
   r19->Data_540 = 1;
   a->dCBWTag++;
   r19->CBW.dTag = a->dCBWTag;
   r19->CBW.dDataTransferLength = dwDataLength;

   r19->CBW.bmFlags = (bHostTx != 0)? 
      0 /*Data-Out from host to the device*/: 
      (1 << 7)/*Data-In from the device to the host*/;
   r19->CBW.bLUN = lun;
   r19->CBW.bCBLength = bCBLength_;
   
   r19->Data_540 = 1;
   
   musb_memcpy(r19->CBW.CB, pCB, bCBLength_);
   
   r19->BulkCommand.pCompleteParam = r19;
   r19->pfBulkDataTransferComplete = pfBulkDataTransferComplete;
   r19->Data_12 = c;
   r19->BulkDataTransfer.pBuffer = pDataBuffer;
   r19->BulkDataTransfer.dwLength = dwDataLength;
   r19->BulkCommand.bAllowDma = 0;
   r19->BulkDataTransfer.bAllowDma = bDMA;

   a->BulkStatus.bAllowDma = 0;
   a->Data_204 = r19;

   return (0 == MUSB_StartTransfer(&r19->BulkCommand))? 1: 0;
}


/* 4987c4 - complete */
static inline void func_4987c4(Struct_48b494_Inner1* a)
{
//   FAPI_SYS_PRINT_MSG("TODO: func_4987c4\n");

   a->CBW.dSignature = USB_BULK_CB_SIGN;
   a->CBW.dTag = 1; 
   
   a->BulkCommand.hPipe = a->a->pBulkOut;
   a->BulkCommand.pBuffer = &a->CBW;
   a->BulkCommand.dwLength = 31;
   a->BulkCommand.pfIrpComplete = MUSB_StorageDriver_BulkCommandComplete;
   a->BulkCommand.pCompleteParam = a;
   
   a->BulkDataTransfer.pfIrpComplete = MUSB_StorageDriver_BulkDataTransferComplete;
   a->BulkDataTransfer.pCompleteParam = a;
   
   a->Data_472.pPrivateData = a;
   a->Data_472.pfMountVolume = func_4984A4;
   a->Data_472.pfGetMediumInfo = func_49843C;
   a->Data_472.pfReadDevice = MUSB_StorageDriver_ReadBlocks;
   a->Data_472.pfWriteDevice = func_4988B0;
   a->Data_472.pfFlushDevice = func_498300;
   a->Data_472.pfFormatMedium = func_498380;
   a->Data_472.pfGetFormatProgress = func_49842C;
   a->Data_472.pfAbortFormat = func_498138;
   a->Data_472.pfUnmountVolume = func_498744;
   a->Data_472.pfCheckMedium = func_4981B4;
   a->Data_472.pfCheckMediumNotify = func_498238;
}


/* 21bf7c48 - todo */
/* v3.8: 48AB3C - complete */
/* Completion function when Control Transfer was successful */
void MUSB_StorageDriver_DeviceRequestComplete(void* pContext, MUSB_ControlIrp* b)
{
   char i;
   Struct_48b494_Inner1* pLUN;
   USB_StorageBulkOnly* a = pContext;
   
   if ((a == 0) || (a != a->Data_32)) return;
   
#if 1
   FAPI_SYS_PRINT_MSG("MUSB_StorageDriver_DeviceRequestComplete\n");
   FAPI_SYS_PRINT_MSG("\ta->bGetMaxLunActive=%d a->bData_218=%d a->bData_219=%d a->bData_220=%d a->Data_212=%d\n",
         a->bGetMaxLunActive, a->bData_218, a->bData_219, a->bData_220, a->Data_212);
#endif

   if (a->bGetMaxLunActive != 0) 
   {
      //21bf7c78
      a->bGetMaxLunActive = 0;
      a->bNumLUNs = (b->dwStatus == 0)? a->arDeviceResponseBuffer[0] + 1: 
         /* in case of STALL -> no LUNs (one default) */ 1;          
      
      if (a->Data_120->DeviceDescriptor.iSerialNumber != 0)
      {
         //21bf7fd4
         a->bData_218 = 1;
         
         // String 0: get the language codes that the device supports
         
         musb_memcpy(a->arDeviceRequestBuffer, 
            (void*) &MUSB_StorageDriver_rDeviceRequestGetStringDescriptor, 
            sizeof(MUSB_DeviceRequest));
      
         a->ControlUrb.dwInLength = 32;

         MUSB_StartControlTransaction(a->Data_120->pPort, &a->ControlUrb);
      }
      else
      {
         //21bf7cac
         a->bData_219 = 0;
         
         a->arLUN = FAPI_SYS_MALLOC(sizeof(Struct_48b494_Inner1) * a->bNumLUNs);
               
         if (a->arLUN != 0) 
         {
            //21bf7ce0
            musb_memset(a->arLUN, 0, 
               a->bNumLUNs * sizeof(Struct_48b494_Inner1));
            
            for (i = 0; i < a->bNumLUNs; i++) 
            {
               //21bf7d0c
               pLUN = &a->arLUN[i];
               
               pLUN->b = (a->Data_140->a)(a->bNumLUNs); 
               
               if (pLUN->b != 0) 
               {
                  pLUN->a = a;   
                  pLUN->bLun = i;
               
                  // setup LUN driver
                  func_4987c4(pLUN);
               }
            } //for (i = 0; i < a->bData_217; i++)
            //21bf7ddc
            (a->Data_140->SendInquiry)(pLUN->b, a, 0);
         }
      }   
   }
   //21bf7df8
   else if (a->bData_218 != 0) 
   {
      //21bf800c
      a->bData_218 = 0;
      a->bData_219 = 1;
      
      a->arDeviceRequestBuffer[2] = a->Data_120->DeviceDescriptor.iSerialNumber;
      
      musb_memcpy(&a->arDeviceRequestBuffer[4], 
         (void*) &a->arDeviceResponseBuffer[2], 
         2); 

      MUSB_StartControlTransaction(a->Data_120->pPort, &a->ControlUrb);
   }
   //21bf7e04
   else if (a->bData_219 != 0) 
   {
      //21bf7e10
      a->bData_219 = 0;
      
      a->arLUN = FAPI_SYS_MALLOC(sizeof(Struct_48b494_Inner1) * a->bNumLUNs);
            
      if (a->arLUN != 0) 
      {
         //21bf7e44
         musb_memset(a->arLUN, 0, 
            a->bNumLUNs * sizeof(Struct_48b494_Inner1));
         //21bf7e54
         for (i = 0; i < a->bNumLUNs; i++) 
         {
            //21bf7e74
            pLUN = &a->arLUN[i];
            
            pLUN->b = (a->Data_140->a)(a->bNumLUNs); 
            
            if (pLUN->b != 0) 
            {
               pLUN->a = a;   
               pLUN->bLun = i;
            
               // setup LUN driver
               func_4987c4(pLUN);
            }
         } //for (i = 0; i < a->bData_217; i++)
         //21bf7f44
         if (pLUN->b != 0)
         {
            (a->Data_140->SendInquiry)(pLUN->b, a, 0);
         }
      }
   }
   //21bf7f68
   else if (a->bData_220 != 0) 
   {
      //21bf804c
      func_21ccf514(a->pBulkIn);
      MUSB_StartTransfer(&a->BulkStatus);
      //->48ae64
   }
   else 
   {
      //21bf7f74
      switch (a->Data_212) 
      {
         case 1:
            //21bf7f90
            a->Data_212 = 2;
            
            // USB_REQ_CLEAR_FEATURE to USB_RECIP_ENDPOINT
            musb_memcpy(a->arDeviceRequestBuffer, 
               (void*) &Data_598a90,
               sizeof(MUSB_DeviceRequest));

            a->arDeviceRequestBuffer[4] = a->pBulkInEpDescriptor->bEndpointAddress;
            a->ControlUrb.pInBuffer = 0;
            a->ControlUrb.dwInLength = 0;
            
            MUSB_StartControlTransaction(a->ControlUrb.pDevice->pPort, &a->ControlUrb);
            break;  
            
         case 2:
            //21bf80d0
            a->Data_212 = 3;
            
            // USB_REQ_CLEAR_FEATURE to USB_RECIP_ENDPOINT
            musb_memcpy(a->arDeviceRequestBuffer, 
               (void*) &Data_598a90,
               sizeof(MUSB_DeviceRequest));

            a->arDeviceRequestBuffer[4] = a->pBulkOutEpDescriptor->bEndpointAddress;
            a->ControlUrb.pInBuffer = 0;
            a->ControlUrb.dwInLength = 0;
            
            MUSB_StartControlTransaction(a->ControlUrb.pDevice->pPort, &a->ControlUrb);
            break; 
            
         case 3:
            //21bf8064
            a->Data_212 = 0;
            
            func_21ccf514(a->pBulkIn);
            func_21ccf514(a->pBulkOut);
            
            a->Data_204->bData_547 = 0;
            
            if (a->Data_204->pfBulkDataTransferComplete != 0) 
            {
               //21bf8090
               a->Data_204->bData_547 = 0;
               a->bData_220 = 0;

               switch (a->Data_204->Data_540) 
               {
                  case 2: // Data-In from the device to the host
                  case 3: // Data-Out from host to the device
                     //21bf8100
                     (a->Data_204->pfBulkDataTransferComplete)(
                        a->Data_204->Data_12,
                        a->Data_204->BulkDataTransfer.pBuffer,
                        a->Data_204->BulkDataTransfer.dwActualLength,
                        1/*bUsbProtocolError*/, 
                        1/*bCSWStatus*/, 
                        0, 0);
                     break;   

                  case 4:
                     //21bf8100
                     (a->Data_204->pfBulkDataTransferComplete)(
                        a->Data_204->Data_12,
                        a->Data_204->BulkDataTransfer.pBuffer,
                        a->Data_204->BulkDataTransfer.dwActualLength,
                        1/*bUsbProtocolError*/, 
                        1/*bCSWStatus*/, 
                        0, 0);
                     break;   
                     
                  default:
                     //21bf80a8
                     (a->Data_204->pfBulkDataTransferComplete)(
                        a->Data_204->Data_12,
                        0,
                        0,
                        1/*bUsbProtocolError*/, 
                        1/*bCSWStatus*/, 0, 0);
               } //switch (a->Data_204->Data_540)
            } //if (a->Data_204->pfBulkDataTransferComplete != 0)
            
            break;
      } //switch (a->Data_212)
   }
}


/* 21bf78d0 - todo */
/* v3.8: 48AA70 - complete */
/* Completion function when Bulk Command was send */
void MUSB_StorageDriver_BulkCommandComplete(void* pContext, 
                                            MUSB_Irp* pUrb)
{   
   Struct_48b494_Inner1* a = pContext;
   USB_StorageBulkOnly* r1 = a->a;
   
   #if 0
   FAPI_SYS_PRINT_MSG("MUSB_StorageDriver_BulkCommandComplete: pUrb->dwStatus=%d\n",
      pUrb->dwStatus);
   #endif
   
   if ((r1 != 0) && (r1->Data_32 == r1)) 
   {
      if (pUrb->dwStatus != 0) 
      {
         a->bUsbProtocolError = 1;  
         
         func_498698(r1); 
      } //if (pUrb->d != 0)
      else 
      {
         r1->Data_204 = a; 
         
         if (a->CBW.dDataTransferLength != 0) 
         {
            // The host expects a transfer on the Bulk-In or
            // Bulk-Out endpoint
            a->BulkDataTransfer.dwStatus = 0;  
            
            if (a->CBW.bmFlags & 0x80) 
            {
               // Data-In from the device to the host
               a->Data_540 = 2; 
               a->BulkDataTransfer.hPipe = r1->pBulkIn;
            }
            else 
            {
               // Data-Out from host to the device
               a->Data_540 = 3;   
               a->BulkDataTransfer.hPipe = r1->pBulkOut;
            }
            
            a->BulkDataTransfer.bAllowShortTransfer = 0;
            
            MUSB_StartTransfer(&a->BulkDataTransfer);
         } //if (a->CBW.dCBWDataTransferLength != 0)
         else 
         {
            // No data transfer between the CBW and the CSW
            MUSB_StartTransfer(&r1->BulkStatus);
         }
      }
   }  
}


/* 21bf7af0 - todo */
/* v3.8: 48AFFC - complete */
/* Completion function when Bulk Data Transfer was successful */
void MUSB_StorageDriver_BulkDataTransferComplete(void* pContext, 
                                                 MUSB_Irp* pUrb)
{  
   Struct_48b494_Inner1* a = pContext;
   USB_StorageBulkOnly* r13 = a->a;
   
   #if 1
   FAPI_SYS_PRINT_MSG("MUSB_StorageDriver_BulkDataTransferComplete: pUrb->dwStatus=%d\n",
      pUrb->dwStatus);
   #endif
   
   if ((r13 != 0) && (r13 == r13->Data_32)) 
   {
      if (pUrb->dwStatus != 0) 
      {
         a->bUsbProtocolError = 1;  
         
         if (pUrb->dwStatus == 0x80) 
         {
            r13->bData_220 = 1;
            
            if (a->CBW.bmFlags & 0x80) 
            {
               musb_memcpy(&r13->arDeviceRequestBuffer, 
                  (void*) &Data_598a90,    
                  sizeof(MUSB_DeviceRequest));
                  
               r13->arDeviceRequestBuffer[4] = r13->pBulkInEpDescriptor->bEndpointAddress;
            }
            else 
            {
               musb_memcpy(&r13->arDeviceRequestBuffer, 
                  (void*) &Data_598a90,    
                  sizeof(MUSB_DeviceRequest));

               r13->arDeviceRequestBuffer[4] = r13->pBulkOutEpDescriptor->bEndpointAddress;
            }
            
            r13->ControlUrb.pInBuffer = 0;
            r13->ControlUrb.dwInLength = 0;
            
            MUSB_StartControlTransaction(r13->ControlUrb.pDevice->pPort, &r13->ControlUrb);
            return;
         } //if (pUrb->dwStatus == 0x80)
      } //if (pUrb->dwStatus != 0)

      a->Data_540 = 4;   
      MUSB_StartTransfer(&r13->BulkStatus);
   }
}


/* 49854C - complete */
int MUSB_StorageDriver_ReadBlocks(void* a1, 
   int lba, int r8, int numBlocks, void* pDataBuffer, 
   void (*complete)(FDRV_USB_VOLUME_S* a, unsigned short b),
   char r17) 
{
   FAPI_SYS_PRINT_MSG("TODO: MUSB_StorageDriver_ReadBlocks\n");

#if 0
   int bCBLength;
   char CB[16];
   Struct_48b494_Inner1* a = a1;
   USB_StorageBulkOnly* r14 = a->a;

   #if 0
   FAPI_SYS_PRINT_MSG("usb_storage_driver.c: MUSB_StorageDriver_ReadBlocks() lun=%d lba=%d numBlocks=%d\n",
      a->bLun, lba, numBlocks);
   #endif
         
   if ((r14 == 0) || 
      (r14 != r14->Data_32) || 
      (a->bActive == 0)) 
   {
      return 3;      
   }  
   
   bCBLength = (r14->Data_140->CreateRead10CommandBlock)(   
                  a->b, 
                  CB, sizeof(CB), 
                  lba, r8, numBlocks, a->bLun); 
               
   if (bCBLength != 0) 
   {
      a->ReadWriteBlocksComplete = complete;   
            
      if (0 != MUSB_StorageDriver_SendCBW(r14, a, a->bLun, 
         &CB, bCBLength, 
         pDataBuffer, 
         numBlocks * a->Data_112.dwBlockLength, 
         0/*Rx*/, 
         MUSB_StorageDriver_ReadWriteBlocksComplete, 
         r17, 0)) 
      {
         return 0;   
      }
      else return 5;
   }
#endif
   
   return 8;
}


/* 4984A4 - complete */
int func_4984A4(void* a1, void (*b)(FDRV_USB_VOLUME_S* a, int b))
{
   FAPI_SYS_PRINT_MSG("TODO: func_4984A4\n");

#if 0
   Struct_48b494_Inner1* a = a1;
   USB_StorageBulkOnly* r3 = a->a;
   
   if (r3 == 0) return 3;
   if (r3 != r3->Data_32) return 3;
   if (a->bActive == 0) return 3;
   
   if (r3->Data_140->d != 0) 
   {
      a->Func_520 = b;   
      
      if (0 == (r3->Data_140->d)(a->b, func_498474, a, r3, a->bLun)) 
      {
         return 5;
      }
   }
   else 
   {
      (b)(a->Data_532, 1);   
   }
#endif
   
   return 0;
}


/* 49843C - complete */
int func_49843C(void* a1, void* b)
{
   FAPI_SYS_PRINT_MSG("TODO: func_49843C\n");

#if 0
   Struct_48b494_Inner1* a = a1;
   
   musb_memcpy(b, (void*)&a->Data_392, sizeof(MUSB_HfiMediumInfo));
#endif
   
   return 0;   
}


/* 4988B0 - complete */
int func_4988B0(void* a1, int r9, int r8, int r13, 
               void* pDataBuffer, int r5, 
               void (*complete)(FDRV_USB_VOLUME_S* a, unsigned short b), char r18)
{
   FAPI_SYS_PRINT_MSG("TODO: func_4988B0\n");

#if 0
   int bCBLength;
   char CB[16]; //fp16
   Struct_48b494_Inner1* a = a1;
   USB_StorageBulkOnly* r14 = a->a;
      
   if (r5 != 0) return 6;
   
   if ((r14 == 0) || 
      (r14 != r14->Data_32) || 
      (a->bActive == 0)) 
   {
      return 3;      
   }  
   
   bCBLength = (r14->Data_140->g)(a->b, &CB, sizeof(CB), r9, r8, r13, a->bLun);   
   
   if (bCBLength != 0) 
   {
      a->ReadWriteBlocksComplete = complete;   

      if (0 != MUSB_StorageDriver_SendCBW(r14, a, a->bLun, 
         &CB, bCBLength, 
         pDataBuffer, 
         r13 * a->Data_112.dwBlockLength/*dwDataLength*/, 
         1/*Tx*/, 
         MUSB_StorageDriver_ReadWriteBlocksComplete, 
         r18, 0)) 
      {
         return 0;   
      }
      else return 5;
   }
#endif
   
   return 8;
}


/* 498300 - complete */
int func_498300(void* a1)
{
   FAPI_SYS_PRINT_MSG("TODO: func_498300\n");

#if 0
   Struct_48b494_Inner1* a = a1;
   USB_StorageBulkOnly* r1 = a->a;
   
   if (r1 == 0) return 3;   
   if (r1 != r1->Data_32) return 3;
   if (a->bActive == 0) return 3;
   
   if (r1->Data_140->h != 0) 
   {
      if (0 == (r1->Data_140->h)(a->b, r1, a->bLun)) 
      {
         return 5;   
      }
   }
#endif

   return 0;
}


/* 498380 - complete */
int func_498380(void* a1, int b, char* c)
{
   FAPI_SYS_PRINT_MSG("TODO: func_498380\n");

#if 0
   Struct_48b494_Inner1* a = a1;
   USB_StorageBulkOnly* r1 = a->a;
   
   if (r1 == 0) return 3;   
   if (r1 != r1->Data_32) return 3;
   if (a->bActive == 0) return 3;

   if (a->Data_112.bData_86 != 0) 
   {
      if (0 != (r1->Data_140->i)(a->b, r1, b, a->bLun)) 
      {
         a->bData_550 = 1;
         a->Data_536 = c;   
         
         return 0;
      }
   }
#endif
   
   return 1;
}


/* 49842C - complete */
int func_49842C(void* a1)
{
   FAPI_SYS_PRINT_MSG("TODO: func_49842C\n");

#if 0
   Struct_48b494_Inner1* a = a1;
   return a->bData_551;   
#else
   return 0;
#endif
}


/* 498138 - complete */
int func_498138(void* a1)
{
   FAPI_SYS_PRINT_MSG("TODO: func_498138\n");

#if 0
   Struct_48b494_Inner1* a = a1;
   USB_StorageBulkOnly* r1 = a->a;

   if ((r1 == 0) || (r1->Data_32 != r1)) return 0;  

   if (0 != (r1->Data_140->j)(a->b, r1, a->bLun, a->b)) 
   {
      a->bData_550 = 0;
      return 0;      
   }
#endif
   
   return 1;
}


/* 498744 - complete */
int func_498744(void* a1)
{
   FAPI_SYS_PRINT_MSG("TODO: func_498744\n");

#if 0
   Struct_48b494_Inner1* a = a1;
   USB_StorageBulkOnly* r1 = a->a;
      
   if ((r1 == 0) || 
      (r1 != r1->Data_32) || 
      (a->bActive == 0)) 
   {
      return 3;      
   }  
   
   if (r1->Data_140->e != 0) 
   {
      if (0 == (r1->Data_140->e)(a->b, r1, a->bLun)) 
      {
         return 5;   
      }
   }
#endif
   
   return 0;
}


/* 4981B4 - complete */
int func_4981B4(void* a1)
{
   FAPI_SYS_PRINT_MSG("TODO: func_4981B4\n");

#if 0
   Struct_48b494_Inner1* a = a1;
   USB_StorageBulkOnly* r1 = a->a;

   if ((r1 == 0) || (r1->Data_32 != r1)) return 0;  
   
   a->Data_528 = 0;
   
   if (r1->Data_140->l != 0) 
   {
      if (0 == (r1->Data_140->l)(a->b, r1, a->bLun)) 
      {
         return 5;   
      }
   }
#endif
   
   return 0;    
}


/* 498238 - complete */
int func_498238(void* a1, void (*b)(FDRV_USB_VOLUME_S*))
{
   FAPI_SYS_PRINT_MSG("TODO: func_498238\n");

#if 0
   Struct_48b494_Inner1* a = a1;
   USB_StorageBulkOnly* r1 = a->a;
   
   if ((r1 != 0) && (r1->Data_32 == r1))
   {
      void (*b1)(FDRV_USB_VOLUME_S*) = b;
      
      a->Data_528 = b1;
   
      if (r1->Data_140->l != 0) 
      {
         if (0 == (r1->Data_140->l)(a->b, r1, a->bLun)) 
         {
            return 5;   
         }
      }
   }
#endif
   
   return 0;   
}


/* 21bf8e00 - todo */
/* v3.8: 499b0c - complete */
Struct_499b0c* func_21bf8e00(int bNumLUNs)
{
//   FAPI_SYS_PRINT_MSG("func_21bf8e00\n");

   Struct_499b0c* r13 = FAPI_SYS_MALLOC(sizeof(Struct_499b0c));
   
   if (r13 != 0) 
   {
      musb_memset(r13, 0, sizeof(Struct_499b0c)); 
      r13->bNumLUNs = bNumLUNs;  
   }
   
   return r13;
}


/* 21bf8178 - complete */
/* v3.8: 499b7c - complete */
void func_21bf8178(Struct_499b0c* a)
{
//   FAPI_SYS_PRINT_MSG("TODO: func_21bf8178\n");

   FAPI_SYS_Services.freeFunc(a);
}


/* 21bf8d24 - todo */
/* v3.8: 499b9c - complete */
void MUSB_StorageDriver_SendInquiryCommand(Struct_499b0c* a, 
                                          USB_StorageBulkOnly* b, 
                                          char bLUN)
{
   FAPI_SYS_PRINT_MSG("MUSB_StorageDriver_SendInquiryCommand\n");

   a->Data_0 = b;   
   a->bFirstLUN = bLUN;
   a->Data_940 = 0;
   a->Data_944 = 0;
   a->bReadFormatCapacitiesTry = 0;
   a->bData_957 = 0;
   a->bData_958 = 0;
   a->bData_963 = 0;
   a->nStep = 1;
   
   musb_memset(&a->arCommandBlock, 0, 16);
   a->arCommandBlock[0] = /*INQUIRY;*/ 0x12;
   a->arCommandBlock[4] = 36; // Allocation Length
   
   if ((bLUN == 0) && (a->bNumLUNs > 1)) 
   {
      a->bMoreLUNs = 1;
      a->bCurrentLUN = 0;   
   }
   else 
   {
      a->bMoreLUNs = 0;   
   }
   
   (b->pfSendCBW)(b->Data_0, 
      a,  
      bLUN, 
      &a->arCommandBlock, //void* pCB, 
      6, //int bCBLength, 
      a->arInquiryData, //void* pDataBuffer, 
      36, //int dwDataLength, 
      0/*Rx*/, //int bHostTx, 
      MUSB_StorageDriver_ReceiveInquiryResponse, 
      0, //char j, 
      0); //int k);
}


/* 21bf8c58 - todo */
/* v3.8: 499d64 - todo */
int func_21bf8c58(Struct_499b0c* a, 
       void (*b)(Struct_48b494_Inner1* a, int b), 
       Struct_48b494_Inner1* c, 
       USB_StorageBulkOnly* d, 
       char e)
{
   FAPI_SYS_PRINT_MSG("TODO: func_21bf8c58\n");

   return 0;
}


/* 499e84 - todo */
int func_21bf8bcc(Struct_499b0c* a, USB_StorageBulkOnly* b, char c)
{
   FAPI_SYS_PRINT_MSG("TODO: func_21bf8bcc\n");

   return 0;
}


/* 21bf8190 - todo */
/* v3.8: 499cd4 - todo */
int MUSB_StorageDriver_CreateRead10CommandBlock(Struct_499b0c* a,
       char* CB,
       int c,
       unsigned lba, 
       int e,
       int length,
       char g)
{
//   FAPI_SYS_PRINT_MSG("usb_storage_driver.c: MUSB_StorageDriver_CreateRead10CommandBlock\n");
   FAPI_SYS_PRINT_MSG("TODO: MUSB_StorageDriver_CreateRead10CommandBlock\n");

#if 0
   CB[0] = 0x28; //READ(10)
   CB[1] = 0;
   CB[2] = lba >> 24;
   CB[3] = lba >> 16;
   CB[4] = lba >> 8;
   CB[5] = lba;
   CB[6] = 0;
   CB[7] = length >> 8;
   CB[8] = length;
   CB[9] = 0;
#endif

   return 10;
}


/* 499d1c - todo */
int func_21bf81e4(Struct_499b0c* a, 
       void* b,
       int c,
       int d, 
       int e,
       int f,
       char g)
{
   FAPI_SYS_PRINT_MSG("TODO: func_21bf81e4\n");

   return 0;
}


/* 21bf8238 - todo */
/* v3.8: 499cc4 - complete */
int func_21bf8238(Struct_499b0c* a, USB_StorageBulkOnly* b, char c)
{
   FAPI_SYS_PRINT_MSG("TODO: func_21bf8238\n");

   return 1;
}


/* 21bf8240 - todo */
/* v3.8: 499ccc - complete */
int func_21bf8240(Struct_499b0c* a, USB_StorageBulkOnly* b, int c, char d)
{
   FAPI_SYS_PRINT_MSG("TODO: func_21bf8240\n");

   return 1;
}


/* 21bf8248 - todo */
/* v3.8: 499a0c - complete */
int func_21bf8248(Struct_499b0c* a, USB_StorageBulkOnly* b, char c, Struct_499b0c* d)
{
   FAPI_SYS_PRINT_MSG("TODO: func_21bf8248\n");

   return 1;
}


/* 21bf8250 - todo */
/* v3.8: 499e80 - complete */
void func_21bf8250(int a)
{
   FAPI_SYS_PRINT_MSG("TODO: func_21bf8250\n");

   /* empty */
}


/* 21bf8254 - todo */
/* v3.8: 499a14 - todo */
int func_21bf8254(Struct_499b0c* a, USB_StorageBulkOnly* b, char d)
{
   FAPI_SYS_PRINT_MSG("TODO: func_21bf8254\n");

   return 0;
}


/* 21bf8300 - todo */
/* v3.8: 499070 - todo */
void MUSB_StorageDriver_ReceiveInquiryResponse(
      void* a, 
      void* b, int c,
      int bUsbProtocolError/*r24*/, 
      int bCSWStatus/*fp364*/, 
      int f, int g) 
{   
   MUSB_HfiDeviceInfo fp280;
   MUSB_HfiMediumInfo fp360;

   char* r15_;
   int r22;
   int dwLastBlockAddress;
      
   Struct_499b0c* r20 = a;

   int fp368 = 0;
   int fp372 = 0;
   char opCode = 0;
   int r19 = 0;
   char r14 = 0;
   void* r16 = 0;
   int r23 = 0;
   char bRequestSense = r20->bRequestSense;
   USB_StorageBulkOnly* fp380 = r20->Data_0;
   char bLun = r20->bFirstLUN;
   //char* r25 = r20->arCommandBlock;
   
   #if 1
   FAPI_SYS_PRINT_MSG("MUSB_StorageDriver_ReceiveInquiryResponse: r20->nStep=%d\n",
      r20->nStep);
   FAPI_SYS_PRINT_MSG("\tr20->bMoreLUNs=%d r20->bNumLUNs=%d r20->bFirstLUN=%d r20->bCurrentLUN=%d\n",
      r20->bMoreLUNs,
      r20->bNumLUNs,
      r20->bFirstLUN, 
      r20->bCurrentLUN);
   FAPI_SYS_PRINT_MSG("\tbUsbProtocolError=%d bCSWStatus=%d bRequestSense=%d\n",
      bUsbProtocolError, 
      bCSWStatus,
      bRequestSense);
   
//   hex_dump("MUSB_StorageDriver_ReceiveInquiryResponse", b, c);
   #endif
      
   musb_memset(/*r25*/r20->arCommandBlock, 0, 16);

   if (r20->bRequestSense != 0) 
   {
      r20->bRequestSense = 0;
      
      if (bCSWStatus == 0) 
      {
         if (((r20->arRequestSenseData[2] & 0x0F)/*Sense Key*/ == 2/*NOT READY*/) &&
            (r20->arRequestSenseData[12]/*Additional Sense Code*/ == 4/*LOGICAL DRIVE NOT READY*/))
         {
            r20->bData_957 = 1;  
            
            if (r20->arRequestSenseData[13]/*Additional Sense Code Qualifier*/ == 2/*INITIALIZATION REQUIRED*/)  
            {
               //LOGICAL DRIVE NOT READY - INITIALIZATION REQUIRED
               r19 = 6;
               opCode = 0x1B; //START-STOP UNIT
               r20->arCommandBlock[4] = 3/*LoEj|Start*/;
            }
            else 
            {
               //4991a0
               fp372 = 1;   
            }
            //->4991f8
         }
         //4991b0
         else if (((r20->arRequestSenseData[2] & 0x0F)/*Sense Key*/ == 6/*UNIT ATTENTION*/) &&
            (r20->arRequestSenseData[12]/*Additional Sense Code*/ == 0x28/*NOT READY TO READY TRANSITION - MEDIA CHANGED*/))
         {
            fp368 = 1;
            //->4991f8
         }
         //4991d4
         else if (((r20->arRequestSenseData[2] & 0x0F)/*Sense Key*/ == 6/*UNIT ATTENTION*/) &&
            ((r20->arRequestSenseData[12]/*Additional Sense Code*/ == 0x3A/*MEDIUM NOT PRESENT*/) ||
            (r20->arRequestSenseData[12]/*Additional Sense Code*/ == 0x30/*CANNOT READ/WRITE MEDIUM*/)))
         {
            r20->bData_963 = 1;
         }
      } //if (bCSWStatus == 0)
   } //if (r20->bData_956 != 0)
   //4991f8
   if (bCSWStatus != 0) 
   {
      r20->bRequestSense = 1;
      
      if (r20->nStep == 6) 
      {
         r20->Data_944 = 400;
      }
      //499228
      opCode = 0x03; //REQUEST SENSE
      r16 = &r20->arRequestSenseData[0];
      r14 = 20;
      
      r20->arCommandBlock[0] = opCode;
      r20->arCommandBlock[4] = r14;
      
      (fp380/*r20->Data_0*/->pfSendCBW)(
         fp380/*r20->Data_0*/->Data_0, 
        r20, 
         bLun, 
         /*r25*/r20->arCommandBlock/*void* pCB*/, 
         6/*int bCBLength*/, 
         r16/*void* pDataBuffer*/, 
         20/*int dwDataLength*/, 
         0/*int bHostTx*/, 
         MUSB_StorageDriver_ReceiveInquiryResponse, 
         0/*char j*/, 
         2/*int k*/);

      //->4999C8;
   } //if (bCSWStatus != 0)
   else
   {
      //4995a0
      switch (r20->nStep) 
      {
         case 1:
            //499290
            r19 = 10;
            opCode = 0x23; //READ FORMAT CAPACITIES
            r16 = &r20->arCapacityList[0];
            r14 = 252;
            r20->nStep = 3;

            if (r20->bMoreLUNs != 0) 
            {
               bLun = r20->bCurrentLUN;
            }
            //->49958c
            break;  
                     
         case 3:
            //4992C0
            if ((bRequestSense != 0) || (bUsbProtocolError != 0)) 
            {
               //4992f0   
               musb_memset(&r20->arCapacityList[0], 0, 252);

               r20->bRequestSense = 0;
            }
            //4992f0
            if (((bRequestSense != 0) || (bUsbProtocolError != 0)) &&
               (r20->bReadFormatCapacitiesTry < 2)) 
            {
               r20->bReadFormatCapacitiesTry++;
               
               r19 = 10;
               opCode = 0x23; //READ FORMAT CAPACITIES
               r16 = &r20->arCapacityList[0];
               r14 = 252;                  
               r20->nStep = 3;
               break;
            }
            //499334
            if (r20->bMoreLUNs == 0) 
            {               
               //4993c0
               r19 = 10;
               opCode = 0x25; //READ CAPACITY
               r16 = &r20->rReadCapacityData;
               r14 = 8;
               r20->nStep = 4;
               //->49958c
               break;
            }
            else 
            {
               //499340            
               r20->nStep = 1;   
               r19 = 6;
               opCode = INQUIRY;
               r16 = &r20->arInquiryData[0];
               r14 = 36;
               
               if (r20->bCurrentLUN < (r20->bNumLUNs - 1)) 
               {
                  bLun = ++r20->bCurrentLUN;
               }
               else 
               {
                  r20->bMoreLUNs = 0;
               }
            }
            //->49958c
            break;
            
         case 4:
            //499384
            if ((bRequestSense != 0) || (bUsbProtocolError != 0)) 
            {
               //4993a0   
               musb_memset(&r20->rReadCapacityData, 0, 8);
            }
            //4993b0
            if (fp368 != 0) 
            {
               //4993c0
               r19 = 10;
               opCode = 0x25; //READ CAPACITY
               r16 = &r20->rReadCapacityData;
               r14 = 8;
               r20->nStep = 4;
               //->49958c
               break;
            }
            //4993d8
            else 
            {
               if ((r20->bData_963 == 0) && 
                  (bUsbProtocolError == 0)) 
               {
                  //4993ec
                  r19 = 6;
                  opCode = 0x00; //TEST UNIT READY
                  r20->nStep = 5;
                  //->49958c
               }
               else 
               {
                  //->499528
                  r19 = 6;
                  opCode = 0x00; //TEST UNIT READY
                  r20->nStep = 9;
                  //->49958c                     
               }
            }
            break;
            
         case 5:
            //4993FC
            if (bUsbProtocolError != 0) 
            {
               if (r20->Data_940 < 100) 
               {
                  r20->Data_940++;                  
                  r20->nStep = 5;
               }
               else 
               {
                  //499424
                  r20->Data_940 = 0;                  
                  r20->nStep = 9;
               }
               
               r19 = 6;
               opCode = 0x00; //TEST UNIT READY
               //->49958c
               break;
            }
            else 
            {
               //49943c
               if ((r20->rReadCapacityData.arLastBlockAddress[0] != 0) ||
                  (r20->rReadCapacityData.arLastBlockAddress[1] != 0) ||
                  (r20->rReadCapacityData.arLastBlockAddress[2] != 0) ||
                  (r20->rReadCapacityData.arLastBlockAddress[3] != 0)) 
               {
                  //49946c
                  fp372 = 1;
                  r23 = 1;
                  //->499530
                  r20->nStep = 9;
                  //->49958c
                  break;
               }
               else 
               {
                  //499480   
                  r20->Data_940 = 0;
                  r19 = 10;
                  opCode = 0x28; //READ(10)
                  r16 = r20->arData_12;
                  r14 = 1;
                  
                  MUSB_StorageDriver_CreateRead10CommandBlock((Struct_499b0c*)r20,
                     /*r25*/r20->arCommandBlock,
                     16,
                     0,
                     0,
                     1,
                     bLun);

                  r20->nStep = 6;
                  //->49958c
               }
            }
            break;
            
         case 6:
            //4994BC
            if (bRequestSense != 0) 
            {
               r20->bData_957 = 1;
               //->499528
            }
            //4994dc
            else if ((r20->Data_944 < 400) &&
                  (r20->rReadCapacityData.arLastBlockAddress[0] == 0) &&
                  (r20->rReadCapacityData.arLastBlockAddress[1] == 0) &&
                  (r20->rReadCapacityData.arLastBlockAddress[2] == 0) &&
                  (r20->rReadCapacityData.arLastBlockAddress[3] == 0)) 
            {
               //49951c
               r20->Data_944++;
               //->4993c0
               r19 = 10;
               opCode = 0x25; //READ CAPACITY
               r16 = &r20->rReadCapacityData;
               r14 = 8;
               r20->nStep = 4;
               //->49958c
               break;
            }
            //499528
            r19 = 6;
            opCode = 0x00; //TEST UNIT READY
            r20->nStep = 9;
            //->49958c
            break;
            
         case 9:
            //499538
            fp372 = 1;
            r23 = (r20->bData_963 != 0)? 0: 1;
            //->49958c
            break;
            
         case 10:
            //499564
            (r20->Func_4)(r20->Data_8, 1);
            //->4999c8
            return;
            //break;
            
         case 11:
            //499578
            r19 = 6;
            opCode = 0x1B; //START-STOP UNIT
            r20->nStep = 12;
            //49958c
            break;            

         case 12:
         default:
            //->4999C8
            return;
            //break;
      } //switch (r20->nStep)
      //49958c
      r20->arCommandBlock[0] = opCode;
      
      switch (r19) 
      {
         case 6:
            r20->arCommandBlock[4] = r14;
            break;
            
         case 10:
            r20->arCommandBlock[8] = r14;
            break;   
      }
      //499600
      if (fp372 != 0)  
      {  
         r15_ = r20->arInquiryData;
         
         musb_memset(&fp280, 0, sizeof(fp280));
         
         //4996a0         
         switch (r20->arInquiryData[0] & 0x1F) //Peripheral Device Type
         {
            case 0: //direct-access device (e.g., magnetic disk)
            case 14: //Simplified direct-access device (e.g., magnetic disk)
               //49964C
               r22 = 0;
               fp280.bmAccessType = 1;
               r20->bData_957 = 
                  ((r20->arInquiryData[1] & 0x80) == 0)/*RMB*/? 0: 1;
               break;
               
            case 15: //Optical card reader/writer device
               //499688
               r22 = 0;
               fp280.bmAccessType = 1;
               r20->bData_957 = 1;
               break;
               
            case 1: //sequential-access device (e.g., magnetic tape)
            case 4: //write-once device
            case 7: //optical memory device (e.g., some optical disks)
               //499694
               r22 = 1;
               fp280.bmAccessType = 2;
               r20->bData_957 = 1;
               break;

            case 5: //CDROM device
            case 6: //scanner device
               //4996F0
               r22 = 2;
               fp280.bmAccessType = 4;
               r20->bData_957 = 1;
               break;

            default:               
               //499640
               fp280.bmAccessType = 0;
               break;
         }

         fp280.MediaType = (r20->bData_957 != 0)? 2: 1;
         fp280.bCanFormat = 0;
         fp280.bHasCache = 0;
         fp280.dwBlockSize =
            (r20->rReadCapacityData.arBlockLength[0] << 24) |
            (r20->rReadCapacityData.arBlockLength[1] << 16) |
            (r20->rReadCapacityData.arBlockLength[2] << 8) |
            r20->rReadCapacityData.arBlockLength[3];
            
         if (fp280.dwBlockSize == 0)
         {
            fp280.dwBlockSize =
               (r20->arCapacityList[10] << 8) |
               r20->arCapacityList[11];
         }
         //499758
         if (fp280.dwBlockSize == 0)
         {
            fp280.dwBlockSize = 512;
         }
         //49976c
         char r3, r2;
         
         for (r3 = 16, r2 = 0; r2 < r3; r2++) 
         {
            fp280.awSerialNumber[r2] = r20->arData_576[4 + r2];
         }
         
         fp280.awSerialNumber[r2] = 0;
         
         r3 = 8;
         for (r2 = 0; r2 < r3; r2++) 
         {
            fp280.awDiskVendor[r2] = /*r20->arInquiryData*/r15_[8 + r2];
         }
         
         fp280.awDiskVendor[r2] = 0;
         
         r3 = 16;
         for (r2 = 0; r2 < r3; r2++) 
         {
            fp280.awDiskProduct[r2] = /*r20->arInquiryData*/r15_[16 + r2];
         }
         
         fp280.awDiskProduct[r2] = 0;
         
         r3 = 4;
         for (r2 = 0; r2 < r3; r2++) 
         {
            fp280.awDiskRevision[r2] = /*r20->arInquiryData*/r15_[32 + r2];
         }
         
         fp280.awDiskRevision[r2] = 0;
         
         dwLastBlockAddress = (r20->rReadCapacityData.arLastBlockAddress[0] << 24) |
            (r20->rReadCapacityData.arLastBlockAddress[1] << 16) |
            (r20->rReadCapacityData.arLastBlockAddress[2] << 8) |
            r20->rReadCapacityData.arLastBlockAddress[3];
         
         if (dwLastBlockAddress == 0)
         {
            r23 = 0;
         }
      
         if (r23 != 0) 
         {
            fp280.InitialMedium.AccessType = r22;
            fp280.InitialMedium.dwBlockSize = fp280.dwBlockSize;
            fp280.InitialMedium.dwBlockCountLo = dwLastBlockAddress;
            fp280.InitialMedium.dwBlockCountHi = 0;
            fp280.InitialMedium.awSerialNumber[0] = 0;
         }
         //49989c
         (r20->Data_0->Func_16)(
            r20->Data_0->Data_0,
            r20->bFirstLUN,
            &fp280);
            
         if ((r20->bData_957 != 0) &&
            (r23 != 0)) 
         {
            fp360.AccessType = r22;
            fp360.dwBlockSize = fp280.dwBlockSize;
            fp360.dwBlockCountLo = dwLastBlockAddress;
            fp360.dwBlockCountHi = 0;
            fp360.awSerialNumber[0] = 0;

            (r20->Data_0->Func_20)(
               r20->Data_0->Data_0,
               r20->bFirstLUN,
               &fp360);
         }
         //4998f8
         if (r20->bData_958 != 0) 
         {
            (r20->Data_0->Func_24)(
               r20->Data_0->Data_0,
               r20->bFirstLUN,
               r23);
            //->4999c8
         }
         else 
         {
            //499920
            r20->bData_958 = 1;
            
            if ((r20->bNumLUNs - 1) == 
               r20->bFirstLUN) 
            {
               for (bLun = 0; bLun < r20->bNumLUNs; bLun++) 
               {
                  (r20->Data_0->Func_24)(
                     r20->Data_0->Data_0,
                     bLun,
                     1);
               }
            }
            //->4999c8
         }            
      } //if (fp372 != 0)
      else 
      {
         //499978
         (fp380/*r20->Data_0*/->pfSendCBW)(
            fp380/*r20->Data_0*/->Data_0, 
           r20, 
            bLun, 
            /*r25*/r20->arCommandBlock/*void* pCB*/, 
            r19/*int bCBLength*/, 
            r16/*void* pDataBuffer*/, 
            r14/*int dwDataLength*/, 
            0/*int bHostTx*/, 
            MUSB_StorageDriver_ReceiveInquiryResponse, 
            0/*char j*/, 
            2/*int k*/);            
      }
   }
   //4999C8
}


/* 498698 - complete */
/* Send a Bulk-Only Mass Storage Class Reset */
int func_498698(USB_StorageBulkOnly* a)
{
   FAPI_SYS_PRINT_MSG("TODO: func_498698\n");
   
#if 0
   if ((a == 0) || (a != a->Data_32)) 
   {
      return 0;   
   }
   
   a->Data_212 = 1;

   musb_memcpy(&a->arDeviceRequestBuffer, 
      (void*) &Data_598a88, 
      sizeof(MUSB_DeviceRequest));        
   
   a->arDeviceRequestBuffer[4] = a->pInterfaceDescriptor->bInterfaceNumber;
   
   a->ControlUrb.pInBuffer = 0;
   a->ControlUrb.dwInLength = 0;
         
   return (0 == MUSB_StartControlTransaction(a->ControlUrb.Data_0->a, &a->ControlUrb))? 1: 0;
#else
   return 0;
#endif
}


