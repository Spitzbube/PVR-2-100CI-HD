
#include <string.h>
#include <fapi/sys_services.h>
#include <fapi/reg_dma.h>
#include <fapi/drv_dma.h>
#include <fapi/drv_usb.h>
#if 0
#include "fapi_adapter.h"
#include "fapi_dma.h"
#endif
#include "musb/usb_ch9.h"
#include "musb/musbhdrc.h"
#include "musb/musbdefs.h"
#include "musb/list_49cd8c.h"
#include "musb/musb_dma.h"
#include "musb/musb.h"


int Data_21cc3b54 = 0;



int Data_21f02130[] = {10}; //21f02130
MGC_DmaControllerFactory* mgc_pDmaControllerFactory = //21f02134
   &MGC_HdrcDmaControllerFactory; 

MUSB_ControlIrp Data_601744 = //601744 -256
{  
#if 0
   0, //MUSB_Device* Data_0; //0
   (char*) &MUSB_rDeviceRequestSetFeature_A_HNP_SUPPORT, //void* pTxBuffer; //4
   sizeof(MUSB_DeviceRequest), //int dwOutLength; //8
   0, //char* pInBuffer; //12
   0, //int dwInLength; //16
   0, //int Data_20; //20
   0, //int dwActualOutLength; //24
   0, //int dwActualInLength; //28
   func_499FC0, //void (*complete)(void* pContext, MUSB_ControlIrp* pUrb); //32
   0, //void* context; //36 
   0, //char bData_40; //40 
#endif
}; 

MUSB_ControlIrp Data_21f02164 = //21f02164 v3.8: 601770 -212
{
#if 0
      //TODO
   0, //MUSB_Device* Data_0; //0
   (char*) &MUSB_rDeviceRequestSetFeature_B_HNP_ENABLE, //void* pTxBuffer; //4
   sizeof(MUSB_DeviceRequest), //int dwOutLength; //8
   0, //char* pInBuffer; //12
   0, //int dwInLength; //16
   0, //int Data_20; //20
   0, //int dwActualOutLength; //24
   0, //int dwActualInLength; //28
   func_499F50, //void (*complete)(void* pContext, MUSB_ControlIrp* pUrb); //32
   0, //void* context; //36 
   0, //char bData_40; //40
#endif
};

MUSB_DeviceRequest MUSB_rDeviceRequestSetAddress = 
{
   USB_DIR_OUT | USB_TYPE_STANDARD | USB_RECIP_DEVICE, //char bmRequestType; //0
   USB_REQ_SET_ADDRESS, //char bRequest; //1
   0, //unsigned short wValue; //2
   0, //unsigned short wIndex; //4
   0, //unsigned short wLength; //6
}; //21f021fc v3.8: 6018B4 -160

MUSB_DeviceRequest MUSB_rDeviceRequestGetDeviceDescriptor = 
{
   USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_DEVICE, //char bmRequestType; //0
   USB_REQ_GET_DESCRIPTOR, //char bRequest; //1
   USB_DT_DEVICE << 8, //unsigned short wValue; //2
   0, //unsigned short wIndex; //4
   0, //unsigned short wLength; //6
}; //21f02204 v3.8: 6018BC -152

MUSB_DeviceRequest MUSB_rDeviceRequestGetConfigurationDescriptor =
{
   USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_DEVICE, //char bmRequestType; //0
   USB_REQ_GET_DESCRIPTOR, //char bRequest; //1
   USB_DT_CONFIG << 8, //unsigned short wValue; //2
   0, //unsigned short wIndex; //4
   0, //unsigned short wLength; //6
}; //21f0220c v3.8: 6018C4 -144


extern int Data_21f6652c; //21f6652c

unsigned short wData_21f7a040; //21f7a040
List_49cd8c Data_21f7a044; //21f7a044
int Data_21f7a050; //21f7a050
MGC_HsDmaController* MGC_pHsDmaController; //21f7a054
MGC_DmaControllerFactory Data_21f7a058; //21f7a058
int Data_21f7a064 = 0; //21f7a064

char Data_2207f32c[0x100]; //2207f32c
char Data_2207f42c[0x100]; //2207f42c


int musb_log_core_options(struct MGC* a);
void MGC_SetExternalDmaControllerFactory(MGC_DmaControllerFactory* a);

int MGC_HsDmaStartController(void*);
int MGC_HsDmaStopController(void*);
void MGC_HsDmaReleaseChannel(MGC_DmaChannel*);
void* MGC_HsDmaAllocateBuffer(void*, int);
int MGC_HsDmaReleaseBuffer(void*, void*);
int MGC_HsDmaProgramChannel(MGC_DmaChannel* pChannel, 
                        unsigned short /*wPacketSize*/, char /*bMode*/,
                        const char* pBuffer, 
                        int dwLength);
int MGC_HsDmaGetChannelStatus(MGC_DmaChannel*);
int mgc_handle_interrupts(Struct_49d2fc*, char, 
                  unsigned short, unsigned short);
int mgc_hdrc_service_usb(struct MGC*, char);
void func_21cc6694(struct MGC*);
void func_21cc6b68(struct MGC*, char);
void func_21cc6144(Struct_49d2fc*, unsigned short, MGC_Timer*);
void func_21cc686c(char, struct MGC*);
void musb_otg_state_changed(struct MGC*);
void func_21cc5fac(Struct_49d2fc*, unsigned short, MGC_Timer*);
void func_21cc4d40(struct MGC*);
int MUSB_StartControlTransaction(MUSB_Port*,
                  MUSB_ControlIrp*);
void func_21ccbbf0(Struct_49d2fc*, unsigned short, MGC_Timer*);
void MUSB_DeviceRequestGetDeviceDescriptor_8(Struct_49d2fc*, unsigned short, MGC_Timer*);
void MUSB_DeviceRequestGetDeviceDescriptor(Struct_49d2fc*, unsigned short, MGC_Timer*);
void MUSB_DeviceRequestGetConfigurationDescriptor(Struct_49d2fc*, unsigned short, MGC_Timer*);
void func_21ccbb20(Struct_49d2fc*, unsigned short, MGC_Timer*);
void func_21cccbb8(Struct_49d2fc*, unsigned short/*, MGC_Timer**/);
void func_21ccbb3c(Struct_49d2fc*, unsigned short, MGC_Timer*);
void func_21cc6cd8(struct MGC*);

int MGC_HdrcLoadFifo(struct MGC* a, char bEnd, 
      unsigned wCount, const char* pSource);


void* List_49a810_get_element(List_49a810* list, unsigned short b);
int List_49a810_get_element_count(List_49a810* a);
char MGC_CompletedTransfer(MGC_Message* a, MGC_Endpoint* pEndpoint, 
                  char bStatus, int bTx, void* pUrb);



const char MGC_aTestPacket[MGC_TEST_PACKET_SIZE] = //5989C8
{
    /* implicit SYNC then DATA0 to start */

    /* JKJKJKJK x9 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /* JJKKJJKK x8 */
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
    /* JJJJKKKK x8 */
    0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 
    /* JJJJJJJKKKKKKK x8 */
    0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    /* JJJJJJJK x8 */
    0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 
    /* JKKKKKKK x10, JK */
    0xfc, 0x7e, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0x7e

    /* implicit CRC16 then EOP to end */
};
 
 


/* 21cd03f4 - todo */
unsigned char MGC_Read8(int base_addr, unsigned short addr)
{
   register unsigned cpu_sr;
   
   cpu_sr = FAPI_SYS_DISABLE_IRQ;
   
   FREG_DMA_SetUsbMode_LengthInput(0);

   volatile unsigned long* pv = &((volatile unsigned long*)base_addr)[addr];
   
   int data = pv[0] & 0xFF;
   
   if (Data_21f7a064 != 0) 
   {      
      Data_2207f32c[0] = 0; 
      Data_2207f42c[0] = 0;
      
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), "Read8("); 
      func_21cc3e28(Data_2207f42c, 33, addr, 16, 4);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), Data_2207f42c);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), ") = "); 
      
      Data_2207f42c[0] = 0;
      
      func_21cc3e28(Data_2207f42c, 33, data, 16, 4);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), Data_2207f42c);
      func_21cc8328(Data_2207f32c);        
   }
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);  
   
   return data; 
}


/* 21cd02d8 - todo */
unsigned short MGC_Read16(int base_addr, unsigned short addr)
{
   unsigned cpu_sr;
   
   cpu_sr = FAPI_SYS_DISABLE_IRQ;
   
   FREG_DMA_SetUsbMode_LengthInput(1);

   volatile unsigned long* pv = &((volatile unsigned long*)base_addr)[addr];
   
   int data = pv[0] & 0xFFFF;
   
   if (Data_21f7a064 != 0) 
   {
      Data_2207f32c[0] = 0; 
      Data_2207f42c[0] = 0;
      
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), "Read16("); 
      func_21cc3e28(Data_2207f42c, 33, addr, 16, 4);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), Data_2207f42c);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), ") = "); 
      
      Data_2207f42c[0] = 0;
      
      func_21cc3e28(Data_2207f42c, 33, data, 16, 4);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), Data_2207f42c);
      func_21cc8328(Data_2207f32c);        
   }
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);  
   
   return data; 
}


/* 21cd01c4 - todo */
unsigned long MGC_Read32(int base_addr, unsigned short addr)
{
   unsigned cpu_sr;
   
   cpu_sr = FAPI_SYS_DISABLE_IRQ;
   
   FREG_DMA_SetUsbMode_LengthInput(2);

   volatile unsigned long* pv = &((volatile unsigned long*)base_addr)[addr];
   
   unsigned data = pv[0];
   
   if (Data_21f7a064 != 0) 
   {
      Data_2207f32c[0] = 0; 
      Data_2207f42c[0] = 0;
      
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), "Read32("); 
      func_21cc3e28(Data_2207f42c, 33, addr, 16, 4);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), Data_2207f42c);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), ") = "); 
      
      Data_2207f42c[0] = 0;
      
      func_21cc3e28(Data_2207f42c, 33, data, 16, 4);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), Data_2207f42c);
      func_21cc8328(Data_2207f32c);        
   }
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);  
   
   return data; 
}


/* 21cd00ac - todo */
void MGC_Write8(int base_addr, unsigned short addr, char data)
{
   unsigned cpu_sr;
   volatile unsigned long* pv = &((volatile unsigned long*)base_addr)[addr];
   
   cpu_sr = FAPI_SYS_DISABLE_IRQ;
      
   if (Data_21f7a064 != 0) 
   {
      Data_2207f32c[0] = 0; 
      Data_2207f42c[0] = 0;
      
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), "Write8("); 
      func_21cc3e28(Data_2207f42c, 33, addr, 16, 4);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), Data_2207f42c);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), ", "); 
      
      Data_2207f42c[0] = 0;
      
      func_21cc3e28(Data_2207f42c, 33, data, 16, 4);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), Data_2207f42c);
      func_21cc8328(Data_2207f32c); 
   }
   
   FREG_DMA_SetUsbMode_LengthInput(0);
      
   pv[0] = data;    
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);   
}


/* 21ccff94 - todo */
void MGC_Write16(int base_addr, unsigned short addr, unsigned short data)
{
   unsigned cpu_sr;
   volatile unsigned long* pv = &((volatile unsigned long*)base_addr)[addr];
   
   cpu_sr = FAPI_SYS_DISABLE_IRQ;
      
   if (Data_21f7a064 != 0) 
   {
      Data_2207f32c[0] = 0; 
      Data_2207f42c[0] = 0;
      
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), "Write16("); 
      func_21cc3e28(Data_2207f42c, 33, addr, 16, 4);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), Data_2207f42c);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), ", "); 
      
      Data_2207f42c[0] = 0;
      
      func_21cc3e28(Data_2207f42c, 33, data, 16, 4);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), Data_2207f42c);
      func_21cc8328(Data_2207f32c); 
   }
   
   FREG_DMA_SetUsbMode_LengthInput(1);
      
   pv[0] = data & 0xFFFF;    
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);   
}


/* 21ccfe7c - todo */
void MGC_Write32(int base_addr, unsigned short addr, unsigned long data)
{
   unsigned cpu_sr;
   volatile unsigned long* pv = &((volatile unsigned long*)base_addr)[addr];
   
   cpu_sr = FAPI_SYS_DISABLE_IRQ;
      
   if (Data_21f7a064 != 0) 
   {
      Data_2207f32c[0] = 0; 
      Data_2207f42c[0] = 0;
      
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), "Write32("); 
      func_21cc3e28(Data_2207f42c, 33, addr, 16, 8);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), Data_2207f42c);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), ", "); 
      
      Data_2207f42c[0] = 0;
      
      func_21cc3e28(Data_2207f42c, 33, data, 16, 8);
      func_21cc3b54(Data_2207f32c, sizeof(Data_2207f32c), Data_2207f42c);
      func_21cc8328(Data_2207f32c); 
   }
   
   FREG_DMA_SetUsbMode_LengthInput(2);
      
   pv[0] = data;    
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);   
}


/* 21ccf72c - todo */
/* v3.8: 49a12c */
int func_21ccf72c(struct MGC* a, 
                MGC_Endpoint* b, int c)
{   
   char r0;
   MUSB_Irp* r1;
   MGC_Pipe* r2_;
   MGC_Pipe* r3;
   int r6;
   int r4;
   char r2;
   
   int r17 = 0;
   char r13 = 0;
   void* r14 = 0;
   int r18 = 0;
   Struct_49e470_for_TransferType_1* r19 = 0;
   
   MUSB_Irp* r21 = (c != 0)?
      b->pCurrentTxUrb: 
      b->pCurrentRxUrb;   
   
   if (r21 == 0) 
   {
      r21 = List_49cd8c_get_element((c != 0)? 
         &b->tx_urb_list: 
         &b->rx_urb_list, 
         0);   
   }
   
   if (r21 != 0) 
   {
      List_49cd8c_remove_element((c != 0)? 
         &b->tx_urb_list: 
         &b->rx_urb_list, 
         r21);
   
      r1 = r21;
      r2_ = r1->hPipe;
      r3 = r2_;
   
      r0 = (c != 0)? 
         b->bTxTrafficType: 
         b->bRxTrafficType;
      
      switch (r0) 
      {
         case 1:
            //49a208
            r19 = (void*) r21;
            r13 = r19->n;
            r3 = r2_;
            break;
               
         case 2:
         case 3:
            //49a218
            r1 = r21;
            r13 = r1->bAllowDma;
            r3 = r1->hPipe;
            break;
      }
     
      r6 = (r0 == r3->bTransferType);
      r4 = 0;
      if ((a->bHostMode != 0) && (r6 != 0) && (r3->bTransferType == 2))
         r4 = 1;
      r2 = r4;
   
      if ((r3 != 0) && ((r6 != 0) || (r2 != 0)))
      {
         r17 = 1;
      
         switch (r0) 
         {
            case 1:
               //49a294
               r18 = r19->c[0];
               r14 = r19->b;            
               break;  
            
            case 2:
            case 3:
               //49a2a4
               r18 = r1->dwLength - ((c != 0)? b->dwOffset: b->Data_64);
               r14 = r1->pBuffer;
               break; 
         }
      
         if (c != 0) 
         {
            if (r0 != 3) 
            {
               b->pCurrentTxUrb = r21;
            
               (a->pfSendOnEp)(a, b, 
                  r14, 
                  r18, 
                  r21);                                 
            }
         }
         else 
         {
            //49a314
            (a->pfReceiveOnEp)(a, b, 
               r14, 
               r18, 
               r21, 
               r13);
         }
      }
      else 
      {
         //49a340
         if (c != 0) 
         {
            b->pCurrentTxUrb = 0;      
         }
         else
         {
            b->pCurrentRxUrb = 0;      
         }
      }
   }   
   
   return r17;
}


/* 21ccf0d8 - todo */
uint8_t* mgc_search_descriptor(uint8_t* a, uint16_t b, uint8_t descriptor, uint8_t d)
{
   int16_t r7 = -1;
   uint16_t i = 0;
   uint8_t* p = a;
   
   if ((descriptor == p[1]) && (d == 0)) 
   {
      // already found
      return p;   
   }

   if (i < b) 
   {
      signed short r8 = d;
      while ((i < b) && (r7 < r8))
      {
         p = &a[i];

         if (p[0] <= 0) 
         {
            return 0;
         }
         
         // go to the next descriptor
         i += p[0]; //bLength field = Size of Descriptor

         if ((descriptor == p[1]) && (d == ++r7))
         {
            // found it
            return p;   
         }
      }
   }

   if (i < b) 
   {
      return p;   
   }

   return 0;
}


/* 21ccf08c - todo */
void musb_memcpy(void* a, void* b, int c)
{
   unsigned i;
   
   if ((int)a & 0x03)
   {
      for (i = 0; i < c; i++)
      {
         ((char*)a)[i] = ((char*)b)[i];
      }
   }
   else
   {
      memcpy(a, b, c);
   }
}


/* 21ccf048 - todo */
void musb_memset(void* a, unsigned char b, unsigned c)
{   
   unsigned i;
   
   if ((int)a & 0x03)
   {
      for (i = 0; i < c; i++)
      {
         ((char*)a)[i] = b;
      }
   }
   else
   {
      memset(a, b, c);
   }
}


/* 21ccefe0 - todo */
/* v3.8: 48f780 - complete */
/* Increase the memory for data */
void* func_21ccefe0(char* data, unsigned size)
{
   unsigned i = 0;
   char* new_data = (FAPI_SYS_Services.mallocFunc != 0)?
         (FAPI_SYS_Services.mallocFunc)(size): 0;
         
   if (new_data != 0) 
   {
      for (i = 0; i < size; i++) 
      {
         new_data[i] = data[i];
      }
      
      (FAPI_SYS_Services.freeFunc)(data);
   }
   return new_data;
}




/* 21ccef50 - complete */
/* v3.8: func_49754c */
int musb_log_release_info(struct MGC* a)
{
   int res;
   unsigned short wRelease;
   unsigned short wRelMajor;
   unsigned short wRelMinor;
   int addr2 = a->a->addr2;
   
   res = musb_log_core_options(a);
   
   /* log release info by reading the HWVERS register */ 
   wRelease = MGC_Read16(addr2, 0x6c);
   wRelMajor = ((wRelease & 0x7c00) >> 10) & 0xFF;
   wRelMinor = wRelease & 0x3ff;
   
   if ((wRelMajor > 1) || ((wRelMajor == 1) && (wRelMinor >= 400))) 
   {
      a->bData_62 = 1; 
   }

   if ((wRelMajor > 1) || ((wRelMajor == 1) && (wRelMinor >= 600))) 
   {
      a->bData_64 = 1;   
   }
   
   return res;   
}


/* v3.8: 48c35c - nearly complete */
MGC_Endpoint* func_48c35c(struct MGC* a, 
      MUSB_DeviceEndpoint* b,
                     int* c, int d, char* e)
{   
    unsigned i;
   MGC_Endpoint* r20 = 0;
   MGC_Endpoint* r15 = 0;
   unsigned short r19 = 0xFFFF;
   unsigned short r18 = 0xFFFF;
   unsigned short wMaxPacketSize = 
      (((char*)&b->UsbDescriptor.wMaxPacketSize)[1] << 8) |
         ((char*)&b->UsbDescriptor.wMaxPacketSize)[0];
   
   char r13 = (b->UsbDescriptor.bEndpointAddress & USB_ENDPOINT_DIR_MASK)?
      1: 0;
   
   if (a->bHostMode != 0) 
   {
      r13 = !r13;
   }
   
   unsigned r22 = List_49a810_get_element_count(&a->ep_list);
   
   for (i = 0; i < r22; i++)
   {     
      MGC_Endpoint* r0 = List_49a810_get_element(&a->ep_list, i);   

      char r1 = ((r0->bIsSharedFifo != 0) || (r13 != 0))? r0->g: r0->h;

      //48c454
      if ((r0 != 0) && (r1 == 0))
      {
         //48c464
         unsigned short r1 = (r13 != 0)? 
            r0->wMaxPacketSizeTx: r0->wMaxPacketSizeRx;
         
         if (a->bHostMode != 0) 
         {
            //48c484
            int r3 = r1 - wMaxPacketSize;
            int r2 = r1 - c[0];   
            if (r3 >= 0) 
            {
               //48c498
               if (r19 > (unsigned short)r3) 
               {
                  r19 = r3;
                  r20 = r0;   
               }
               //48c4ac
               if (r18 > (unsigned short)r2) 
               {
                  r18 = r2;
                  r15 = r0;   
               }
            }
         } //if (a->bData_54 != 0)
         else 
         {
            //48c4c4
            char r2 = b->UsbDescriptor.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
            if ((wMaxPacketSize <= r1) && (r2 == r0->bEnd))
            {
               //->48c4f4
               r20 = r0;   
               break;
            }
         }
      }
      //48c4e4
   } //for (i = 0; i < r22; i++)
   //48c4f8
   if (r15 != 0) 
   {
      //48c500
      r20 = r15;   
   }
   //48c504
   if ((r20 != 0) && (d != 0)) 
   {
      if ((r20->bIsSharedFifo != 0) || (r13 != 0)) 
      {
         r20->g = 1;  
      }
      else 
      {
         r20->h = 1;   
      }
      
      r20->bData_14 = r13;
      r20->halted_15 = 0;
      
      if (r13 != 0) 
      {
         r20->bData_6 = b->pDevice->bBusAddress;
         r20->bTxRemoteEnd = b->UsbDescriptor.bEndpointAddress;
         r20->bData_8 = b->UsbDescriptor.bInterval;
         r20->bTxTrafficType = b->UsbDescriptor.bmAttributes;
         r20->wTxPacketSize = wMaxPacketSize;
      }
      else 
      {
         r20->bData_10 = b->pDevice->bBusAddress;
         r20->bRxRemoteEnd = b->UsbDescriptor.bEndpointAddress;
         r20->bData_12 = b->UsbDescriptor.bInterval;
         r20->bRxTrafficType = b->UsbDescriptor.bmAttributes;
         r20->wRxPacketSize = wMaxPacketSize;
      }
   }
   //48c598
   *e = r13;
   
   return r20;   
}


/* 21cceaac - todo */
/* v3.8: 497028 - todo */
MGC_Endpoint* func_21cceaac(struct MGC* a, 
      MUSB_DeviceEndpoint* b,
                          void* c, int d)
{
//   FAPI_SYS_PRINT_MSG("TODO: func_21cceaac\n");   
   
   unsigned short wTxCsr;
   unsigned short wRxCsr;
   char bInterval;
   char bTx;
   char bIsMulti = 0;
   char bHubAddr = 0;
   char bHubPort = 0;
   char r19 = 0;
   
   int pBase = a->a->addr2;
   
   MGC_Endpoint* pEnd = func_48c35c(a, b, c, d, &bTx);
   
   if ((pEnd != 0) && (d != 0))
   {
      char bTrafficType = (bTx != 0)? 
         pEnd->bTxTrafficType: 
         pEnd->bRxTrafficType;
            
      char bEnd = pEnd->bEnd;
   
      MGC_SelectEnd(pBase, bEnd);
   
      if (a->bHostMode != 0) 
      {
         //4970e8         
         switch (b->pDevice->ConnectionSpeed)
         {
            case 1:
               r19 |= 0xC0;   
               break;
            
            case 2:
               r19 |= 0x80;
               break;
            
            default:
               r19 |= 0x40;
               break;
         }
         //497118
         if (b->pDevice->pParentUsbDevice != 0)
         {
            //497124
            bHubAddr = ((MUSB_Device*)b->pDevice->pParentUsbDevice)->bBusAddress;
            bHubPort = b->pDevice->bHubPort;
            if (b->pDevice->DeviceDescriptor.bDeviceProtocol == 2)
            {
               bIsMulti = 1;   
            }
         }
         //49713c
         if (bTx != 0) 
         {
            //497148 
            MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_TXFUNCADDR), 
               b->pDevice->bBusAddress);
            MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_TXHUBADDR), 
               (bIsMulti != 0)? bHubAddr | 0x80: bHubAddr);
            MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_TXHUBPORT), 
               bHubPort);
         }

           /* also, try Rx (this is a bug ion the core: I always need to to do 
            * both (at least for ep0), needs to be changed when the core is
            * fixed */ 
         if (!bTx || (bEnd == 0))
         {
            //4971b0                        
            MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_RXFUNCADDR), 
               b->pDevice->bBusAddress);
            MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_RXHUBADDR), 
               (bIsMulti != 0)? bHubAddr | 0x80: bHubAddr);   
            MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_RXHUBPORT), 
               bHubPort);
         }
         //49720c         
      } //if (a->bHostMode != 0)
      //497214
      if (bTx != 0) 
      {
         r19 |= (pEnd->bTxRemoteEnd & MGC_M_TYPE_REMOTE_END);            
      }
      else
      {
         r19 |= (pEnd->bRxRemoteEnd & MGC_M_TYPE_REMOTE_END);
      }
   
      if (bEnd != 0) 
      {
         //497240
         r19 |= (bTrafficType << MGC_S_TYPE_PROTO);
      }
      //49724c      
      if (bTx) 
      {
         if (a->bHostMode != 0)    
         {
            if (bEnd != 0) 
            {
               //497268
               MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, r19); 

               bInterval = b->UsbDescriptor.bInterval;
            
               if (bInterval >= 16) 
               {
                  bInterval = 16;   
               }
            
               if (pEnd->bTxTrafficType == 2) 
               {
                  if (b->wNakLimit < 16)
                  {
                     bInterval = b->wNakLimit;
                  }
                  else 
                  {
                     bInterval = 16;   
                  }
               
                  if (bInterval < 2) 
                  {
                     bInterval = 2;   
                  }
               
                  if (b->wNakLimit == 0xFFFF)
                  {
                     bInterval = 0;   
                  }
               }
               //4972c4
               MGC_WriteCsr8(pBase, MGC_O_HDRC_TXINTERVAL, bEnd, bInterval);
            } //if (bEnd != 0)
            else 
            {
               //4972d0
               MGC_WriteCsr8(pBase, MGC_O_HDRC_TXINTERVAL, bEnd, 16);
               MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, 
                  r19 & MGC_M_TYPE_SPEED);  
            }
         } //if (a->bHostMode != 0)
         //4972f0
         if (bEnd != 0) 
         {
            //4972f8
            if ((pEnd->bTxTrafficType == 2) && 
               (a->bBulkSplit != 0))
            {
               //497310
               MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, pEnd->wTxPacketSize | 
                  (((pEnd->wMaxPacketSizeTx / pEnd->wTxPacketSize)*2048) + -2048));
            }
            else 
            {
               //497340
               MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 
                  pEnd->wTxPacketSize);
            }
            //497354            
            wTxCsr = MGC_M_TXCSR_MODE;         
            if ((a->bHostMode == 0) && 
               (pEnd->bTxTrafficType == 1/*USB_ENDPOINT_XFER_ISOC*/))
            {
               wTxCsr |= MGC_M_TXCSR_ISO;
            }
            //49737c
            /* if not ready, flush and restore data toggle */ 
            /* twice in case of double packet buffering */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, 
               MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, 
               MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wTxCsr);
         } //if (bEnd != 0)
         else 
         {
            //4973a8
            /* endpoint 0: just flush */ 
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_CSR0_FLUSHFIFO);   
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_CSR0_FLUSHFIFO);   
         }
      } //if (bTx != 0)
      //4973c8
      else 
      {
         if (a->bHostMode != 0) 
         {
            if (bEnd != 0) 
            {
               MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, 0);
               MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, r19);
         
               bInterval = b->UsbDescriptor.bInterval;
               if (bInterval >= 16)    
               {
                  bInterval = 16;   
               }
         
               if (pEnd->bRxTrafficType == 2) 
               {
                  if (b->wNakLimit < 16)
                  {
                     bInterval = b->wNakLimit;
                  }
                  else 
                  {
                     bInterval = 16;   
                  }
            
                  if (bInterval < 2) 
                  {
                     bInterval = 2;   
                  }
            
                  if (b->wNakLimit == 0xFFFF)
                  {
                     bInterval = 0;   
                  }
               }
               //497448
               MGC_WriteCsr8(pBase, MGC_O_HDRC_RXINTERVAL, bEnd, bInterval);
            } //if (bEnd != 0)
            else 
            {
               //497454
               MGC_WriteCsr8(pBase, MGC_O_HDRC_NAKLIMIT0, bEnd, 
                  16);
               MGC_WriteCsr8(pBase, MGC_O_HDRC_TYPE0, bEnd, 
                  r19 & MGC_M_TYPE_SPEED);
            }
         }
         //497474
         if (bEnd != 0) 
         {
            wRxCsr = 0;
      
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 
               pEnd->wRxPacketSize);
      
            if ((a->bHostMode == 0) && 
               (pEnd->bRxTrafficType == 1/*USB_ENDPOINT_XFER_ISOC*/))
            {
               wRxCsr |= MGC_M_RXCSR_P_ISO;            
            }
            //4974b0
            /* twice in case of double packet buffering */ 
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 
               MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 
               MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);

            if (pEnd->bRxTrafficType == 3/*USB_ENDPOINT_XFER_INT*/) 
            {
               wRxCsr |= MGC_M_RXCSR_DISNYET;   
            }
      
            if (a->bHostMode == 0) 
            {
               wRxCsr |= 0x8000;   
            }
      
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wRxCsr);
         } //if (bEnd != 0)            
      }
   }
   //497510 
   return pEnd;   
}


/* v3.8: 498c10 - todo */
char func_498c10(struct MGC* a, 
                 int b/*status*/, 
                 MGC_Endpoint* c/*pEnd*/, 
                 unsigned short d/*wRxCount*/, 
                 int e, 
                 MUSB_Irp* f)
{   
   struct 
   {
      int fill_0; //0 
      char* Data_4; //4
      int* Data_8; //8
      int* Data_12; //12
      int* Data_16; //16
      int fill_20; //20
      int fill_24; //24
      unsigned short wData_28; //28
      unsigned short wData_30; //30
      unsigned short wData_32; //32
      char fill_34; //34
      char bData_35; //35   
   }* r1;
   
   struct 
   {
      int fill_0; //0
      char* Data_4; //4
      int Data_8; //8
      int fill_12; //12
      int fill_16; //16
      int fill_20; //20
      int fill_24; //24
      char bData_28; //28
      char fill_29; //29
      char bData_30; //30
   }* r2;
   
   char* r23;
   char r17 = 0;
   unsigned r14 = 0;
   char r16 = 0;
   
   unsigned short r13 = d;
   
   switch (c->bRxTrafficType) 
   {
      case 1:
         //498c7c
         r1 = (void*) f;
         r17 = r1->bData_35;
         #if 1 
         {
            int r3 = r1->wData_30;
            r1->Data_16[r3] = d;
            r1->Data_12[r3] = 0;
            r1->wData_30++;
            r14 = r1->Data_8[r3+1];
         }
         #else
         r1->Data_16[r1->wData_30] = d;
         r1->Data_12[r1->wData_30] = 0;
         r1->wData_30++;
         r14 = r1->Data_8[r1->wData_30];
         #endif
         
         r23 = &r1->Data_4[c->Data_64];
         
         r16 = 0;
         
         if (r1->wData_30 >= r1->wData_28) 
         {
            r16 = 1;   
         }
         if ((r1->wData_32 != 0) &&
            ((r1->wData_30 % r1->wData_32) == 0))
         {
            r16 = 1;  
         }
         //->498d74
         break;
            
      case 2:
      case 3:
         //498d00
         r2 = (void*) f;
         r17 = r2->bData_30;
         r14 = r2->Data_8;
         r23 = &r2->Data_4[c->Data_64];
         
         if (d < c->wRxPacketSize) 
         {
            r16 = 1;
         }
         
         if (r14 <= (c->Data_64 + d)) 
         {
            r16 = 1;            
            if ((r2->bData_28 != 0) && 
               ((d % c->wRxPacketSize) == 0))
            {
               r16 = 0;   
            }
            //498d58
            r14 -= c->Data_64;
            r13 = r14;
            if (r13 >= d) 
            {
               r13 = d;   
            }
         }
         else 
         {
            //498d70
            r14 -= c->Data_64;
         }
         //->498d74
         break;
         
      default:
         //498e20
         r23 = 0;
         r13 = 0;
         //->498cf8
         r16 = 1;
         //->498d74
         break;
   }
   //498d74
   (a->pfUnloadFifo)(a, c->bEnd, r13, r23);
   c->Data_64 += r13;
   r23 += r13;
   if ((r16 == 0) && (e != 0))
   {
      (a->pfReceiveOnEp)(a, c, r23, r14, f, r17);   
   }
   return r16;
}


/* 21cce3a8 - todo */
/* v3.8: 49760C - todo */
int mgc_receive_on_endpoint(struct MGC* a, 
                  MGC_Endpoint* b, 
                  char* c, int d, 
                  void* e, char bDMA)
{
   int r15_;
   MGC_DmaController* pDmaController;
   MGC_DmaChannel* pDmaChannel;
   unsigned short wRxCount;
   int fp28;
   unsigned short wIntrRxE;
   MUSB_Device* r14;
   unsigned short r18;
   char bRxType;
   char bHubAddr = 0;
   char bHubPort = 0;
   char bIsMulti = 0;
   int pBase = a->a->addr2;
   MUSB_SystemServices* pOS = a->a->pOS;
   char bEnd = b->bEnd;
   
//   FAPI_SYS_PRINT_MSG("mgc_receive_on_endpoint\n");   

   MGC_SelectEnd(pBase, bEnd);
   
   r18 = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
   
   if ((r18 & MGC_M_TXCSR_MODE) && (b->bIsSharedFifo)) 
   {
      r18 &= ~MGC_M_TXCSR_MODE; 
      
      MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, r18);  
   }
   //4976e4
   r18 = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd) &
      ~(MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE);
   
   if (a->bHostMode) 
   {
      r18 = (r18 & ~MGC_M_RXCSR_RXPKTRDY) | 
         MGC_M_RXCSR_H_REQPKT;
      //r18 &= ~MGC_M_RXCSR_RXPKTRDY;
      //r18 |= MGC_M_RXCSR_H_REQPKT;
      
      bRxType = (b->bRxTrafficType << MGC_S_TYPE_PROTO);
      bRxType |= (b->bRxRemoteEnd & MGC_M_TYPE_REMOTE_END);  
   
      r14 = (b->bRxTrafficType == 0)?
         ((MUSB_ControlIrp*)e)->pDevice:
         ((struct MGC_Pipe*)(((MUSB_Irp*)e)->hPipe))->Data_16;
      
      switch (r14->ConnectionSpeed)
      {
         case 1:
            bRxType |= (MGC_TYPE_SPEED_LOW << MGC_S_TYPE_SPEED);
            break;
         
         case 2:
            bRxType |= (MGC_TYPE_SPEED_FULL << MGC_S_TYPE_SPEED);
            break;
            
         default:
            bRxType |= (MGC_TYPE_SPEED_HIGH << MGC_S_TYPE_SPEED);
            break;
      }
      
      MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, bRxType);
      
      if (r14->pParentUsbDevice != 0)
      {
         bHubAddr = ((MUSB_Device*)(r14->pParentUsbDevice))->bBusAddress;
         bHubPort = r14->bHubPort;
         
         if (((MUSB_Device*)(r14->pParentUsbDevice))->DeviceDescriptor.bDeviceProtocol == 2)
         {
            bIsMulti = 1;   
         }
      }
      //4977ac
      MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_RXFUNCADDR), 
         r14->bBusAddress);
      MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_RXHUBADDR), 
         bIsMulti? bHubAddr | 0x80: bHubAddr);   
      MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_RXHUBPORT), 
         bHubPort);   
   } //if (a->bHostMode)
   //497808
   if (MGC_ReadCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd) != 
      b->wRxPacketSize) 
   {
      MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 
         b->wRxPacketSize);   
   }
   //497830
   b->bData_95 = b->bData_94;
   
   wIntrRxE = MGC_Read16(pBase, MGC_O_HDRC_INTRRXE) |
      (1 << bEnd);
   MGC_Write16(pBase, MGC_O_HDRC_INTRRXE,
      wIntrRxE & ~(1 << bEnd));
      
   if ((!a->bHostMode) &&
      ((MGC_M_RXCSR_RXPKTRDY & MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd)) ||
      (b->bData_95 != b->bData_94)))
   {
      //4978a0
      while (1) 
      {
         //->4978c4
         if (r18 & MGC_M_RXCSR_RXPKTRDY) 
         {
            //4978d0               
            wRxCount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);
            pDmaController = a->pDmaController;
            pDmaChannel = b->pRxDmaChannel;
            if ((bDMA == 0) && (pDmaChannel != 0)) 
            {
               //4978fc
               (pDmaController->pfDmaReleaseChannel)(
                  pDmaChannel); 
               b->pRxDmaChannel = 0;
               //->497a1c  
            }
            //497910
            else if ((bDMA != 0) && 
               (0 != (pOS->pfSystemToBusAddress)(pOS->pPrivateData, c)))
            {
               if ((pDmaController != 0) && 
                  (pDmaChannel == 0)) 
               {
                  //497940
                  pDmaChannel = b->pRxDmaChannel = (void*)
                     (pDmaController->pfDmaAllocateChannel)(
                        pDmaController->pPrivateData, 
                         bEnd, 
                        0/*bTransmit*/, 
                        b->bRxTrafficType/*bProtocol*/, 
                        b->wRxPacketSize/*wMaxPacketSize*/);   
               }
               //497964
               if (pDmaChannel != 0) 
               {
                  //49796c
                  pDmaChannel->dwActualLength = 0;
                  b->bData_80 = 0;
                  b->bData_81 = 1;
                  b->pCurrentRxUrb = e;
                                    
                  if (0 == (pDmaController->pfDmaProgramChannel)(
                     pDmaChannel,
                          b->wRxPacketSize, 
                     0, //bMode,
                          c, //const char* pBuffer,
                     (d <= pDmaChannel->dwMaxLength)? 
                        d: 
                        pDmaChannel->dwMaxLength
                          //int dwLength)) 
                     )) 
                  {
                     b->bData_81 = 0;
                     b->pCurrentRxUrb = 0;
                     //->4978fc   
                     (pDmaController->pfDmaReleaseChannel)(
                        pDmaChannel); 
                     b->pRxDmaChannel = 0;
                     //->497a1c  
                  }
                  else 
                  {
                     //4979c4
                     if ( ((volatile MGC_Endpoint*)b)->pCurrentRxUrb ) 
                     {
                        //4979d0
                        r18 = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
                        r18 |= (MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_DMAENAB);
                        MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR+1, bEnd,
                           (r18 | MGC_M_RXCSR_DMAMODE) >> 8); 
                        MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR+1, bEnd,
                           r18 >> 8); 
                     }
                     //->497c8c
                     return 0;
                  }
               } //if (pDmaChannel != 0)
            }
            //497a1c
            r15_ = func_498c10(a, 0, b, wRxCount, 0, e);
            unsigned short r1 = r18;
            r18 &= ~MGC_M_RXCSR_RXPKTRDY;
            //r18 = r1 & ~MGC_M_RXCSR_RXPKTRDY;
            if ((wRxCount < b->wRxPacketSize) || 
               !(r1 & MGC_M_RXCSR_AUTOCLEAR)) 
            {
               //497a64
               MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, r18);   
            }
            //497a74
            if (r15_ != 0) 
            {
               //497a7c
               MGC_Message fp8;
               
               if (0 != MGC_CompletedTransfer(&fp8, b, 0, 0, e)) 
               {
                  (pOS->pfQueueBackgroundItem)(
                     pOS->pPrivateData, &fp8);
               }
               //497ab0
               MGC_Write16(pBase, MGC_O_HDRC_INTRRXE, wIntrRxE);
               //->497c8c
               return 0;
            }
            //->4978b4
         } //if (r18 & MGC_M_RXCSR_RXPKTRDY)
         //4978b4
         r18 = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
         //->4978c4            
      } //while (1)
   }
   else
   {
      //497ac4
      b->pCurrentRxUrb = e; 
      pDmaController = a->pDmaController;
      MGC_DmaChannel* pDmaChannel = b->pRxDmaChannel;
   
      if ((bDMA == 0) && (pDmaChannel != 0))
      {
         (pDmaController->pfDmaReleaseChannel)(
            pDmaChannel);
         b->pRxDmaChannel = 0;
         //->497c3c   
      }
      //497afc
      else if ((bDMA != 0) && (0 != (pOS->pfSystemToBusAddress)(pOS->pPrivateData, c)))
      {
         if ((pDmaController != 0) && (pDmaChannel == 0)) 
         {
            pDmaChannel = b->pRxDmaChannel = (void*)
               (pDmaController->pfDmaAllocateChannel)(
                  pDmaController->pPrivateData, 
                   bEnd, 
                  0, //bTransmit, 
                  b->bRxTrafficType, //bProtocol, 
                  b->wRxPacketSize);   
         }
         //497b50
         if (pDmaChannel != 0) 
         {
            b->bData_80 = 0;
            pDmaChannel->dwActualLength = 0;  

            fp28 = (pDmaController->pfDmaProgramChannel)(
               pDmaChannel,
                    b->wRxPacketSize, 
               pDmaChannel->bDesiredMode, //bMode,
                    c, //const char* pBuffer,
               (d <= pDmaChannel->dwMaxLength)? 
                  d: pDmaChannel->dwMaxLength);
                  
            if (fp28 != 0) 
            {
               if ((a->bHostMode) && (a->bData_64))
               {
                  r18 |= MGC_M_RXCSR_H_AUTOREQ;
                  
                  unsigned short r2 =  
                     (((d <= pDmaChannel->dwMaxLength)?
                     d: pDmaChannel->dwMaxLength) + 
                     b->wRxPacketSize - 1) / b->wRxPacketSize;
                     
                  MGC_Write16(pBase, 
                     (unsigned short)((b->bEnd * 4) + 0x300), 
                     r2);
               }
               //497c08
               r18 |= ((pDmaChannel->bDesiredMode != 0)? 
                  MGC_M_RXCSR_DMAMODE: 0)
                  | (MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_DMAENAB); 
               //->497c40
            } //if (fp28 != 0)
            else 
            {
               //497c2c
               (pDmaController->pfDmaReleaseChannel)(
                  pDmaChannel);
               b->pRxDmaChannel = 0;
            }
         } //if (pDmaChannel != 0)
      }
      //497c3c
      if (a->bHostMode) 
      {
         MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, r18);   
         //->497c70
      }
      else 
      {
         //497c5c
         MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR+1, bEnd, 
            r18 >> 8);   
         
      }

      if ((bDMA == 0) || 
         (pDmaChannel == 0) ||
         (fp28 == 0)) 
      {
         //->497ab0   
         MGC_Write16(pBase, MGC_O_HDRC_INTRRXE, wIntrRxE);
      }   
   }
   //497c8c
   return 0;
}


/* 21ccdf80 - todo */
int mgc_send_on_endpoint(struct MGC* a, 
      MGC_Endpoint* b, 
      char* pBuffer, unsigned dwLength, 
      void* pUrb)
{
   MGC_DmaController* pDmaController;
   MGC_DmaChannel* pDmaChannel;
   uint8_t bAllowDma;
   int bDmaOk;
   unsigned wLoadCount;
   unsigned short wCsr = 0;
   char bHubAddr = 0;
   char bHubPort = 0;
   int bIsMulti = 0;
   
   int pBase = a->a->addr2; //r7
   MUSB_SystemServices* pOS = a->a->pOS; //ip
   unsigned char bEnd = b->bEnd; //r6
   
   #if 0
   hex_dump("mgc_send_on_endpoint", 
      pBuffer, dwLength);
   #endif
   
   MGC_SelectEnd(pBase, bEnd);

   if (a->bHostMode != 0) 
   {
      //21ccdfc8 
      MUSB_Device* r22;
      
      //hdrc_set_protocol?
      char reg = b->bTxTrafficType << MGC_S_TYPE_PROTO;
      reg |= (b->bTxRemoteEnd & MGC_M_TYPE_REMOTE_END);      

#if 0
      r22 = (b->bTxTrafficType == 0)? 
         ((MUSB_ControlIrp*)pUrb)->Data_0:
         ((MUSB_Irp*)pUrb)->pPipe->Data_16;
#else
      switch (b->bTxTrafficType)
      {
         case 0:
         {
            //21cce27c
            MUSB_ControlIrp* r1 = pUrb;
            r22 = r1->pDevice;
            break;
         }
         
         default:
         {
            //21ccdfe4
            MUSB_Irp* r1 = pUrb;
            r22 = ((struct MGC_Pipe*)(r1->hPipe))->Data_16;
            break;
         }
      }
#endif
      //21ccdff0
      switch (r22->ConnectionSpeed)
      {
         case 1:
            reg |= (MGC_TYPE_SPEED_LOW << MGC_S_TYPE_SPEED);
            break;
            
         case 2:
            reg |= (MGC_TYPE_SPEED_FULL << MGC_S_TYPE_SPEED);
            break;
               
         default:
            reg |= (MGC_TYPE_SPEED_HIGH << MGC_S_TYPE_SPEED);
            break;
      }
      
#if 0
      if (bEnd == 0) 
      {
         MGC_WriteCsr8(pBase, MGC_O_HDRC_TYPE0, bEnd, reg & MGC_M_TYPE_SPEED);
      }
      else 
      {
         MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, reg);
      }
#else
      MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, 
            (bEnd == 0)? reg & MGC_M_TYPE_SPEED: reg);
#endif
      
      //hdrc_set_address?
      
      if (r22->pParentUsbDevice != 0)
      {   
         //21cce02c
         bHubAddr = ((MUSB_Device*)(r22->pParentUsbDevice))->bBusAddress;
         bHubPort = r22->bHubPort;
         
         if (((MUSB_Device*)(r22->pParentUsbDevice))->DeviceDescriptor.bDeviceProtocol == 2)
         {
            bIsMulti = 1;   
         }
      }
      //21cce338
      MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_TXFUNCADDR),             
         r22->bBusAddress);
      
      MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_TXHUBADDR), 
         bIsMulti? bHubAddr | 0x80: bHubAddr);  
      
      MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_TXHUBPORT), 
         bHubPort);   
      
      /* also, try Rx (this is a bug ion the core: I always need to to do 
       * both (at least for ep0), needs to be changed when the core is
       * fixed */ 
      if (bEnd == 0) 
      {
         //21cce2b0
         MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_RXFUNCADDR), 
            r22->bBusAddress);
          
         MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_RXHUBADDR), 
            bIsMulti? bHubAddr | 0x80: bHubAddr);   
          
         MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_HDRC_RXHUBPORT), 
            bHubPort);   
      }
   } //if (a->bHostMode != 0)
   //21cce094
   if ((b->bTxTrafficType == 2)/*bIsBulk*/ && a->bBulkSplit)
   {
      //21cce250
      wLoadCount = b->wMaxPacketSizeTx;   
      if (wLoadCount > dwLength) 
      {
         wLoadCount = dwLength;   
      } 
   }
   else 
   {
      //21cce0a0
      wLoadCount = (b->wTxPacketSize > dwLength)? 
         dwLength: 
         b->wTxPacketSize;
   }
   //21cce0b0
   if (bEnd != 0) 
   {
      //21cce0b8
      wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
      wCsr &= ~MGC_M_TXCSR_H_ERROR;
   }
   else 
   {
      //21cce22c
      MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, 0);   
   }
   //21cce0cc
   bDmaOk = 0;
   
   switch (b->bTxTrafficType) 
   {
      case 0: // Control?
         //21cce0d8
         bAllowDma = ((MUSB_ControlIrp*)pUrb)->bAllowDma;
         break;
         
      case 1: // Isochronous?
         //21cce220
         bAllowDma = ((MUSB_IsochIrp*)pUrb)->bAllowDma;
         break;
      
      default: // Bulk / Interrupt?
         //21cce168
         bAllowDma = ((MUSB_Irp*)pUrb)->bAllowDma;
         break;   
   } //switch (b->bPipeType)
   //21cce0e0
   pDmaController = a->pDmaController;
   pDmaChannel = b->pTxDmaChannel;
   
   #if 0
   bAllowDma = 1;
   #endif
   
   if ((bAllowDma != 0) || (pDmaChannel == 0))
   {
      //21cce174   
      if ((bAllowDma != 0) && (0 != (pOS->pfSystemToBusAddress)(pOS->pPrivateData, pBuffer)))
      {         
         /* candidate for DMA */
         if (pDmaController && (pDmaChannel == 0))
         {
            //21cce344
            pDmaChannel = b->pTxDmaChannel = (void*) (pDmaController->pfDmaAllocateChannel)(
               pDmaController->pPrivateData, 
               bEnd,
               1, //out
               b->bTxTrafficType, //bStdType
               b->wTxPacketSize);
         }
         //21cce1ac
         if (pDmaChannel) 
         {
            b->bData_80 = 1;
            pDmaChannel->dwActualLength = 0;   
            
            bDmaOk = (pDmaController->pfDmaProgramChannel)(
               pDmaChannel, 
               b->wTxPacketSize, 
               pDmaChannel->bDesiredMode, 
               pBuffer, 
               (dwLength <= pDmaChannel->dwMaxLength)? dwLength: pDmaChannel->dwMaxLength);
            
            if (bDmaOk) 
            {
               //498014
               wLoadCount = 0;
               if (bEnd != 0) 
               {
                  wCsr |= (pDmaChannel->bDesiredMode ? MGC_M_TXCSR_DMAMODE: 0) 
                     | MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB;
               }
            } //if (bDmaOk)
            else 
            {
               //21cce380
               (pDmaController->pfDmaReleaseChannel)(pDmaChannel);
               b->pTxDmaChannel = 0;
            }
         } //if (pDmaChannel)
      }
   } //if ((bAllowDma != 0) || (pDmaChannel == 0))
   else 
   {
      (pDmaController->pfDmaReleaseChannel)(pDmaChannel);
      b->pTxDmaChannel = 0;
   }
   //21cce108
   if (!bDmaOk) 
   {
      (a->pfLoadFifo)(a, bEnd, wLoadCount, pBuffer); 
      
      b->dwRequestSize = wLoadCount;     
      
      if (bEnd != 0) 
      {
         wCsr &= ~(MGC_M_TXCSR_DMAENAB|MGC_M_TXCSR_AUTOSET);
         wCsr |= MGC_M_TXCSR_TXPKTRDY;
      }
      else 
      {
         //4980a8 - A Control Transaction on Endpoint 0 starts with a SETUP phase
         wCsr = (MGC_M_CSR0_TXPKTRDY | MGC_M_CSR0_H_SETUPPKT);
         
         if (a->bData_62 != 0) 
         {
            wCsr = (MGC_M_CSR0_H_NO_PING |
               MGC_M_CSR0_TXPKTRDY | MGC_M_CSR0_H_SETUPPKT);
         }
      }
   } //if (!bDmaOk)
   //4980c0
   b->pCurrentTxUrb = pUrb;
   
   if (bEnd != 0) 
   {
      MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr | MGC_M_TXCSR_MODE);   
   }
   else 
   {
      MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, bEnd, wCsr);   
   }
   
   return 0;   
}


/* 21ccdeb0 - todo */
int List_49a810_add_element(List_49a810* list, void* data)
{
   unsigned short r17;
   void* r0;
   //int res = 0;
   
   if ((list == 0) || (data == 0)) 
   {
      return 0;
   }
   
   r17 = list->num_elements;   
   if (r17 < list->count) 
   {
      //res = 1;
      r0 = (r17 * list->elem_size) + (char*)(list->data);
   }
   else if (r17 < (list->e + list->count))
   {
      //res = 1;
      r0 = ((r17 - list->count) * list->elem_size) + (char*)(list->more_data);     
   }
   else 
   {
      int r16;
      if (list->count < 2)
      {
         r16 = 2;
      }
      else
      {
         r16 = list->count;
      }
   
      unsigned short r1 = r16 + r17;
      r0 = func_21ccefe0(list->more_data, r1 * list->elem_size);
      //if (r0 != 0)
      if (r0 == 0) return 0;
      {
         //res = 1;
         list->more_data = r0;
         list->e += r16;
         r0 = ((r17 - list->count) * list->elem_size) + (char*)(list->more_data);
      }
   }
   
   //if (res != 0) 
   {
      musb_memcpy(r0, data, list->elem_size);      
      list->num_elements++;   
   }
   
   return 1; //res;
}


/* 21ccdb14 - complete */
/* Get the data from the list at index b */
void* List_49a810_get_element(List_49a810* list, unsigned short b)
{
   void* data = 0;
   
   if ((list != 0) && (b < list->num_elements)) 
   {
      if (b < list->count) 
      {
         data = (char*)(list->data) + (b * list->elem_size);   
      }
      else 
      {
         data = (char*)(list->more_data) + ((b - list->count) * list->elem_size);
      }
   }
   
   return data;
}


/* 21ccdb08 - complete */
int List_49a810_get_element_count(List_49a810* a)
{
   int res = 0;
   if (a != 0) res = a->num_elements;
   return res;
}


/* 21ccda9c - todo */
List_49a810* List_49a810_create(List_49a810* a, 
   unsigned short elem_size, unsigned short count, void* data)
{
   if (a == 0) 
   {
      a = (FAPI_SYS_Services.mallocFunc != 0)? 
            (FAPI_SYS_Services.mallocFunc)(sizeof(List_49a810)): 0;
   }   
   
   if (a != 0) 
   {
      a->elem_size = elem_size;
      
      if (data != 0) 
      {
         a->count = count;      
      }
      else 
      {
         a->count = 0;   
      }
      
      a->data = data;
      a->num_elements = a->e = 0;
      a->more_data = 0;
   }

   return a;
}


/* 21ccbb58 - complete */
/* v3.8: func_499fc4 */
unsigned short func_21ccbb58(char* a, unsigned short b, unsigned short c)
{
   unsigned short r0;
   
   for (r0 = b; r0 < c; )
   {
      switch (a[r0]) 
      {
         case 0:
         case 4:
         case 5:
         case 6:
         case 7:
         case 8:
            //21ccbbe4
            r0 += 2;
            break;
            
         case 1:
         case 2:
         case 3:
            //21ccbbd0
            r0 += 3;
            break;
            
         case 9:
            //21ccbbb4
            r0 += 2;
            return r0;
            
         case 10:
            //21ccbbc4
            r0++;
            return r0;
      }
   }

   return r0;
}


/* 21cccd04 - complete */
/* Get a matching device driver */
/*
func_499fc4 in this module
mgc_search_descriptor outside this module
*/
MUSB_DeviceDriver* MUSB_FindMatchingDeviceDriver(
                  MUSB_HostClient* pDeviceDriverList,
                  MUSB_Device* r7,
                  char* pConfigurationDescriptor,
                  char** r22)
{
   unsigned short wValue;
   unsigned char bValue;
   int bPerInterface = 0;
   unsigned short i = 0;
   unsigned short r18 = 0;
   
   //char* pConfigurationDescriptor = ((void**)(r7->pConfigurationOffsets))[0];
   char* pInterfaceDescriptor = 0;
   unsigned short wTotalLength = (pConfigurationDescriptor[3] << 8) | 
      pConfigurationDescriptor[2]; //sp32
   unsigned short wStringLength = pDeviceDriverList->wPeripheralListLength; //r8
   char* pString = pDeviceDriverList->pPeripheralList; //r5
   
   //21cccde0
   while (i < wStringLength) 
   {
      //494b70
      switch (pString[i]) 
      {
         case 0:
            //21cccfac - Fill?
            i++;
            break;
               
         case 1:
            //21cccf84 - VendorID Filter
            {
               i++;
               wValue = pString[i];
               i++;
               wValue |= (pString[i] << 8);
               i++;
               
               int r0 =  
                  (((char*)&r7->DeviceDescriptor.idVendor)[1] << 8) |
                  ((char*)&r7->DeviceDescriptor.idVendor)[0];
               
               if (wValue != r0)
               {
                  //21cccf0c   
                  bPerInterface = 0;
                  i = func_21ccbb58(pString, i, wStringLength);
                  r18 = i;
               }
               //->21cccde0
            }
            break;
               
         case 2:
            //21cccf5c - ProductID Filter
            {
               i++;
               wValue = pString[i];
               i++;
               wValue |= (pString[i] << 8);
               i++;
               
               int r0 = 
                  (((char*)&r7->DeviceDescriptor.idProduct)[1] << 8) |
                  ((char*)&r7->DeviceDescriptor.idProduct)[0];
               
               if (wValue != r0)
               {
                  bPerInterface = 0;
                  i = func_21ccbb58(pString, i, wStringLength);
                  r18 = i;
               }
            }
            break;
               
         case 3:
            //21cccecc
            {
               i++;
               wValue = pString[i];
               i++;
               wValue |= (pString[i] << 8);
               i++;
               
               int r0 = 
                  (((char*)&r7->DeviceDescriptor.bcdDevice)[1] << 8) |
                  ((char*)&r7->DeviceDescriptor.bcdDevice)[0];
                
               if (wValue != r0) 
               {
                  bPerInterface = 0;
                  i = func_21ccbb58(pString, i, wStringLength);
                  r18 = i;
               }
            }
            break;
               
         case 4:
            //21ccce8c - Select Configuration
            {
               i++;
               bValue = pString[i];
               i++;
               bPerInterface = 0;
               if (bValue < r7->DeviceDescriptor.bNumConfigurations)
               {
                  pConfigurationDescriptor = 
                     ((void**)(r7->apConfigDescriptors))[bValue];
                  
                  wTotalLength = (pConfigurationDescriptor[3] << 8) | 
                     pConfigurationDescriptor[2];
               }
               else 
               {
                  //21cccf0c
                  i = func_21ccbb58(pString, i, wStringLength);
                  r18 = i;
               }
            }
            break;
               
         case 5:
            //21cccdac - Select Interface
            {
               i++;
               bValue = pString[i];
               i++;
               bPerInterface = 1;
               pInterfaceDescriptor = mgc_search_descriptor(
                  pConfigurationDescriptor, 
                  wTotalLength, 
                  USB_DT_INTERFACE, 
                  bValue);
               
               if (pInterfaceDescriptor == 0) 
               {
                  //21ccd030   
                  bPerInterface = 0;
                  i = func_21ccbb58(pString, i, wStringLength);
                  r18 = i;
               }
            }
            break;
               
         case 6:
            //21cccf2c - Device/Interface Class
            {
               i++;
               bValue = pString[i];
               i++;
               if (bPerInterface != 0) 
               {
                  //21cccf48
                  if (bValue != pInterfaceDescriptor[5]/*bInterfaceClass*/) 
                  {
                     //21cccf0c   
                     bPerInterface = 0;
                     i = func_21ccbb58(pString, i, wStringLength);
                     r18 = i;
                  }
               }
               else 
               {
                  //21ccd008
                  if (bValue != r7->DeviceDescriptor.bDeviceClass)
                  {
                     //21ccd014   
                     bPerInterface = 0;
                     i = func_21ccbb58(pString, i, wStringLength);
                     r18 = i;
                  }
               }
            }
            break;
               
         case 7:
            //21ccce5c Device/Interface Subclass
            {
               i++;
               bValue = pString[i];
               i++;
               if (bPerInterface != 0) 
               {
                  //21ccce78
                  if (bValue != pInterfaceDescriptor[6]/*bInterfaceSubClass*/) 
                  {
                     //21cccf0c   
                     bPerInterface = 0;
                     i = func_21ccbb58(pString, i, wStringLength);
                     r18 = i;
                  }
               }
               else 
               {
                  //21cccfe0
                  if (bValue != r7->DeviceDescriptor.bDeviceSubClass)
                  {
                     //21cccfec   
                     bPerInterface = 0;
                     i = func_21ccbb58(pString, i, wStringLength);
                     r18 = i;
                  }
               }
            }
            break;
               
         case 8:
            //21ccce2c Device/Interface Protocol
            {
               i++;
               bValue = pString[i];
               i++;
               if (bPerInterface != 0) 
               {
                  //21ccce48
                  if (bValue != pInterfaceDescriptor[7]/*bInterfaceProtocol*/) 
                  {
                     //21cccf0c   
                     bPerInterface = 0;
                     i = func_21ccbb58(pString, i, wStringLength);
                     r18 = i;
                  }
               }
               else 
               {
                  //21cccfb8
                  if (bValue != r7->DeviceDescriptor.bDeviceProtocol)
                  {
                     //21cccfc4   
                     bPerInterface = 0;
                     i = func_21ccbb58(pString, i, wStringLength);
                     r18 = i;
                  }
               }
            }
            break;
               
         case 9:
            //21cccdf4 - End
            {
               i++;
               bValue = pString[i];
               if (bValue < pDeviceDriverList->bDeviceDriverListLength)
               {
                  //21ccce0c
                  *r22 = &pString[r18];   
                  return &pDeviceDriverList->aDeviceDriverList[bValue];
               }
               else 
               {
                  //21cccde8
                  return 0;
               }
            }
            break;
               
         case 10:
            //21cccde8
            return 0;
            //break;
      } //switch (pString[i])
   } //while (i < wStringLength)
   //21cccde8
   return 0;
}


/* 498ff4 - complete */
static inline void MGC_ReleaseDeviceAddress(Struct_49d2fc_Inner1_3* a, char bAddress)
{
   unsigned char byte = (bAddress - 1) >> 3;
   unsigned char bit = (bAddress - 1) & 0x07;
   
   if (byte < 16) 
   {
      a->arDeviceAddressBitmap[byte] &= ~(1 << bit);   
   }   
}


/* 48e0a8 - complete */
static void func_48e0a8(struct MGC* a, char b, unsigned short c)
{
   Struct_49d2fc* r13 = a->a;
   
   switch (c) 
   {
      case 0x101: //21ccd5fc
         //Test_SE0_NAK
         (a->Func_680)(a, 3);
         break;  
         
      case 0x102: //21ccd5e8
         //Test_J
         (a->Func_680)(a, 1);
         break; 
         
      case 0x103: //21ccd5d4
         //Test_K
         (a->Func_680)(a, 2);
         break; 
         
      case 0x104: //21ccd5c0
         //Test_Packet
         (a->Func_680)(a, 4);
         break; 
         
      #if 0   
      case 0x105:
         //Reserved
         break; 
      #endif
         
      case 0x106: //21ccd638
         //HS_HOST_PORT_SUSPEND_RESUME
         (a->Func_680)(a, 6);         
         (r13->pOS->pfArmTimer)(r13->pOS->pPrivateData, 0, 20000, 0, func_21ccbb20);
         break; 
         
      case 0x107: //21ccd610
         //SINGLE_STEP_GET_DEV_DESC
         
         // Send GetDescriptor(Device) after 15s of SOFs
         (r13->pOS->pfArmTimer)(r13->pOS->pPrivateData, 0, 15000, 0, func_21cccbb8);
         break; 
         
      case 0x108: //21ccd670
         //SINGLE_STEP_ GET_DEV_DESC_DATA

         // Send GetDescriptor(Device)
         func_21cccbb8(r13, 0);         
         
         // Send an IN packet after 15s of SOFs
         (r13->pOS->pfArmTimer)(r13->pOS->pPrivateData, 0, 15000, 0, func_21ccbb3c);
         break;
   }
}


/* 21ccd050 - todo */
void MUSB_DeviceRequestComplete(void* pContext, MUSB_ControlIrp* b)
{
   MUSB_DeviceDriver* pDeviceDriver;
   unsigned short wTotalLength;
   Struct_49d2fc* a = pContext;
   struct MGC* r13 = a->Data_20;
   Struct_49d2fc_Inner1_3* r15 = &r13->Data_156;
   MUSB_SystemServices* pOS = a->pOS;
      
   MGC_Endpoint* ep0 = List_49a810_get_element(&r13->ep_list, 0);
   
   #if 0
   FAPI_SYS_PRINT_MSG("MUSB_DeviceRequestComplete: r15->bStep=%d, r15->control_urb.dwStatus=%d\n",
      r15->bStep, r15->control_urb.dwStatus);
   #endif
   
   if (ep0 != 0) 
   {
      if (r15->control_urb.dwStatus != 0) 
      {
         //21ccd084
         r15->bRetry++;
         
         if (r15->bRetry > 10) 
         {
            //21ccd09c
            r15->bStep = 0;   
            
            MGC_ReleaseDeviceAddress(r15, r15->Data_80->bBusAddress);
            //21ccd0c0
            if (r15->Data_80->apConfigDescriptors != 0)
            {
               (FAPI_SYS_Services.freeFunc)(r15->Data_80->apConfigDescriptors);
            }
            
            if (r15->Data_80->pDescriptorBuffer != 0)
            {
               (FAPI_SYS_Services.freeFunc)(r15->Data_80->pDescriptorBuffer);
            }
            
            (FAPI_SYS_Services.freeFunc)(r15->Data_80->pPrivateData);
            
            if (r15->Data_80->pParentUsbDevice == 0)
            {
               //21ccd478
               r15->bData_419++;            
               if (r15->bData_419 < 3) 
               {
                  r13->bData_33 = 0;
            
                  (r13->Func_620)(r13);
            
                  (pOS->pfArmTimer)(pOS->pPrivateData, 0, 100, 0, func_21ccbbf0);
               }            
            }            
         } //if (r15->bRetry > 10)
         else 
         {
            //21ccd4e8 - Send the Device Control again
            MUSB_StartControlTransaction(r13->b, &r15->control_urb);
         }
         //->494aa4
      } //if (r15->control_urb.dwStatus != 0)
      else 
      {
         //21ccd11c
         #if 0
         if ((r15->control_urb.dwInLength != 0) &&
               (r15->control_urb.dwInLength < 256))
         {
            hex_dump("MUSB_DeviceRequestComplete", 
               r15->control_urb.pInBuffer,
               r15->control_urb.dwInLength);
         }
         #endif
         
         r15->bRetry = 0;
         r15->bData_419 = 0;
         
         switch (r15->bStep) 
         {
            case 1:
               //21ccd39c
               r15->Data_80->bBusAddress = r15->bDeviceAddress;
               
               if (r13->bData_42 == 0) 
               {
                  //21ccd528
                  r13->address_32 = r15->bDeviceAddress;  
                  
                  (r13->Func_620)(r13);
               }
               //21ccd3b4 - Get the max packet size of Pipe 0
               // which is in byte 7 of the Device descriptor                          
               (pOS->pfArmTimer)(pOS->pPrivateData, 0, 10, 0,
                     MUSB_DeviceRequestGetDeviceDescriptor_8);             
               //->21ccd114
               break;  
         
            case 2:
               //21ccd3d8
               {
                  struct usb_device_descriptor* dev_descr = (void*) r15->descriptor_buffer;

#if 1
                  FAPI_SYS_PRINT_MSG("MUSB_DeviceRequestComplete: bMaxPacketSize0=%d\n",
                        dev_descr->bMaxPacketSize0);
#endif

                  (pOS->pfLock)(pOS->pPrivateData, 1);
               
                  ep0->wTxPacketSize = dev_descr->bMaxPacketSize0;
               
                  (pOS->pfUnlock)(pOS->pPrivateData, 1);
                                                 
                  signed short r3 = (dev_descr->idProduct << 8) | dev_descr->idVendor;
                  unsigned short r2 = (dev_descr->bcdDevice << 8) | dev_descr->idProduct;
                     
                  if ((r3 == 0x1a0a) && 
                        /*(r2 >= 0x101) && (r2 <= 0x108)*/
                        ((unsigned short)(r2 - 0x101) < 8)) 
                  {
                     func_48e0a8(r13, r15->Data_80->bBusAddress, r2);
                  }
                  else 
                  {
                     //21ccd4c4 - Get the full Device descriptor
                     (pOS->pfArmTimer)(pOS->pPrivateData, 0, 10, 0,
                        MUSB_DeviceRequestGetDeviceDescriptor);             
                  }
               }
               //->494aa4
               break; 
               
            case 3:
               //21ccd2fc - Got the Device descriptor:
               // - Save the bNumConfigurations field 
               // - Allocate array space for all wTotalLength's in the Configuration descriptors
               // - Start to get the first Configuration descriptor
               {
                  void* descr_buf = r15->descriptor_buffer;
                  struct usb_device_descriptor* device_descr = descr_buf; 
                  
#if 1
                  FAPI_SYS_PRINT_MSG("MUSB_DeviceRequestComplete: bNumConfigurations=%d\n",
                        device_descr->bNumConfigurations);
#endif
                  
                  r15->bNumConfigurations = device_descr->bNumConfigurations;  
                  r15->iConfiguration = 0;
                  r15->dwTotalLengthSum = 0;
                  
                  musb_memcpy(&r15->Data_80->DeviceDescriptor,
                     device_descr, sizeof(struct usb_device_descriptor)); //USB_DT_DEVICE_SIZE);
                  
                  r15->Data_80->apConfigDescriptors =
                     FAPI_SYS_MALLOC(device_descr->bNumConfigurations * sizeof(int));
                  
                  if (r15->Data_80->apConfigDescriptors != 0)
                  {
                     r15->bStep = 4; // Next step
                     r15->control_urb.pInBuffer = descr_buf;
                     r15->control_urb.dwInLength = sizeof(struct usb_config_descriptor); //USB_DT_CONFIG_SIZE+1; //10;
                     
                     //Start to receive the first Configuration descriptor
                     (pOS->pfArmTimer)(pOS->pPrivateData, 0, 10, 0,
                           MUSB_DeviceRequestGetConfigurationDescriptor);
                  }
                  else 
                  {
                     //21ccd6d0 - Failed to allocate space for Configuration info  
                     (FAPI_SYS_Services.freeFunc)(r15->Data_80->pPrivateData);
                     r15->Data_80 = 0;
                     r15->bStep = 0; // Rewind to first step
                  }
               }
               break;
               
            case 4:
               //21ccd248 - Got a Configuration descriptor
               {
                  // Get the value of wTotalLength in the Configuration descriptor
                  wTotalLength = (r15->control_urb.pInBuffer[3] << 8) |
                     r15->control_urb.pInBuffer[2];
                                    
                  ((int*)r15->Data_80->apConfigDescriptors)[r15->iConfiguration] = wTotalLength;
                  
                  r15->dwTotalLengthSum += wTotalLength;                    
                  
                  r15->iConfiguration++;                  
                  if (r15->iConfiguration < r15->bNumConfigurations) 
                  {
                     //->21ccd380 - Receive the next Configuration descriptor  
                     (pOS->pfArmTimer)(pOS->pPrivateData, 0, 10, 0,
                        MUSB_DeviceRequestGetConfigurationDescriptor);
                  }
                  else 
                  {
                     //21ccd298 - Got all Configuration descriptors
                     r15->iConfiguration = 0;
                     r15->bStep = 5; // Next step
                     
                     // Get the total length of the first Configuration
                     wTotalLength = 
                        ((unsigned short*)r15->Data_80->apConfigDescriptors)[r15->iConfiguration * 2];

                     // Allocate space for the content of all configurations (interfaces, endpoints)                     
                     void* pConfigurations = 
                        FAPI_SYS_MALLOC(r15->dwTotalLengthSum);
                           
                     if (pConfigurations != 0) 
                     {
                        //21ccd2d0
                        r15->Data_80->pDescriptorBuffer = pConfigurations;
                        r15->Data_80->wDescriptorBufferLength = r15->dwTotalLengthSum;

                        // Set the pointer to the first Configuration
                        ((void**)(r15->Data_80->apConfigDescriptors))[0/*r15->iConfiguration*/] = pConfigurations;
                                                                         
                        r15->control_urb.pInBuffer = pConfigurations; //r15->Data_80->pConfigurations;
                        r15->control_urb.dwInLength = wTotalLength;
                        
                        //21ccd380 - Start to receive the full content 
                        // of the first Configuration
                        (pOS->pfArmTimer)(pOS->pPrivateData, 0, 10, 0,
                           MUSB_DeviceRequestGetConfigurationDescriptor);
                     }
                     else 
                     {
                        //21ccd5ac - Failed to allocate space for all Configuration contents
                        (FAPI_SYS_Services.freeFunc)(r15->Data_80->pPrivateData);
                     }
                  }
                  //->494aa4
               }
               break;
               
            case 5:
               //21ccd170               
               if (r15->iConfiguration != 0) 
               {
                  //21ccd4f8
                  
                  // Get the pointer to the full content
                  // of the previous Configuration
                  char* r4 = ((void**)r15->Data_80->apConfigDescriptors)[r15->iConfiguration - 1];
                 
                  // Save the pointer to the full content 
                  // of the next Configuration
                  int r3 = (r4[3] << 8) | r4[2];
                  
                  ((void**)r15->Data_80->apConfigDescriptors)[r15->iConfiguration] = &r4[r3];
               }
               //21ccd180
               r15->iConfiguration++;
               if (r15->iConfiguration >= r15->bNumConfigurations) 
               {
                  //21ccd198 - Got the full content of all Configurations
                  char* fp4;
                  unsigned char i;
                  
                  for (i = 0; i < r15->Data_80->DeviceDescriptor.bNumConfigurations; i++)
                  {
                     //21ccd1c0
                     void* r6 = ((void**)r15->Data_80->apConfigDescriptors)[i];
                     
                     pDeviceDriver = MUSB_FindMatchingDeviceDriver(
                           r13->pDeviceDriverList,
                           r15->Data_80,
                           r6,
                           &fp4);
                     
#if 1
                     FAPI_SYS_PRINT_MSG("MUSB_DeviceRequestComplete: pDeviceDriver=0x%x\n",
                           pDeviceDriver);
#endif
                     
                     if (0 != pDeviceDriver)
                     {
                        r15->Data_80->pCurrentConfiguration = r6;
                        break;
                     }
                  }
                  //21ccd1f4
                  if (0 != (r13->Func_672)(r13, r15->Data_80, pDeviceDriver)) 
                  {
                     //21ccd540
//                     r15->Data_80->pPrivateData->pDeviceDriver = pDeviceDriver;
((struct Struct_49d2fc_Inner1_3_2_t*)r15->Data_80->pPrivateData)->pDeviceDriver = pDeviceDriver;
                     
                     List_49cd8c_add_element(&r13->List_524, 
                        r15->Data_80, 0);      
                     
                     if (r13->Data_432 == 0) 
                     {
                        r13->Data_432 = r15->Data_80;   
                     }

                     if (r15->Func_76 != 0) 
                     {
                        (r15->Func_76)(r15->Data_80->pParentUsbDevice,
                           r15->Data_80);
                           
                        r15->Func_76 = 0;   
                     }
                     //21ccd590
                     (pDeviceDriver->pfDeviceConnected)(
                           pDeviceDriver->pPrivateData,
                              r13, 
                              r15->Data_80, 
                              fp4);
                     //->21ccd23c
                  }
                  else
                  {
                     //21ccd210
                     if (r13->Data_432 != 0)
                     {
                        //21ccd21c
                        if (r15->Func_76 != 0) 
                        {
                           (r15->Func_76)(r15->Data_80->pParentUsbDevice,
                              r15->Data_80);
                              
                           r15->Func_76 = 0;   
                        }
                        //->21ccd23c
                     }
                     else
                     {
                        //21ccd6e8
                        MGC_ReleaseDeviceAddress(r15, r15->Data_80->bBusAddress);
                        //21ccd724
                        if (r15->Data_80->apConfigDescriptors != 0)
                        {
                           (FAPI_SYS_Services.freeFunc)(r15->Data_80->apConfigDescriptors);
                        }
                        //21ccd748
                        if (r15->Data_80->pDescriptorBuffer != 0)
                        {
                           (FAPI_SYS_Services.freeFunc)(r15->Data_80->pDescriptorBuffer);
                        }
                        //21ccd760
                        (FAPI_SYS_Services.freeFunc)(r15->Data_80->pPrivateData);
                        //->21ccd23c
                     }
                  }
                  //21ccd23c - Finish
                  r15->bStep = 0; // Rewind to first step
               } //if (r15->iConfiguration >= r15->bNumConfigurations)
               else 
               {
                  //21ccd6a4 - Get the total length of the next Configuration
#if 0
                  wTotalLength = 
                     *(((unsigned short**)r15->Data_80->apConfigDescriptors)[r15->iConfiguration]);
#else                  
                  wTotalLength = 
                     ((unsigned short*)r15->Data_80->apConfigDescriptors)[r15->iConfiguration * 2];
#endif
                  
                  // Increase the rx pointer by the total length of 
                  // the previous configuration
                  r15->control_urb.pInBuffer += r15->control_urb.dwInLength;
                  
                  //->4948f4 - set the new expected total length
                  r15->control_urb.dwInLength = wTotalLength;
                  
                  //21ccd380 - Start to receive the full content
                  // of the next Configuration
                  (pOS->pfArmTimer)(pOS->pPrivateData, 0, 10, 0,
                     MUSB_DeviceRequestGetConfigurationDescriptor);
               }
               //->494aa4
               break;
               
            case 6:
               //->494AA4
               break;
         } //switch (r15->bData_415)
      }
   } //if (ep0 != 0)
   //21ccd114
}


/* 21ccccb0 - complete */
void MUSB_DeviceRequestGetDeviceDescriptor_8(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
   struct MGC* r13 = a->Data_20;   
   Struct_49d2fc_Inner1_3* r1 = &r13->Data_156;
   
   musb_memcpy(&r1->device_request, 
      &MUSB_rDeviceRequestGetDeviceDescriptor, 
      sizeof(MUSB_DeviceRequest));

   r1->device_request.wLength = 8;

   r1->control_urb.pOutBuffer = &r1->device_request;
   r1->control_urb.dwOutLength = sizeof(MUSB_DeviceRequest);
   r1->control_urb.pInBuffer = r1->descriptor_buffer;
   r1->control_urb.dwInLength = 8;
   
   r1->bStep = 2;
   
   MUSB_StartControlTransaction(r13->b, &r1->control_urb);
}


/* 21cccc58 - complete */
void MUSB_DeviceRequestGetDeviceDescriptor(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
   struct MGC* r13 = a->Data_20;   
   Struct_49d2fc_Inner1_3* r1 = &r13->Data_156;
   
   musb_memcpy(&r1->device_request, 
      &MUSB_rDeviceRequestGetDeviceDescriptor, 
      sizeof(MUSB_DeviceRequest));

   r1->device_request.wLength = sizeof(struct usb_device_descriptor); //USB_DT_DEVICE_SIZE;

   r1->control_urb.pOutBuffer = &r1->device_request;
   r1->control_urb.dwOutLength = sizeof(MUSB_DeviceRequest);
   r1->control_urb.pInBuffer = r1->descriptor_buffer;
   r1->control_urb.dwInLength = sizeof(struct usb_device_descriptor); //USB_DT_DEVICE_SIZE;
   
   r1->bStep = 3;
   
   MUSB_StartControlTransaction(r13->b, &r1->control_urb);
}


/* 21cccc0c - complete */
void MUSB_DeviceRequestGetConfigurationDescriptor(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
   struct MGC* r13 = a->Data_20;   
   Struct_49d2fc_Inner1_3* r1 = &r13->Data_156;
   
   musb_memcpy(&r1->device_request, 
      &MUSB_rDeviceRequestGetConfigurationDescriptor, 
      sizeof(MUSB_DeviceRequest));
   
   ((char*)&r1->device_request.wValue)[0] = r1->iConfiguration;
   r1->device_request.wLength = r1->control_urb.dwInLength;
   
   r1->control_urb.pOutBuffer = &r1->device_request;
   r1->control_urb.dwOutLength = sizeof(MUSB_DeviceRequest);
   
   MUSB_StartControlTransaction(r13->b, &r1->control_urb);
}


/* 21cccbb8 - complete */
/* v3.8: func_48E224 */
/* Send GetDescriptor(Device) for test */
void func_21cccbb8(Struct_49d2fc* a, unsigned short b/*, MGC_Timer* c*/)
{
   struct MGC* r13 = a->Data_20;  
   
   musb_memcpy(&r13->Data_156.device_request, 
      &MUSB_rDeviceRequestGetDeviceDescriptor, 
      sizeof(MUSB_DeviceRequest)); 

   r13->Data_156.device_request.wLength = sizeof(struct usb_device_descriptor); //USB_DT_DEVICE_SIZE;
   
   (r13->pfLoadFifo)(r13, 0/*bEnd*/, 
      sizeof(MUSB_DeviceRequest), 
      (void*) &MUSB_rDeviceRequestGetDeviceDescriptor);
   
   (r13->Func_680)(r13, 8);
}


/* 21cccb58 - complete */
void MUSB_DeviceRequestSetAddress(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
   struct MGC* r13 = a->Data_20;
   Struct_49d2fc_Inner1_3* r1 = &r13->Data_156;
   MUSB_DeviceRequest* pRequest = &r1->device_request;
   
   r13->address_32 = 0;
   
   (r13->Func_620)(r13);
   
   musb_memcpy(pRequest, 
      &MUSB_rDeviceRequestSetAddress,  
      sizeof(MUSB_DeviceRequest));
   
   r1->device_request.wValue = r1->bDeviceAddress;
    
   r1->control_urb.pOutBuffer = pRequest;
   r1->control_urb.dwOutLength = sizeof(MUSB_DeviceRequest);
     
   r1->bStep = 1;
   
   MUSB_StartControlTransaction(r13->b, &r1->control_urb);
}


/* 21ccc97c - todo */
int MUSB_StartEnumeration(struct MGC* a, void* b, 
                char bDeviceAddress, char d, char dwSpeed, 
                void (*f)(void* a, void* b))
{
#if 1
   FAPI_SYS_PRINT_MSG("MUSB_StartEnumeration: bDeviceAddress=%d\n", 
         bDeviceAddress);
#endif
   
   MUSB_SystemServices* pOS = a->a->pOS;
   Struct_49d2fc_Inner1_3* r14 = &a->Data_156;
      
   Struct_49d2fc_Inner1_3_2* r13 = (FAPI_SYS_Services.mallocFunc != 0)?
         (FAPI_SYS_Services.mallocFunc)(sizeof(Struct_49d2fc_Inner1_3_2)): 0;
   
   if (r13 != 0)
   {
      musb_memset(r13, 0, sizeof(Struct_49d2fc_Inner1_3_2));
      
      List_49cd8c_clear(&r13->c);   
   
      r13->d.pPrivateData = r13;
     
      r14->dwTotalLengthSum = 0;       
      r14->bDeviceAddress = bDeviceAddress;
      r14->Data_80 = &r13->d;    

      r13->d.pPort = a->b;
      
      r13->d.bHubPort = d;
      r13->d.ConnectionSpeed = dwSpeed;
      r13->d.pParentUsbDevice = b;
      
      if (b != 0) 
      {
         char* r0 = r13->d.pParentUsbDevice; //b; //r14->Data_80->pParentUsbDevice;
         r13->bData_88 = (r0[18] == 2)? 1: 0;
      }
      
      r14->Func_76 = f;
      
      r14->control_urb.pDevice = r14->Data_80;
      r14->control_urb.pfIrpComplete = MUSB_DeviceRequestComplete;
      r14->control_urb.pCompleteParam = a->a;
      r14->control_urb.pInBuffer = (void*) &r14->descriptor_buffer;
      r14->control_urb.dwInLength = sizeof(r14->descriptor_buffer);
      
      r14->bRetry = 0;
      r14->bData_419 = 0;
      
      (pOS->pfArmTimer)(pOS->pPrivateData, 0, 10, 0, MUSB_DeviceRequestSetAddress);
      
      return 1;
   }
   
   return 0;
}


/* 21ccc488 - todo */
/* v3.8: func_494510 */
int func_21ccc488(struct MGC* a)
{
#if 0
   unsigned short i;
   List_49cd8c* r15 = &a->List_524;   
   int r16 = List_49cd8c_get_element_count(r15);
   
   for (i = 0; i < r16; i++) 
   {
      MUSB_Device* r14 = List_49cd8c_get_element(r15, 0);

      func_21ccc430(r14); 
      
      List_49cd8c_remove_element(r15, r14);     
   }
#else
   FAPI_SYS_PRINT_MSG("21ccc488\n");   
#endif
   
   return 1;   
}


/* 21ccc430 - todo */
/* v3.8: func_49b418 */
void func_21ccc430(MUSB_Device* a)
{
#if 0
   MUSB_DeviceDriver* pDeviceDriver = 0;
   
   if (a != 0) 
   {
      struct MGC* r14 = a->a->Data_688;
      Struct_49d2fc_Inner1_3_2* r1 = a->b;
      
      if (r1 != 0) 
      {
         pDeviceDriver = r1->pDeviceDriver;   
      }
      
      if ((pDeviceDriver != 0) && (pDeviceDriver->e != 0)) 
      {
         (pDeviceDriver->e)(pDeviceDriver->a, r14, a);   
      }
      
      // Cleanup
      func_21ccc2b4(r14, a);
   }
#else
   FAPI_SYS_PRINT_MSG("21ccc430\n");   
#endif
}


/* 21ccc030 - todo */
int MUSB_StartControlTransaction(MUSB_Port* a,
                  MUSB_ControlIrp* pUrb)
{
#if 0
   FAPI_SYS_PRINT_MSG("MUSB_StartControlTransaction\n");
#endif
   
   int res = 165;
   struct MGC* r15 = a->pPrivateData;
   MUSB_SystemServices* pOS = r15->a->pOS;
   
   pUrb->dwActualOutLength = 0;
   pUrb->dwActualInLength = 0;
   pUrb->dwStatus = 0;

   (pOS->pfLock)(pOS->pPrivateData, 1);
   
   MGC_Endpoint* ep0 = List_49a810_get_element(&r15->ep_list, 0);
   if (ep0 != 0) 
   {
      if (ep0->pCurrentTxUrb == 0) 
      {
         ep0->bData_14 = 1;
         ep0->wTxPacketSize = pUrb->pDevice->DeviceDescriptor.bMaxPacketSize0;
         if (ep0->wTxPacketSize == 0) 
         {
            ep0->wTxPacketSize = 8;
         }
         
         res = (r15->pfSendOnEp)(r15, ep0, 
                  pUrb->pOutBuffer, 8, pUrb);
      } //if (ep0->pCurrentTxUrb == 0)
      else 
      {
         if (0 == List_49cd8c_add_element(&ep0->tx_urb_list, pUrb, 0)) 
         {
            res = 166;   
         }
      }
   } 
   
   (pOS->pfUnlock)(pOS->pPrivateData, 1);

   return res;
}


/* 21ccbd04 - complete */
/* v3.8: func_499068 */
int func_21ccbd04(struct MGC* a)
{
//   FAPI_SYS_PRINT_MSG("21ccbd04\n");   
   
   return 1;
}


/* 21ccbc10 - todo */
unsigned char MGC_AllocateDeviceAddress(Struct_49d2fc_Inner1_3* a)
{
   unsigned char bmp;
   unsigned char bit;
   unsigned char byte;
   unsigned char bDeviceAddress = 0;
      
   for (byte = 0; byte < 16; byte++) 
   {
      bmp = a->arDeviceAddressBitmap[byte];
      if (bmp < 0xFF) break;
   }
   
   if (bmp < 0xFF) 
   {
      bmp = ~bmp;
      
      for (bit = 0; bit < 8; bit++) 
      {
         if (bmp & 1) break;
         bmp >>= 1;            
      }
      
      bDeviceAddress = bit | (byte << 3);
      a->arDeviceAddressBitmap[byte] |= (1 << bit);
   }
   
   bDeviceAddress++;
   
   if ((bDeviceAddress > 127) || 
         ((bDeviceAddress == 1) && (byte != 0))) 
   {
      bDeviceAddress = 0;   
   }
   
   return bDeviceAddress;  
}


/* 21ccbbf0 - todo */
/* v3.8: func_4951A4 */
void func_21ccbbf0(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
   FAPI_SYS_PRINT_MSG("func_21ccbbf0\n");   
}


/* 21ccbb3c - complete */
/* v3.8: func_48E1F0 */
void func_21ccbb3c(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
   struct MGC* r0 = a->Data_20;   

   (r0->Func_680)(r0, 9);
}


/* 21ccbb20 - complete */
/* v3.8: func_48E2B0 */
void func_21ccbb20(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
   struct MGC* r0 = a->Data_20;   

   (r0->Func_680)(r0, 7);
}


/* 21ccba9c - complete */
/* v3.8: func_4923b8 */
int musb_log_core_options(struct MGC* a)
{
   char reg;
   int addr2 = a->a->addr2;
   
   /* log core options */ 
   MGC_SelectEnd(addr2, 0); 
   
   reg = MGC_ReadCsr8(addr2, MGC_O_HDRC_CONFIGDATA, 0);
   
   if (reg & MGC_M_CONFIGDATA_MPRXE) 
   {
      a->bBulkCombine = 1; 
      //"bulk combine"  
   }
   if (reg & MGC_M_CONFIGDATA_MPTXE) 
   {
      a->bBulkSplit = 1;
      //"bulk split"   
   }
   if (reg & MGC_M_CONFIGDATA_HBRXE) 
   {
      a->bData_41 = 1;
      //"HB-ISO Rx"   
   }
   if (reg & MGC_M_CONFIGDATA_HBTXE) 
   {
      a->bData_40 = 1;  
      //"HB-ISO Tx" 
   }
   if (reg & MGC_M_CONFIGDATA_DYNFIFO) 
   {
      //ERR("Dynamic FIFOs detected in hardware; please rebuild\n"); 
      return 0;   
   }
   
   return MGC_HdrcConfigureEps(a);   
}


/* 21ccba24 - complete */
int mgc_isr(MUSB_Controller* a)
{
   int res;
   Struct_49d2fc* p = a->pPrivateData;
   int addr2 = p->addr2;
   
   char i = MGC_Read8(addr2, MGC_O_HDRC_INDEX);
   
   char bIntrUsbValue = MGC_Read8(addr2, MGC_O_HDRC_INTRUSB);
   unsigned short wIntrTxValue = MGC_Read16(addr2, MGC_O_HDRC_INTRTX);
   unsigned short wIntrRxValue = MGC_Read16(addr2, MGC_O_HDRC_INTRRX);
         
   res = mgc_handle_interrupts(p, bIntrUsbValue, wIntrTxValue, wIntrRxValue);
   
   MGC_Write8(addr2, MGC_O_HDRC_INDEX, i);
   
   return res;   
}


/* 21ccb9ec - todo */
void func_21ccb9ec(void)
{
   FAPI_SYS_PRINT_MSG("func_21ccb9ec\n");   
}


/* 21ccb504 - todo */
void func_21ccb504(void)
{
   FAPI_SYS_PRINT_MSG("func_21ccb504\n");   
}


/*  - todo */
/* v3.8: 48ba20 - complete */
char MGC_CompletedTransfer(MGC_Message* a, MGC_Endpoint* pEndpoint, 
                  char bStatus, int bTx, void* pUrb)
{      
   MGC_Pipe* r1_;    
   MUSB_ControlIrp* pControlUrb;
   MUSB_IsochIrp* pIsochronousUrb;
   MUSB_Irp* pBulkOrInterruptUrb;
                        
   char bTrafficType;
   
   if (pUrb == 0) 
   {
      return 0;   
   }
   
   if (bTx) 
   {
      bTrafficType = pEndpoint->bTxTrafficType;
      a->bTag = 35;   
   }
   else 
   {
      bTrafficType = pEndpoint->bRxTrafficType;
      a->bTag = 36;      
   }
   
   #if 0
   FAPI_SYS_PRINT_MSG("MGC_CompletedTransfer: bStatus=0x%x, bTx=%d\n",
      bStatus, bTx);
   #endif
   
   switch (bTrafficType) 
   {
      case 1: 
         //48ba88
         pIsochronousUrb = pUrb;
         
         if (pIsochronousUrb->wCurrentFrame >= pIsochronousUrb->wFrameCount)
         {
            //48ba9c
            if (bTx) 
            {
               //48baa8
               pEndpoint->pCurrentTxUrb = 0;   
            }
            else 
            {
               //48bab0
               pEndpoint->pCurrentRxUrb = 0;
            }
         }
         //->48bac0
         break;
      
      case 3:
         //->48bac0
         break;

      default:
         //->48ba9c
         if (bTx) 
         {
            //48baa8
            pEndpoint->pCurrentTxUrb = 0;   
         }
         else 
         {
            //48bab0
            pEndpoint->pCurrentRxUrb = 0;
         }
         break;
   }
   //48bac0
   a->bEnd = pEndpoint->bEnd;
   a->bStatus = bStatus;
   a->pUrb = pUrb;
   
   switch (bTrafficType) 
   {
      case 0:
         //48BAEC 
         pControlUrb = pUrb;
         pControlUrb->dwStatus = bStatus;
         break;
         
      case 2:
      case 3:
         //48BAF4    
         pBulkOrInterruptUrb = pUrb;         
         pBulkOrInterruptUrb->dwStatus = bStatus;
         
         if (bTx) 
         {
            pBulkOrInterruptUrb->dwActualLength = pEndpoint->dwOffset;
            pEndpoint->dwOffset = 0;
            pEndpoint->dwRequestSize = 0;   
         }
         else 
         {
            pBulkOrInterruptUrb->dwActualLength = pEndpoint->Data_64;
            pEndpoint->Data_64 = 0;   
         }
         //48bb28
         if (pBulkOrInterruptUrb->bIsrCallback != 0)
         {
            (pBulkOrInterruptUrb->pfIrpComplete)(
               pBulkOrInterruptUrb->pCompleteParam,
               pBulkOrInterruptUrb); 
            
            r1_ = pBulkOrInterruptUrb->hPipe;
            
            if (!bTx) 
            {
               //48bb50
               if (((bTrafficType == 3) || (pEndpoint->pCurrentRxUrb == 0)) &&
                  (pEndpoint->bData_19 == 0))
               {
                  //48bb64 
                  func_21ccf72c(r1_->Data_8, pEndpoint, 0);
               }
            } //if (!bTx)
            else 
            {
               //48bc14
               if ((pEndpoint->pCurrentTxUrb == 0) && (pEndpoint->bData_18 == 0))
               {
                  //48bb80
                  func_21ccf72c(r1_->Data_8, pEndpoint, 1);   
               }
            }
            //48bc30
            return 0;
         } //if (r14->i != 0)
         //->48bc3c
         break;
         
      case 1:
         //48BBB4  
         pIsochronousUrb = pUrb;
      
         if ((bStatus != 0) || 
            (pIsochronousUrb->wCurrentFrame >= pIsochronousUrb->wFrameCount))
         {
            //48bbd0
            if (bTx) 
            {
               //48bbdc
               pEndpoint->dwOffset = 0;   
               pEndpoint->dwRequestSize = 0;
            }
            else 
            {
               //48bbe8
               pEndpoint->Data_64 = 0;
            }
         }
         //48bbec
         if (pIsochronousUrb->bIsrCallback != 0)
         {
            //48bbf8
            (pIsochronousUrb->pfIrpComplete)(
               pIsochronousUrb->pCompleteParam, pIsochronousUrb);

            r1_ = pBulkOrInterruptUrb->hPipe;
         
            if (bTx) 
            {
               //48bc14
               if ((pEndpoint->pCurrentTxUrb == 0) && (pEndpoint->bData_18 == 0)) 
               {
                  //48bb80
                  func_21ccf72c(r1_->Data_8, pEndpoint, 1);   
               }
            } //if (d != 0)
            else 
            {
               //48bc24
               if ((pEndpoint->pCurrentRxUrb == 0) && (pEndpoint->bData_19 == 0))
               {
                  //48bb64
                  func_21ccf72c(r1_->Data_8, pEndpoint, 0);
               }
            }
            //48bc30
            return 0;
         } //if (r1->bData_34 != 0)
         //->48bc3c
         break;
         
      default:
         //48bad8
         if (bTx) 
         {
            pEndpoint->dwOffset = 0;   
         }
         else 
         {
            pEndpoint->Data_64 = 0;   
         }
         //->48bc3c
         break;
   }
   
   return 1;
}


/*  - todo */
/* v3.8: 490980 - todo */
int func_490980(struct MGC* a, char* b)
{
   FAPI_SYS_PRINT_MSG("func_490980\n");
   
   return 0;
}


/*  - todo */
/* v3.8: 48d9bc - complete */
int func_48d9bc(struct MGC* a, unsigned short b, 
                  char c, char* d)
{
   void* r21;
   unsigned short r14;
   
   int res = 0;
   
   MGC_Endpoint* ep0 = List_49a810_get_element(&a->ep_list, 0);
   MUSB_ControlIrp* pControlUrb = ep0->pCurrentTxUrb;
   char* r2 = pControlUrb->pOutBuffer;
   
   if (c != 0) 
   {
      pControlUrb->dwStatus = c;
      return 0;   
   }

   #if 0
   FAPI_SYS_PRINT_MSG("func_48d9bc: ep0->bData_14=%d\n",
      ep0->bData_14); 
   #endif

   if (ep0->bData_14 == 0) 
   {
      r21 = &pControlUrb->pInBuffer[pControlUrb->dwActualInLength];
      
      r14 = (b < (pControlUrb->dwInLength - pControlUrb->dwActualInLength))?
         b: 
         (pControlUrb->dwInLength - pControlUrb->dwActualInLength);
         
      if (ep0->pTxDmaChannel != 0) 
      {
         if (0 != (a->pDmaController->pfDmaProgramChannel)(
            ep0->pTxDmaChannel,
            ep0->wTxPacketSize,
            ep0->pTxDmaChannel->bDesiredMode,
            r21,
            (r14 <= ep0->pTxDmaChannel->dwMaxLength)?
               r14: ep0->pTxDmaChannel->dwMaxLength)) 
         {
            *d = 0;
            return 1;               
         }
      }

      (a->pfUnloadFifo)(a, 0/*bEnd*/, r14/*wCount*/, r21/*pDest*/);
      
      pControlUrb->dwActualInLength += r14;
      
      if ((pControlUrb->dwActualInLength < pControlUrb->dwInLength) && (b == ep0->wTxPacketSize))
      {
         res = 1;
      }
   } //if (ep0->bData_14 == 0)
   else 
   {
      pControlUrb->dwActualOutLength += ep0->dwRequestSize;
      ep0->dwRequestSize = 0;
      
      if (r2[0] & USB_DIR_IN) 
      {
         ep0->bData_14 = 0; 
         
         if (ep0->pTxDmaChannel != 0) 
         {
            (a->pDmaController->pfDmaReleaseChannel)(ep0->pTxDmaChannel);
            
            ep0->pTxDmaChannel = (void*) (a->pDmaController->pfDmaAllocateChannel)(
               a->pDmaController->pPrivateData,
               0, 0, 0, ep0->wTxPacketSize);
            
            if (ep0->pTxDmaChannel != 0) 
            {
               ep0->pTxDmaChannel->dwActualLength = 0;   
               
               if (0 == (a->pDmaController->pfDmaProgramChannel)(
                  ep0->pTxDmaChannel,
                  ep0->wTxPacketSize,
                  ep0->pTxDmaChannel->bDesiredMode,
                  pControlUrb->pInBuffer,
                  (ep0->pTxDmaChannel->dwMaxLength < pControlUrb->dwInLength)?
                     ep0->pTxDmaChannel->dwMaxLength: pControlUrb->dwInLength))
               {
                  (a->pDmaController->pfDmaReleaseChannel)(ep0->pTxDmaChannel);
                  ep0->pTxDmaChannel = 0;
               }
            }
         }
         res = 1;
      } //if (r2[0] & USB_DIR_IN)
      else 
      {
         //USB_DIR_OUT
         
         r21 = &((char*)pControlUrb->pOutBuffer)[pControlUrb->dwActualOutLength];
         
         r14 = ((pControlUrb->dwOutLength - pControlUrb->dwActualOutLength) > ep0->wTxPacketSize)?
            ep0->wTxPacketSize:
            pControlUrb->dwOutLength - pControlUrb->dwActualOutLength;
            
         if (ep0->pTxDmaChannel != 0) 
         {
            if (0 != (a->pDmaController->pfDmaProgramChannel)(
               ep0->pTxDmaChannel,
               ep0->wTxPacketSize,
               ep0->pTxDmaChannel->bDesiredMode,
               r21,
               (r14 <= ep0->pTxDmaChannel->dwMaxLength)? 
                  r14: ep0->pTxDmaChannel->dwMaxLength)) 
            {
               *d = 0;
               return 1;
            }               
         }

         if (r14 != 0) 
         {
            (a->pfLoadFifo)(a, 0/*bEnd*/, 
               r14/*wCount*/, r21/*pDest*/);

            ep0->dwRequestSize = r14;  

            res = 1; 
         }
      } //USB_DIR_OUT
   }

   if (res != 0) 
   {
      *d = (ep0->bData_14 != 0)? 
         MGC_M_CSR0_TXPKTRDY: 0x20;            
      
      return res;
   }

   *d = 0;
   return res;
}


/*  - todo */
/* v3.8: 48d5c0 - todo */
int MUSB_ContinueControlTransaction(struct MGC* a, MGC_Message* b, 
                  unsigned short* pwCsrVal, unsigned short wCount)
{
   char* pBuffer;
   //MGC_DmaChannel* pDmaChannel;
   unsigned short wDeviceTransferSize;
   unsigned short r19/*wCsrVal*/ = *pwCsrVal;
   char fp1 = 0;
   int r15 = 0;
   char fp2 = 0;
   
   MGC_Endpoint* r17 = List_49a810_get_element(&a->ep_list, 0);
   
   if (a->bHostMode != 0) 
   {
      #if 0
      FAPI_SYS_PRINT_MSG("MUSB_ContinueControlTransaction: r17->bData_14=%d\n",
         r17->bData_14);
      #endif

      // Control Transfer can be:
      //Zero Data Request: SETUP command -> IN Status Phase
      //Write Request: SETUP command -> OUT Data Phase -> IN Status Phase
      //Read Request: SETUP command > IN Data Phase -> OUT Status Phase 
      
      if ((fp2 != 0) || 
         ((r17->bData_14 != 0) && (r17->dwRequestSize == 0)) ||
         ((r17->bData_14 == 0) && (wCount == 0)))
      {
         //48d664
         *pwCsrVal = 0;
         
         MGC_CompletedTransfer(b, r17, 
            fp2/*bStatus*/, 
            1/*bTx*/, 
            r17->pCurrentTxUrb);

         func_49a050(a);
         //->48d928
         return 1;
      }
      else 
      {
         //48d688  
         if (0 == func_48d9bc(a, wCount, fp2, &fp1)) 
         {
            if (r17->bData_14 != 0) 
            {
               // IN Status phase (following a SETUP phase or
               // a OUT Data phase)
               
               r17->bData_14 = 0;
               //->48d988   
               *pwCsrVal = (MGC_M_CSR0_H_STATUSPKT|MGC_M_CSR0_H_REQPKT);
            }
            else 
            {
               // OUT Status phase (following an IN Data phase)
            
               r17->bData_14 = 1;
               r17->dwRequestSize = 0;   
               //->48d988
               *pwCsrVal = (MGC_M_CSR0_H_STATUSPKT|MGC_M_CSR0_TXPKTRDY);   
            }
            return r15;
         }
         #if 0
         else 
         {
            //->48d984
            *pwCsrVal = fp1;
         }
         #endif
      }
   } //if (a->bHostMode != 0)
   else 
   {
      //48d6cc - Device Mode
      if (r19 & MGC_M_CSR0_RXPKTRDY) 
      {
         a->bData_57 = 0;
         
         wDeviceTransferSize = (a->pDevice != 0)? 
            a->pDevice->wControlBufferLength -
               a->wDeviceBufferOffset: 
            0;

         if (wDeviceTransferSize > wCount) 
         {
            wDeviceTransferSize = wCount;   
         }
         
         if ((a->pDevice == 0) || (wDeviceTransferSize < wCount)) 
         {
            //->48d988
            fp1 |= MGC_M_CSR0_P_SENDSTALL;      
            
            #if 0
            *pwCsrVal = fp1;
            #endif
         }
         else 
         {
            //48d734
            pBuffer = &a->pDevice->pControlBuffer[a->wDeviceBufferOffset];
            
            MGC_DmaChannel* pDmaChannel = r17->pTxDmaChannel;
            if (pDmaChannel != 0) 
            {
               if (0 != (a->pDmaController->pfDmaProgramChannel)(
                  pDmaChannel, 
                  r17->wTxPacketSize, 
                  pDmaChannel->bDesiredMode,
                  pBuffer,
                  (wDeviceTransferSize <= pDmaChannel->dwMaxLength)? 
                     wDeviceTransferSize: pDmaChannel->dwMaxLength))                                          
               {
                  //->48d924
                  *pwCsrVal = 0;
                  return 1;
               }
            }
            //48d77c
            (a->pfUnloadFifo)(a, 0/*bEnd*/, wDeviceTransferSize, pBuffer);
            
            a->wDeviceBufferOffset += wDeviceTransferSize;
            
            char* r2 = a->pDevice->pControlBuffer;
            
            if (r2[0] & 0x80) goto label_48d7d8;
            else 
            {
               int r5 = a->wDeviceBufferOffset - 8;
               if (r5 >= ((r2[7] << 8) | r2[6]))
               {          
label_48d7d8:     
                  //48d7d8
                  a->dwDeviceSequenceNum++;
                  a->wDeviceTxSize = a->wDeviceBufferOffset;
                  a->wDeviceBufferOffset = 0;
               
                  if (a->bData_581 != 0) 
                  {
                     if (0 == func_490980(a, &fp2)) 
                     {
                        if (0 == (a->pDevice->pfDeviceRequest)(
                           a->pDevice->pPrivateData,
                           a, 
                           a->dwDeviceSequenceNum,
                           a->pDevice->pControlBuffer,
                           a->wDeviceBufferOffset)) 
                        {
                           (a->Func_648)(a, 1);                           
                        }
                        #if 0
                        //->48d984
                        *pwCsrVal = fp1;
                        #endif
                     }
                     else 
                     {
                        //48d848
                        if (fp2 == 0x80) 
                        {
                           a->wDeviceBufferOffset = 0;
                           a->wDeviceTxSize = 0;
                           //->48d834
                           (a->Func_648)(a, 1);
                           #if 0
                           //->48d984
                           *pwCsrVal = fp1;
                           #endif
                        }
                        else 
                        {
                           //48d860
                           (a->Func_648)(a, 0);
                           #if 0
                           //->48d984
                           *pwCsrVal = fp1;
                           #endif
                        }
                     }
                  } //if (a->bData_581 != 0)
                  else 
                  {
                     //48d874
                     if (0 == (a->pDevice->pfDeviceRequest)(
                        a->pDevice->pPrivateData,
                        a, 
                        a->dwDeviceSequenceNum,
                        a->pDevice->pControlBuffer,
                        0)) 
                     {
                        (a->Func_648)(a, 1);                           
                     }
                     #if 0
                     //->48d984
                     *pwCsrVal = fp1;
                     #endif
                  }
               }
               else 
               {
                  //48d88c
                  *pwCsrVal = 0x40;
                  return r15;
               }
            }
         }
      } //if (r19 & MGC_M_CSR0_RXPKTRDY)
      //48d894
      else if ((r19 & MGC_M_CSR0_TXPKTRDY) == 0)
      {
         //48d8a0
         wDeviceTransferSize = 
            ((a->wDeviceTxSize - a->wDeviceBufferOffset) > 64)? 64: 
               (a->wDeviceTxSize - a->wDeviceBufferOffset);
         
         if (wDeviceTransferSize == 0) 
         {
            // Nothing to send
            a->wDeviceBufferOffset = 0;
            a->wDeviceTxSize = 0;
            return 0;
         }
         else 
         {
            //48d8e0
            pBuffer = &a->pDeviceTransferBuffer[a->wDeviceBufferOffset];
            
            MGC_DmaChannel* pDmaChannel = r17->pTxDmaChannel;
            if (pDmaChannel != 0) 
            {
               if (0 != (a->pDmaController->pfDmaProgramChannel)(
                  pDmaChannel, 
                  r17->wTxPacketSize, 
                  pDmaChannel->bDesiredMode,
                  pBuffer,
                  (wDeviceTransferSize <= pDmaChannel->dwMaxLength)? 
                     wDeviceTransferSize: pDmaChannel->dwMaxLength)) 
               {
                  *pwCsrVal = 0;
                  return 1;   
               }
               //48d930
            }
            //48d930
            (a->pfLoadFifo)(a, 0/*bEnd*/, wDeviceTransferSize, pBuffer);

            a->wDeviceBufferOffset += wDeviceTransferSize;
            
            fp1 |= MGC_M_CSR0_TXPKTRDY;
            
            if (a->wDeviceBufferOffset >= a->wDeviceTxSize) 
            {
               fp1 |= MGC_M_CSR0_P_DATAEND;
               
               a->wDeviceBufferOffset = 0;
               a->wDeviceTxSize = 0;
               
               #if 0
               *pwCsrVal = fp1;   
               #endif
            }
            else 
            {
               #if 0
               *pwCsrVal = fp1;   
               #endif
            }            
         }
      } //else if ((r19 & MGC_M_CSR0_TXPKTRDY) == 0)
      else 
      {
         #if 0
         //48d984
         *pwCsrVal = fp1;
         #endif
      }
   } //Device Mode
   
   //48d984
   *pwCsrVal = fp1;
   
   return r15;   
}


/*  - todo */
/* 49a050 - complete */
int func_49a050(struct MGC* a)
{
   int res = 0;
   
   MGC_Endpoint* ep0 = List_49a810_get_element(&a->ep_list, 0);
   if (ep0 != 0) 
   {
      MUSB_ControlIrp* pControlUrb = List_49cd8c_get_element(&ep0->tx_urb_list, 0);
      if (pControlUrb != 0) 
      {
         res = 1;
         List_49cd8c_remove_element(&ep0->tx_urb_list, pControlUrb);
         
         pControlUrb->dwActualOutLength = 0;
         pControlUrb->dwActualInLength = 0;
         pControlUrb->dwStatus = 0;
         ep0->bData_14 = 1;
         
         (a->pfSendOnEp)(a, ep0, pControlUrb->pOutBuffer, 8, pControlUrb);
      }
      else 
      {
         ep0->pCurrentTxUrb = 0;   
      }
   }
   
   return res;
}


/* 21ccb22c - todo */
int MGC_HdrcServiceDefaultEnd(struct MGC* a, MGC_Message* b)
{   
   unsigned short wCsrVal, wCount;
   
   int bError = 0;
   int res = 0;
   char status = 0;
   
   MGC_Endpoint* r19 = List_49a810_get_element(&a->ep_list, 0);
   int pBase = a->a->addr1;
   
   MGC_SelectEnd(pBase, 0);
   wCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_CSR0, 0);
   wCount = MGC_ReadCsr8(pBase, MGC_O_HDRC_COUNT0, 0);
   
   #if 0
   FAPI_SYS_PRINT_MSG("MGC_HdrcServiceDefaultEnd: wCsrVal=0x%x, wCount=%d\n",
      wCsrVal, wCount);
   #endif
   
   if (a->bHostMode != 0) 
   {      
      /* MGC_HdrcServiceDefaultEnd ???*/
   
      if (wCsrVal & MGC_M_CSR0_H_RXSTALL) 
      {
         #if 1
         FAPI_SYS_PRINT_MSG("MGC_HdrcServiceDefaultEnd: STALLING ENDPOINT 0\n");
         #endif
          
         status = 0x80; //USB_ST_STALL
         bError = 1;   
      }
      else if (wCsrVal & MGC_M_CSR0_H_ERROR)
      {
         #if 1
         FAPI_SYS_PRINT_MSG("MGC_HdrcServiceDefaultEnd: ep0 no response (error)\n");
         #endif
           
         status = 0x82; //USB_ST_NORESPONSE
         bError = 1;   
      }
      else if (wCsrVal & MGC_M_CSR0_H_NAKTIMEOUT) 
      {
         #if 1
         FAPI_SYS_PRINT_MSG("MGC_HdrcServiceDefaultEnd: ep0 NAK timeout\n");
         #endif
         
         status = 0x82; //USB_ST_NORESPONSE
         bError = 1;   
      }

      if (0x82/*USB_ST_NORESPONSE*/ == status) 
      {
         #if 1
         FAPI_SYS_PRINT_MSG("MGC_HdrcServiceDefaultEnd: ep0 aborting\n");
         #endif
         
         /* use the proper sequence to abort the transfer */ 
         if (wCsrVal & MGC_M_CSR0_H_REQPKT) 
         {
            wCsrVal &= ~MGC_M_CSR0_H_REQPKT;   
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);
            wCsrVal &= ~MGC_M_CSR0_H_NAKTIMEOUT;         
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);         
         }
         else 
         {
            wCsrVal |= MGC_M_CSR0_FLUSHFIFO;            
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);
            wCsrVal &= ~MGC_M_CSR0_H_NAKTIMEOUT;         
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);         
         }
         
         #if 0
         // Why not?
         MGC_WriteCsr8(r16, MGC_O_HDRC_NAKLIMIT0, 0, 0); 
         #endif
      }

      if (bError != 0) 
      {
         #if 1
         FAPI_SYS_PRINT_MSG("MGC_HdrcServiceDefaultEnd: ep0 handling error\n");
         #endif
         
         /* clear it */    
         MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, 0);         
      }
   } //if (a->bHostMode != 0)
   else 
   {
      // Device Mode??
      
      /* MGC_HdrcServiceDeviceDefaultEnd -> */
      /* MGC_HdrcServiceFunctionEp0 ??? */
      
      if (wCsrVal & MGC_M_CSR0_P_SENTSTALL) 
      {
         //DBG(3, "sentstall\n");
         
         bError = 1;
         wCsrVal &= ~MGC_M_CSR0_P_SENTSTALL;
         MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);         
      }
      else if (wCsrVal & MGC_M_CSR0_P_SETUPEND) 
      {
         //DBG(3, "setup end\n");
         
         a->wDeviceBufferOffset = 0;
         a->wDeviceTxSize = 0;  
      }         
      else if ((wCount == 0) && (a->bData_57 != 0))
      {
         //492c80
         a->bData_57 = 0;   
         
         MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, 0);
         
         if (a->bDeviceHasAddress != 0) 
         {
            //492cac
            func_21cc96e8(a);
            
            a->bDeviceHasAddress = 0;   
         }

         if (a->bTestMode != 0) 
         {
            a->bTestMode = 0;
            char bTestMode = 0;
            
            switch (a->bTestModeValue) 
            {
               case 1:
                  bTestMode = MGC_M_TEST_J;
                  break;  
                  
               case 2:
                  bTestMode = MGC_M_TEST_K;
                  break; 
                  
               case 3:
                  bTestMode = MGC_M_TEST_SE0_NAK;
                  break;
                  
               case 4:
                  bTestMode = MGC_M_TEST_PACKET;

                  MGC_HdrcLoadFifo(a, /*bEnd*/0, 
                     sizeof(MGC_aTestPacket), MGC_aTestPacket);                     
                  /* despite explicit instruction, we still must kick-start */ 
                  MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_TXPKTRDY);                     
                  break;
            }
            
            MGC_Write8(pBase, MGC_O_HDRC_TESTMODE, bTestMode);                              
         }

         return 0;
      } //else if ((wCount == 0) && (a->bData_57 != 0))
   } // Device Mode

   if (bError != 0) 
   {
      if (a->bHostMode != 0) 
      {
         if (0 != MGC_CompletedTransfer(b, r19, status, 1, r19->pCurrentTxUrb)) 
         {
            res = 1;   
         }
         
         func_49a050(a);
      }
   } //if (bError != 0)
   else 
   {
      res = MUSB_ContinueControlTransaction(a, b, &wCsrVal, wCount);
      
      if (wCsrVal != 0) 
      {
         if ((a->bHostMode != 0) && (a->bData_62 != 0)) 
         {
            wCsrVal |= MGC_M_CSR0_H_NO_PING;               
         }
         
         MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);
      }
   }

   return res;   
}


/* 21ccb018 - todo */
void func_21ccb018(void)
{
   FAPI_SYS_PRINT_MSG("func_21ccb018\n");   
}


/* 21ccacd8 - todo */
/* v3.8: 492E30 - todo */
int MGC_HdrcServiceRxReady(struct MGC* a, unsigned short wEnd, MGC_Message* c)
{
//   FAPI_SYS_PRINT_MSG("MGC_HdrcServiceRxReady\n");   

   unsigned short wRxCount;
   unsigned short wRxCsrVal;
   MGC_Endpoint* pEnd;
   
   char bEnd = wEnd;
   char r13 = 0;
   int status = 0;
   int pBase = a->a->addr2;
   
   c->bStatus = 0;
   c->bTag = 0;
   
   MGC_SelectEnd(pBase, bEnd);   
   wRxCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
   pEnd = List_49a810_get_element(&a->ep_list, wEnd);

   if (a->bHostMode != 0) 
   {
      //MGC_HdrcServiceRxReady
      
      if (wRxCsrVal & MGC_M_RXCSR_H_RXSTALL) 
      {
         status = 128; //USB_ST_STALL
         wRxCsrVal &= ~MGC_M_RXCSR_H_REQPKT;
         MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wRxCsrVal);   
      }
      //492ef8
      if (wRxCsrVal & (MGC_M_RXCSR_H_ERROR | MGC_M_RXCSR_DATAERROR)) 
      {
         status = 137;
         wRxCsrVal &= ~MGC_M_RXCSR_H_REQPKT;
         MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wRxCsrVal);               
      }
      //492f20
      /* be sure a packet is ready for unloading */
      if ((status == 0) && !(wRxCsrVal & MGC_M_RXCSR_RXPKTRDY))
      {
         status = 135; //USB_ST_INTERNALERROR  
         wRxCsrVal &= ~MGC_M_RXCSR_H_REQPKT;
         MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wRxCsrVal);               
      }
      //492f50
      if (status != 0) 
      {
         wRxCsrVal &= ~(MGC_M_RXCSR_H_ERROR | MGC_M_RXCSR_DATAERROR |
            MGC_M_RXCSR_H_RXSTALL | MGC_M_RXCSR_RXPKTRDY);             
         //wRxCsrVal &= ~MGC_M_RXCSR_RXPKTRDY;
         MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wRxCsrVal);               

         r13 = status;         
         MGC_CompletedTransfer(c, pEnd, r13, 0, pEnd->pCurrentRxUrb);
         //->493084
         r13 = 1; 
      } //if (status != 0)
      else 
      {
         //492f94
         if (pEnd != 0) 
         {
            if (pEnd->pCurrentRxUrb != 0) 
            {
               if (pEnd->pRxDmaChannel != 0) 
               {
                  if (1 == (a->pDmaController->pfDmaGetChannelStatus)(
                     pEnd->pRxDmaChannel)) 
                  {
                     pEnd->Data_64/*dwOffset*/ = pEnd->pRxDmaChannel->dwActualLength;                     
                  }
               }
               //492fdc
               wRxCount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);
               r13 = func_498c10(a, status, pEnd, wRxCount, 1, pEnd->pCurrentRxUrb);
               if (r13 != 0)
               {
                  if (pEnd->pRxDmaChannel != 0) 
                  {
                     (a->pDmaController->pfDmaReleaseChannel)(pEnd->pRxDmaChannel);
                     pEnd->pRxDmaChannel = 0;
                  }
                  //493038
                  wRxCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
                  wRxCsrVal &= ~MGC_M_RXCSR_RXPKTRDY;
                  MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wRxCsrVal);
                  
                  r13 = status;
                  r13 = MGC_CompletedTransfer(c, pEnd, r13, 0/*bTx*/, pEnd->pCurrentRxUrb);
                  //->493290
               }
            } //if (pEnd->Data_48 != 0)
            else 
            {
               //493084
               r13 = 1;
            }
         } //if (pEnd != 0)
         else 
         {
            //493084
            r13 = 1;
         }
      }
   } //if (a->bHostMode != 0)
   else 
   { 
      //MGC_HdrcServiceDeviceRxReady

      if (wRxCsrVal & MGC_M_RXCSR_P_SENTSTALL) 
      {
         wRxCsrVal &= ~MGC_M_RXCSR_P_SENTSTALL; 
         MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wRxCsrVal);
         //->493290  
      }
      //4930a0
      else if (wRxCsrVal & MGC_M_RXCSR_P_OVERRUN) 
      {
         wRxCsrVal &= ~MGC_M_RXCSR_P_OVERRUN;
         MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wRxCsrVal);   
         //->493290
      }
      else 
      {
         //4930c4   
         if ((pEnd == 0) || (pEnd->halted_15 != 0))
         {
            //4930d8
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_P_SENDSTALL);   
            //->493290
         }
         else 
         {
            //4930ec
            if (pEnd->pCurrentRxUrb != 0) 
            {
               while ((pEnd->pCurrentRxUrb != 0) && 
                  ((pEnd->bData_95 != pEnd->bData_94) || 
                  (wRxCsrVal & MGC_M_RXCSR_RXPKTRDY))) 
               {
                  //493114
                  if (pEnd->bData_95 != pEnd->bData_94) 
                  {
                     pEnd->bData_95++;   
                  }
               
                  if (pEnd->pRxDmaChannel != 0) 
                  {
                     if (1 == (a->pDmaController->pfDmaGetChannelStatus)(
                        pEnd->pRxDmaChannel)) 
                     {
                        pEnd->Data_64 = pEnd->pRxDmaChannel->dwActualLength;                     
                        pEnd->bData_81 = 0;
                     }
                  }
                  //493164
                  if (pEnd->bData_81 == 0) 
                  {
                     wRxCount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);
                     r13 = func_498c10(a, status, pEnd, wRxCount, 0, pEnd->pCurrentRxUrb);
                     wRxCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
                     if ((wRxCount < pEnd->wRxPacketSize) || 
                        !(wRxCsrVal & MGC_M_RXCSR_AUTOCLEAR))
                     {
                        //4931cc
                        wRxCsrVal &= ~MGC_M_RXCSR_RXPKTRDY;
                        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wRxCsrVal);   
                     }
                     //4931e4
                     if (r13 != 0) 
                     {
                        r13 = status;
                     
                        if (pEnd->pRxDmaChannel != 0) 
                        {
                           (a->pDmaController->pfDmaReleaseChannel)(pEnd->pRxDmaChannel);
                           pEnd->pRxDmaChannel = 0;
                        }
                        //493214
                        r13 = MGC_CompletedTransfer(c, pEnd, r13, 0, pEnd->pCurrentRxUrb);
                     
                        int r0 = (wRxCsrVal & ~(MGC_M_RXCSR_DMAMODE | MGC_M_RXCSR_DMAENAB));
                        MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR+1, bEnd, 
                            r0 >> 8);
                     }
                  } //if (pEnd->bData_81 == 0)
                  else 
                  {
                     //49327c
                     return 0;
                  }
               } //while ((pEnd->Data_48 != 0) && 
               //   ((pEnd->bData_95 != pEnd->bData_94) || (wRxCsrVal & 0x01))) 
            } //if (pEnd->Data_48 != 0)
            else 
            {
               //493284
               pEnd->bData_94++;
            }
         }
      }
   }   
   //493290
   return r13;   
}


/* 21ccabf0 - todo */
void func_21ccabf0(void)
{
   FAPI_SYS_PRINT_MSG("func_21ccabf0\n");   
}


/* 21cca940 - todo */
void func_21cca940(void)
{
   FAPI_SYS_PRINT_MSG("func_21cca940\n");   
}


/* 21cca898 - todo */
int MGC_HdrcLoadFifo(struct MGC* a, char bEnd, 
      unsigned wCount, const char* pSource)
{
   char bFifoOffset;
   unsigned wIndex32;
   unsigned wCount32 = ((unsigned)pSource%4)? 0: wCount/4;
   int pBase = a->a->addr2;
   
   bFifoOffset = MGC_FIFO_OFFSET(bEnd);

   /* doublewords when possible */
   unsigned wIndex = 0;
   for (wIndex32 = 0; wIndex32 < wCount32; wIndex32++, wIndex += 4) 
   {
      MGC_Write32(pBase, bFifoOffset, *((int*)&(pSource[wIndex])));
   }
    
   for (; wIndex < wCount; wIndex++)
   {
      MGC_Write8(pBase, bFifoOffset, pSource[wIndex]);
   } 
   
   return 1;
}


/* 21cca7f0 - todo */
int MGC_HdrcUnloadFifo(struct MGC* a, char bEnd, 
      unsigned short wCount, char* pDest)
{
   unsigned wIndex32, wIndex;
   unsigned wCount32 = (((int)pDest) & 0x03)? 0: wCount/4;   
   int pBase = a->a->addr2;
   char bFifoOffset = MGC_FIFO_OFFSET(bEnd);
   
   for (wIndex = wIndex32 = 0; wIndex32 < wCount32; wIndex32++, wIndex += 4) 
   {
      *((unsigned*)(&pDest[wIndex])) = MGC_Read32(pBase, bFifoOffset);   
   }
   
   while (wIndex < wCount) 
   {
      pDest[wIndex++] = MGC_Read8(pBase, bFifoOffset);   
   }
   
   return 1;
}


/* v3.8: 498e2c - todo */
int func_498e2c(struct MGC* r14, MGC_Endpoint* pEndpoint)
{   
   unsigned r18;
   void* pUrb;
   char* r17 = 0;
   int res = 0;   
   
   if (pEndpoint->pCurrentTxUrb != 0) 
   {
      MUSB_IsochIrp* r20;
      MUSB_Irp* r21;
      
      switch (pEndpoint->bTxTrafficType) 
      {
         case 1:
            //498e84 - Isochronous
            pUrb = r20 = pEndpoint->pCurrentTxUrb;
                     
            int r0 = r20->wCurrentFrame;
            
            r20->adwLength[r0] = r20->adwLength[r0];
                     
            r20->adwStatus[r0] = 0;
                     
            r20->wCurrentFrame++;
                     
            r18 = r20->adwLength[r0 + 1];

            r17 = r20->pBuffer + pEndpoint->dwOffset;
         
            if (r20->wCurrentFrame >= r20->wFrameCount)
            {
               res = 1;   
            }
         
            if ((r20->wCallbackInterval != 0) &&
               ((r20->wCurrentFrame % r20->wCallbackInterval) == 0))
            {
               res = 1;     
            }
            //->498f98            
            break;

         case 2:
         case 3: 
            //498f0c - Bulk / Interrupt
            pUrb = r21 = pEndpoint->pCurrentTxUrb;

            r18 = r21->dwLength;
         
            if (r18 <= pEndpoint->dwOffset) 
            {
               if ((r21->bAllowShortTransfer != 0) &&
                  ((pEndpoint->dwRequestSize % pEndpoint->wTxPacketSize) == 0)) 
               {
                  (r14->pfSendOnEp)(r14, pEndpoint, r17, 0, r21);
               
                  return 0;
               }
               //498f6c
               res = 1;
            }
            //498f70
            r18 -= pEndpoint->dwOffset;
            r17 = (char*) ((MUSB_Irp*)r21)->pBuffer + pEndpoint->dwOffset;
            //->498f98                        
            break;               
                
         default: 
            //498f90 
            res = 1;
            r18 = 0;
            break;   
      } //switch (pEndpoint->bTxTrafficType)
   } //if (pEndpoint->pCurrentTxUrb != 0)
   else 
   {
      //498f04
      res = 1;  
   }
   //498f98
   if (res == 0) 
   {
      (r14->pfSendOnEp)(r14, pEndpoint, r17, r18, pUrb);   
   }
   
   return res;
}


/* 21cc9e6c - todo */
/* v3.8: 4932C8 - complete */
int MGC_HdrcServiceTxAvail(struct MGC* a, unsigned short wEnd, MGC_Message* c)
{
//   FAPI_SYS_PRINT_MSG("MGC_HdrcServiceTxAvail\n");
   
   unsigned short wVal;
   unsigned short wTxCsrVal;
   MGC_Endpoint* pEnd;
   
   int r14 = 0;
   int status = 0;
   char bEnd = wEnd;
   int pBase = a->a->addr2;
   
   c->bStatus = 0;
   c->bTag = 0;
   
   MGC_SelectEnd(pBase, bEnd);
   wTxCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
   pEnd = List_49a810_get_element(&a->ep_list, wEnd);
   
   if (a->bHostMode != 0) 
   {
      //MGC_HdrcServiceTxAvail ??
      
      if (wTxCsrVal & MGC_M_TXCSR_H_RXSTALL) 
      {
         //DBG(1, "TX end %d stall\n", bEnd);
         status = 128; //USB_ST_STALL  
      }

      if ((wTxCsrVal & MGC_M_TXCSR_H_ERROR) || 
         (wTxCsrVal & MGC_M_TXCSR_H_NAKTIMEOUT)) 
      {
         //WARN("TX data error on ep=%d\n", bEnd);
         
         status = 136; //USB_ST_NORESPONSE
         
         /* do the proper sequence to abort the transfer */
         wTxCsrVal &= ~MGC_M_TXCSR_FIFONOTEMPTY;
         wTxCsrVal |= MGC_M_TXCSR_FLUSHFIFO;
         
         MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wTxCsrVal);
         MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wTxCsrVal);
      }

      if (status != 0) 
      {
         /* reset error bits */
         wTxCsrVal &= ~(MGC_M_TXCSR_H_ERROR | MGC_M_TXCSR_H_RXSTALL |
                  MGC_M_TXCSR_H_NAKTIMEOUT |
                  MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE);                  
         wTxCsrVal |= MGC_M_TXCSR_FRCDATATOG;
         MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wTxCsrVal);
         
         r14 = MGC_CompletedTransfer(c, pEnd, status, 1/*bTx*/, pEnd->pCurrentTxUrb);
         if (pEnd->bData_18 != 0) 
         {
            pEnd->halted_15 = 1;   
         }
         return r14;
      } //if (status != 0)
      else if (pEnd != 0) 
      {
         if (pEnd->pTxDmaChannel != 0) 
         {
            if (1 == (a->pDmaController->pfDmaGetChannelStatus)(
               pEnd->pTxDmaChannel)) 
            {
               pEnd->dwOffset = pEnd->pTxDmaChannel->dwActualLength;   
            }
         }
         else 
         {
            pEnd->dwOffset += pEnd->dwRequestSize;
         }

         r14 = func_498e2c(a, pEnd);
      } //else if (pEnd != 0)
      else 
      {
         r14 = 1;
         status = 135;
      }

      if (r14 != 0) 
      {
         if (pEnd->pTxDmaChannel != 0) 
         {
            (a->pDmaController->pfDmaReleaseChannel)(
               pEnd->pTxDmaChannel);
            pEnd->pTxDmaChannel = 0;      
         }

         wVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
         wVal &= ~(MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE);
         MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR+1, bEnd, wVal >> 8);

         r14 = MGC_CompletedTransfer(c, pEnd, status, 1/*bTx*/, pEnd->pCurrentTxUrb);
         if (pEnd->bData_18 != 0) 
         {
            pEnd->halted_15 = 1;   
         }
      }
   } //if (a->bHostMode != 0)
   else 
   {
      //MGC_HdrcServiceDeviceTxAvail ??
   
      if (wTxCsrVal & MGC_M_TXCSR_P_SENTSTALL) 
      {
         wTxCsrVal &= ~MGC_M_TXCSR_P_SENTSTALL; 
         MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wTxCsrVal);
      }
      else 
      {
         if (wTxCsrVal & MGC_M_TXCSR_P_UNDERRUN) 
         {
            wTxCsrVal &= ~MGC_M_TXCSR_P_UNDERRUN;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wTxCsrVal);            
         }

         if (pEnd != 0) 
         {
            if (pEnd->pTxDmaChannel != 0) 
            {
               if (/*MGC_DMA_STATUS_FREE ??*/ 1 == 
                  (a->pDmaController->pfDmaGetChannelStatus)(
                     pEnd->pTxDmaChannel)) 
               {
                  pEnd->dwOffset = pEnd->pTxDmaChannel->dwActualLength;
               }
            }
            else 
            {
               pEnd->dwOffset += pEnd->dwRequestSize;
            }

            if (pEnd->pCurrentTxUrb != 0) 
            {
               r14 = func_498e2c(a, pEnd); 
               if (r14 != 0) 
               {
                  if (pEnd->pTxDmaChannel != 0) 
                  {
                     (a->pDmaController->pfDmaReleaseChannel)(pEnd->pTxDmaChannel);
                     pEnd->pTxDmaChannel = 0; 
                  }

                  wVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
                  wVal &= ~(MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE);
                  MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR+1, bEnd, wVal >> 8);

                  r14 = MGC_CompletedTransfer(c, pEnd, status, 1, pEnd->pCurrentTxUrb);
               }
            } //if (pEnd->Data_28 != 0)
         } //if (pEnd != 0)
      }
   }
   
   return r14;   
}


/* 21cc9e18 - complete */
int mgc_set_interrupt_enables(Struct_49d2fc* a)
{
   int addr1 = a->addr1;
   struct MGC* r13 = a->Data_20;
   
   MGC_Write16(addr1, MGC_O_HDRC_INTRTXE, r13->wEndMask);
   
   MGC_Write16(addr1, MGC_O_HDRC_INTRRXE, r13->wEndMask & ~1);
   
   MGC_Write8(addr1, MGC_O_HDRC_INTRUSBE, 0xF7); //1111 0111
   
   r13->bData_68 = 0;
   
   return 0;   
}


/* 21cc9db4 - complete */
int mgc_set_interrupt_disables(Struct_49d2fc* a)
{
   int pBase = a->addr2;
   
   MGC_Write8(pBase, MGC_O_HDRC_INTRUSBE, 0);
   MGC_Write16(pBase, MGC_O_HDRC_INTRTXE, 0);
   MGC_Write16(pBase, MGC_O_HDRC_INTRRXE, 0);
   MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);
   MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
   MGC_Read16(pBase, MGC_O_HDRC_INTRRX);
   
   return 0;   
}


/* 21cc993c - todo */
/* v3.8: 491FF8 - complete */
int func_21cc993c(struct MGC* a, 
      MGC_Endpoint* pEnd, 
      char c, int d)
{
//   FAPI_SYS_PRINT_MSG("func_21cc993c\n");
   
   unsigned short wCsrVal;
   int pBase = a->a->addr2;
   int bTx = 0;
   
   char bEnd = pEnd->bEnd;
   
   MGC_SelectEnd(pBase, bEnd);
   
   if (c & 0x80) 
   {
      if (!a->bHostMode) bTx = 1;   
   }
   else 
   {
      if (a->bHostMode) bTx = 1;   
   }
   
   if (bTx) 
   {
      if ((pEnd->pTxDmaChannel != 0) &&
         (pEnd->bData_80 != 0))
      {
         (a->pDmaController->pfDmaReleaseChannel)(
            pEnd->pTxDmaChannel);
         pEnd->pTxDmaChannel = 0;      
      }
      
      wCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
      MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd,
         wCsrVal | MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);
      MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd,
         wCsrVal | MGC_M_TXCSR_FLUSHFIFO);
   } //if (bTx)
   else 
   {
      if (a->bHostMode) 
      {
         //MGC_HdrcStopEnd?
         
         if (bEnd) 
         {
            wCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
            wCsrVal &= ~MGC_M_RXCSR_H_REQPKT; 
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wCsrVal);  
         } //if (bEnd)
         else 
         {
            wCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_CSR0, bEnd);
            wCsrVal &= ~MGC_M_CSR0_H_REQPKT; 
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, bEnd, wCsrVal);  
         }
      } //if (a->bHostMode)

      if ((pEnd->pTxDmaChannel != 0) && 
         (pEnd->bData_80 == 0))
      {
         (a->pDmaController->pfDmaReleaseChannel)(
            pEnd->pTxDmaChannel);
         pEnd->pTxDmaChannel = 0;      
      }

      //mgc_hdrc_flush_end ?
      wCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
      MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 
         wCsrVal | MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);  
      MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 
         wCsrVal | MGC_M_RXCSR_FLUSHFIFO);        
   }
   
   return 0;
}


/* 21cc97ec - complete */
int func_21cc97ec(struct MGC* a)
{
   char vbus;
   char power;
   int pBase = a->a->addr2;
   unsigned char devctl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);
   
   if (devctl & MGC_M_DEVCTL_SESSION) 
   {
      a->bData_55 = (devctl /*& MGC_M_DEVCTL_BDEVICE*/) >> 7;   
   }
   else 
   {
      a->bData_55 = 2;   
   }
   
   a->bSession = (devctl & MGC_M_DEVCTL_SESSION)? 1: 0;
   a->bHostMode = (devctl & MGC_M_DEVCTL_HM)? 1: 0;
   a->bData_49 = (devctl & MGC_M_DEVCTL_LSDEV)? 1: 0;
   
   power = MGC_Read8(pBase, MGC_O_HDRC_POWER);
   a->bRootSpeed = (power & MGC_M_POWER_HSMODE)? 1: 0;
   a->bSuspend = (power & MGC_M_POWER_SUSPENDM)? 1: 0;
   a->bReset = (power & MGC_M_POWER_RESET)? 1: 0;
   a->bData_52 = (power & MGC_M_POWER_RESUME)? 1: 0;
   
   vbus = (devctl & MGC_M_DEVCTL_VBUS) >> MGC_S_DEVCTL_VBUS;
   
   switch (vbus) 
   {
      default:
         //4928e8
         a->bData_56 = 0xFF;
         break;
         
      case 0:
         //4928F4
         a->bData_56 = 0;
         break;  
         
      case 1:
         //4928FC
         a->bData_56 = 1;
         break; 
         
      case 2:
         //492908
         a->bData_56 = 2;
         break;
         
      case 3:
         //492934
         a->bData_56 = 3;
         break;
   }
   
   a->Data_44 = MGC_Read16(pBase, MGC_O_HDRC_FRAME);
   
#if 0
   FAPI_SYS_PRINT_MSG("func_21cc97ec: devctl=0x%x power=0x%x vbus=0x%x Data_44=%d\n", 
         devctl, power, vbus, a->Data_44);
#endif
   
   return 0;
}


/* 21cc96e8 - complete */
/* v3.8: 49260c - complete */
int func_21cc96e8(struct MGC* a)
{
//   FAPI_SYS_PRINT_MSG("func_21cc96e8\n");   

   int r15 = a->a->addr2;
   char bOldPower = MGC_Read8(r15, MGC_O_HDRC_POWER);
   char bPower = bOldPower;
   char bOldDevCtl = MGC_Read8(r15, MGC_O_HDRC_DEVCTL);
   char devctl = bOldDevCtl;
   char bAddress = MGC_Read8(r15, MGC_O_HDRC_FADDR);   
   
   if (a->bData_29 != 0) 
   {
      bPower |= MGC_M_POWER_RESET;   
   }
   else 
   {
      bPower &= ~MGC_M_POWER_RESET;   
   }
   
   if (a->bData_31 != 0) 
   {
      bPower |= MGC_M_POWER_RESUME;   
   }
   else 
   {
      bPower &= ~MGC_M_POWER_RESUME;   
   }
   
   if (a->bData_30 != 0) 
   {
      if (a->bHostMode != 0) 
      {
         bPower |= MGC_M_POWER_SUSPENDM;   
      }
      else 
      {
         bPower |= MGC_M_POWER_ENSUSPEND;   
      }
   }
   else 
   {
      bPower &= ~MGC_M_POWER_SUSPENDM;
      bPower &= ~MGC_M_POWER_ENSUSPEND;   
   }
   
   if (a->bData_28 != 0) 
   {
      bPower |= MGC_M_POWER_HSENAB;   
   }
   else 
   {
      bPower &= ~MGC_M_POWER_HSENAB;   
   }
   
   if (a->bData_33 != 0) 
   {
      devctl |= MGC_M_DEVCTL_SESSION;   
   }
   else 
   {
      devctl &= ~MGC_M_DEVCTL_SESSION;   
   }

   if (a->bData_34 != 0) 
   {
      devctl |= MGC_M_DEVCTL_HR;   
   }
   else 
   {
      devctl &= ~MGC_M_DEVCTL_HR;   
   }
   
   if (bAddress != a->address_32) 
   {
      MGC_Write8(r15, MGC_O_HDRC_FADDR, a->address_32);   
   }
   
   if (bOldPower != bPower) 
   {
      MGC_Write8(r15, MGC_O_HDRC_POWER, bPower);   
   }
   
   if (bOldDevCtl != devctl) 
   {
      MGC_Write8(r15, MGC_O_HDRC_DEVCTL, devctl);   
   }
   
   return 0;
}


/* 21cc9644 - complete */
int MGC_HdrcUlpiVbusControl(struct MGC* a, 
      char bExtSource, 
      char bExtIndicator)
{
   char bVal;
   int pBase = a->a->addr2;
   
   /* ensure not powered down */
   if (MGC_Read8(pBase, MGC_O_HDRC_POWER) & MGC_M_POWER_ENSUSPEND) 
   {
      return 0;   
   }
   
   bVal = bExtSource? MGC_M_ULPI_VBUSCTL_USEEXTVBUS: 0;
   bVal |= bExtIndicator? MGC_M_ULPI_VBUSCTL_USEEXTVBUSIND: 0;
   
   MGC_Write8(pBase, MGC_O_HDRC_ULPI_VBUSCTL, bVal);
   
   return 1;
}


/* 21cc9530 - complete */
void func_21cc9530(void)
{
//   FAPI_SYS_PRINT_MSG("func_21cc9530\n");

   /*nothing*/
}


/* 21cc88ac - todo */
/* v3.8: func_4912b0 */
int32_t func_21cc88ac(struct MGC* a, MUSB_FunctionClient* b)
{
#if 0
   int res;
   char i;
   MGC_Pipe* pEndpoint;
   struct usb_device_descriptor* pDeviceDescriptor = (void*) 
      b->pDeviceDescriptor;
   struct usb_qualifier_descriptor* pQualifierDescriptor = (void*) 
      b->pQualifierDescriptor;
   
   char bNumConfigurations = pDeviceDescriptor->bNumConfigurations;
   
   a->bData_581 = 1;
   
   if ((b->wQualifierDescriptorLength >= 10) && 
      (bNumConfigurations < pQualifierDescriptor->bNumConfigurations)) 
   {
      bNumConfigurations = pQualifierDescriptor->bNumConfigurations;   
   }
   
   a->ppConfigurations = FAPI_SYS_MALLOC(bNumConfigurations * 4, 1);
   if (a->ppConfigurations != 0) 
   {
      a->bDeviceNumTotalEndpoints = 0;
      
      res = func_4916a4(a, 
         b->pDeviceDescriptor, 
         b->wDeviceDescriptorLength, 
         pDeviceDescriptor->bNumConfigurations, 
         USB_DT_CONFIG, 1);   
      
      if ((res == 0) && 
         (b->wQualifierDescriptorLength > 10))
      {
         res = func_4916a4(a, 
            b->pQualifierDescriptor, 
            b->wQualifierDescriptorLength, 
            pQualifierDescriptor->bNumConfigurations, 
            USB_DT_OTHER_SPEED_CONFIG, 1);   
      }
   }
   else 
   {
      res = 166;   
   }

   if ((a->bDeviceNumTotalEndpoints != 0) && (res == 0))
   {
      a->ppDeviceEndpoints = FAPI_SYS_MALLOC(a->bDeviceNumTotalEndpoints * 
         sizeof(MGC_Pipe*), 1);
      
      if (a->ppDeviceEndpoints != 0) 
      {         
         pEndpoint = FAPI_SYS_MALLOC(a->bDeviceNumTotalEndpoints * 
            sizeof(MGC_Pipe), 1);   
         
         if (pEndpoint != 0) 
         {
            for (i = 0; i < a->bDeviceNumTotalEndpoints; i++) 
            {
               a->ppDeviceEndpoints[i] = pEndpoint++;
            }
         }
         else 
         {
            FAPI_SYS_FREE(a->ppDeviceEndpoints);
            a->ppDeviceEndpoints = 0;
            res = 166;
         }
      }
      else 
      {
         res = 166;   
      }
   }

   if ((res != 0) && (a->ppConfigurations != 0))
   {
      FAPI_SYS_FREE(a->ppConfigurations);
      a->ppConfigurations = 0;      
   }
   
   return res; 
#else
   FAPI_SYS_PRINT_MSG("func_21cc88ac\n");
   return 0;
#endif
}


/* 21cc8838 - todo */
/* v3.8: func_491624 */
void func_21cc8838(struct MGC* a)
{
#if 0
   MUSB_FunctionClient* r4 = a->pDevice;
   
   if (r4 != 0) 
   {
      struct usb_device_descriptor* r6 = (void*) r4->pDeviceDescriptor;
      struct usb_qualifier_descriptor* r5 = (void*) r4->pQualifierDescriptor;
      
      if ((a->bRootSpeed != 0) && (r5 != 0)) 
      {
         func_4916a4(a, 
            r4->pQualifierDescriptor, 
            r4->wQualifierDescriptorLength, 
            r5->bNumConfigurations, 
            USB_DT_OTHER_SPEED_CONFIG, 0);         
      }
      else
      {
         func_4916a4(a, 
            r4->pDeviceDescriptor, 
            r4->wDeviceDescriptorLength, 
            r6->bNumConfigurations, 
            USB_DT_CONFIG, 0);         
      }
   }
#else
   FAPI_SYS_PRINT_MSG("func_21cc8838\n");
#endif
}


/* 21cc849c - complete */
void MGC_SetDeviceState(MUSB_BusHandle busHandle, MUSB_State state)
{
//   FAPI_SYS_PRINT_MSG("MGC_SetDeviceState\n");

   struct MGC* a = busHandle;

   if (state != a->bDeviceState)
   {
      a->bDeviceState = state;

      if (a->pDevice != 0)
      {
         (a->pDevice->pfUsbState)(a->pDevice->pPrivateData,
               busHandle, state);
      }
   }
}


/* v3.6: 49b49c - todo */
/* 21cc84d4 - complete */
uint32_t MUSB_DeviceResponse(MUSB_BusHandle busHandle,
      uint32_t sequenceNum,
      uint8_t* pbBuffer,
      uint16_t wSize,
      uint8_t e)
{
   struct MGC* a = busHandle;

   if (a->dwDeviceSequenceNum != sequenceNum)
   {
      return 129;
   }

   a->wDeviceTxSize = wSize;
   a->pDeviceTransferBuffer = pbBuffer;

   (a->Func_648)(a, e);

   return 0;
}


/* v3.6: 49e1d4 - complete */
/* 21ccf4cc - complete */
void MUSB_SetPipeHalt(void* hPipe, uint8_t b)
{
   int r3;
   MGC_Pipe* a = hPipe;
   struct MGC* r2 = a->Data_0;
   int r1 = (a->Data_28 & 0x80)? 1: 0;

   if (r2->bHostMode)
   {
      r1 = !r1;
   }

   r3 = r1;

   a->pEndpoint->halted_15 = b;

   (r2->Func_644)(r2, a->pEndpoint, r3? 0x80: 0, b);
}


/* v3.6: 49c1c8 - complete */
/* 21ccf4c0 - complete */
uint32_t MUSB_GetPipeHalt(MUSB_Pipe hPipe)
{
   MGC_Pipe* p = hPipe;
   return p->pEndpoint->halted_15;
}


/* v3.6: 49adb4 - complete */
/* 21ccf8f0 - todo */
int MUSB_CancelTransfer(MUSB_Irp* pIrp)
{
   uint32_t r2; // = 0;
   int res = 0;
   MGC_Pipe* r3 = pIrp->hPipe;
   struct MGC* r15 = r3->Data_0;
   MGC_Endpoint* pEndpoint = r3->pEndpoint;
   MUSB_SystemServices* pOS = r15->a->pOS;
   uint8_t bHostMode = r15->bHostMode;

   uint32_t bTx = (r3->Data_28 & 0x80)? 1: 0;

   if (r3->Data_28 & 1)
   {
//      r2 = (bTx != 0)? 0: 0x80;
      if (bTx) r2 = 0; else r2 = 0x80;
   }
   else
   {
//      r2 = (bTx != 0)? 0x80: 0;
      if (bTx) r2 = 0x80; else r2 = 0;
   }

   if (((bTx != 0) && (pIrp == pEndpoint->pCurrentTxUrb)) ||
      ((bTx == 0) && (pIrp == pEndpoint->pCurrentRxUrb)))
   {
      res = (r15->Func_640)(r15, pEndpoint, r2, 0);

      if (bTx != 0)
      {
         pEndpoint->pCurrentTxUrb = 0;
      }
      else
      {
         pEndpoint->pCurrentRxUrb = 0;
      }
   }
   else
   {
      if (bHostMode) //r15->bHostMode)
      {
         (pOS->pfLock)(pOS->pPrivateData, pEndpoint->bEnd + 1);
      }

      if (bTx != 0)
      {
         List_49cd8c_remove_element(&pEndpoint->tx_urb_list, pIrp);
      }
      else
      {
         List_49cd8c_remove_element(&pEndpoint->rx_urb_list, pIrp);
      }

      if (bHostMode) //r15->bHostMode)
      {
         (pOS->pfUnlock)(pOS->pPrivateData, pEndpoint->bEnd + 1);
      }
   }

   func_49a12c(r15, pEndpoint, bTx);

   return res;
}


/* 21cc8328 - todo */
void func_21cc8328(char* a)
{
   FAPI_SYS_PRINT_MSG("musb: %s\n", a);
   
   Data_21cc3b54 = 0;
}


/* 21cc8000 - todo */
int MGC_HdrcConfigureEps(struct MGC* a)
{   
   MGC_Endpoint ep;
   int reg;
   char bEnd;
   int addr2 = a->a->addr2;
   int res = 0;
   
   /* At least Endpoint 0 */
   a->bEndCount = 1;
   a->wEndMask = (1 << 0);
   int r6 = 1;
   
   for (bEnd = 1; bEnd < MUSB_C_NUM_EPS; bEnd++) 
   {
      MGC_SelectEnd(addr2, bEnd);

      if (0 == MGC_ReadCsr8(addr2, MGC_O_HDRC_FIFOSIZE, bEnd)) 
      {
         /* 0's returned when no more endpoints */ 
         break;   
      }
      
      a->bEndCount++;
      a->wEndMask |= (r6 << bEnd);
   }
   
   a->c = (FAPI_SYS_Services.mallocFunc != 0)?
         (FAPI_SYS_Services.mallocFunc)(a->bEndCount * sizeof(MGC_Endpoint)): 0;
         
   if (a->c != 0) 
   {
      // Add a->c to a->Data_12
      if (0 != List_49a810_create(&a->ep_list, 
         sizeof(MGC_Endpoint), a->bEndCount, a->c)) 
      {
         res = 1;
     
         musb_memset(&ep, 0, sizeof(MGC_Endpoint));
      
         ep.bIsSharedFifo = 1;
         ep.bTxTrafficType = 0;
         ep.bRxTrafficType = 0;
         ep.wMaxPacketSizeTx = (1 << 6); //64
         ep.wMaxPacketSizeRx = (1 << 6); //64
         ep.g = 1;
         ep.h = 1;
      
         // Add ep0 to a->ep_list
         List_49a810_add_element(&a->ep_list, &ep);
      
         for (bEnd = 1; bEnd < a->bEndCount; bEnd++) 
         {
            MGC_SelectEnd(addr2, bEnd);
            
            reg = MGC_ReadCsr8(addr2, MGC_O_HDRC_FIFOSIZE, bEnd);
            
            musb_memset(&ep, 0, sizeof(MGC_Endpoint));   
            
            ep.bEnd = bEnd;
            ep.wMaxPacketSizeTx = 1 << (reg & 0x0F);
            /* shared TX/RX FIFO? */ 
            if ((reg & 0xF0) == 0xF0) 
            {
               ep.wMaxPacketSizeRx = 1 << (reg & 0x0F);
               ep.bIsSharedFifo = 1;   
            }
            else 
            {
               ep.wMaxPacketSizeRx = 1 << ((reg & 0xF0) >> 4);   
               ep.bIsSharedFifo = 0;
            }
         
            // Add ep to a->list_12
            List_49a810_add_element(&a->ep_list, &ep);
         }
      }
      else 
      {
         (FAPI_SYS_Services.freeFunc)(a->c);   
      }
   }   

   MGC_Write8(addr2, 14, 0);
      
   return res;
}


/* 21cc7bbc - complete */
/* v3.8: func_495080 */ 
void func_21cc7bbc(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
   int bDeviceAddress;
   int dwSpeed = 2;
   struct MGC* r13 = a->Data_20;
   
   r13->bData_29 = 0;
   
   (r13->Func_620)(r13);
   (r13->Func_616)(r13);

   if (r13->bRootSpeed != 0) 
   {
      dwSpeed = 3;   
   }
   else if (r13->bData_49 != 0) 
   {
      dwSpeed = 1;
   }
   
   bDeviceAddress = MGC_AllocateDeviceAddress(&r13->Data_156);
   if (bDeviceAddress != 0) 
   {
      MUSB_StartEnumeration(r13, 0, bDeviceAddress, 0, dwSpeed, 0);   
   }
}


/* 21cc7ab4 - todo */
/* v3.8: func_48A91C */
void func_21cc7ab4(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
   struct MGC* r14 = a->Data_20;
   MUSB_OtgClient* r13 = r14->pOtgClient;
   
   if ((r14->bData_68 == 20) || (r14->bData_68 == 0))
   {
      if (r14->bData_56 >= 2) 
      {
         if (r13 != 0) 
         {
            (r13->pfOtgError)(r13->pPrivateData, r13, 178);
         }
      }
      else 
      {
         r14->bData_33 = 0;
         r14->bData_34 = 0;
         
         (r14->Func_620)(r14);   
         
         if (r13 != 0) 
         {
            (r13->pfOtgError)(r13->pPrivateData, r13, 177);
         }
         
         func_21cc686c(0, r14);
      }
   }
}


/* 21cc7a64 - todo */
/* v3.8: func_49A390 */
void func_21cc7a64(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
   struct MGC* r13 = a->Data_20;
   MUSB_OtgClient* r1 = r13->pOtgClient;
   
#if 0
   if ((r13->bSuspend == 0) &&
      (r13->bReset == 0))
#endif
   if (r13->bSuspend == 0)
   {
      if (r13->bReset == 0)
      {
         if (r1 != 0) 
         {
            (r1->pfOtgError)(r1->pPrivateData, r1, 177);
         }
         
         func_21cc686c(0, r13);
      }
   }
}


/* 21cc7a0c - complete */
/* v3.8: func_48A8A4 */
void func_21cc7a0c(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
   struct MGC* r13 = a->Data_20;
   MUSB_OtgClient* r1 = r13->pOtgClient;
   
   if (r13->bData_68 == 18) 
   {
      if (r1 != 0) 
      {
         (r1->pfOtgError)(r1->pPrivateData, r1, 177);
      }
      
      r13->bData_34 = 0;
      
      (r13->Func_620)(r13);
      
      func_21cc686c(17, r13);
   }
}


/* 21cc7830 - todo */
int mgc_handle_interrupts(Struct_49d2fc* a, 
      char bIntrUsbValue,
      unsigned short wIntrTxValue, 
      unsigned short wIntrRxValue)
{
   unsigned short reg;
   int bShift;
   MGC_Message msg; 
   
   int res = -1;

   MUSB_SystemServices* pOS = a->pOS;
   struct MGC* r16 = a->Data_20;
   
#if 0
   FAPI_SYS_PRINT_MSG("mgc_handle_interrupts: bIntrUsbValue=0x%x wIntrTxValue=0x%x wIntrRxValue=0x%x\n",
         bIntrUsbValue, wIntrTxValue, wIntrRxValue);
#endif
   
   r16->bData_60 = 0;
   
   if ((r16->pDmaController != 0) && 
      (r16->pDmaController->pfDmaControllerIsr != 0))
   {
      if (0 != (r16->pDmaController->pfDmaControllerIsr)(
         r16->pDmaController->pPrivateData)) 
      {
         res = r16->bData_60;   
      }
   }

   /* the core can interrupt us for multiple reasons, I.E. more than an
    * interrupt line can be asserted; service the global interrupt first.
    * Global interrups are used to signal connect/disconnect/vbuserr
    * etc. processed in two phase */ 
   if (bIntrUsbValue != 0) 
   {
      res = mgc_hdrc_service_usb(r16, bIntrUsbValue);   
   }
  
    /* handle tx/rx on endpoints; each bit of wIntrTxValue is an endpoint, 
     * endpoint 0 first (p35 of the manual) bc is "SPECIAL" treatment; 
     * WARNING: when operating as device you might start receving traffic 
     * to ep0 before anything else happens so be ready for it */     
       
   reg = wIntrTxValue;
   
   if ((reg != 0) && (res < 0)) 
   {
      res = 0;   
   }
   
   if (reg & 1) 
   {
      /* EP0 */ 
      if (0 != (r16->pfDefaultEndHandler)(r16, &msg)) 
      {
         res++;
         
         (pOS->pfQueueBackgroundItem)(pOS->pPrivateData, &msg);
      }
   }

   /* TX on endpoints 1-15 */ 
   bShift = 1;
   reg >>= 1;
   while (reg != 0) 
   {
      if (reg & 1) 
      {
         //MGC_HdrcServiceTxAvail / MGC_HdrcServiceDeviceTxAvail
         if (0 != (r16->pfServiceTxAvail)(r16, bShift, &msg)) 
         {
            res++;
            msg.bEnd = bShift; 
            
            (pOS->pfQueueBackgroundItem)(pOS->pPrivateData, &msg);
         }
      }
      reg >>= 1;
      bShift++;   
   }

   /* RX on endpoints 1-15 */ 
   reg = wIntrRxValue;
   if ((reg != 0) && (res < 0)) 
   {
      res = 0;   
   }
 
   bShift = 1;
   reg >>= 1;
   while (reg != 0) 
   {
      if (reg & 1) 
      {
         //MGC_HdrcServiceRxReady / MGC_HdrcServiceDeviceRxReady
         if (0 != (r16->pfServiceRxReady)(r16, bShift, &msg)) 
         {
            res++;
            msg.bEnd = bShift; 
            
            (pOS->pfQueueBackgroundItem)(pOS->pPrivateData, &msg);
         }
      }
      reg >>= 1;
      bShift++;   
   }

   return res;
}


/* 21cc7538 - todo */
int mgc_hdrc_service_usb(struct MGC* a, char bIntrUSB)
{      
   MGC_Message msg;
   char handled = 0;
   
   unsigned bEndCount;
   unsigned bEnd;
   MUSB_SystemServices* pOS = a->a->pOS;
   
   #if 0
   FAPI_SYS_PRINT_MSG("mgc_hdrc_service_usb: 0x%x\n", bIntrUSB);
   #endif
   
   if (bIntrUSB != 0) 
   {
      if (bIntrUSB & MGC_M_INTR_RESUME) 
      {
         //21cc771c
         handled++;
         
         #if 0
         //DBG(2, "RESUME\n");
         FAPI_SYS_PRINT_MSG("mgc_hdrc_service_usb: RESUME\n");
         #endif
         
         msg.bTag = 7;         
         (pOS->pfQueueBackgroundItem)(pOS->pPrivateData, &msg);
      }
      
      if (bIntrUSB & MGC_M_INTR_SESSREQ) 
      {
         //21cc7654
         handled++;

         #if 0        
         //DBG(2, "SESSION_REQUEST\n");
         FAPI_SYS_PRINT_MSG("mgc_hdrc_service_usb: SESSION_REQUEST\n");
         #endif
         
         msg.bTag = 1;            
         (pOS->pfQueueBackgroundItem)(pOS->pPrivateData, &msg);
      }
      
      if (bIntrUSB & MGC_M_INTR_VBUSERROR) 
      {
         //21cc7744
         handled++;
         
         #if 0         
         FAPI_SYS_PRINT_MSG("mgc_hdrc_service_usb: VBUSERROR\n");
         #endif
         
         msg.bTag = 8;            
         (pOS->pfQueueBackgroundItem)(pOS->pPrivateData, &msg);
      }
      
      if (bIntrUSB & MGC_M_INTR_SUSPEND) 
      {
         //21cc76f0
         handled++;
        
         #if 0      
         FAPI_SYS_PRINT_MSG("mgc_hdrc_service_usb: SUSPEND\n");
         #endif
                 
         msg.bTag = 6;            
         (pOS->pfQueueBackgroundItem)(pOS->pPrivateData, &msg);
      }
      
      if (bIntrUSB & MGC_M_INTR_CONNECT) 
      {
         //21cc76bc
         
         #if 0
         //DBG(2, "RECEIVED A CONNECT (goto host mode)\n");
         FAPI_SYS_PRINT_MSG("mgc_hdrc_service_usb: CONNECT\n");
         #endif
         
         a->bHostMode = 1;
         
         handled++;
         
         msg.bTag = 2;            
         (pOS->pfQueueBackgroundItem)(pOS->pPrivateData, &msg);
      }
      
      if (bIntrUSB & MGC_M_INTR_DISCONNECT) 
      {
         //21cc7590
         
         #if 0
         FAPI_SYS_PRINT_MSG("mgc_hdrc_service_usb: DISCONNECT\n");
         #endif
         
         if (a->bData_29 == 0) 
         {
            //21cc7768
            func_21cc6694(a); 
            MGC_SetDeviceState(a, MUSB_DEFAULT);

            handled++;
         
            msg.bTag = 3;            
            (pOS->pfQueueBackgroundItem)(pOS->pPrivateData, &msg);
         }         
      }

       /* saved one bit: bus reset and babble share the same bit;
        * If I am host is a babble! i must be the only one allowed
        * to reset the bus; when in otg mode it means that I have 
        * to switch to device
        */
      if (bIntrUSB & MGC_M_INTR_RESET) 
      {
         //21cc7680
         
         #if 0
         //DBG(2, "BUS RESET\n");
         FAPI_SYS_PRINT_MSG("mgc_hdrc_service_usb: BUS RESET\n");
         #endif
         
         (a->Func_616)(a);
         
         if (a->bHostMode != 0) 
         {
            //21cc7698
            handled++;
         
            msg.bTag = 5;            
            (pOS->pfQueueBackgroundItem)(pOS->pPrivateData, &msg);
            //->21cc75a4
         }
         else 
         {
            //21cc77b8
            a->bData_33 = a->bSession;   
            a->bData_34 = 0;
            a->address_32 = 0;
            a->wDeviceBufferOffset = 0;
            a->wDeviceTxSize = 0;
            
            func_21cc6694(a);
            MGC_SetDeviceState(a, MUSB_DEFAULT);
            func_21cc8838(a);
            
#if 1
            switch (a->bData_68) 
            {
               case 0:
                  func_21cc6b68(a, 1);               
                  break;
                  
               case 20:
                  func_21cc686c(17, a);
                  break;
            }
#else
            if (a->bData_68 == 0)
            {
               func_21cc6b68(a, 1);
            }
            else if (a->bData_68 == 20)
            {
               func_21cc686c(17, a);
            }
#endif
            //->21cc75a4
         }
      }
      
      if (bIntrUSB & MGC_M_INTR_SOF) 
      {
         //21cc75b8
         
         #if 0
         FAPI_SYS_PRINT_MSG("mgc_hdrc_service_usb: SOF\n");
         #endif

         (a->Func_616)(a);
         
         bEndCount = List_49a810_get_element_count(&a->ep_list);
         for (bEnd = 1; bEnd < bEndCount; bEnd++) 
         {
            MGC_Endpoint* r1 = List_49a810_get_element(&a->ep_list, bEnd); 
            
            // Isochronous transfers?
              
            if ((r1 != 0) && 
               (r1->bTxTrafficType == 1) && 
               (r1->Data_88 != 0))
            {
               Struct_49e470_for_TransferType_1* r4 = (void*) r1->pCurrentTxUrb;

               if (--r1->Data_88 == 0) 
               {
                  (a->pfSendOnEp)(a, 
                                r1, 
                                r4->b, 
                                r4->c[0], 
                                r4);
               }
            }
         }
         
         if (a->bHostMode != 0) 
         {
            func_21ccbd04(a); //->return 1;
         }
      }
   }
   
   return handled;
}


/* 21cc6ed0 - todo */
void mgc_handle_messages(MUSB_Port* a)
{
   MGC_Message sp8;      
   struct MGC* r4 = a->pPrivateData;
   Struct_49d2fc* fp = r4->a;
   MUSB_SystemServices* pOS/*r7*/ = fp->pOS;
   MUSB_OtgClient* r8 = r4->pOtgClient;
   
   while (0 != (pOS->pfDequeueBackgroundItem)(pOS->pPrivateData, &sp8))
   {
      #if 0
      FAPI_SYS_PRINT_MSG("mgc_handle_messages: r4->bData_68=%d, sp8.bTag=%d, sp8.bEnd=%d, sp8.bStatus=0x%x\n", 
         r4->bData_68, sp8.bTag, sp8.bEnd, sp8.bStatus);
      #endif

      //List_49a810* sp4 = &r4->ep_list;
      //int r9 = 0;      
      
      switch (sp8.bTag) 
      {         
         case 7:
            //21cc71e4 - Resume
            if (r4->pDevice != 0) 
            {
               (r4->pDevice->pfUsbState)(
                  r4->pDevice->pPrivateData,
                  r4,
                  r4->bDeviceState);   
            }
            //21cc7204
            switch (r4->bData_68) 
            {
               case 18:
                  //21cc7394
                  func_21cc686c(17, r4);
                  break;
                  
               case 36:
                  //21cc7218
                  func_21cc686c(35, r4);
                  
                  r4->bData_31 = 1;
                  
                  (r4->Func_620)(r4);
                  
                  (fp->pOS->pfArmTimer)(fp->pOS->pPrivateData,
                                    0, 20, 0, 
                                    func_21cc6144);
                  
                  break;
               
               #if 0
               default:
                  //->48cab8
                  break;
               #endif      
            }
            //->48cab8
            break;
            
         case 1:
            //21cc70dc - Session Request
            if (r4->bData_68 == 0) 
            {
               func_21cc686c(34, r4);
               
               r4->bData_33 = 1;   
               
               (r4->Func_620)(r4);
            }
            //->48cab8
            break;  
      
         case 8:
            //21cc6fb4 - VBUS Error
            if (r4->bData_61 != 0) 
            {
               //21cc73bc
               r4->bData_61--;   
               
               func_21cc686c(0, r4);
               func_21cc6b68(r4, 0);
            }
            else 
            {
               //21cc6fc0
#if 1               
               // New in HD
               if (r8 != 0)
               {
                  (r8->pfOtgError)(r8->pPrivateData, r8, 176/*0xb0*/);
               }
#endif
               //21cc6fdc
               func_21cc686c(0, r4);   
            }
            //->48cab8
            break;
            
         case 6: 
            //21cc7124 - Suspend
            if (r4->pDevice != 0) 
            {
               (r4->pDevice->pfUsbState)(
                  r4->pDevice->pPrivateData,
                  r4,
                  MUSB_SUSPENDED);
            }
            //21cc7144
            switch (r4->bData_68) 
            {
               case 17:
                  //21cc7160
                  r4->bSuspend = 1;
               
                  if (r4->bHostMode != 0) 
                  {
                     func_21cc686c(18, r4);   
                  }
                  break;
                  
               case 35:
                  //21cc7488
                  r4->bSuspend = 1;
                  
                  func_21cc686c(36, r4);
                  break;
                  
               case 33:
                  //21cc74b8
                  func_21cc686c(34, r4);
                  break;
            }
            //->48cab8
            break;
            
         case 2:
            //21cc7090 - Connect
            (r4->Func_616)(r4);
            
            r4->Data_432 = 0;
            
            if ((r4->bData_68 == 18) ||
               (r4->bData_68 == 17)) 
            {
               func_21cc686c(19, r4);               
            }
            else
            {
               func_21cc686c(35, r4);               
            }
            //->48cab8
            break; 
      
         case 3:
            //21cc7048 - Disconnect?
            r4->bSuspend = 0;
            
            func_21ccc488(r4);
            
            switch (r4->bData_68) 
            {
               case 19:
                  //21cc7378
                  func_21ccc430(r4->Data_432);
                  func_21cc686c(17, r4);
                  break;
                  
               case 35:
                  //21cc7420   
                  func_21ccc430(r4->Data_432);
                  func_21cc686c(34, r4);
                  break;
                  
               case 36:
                  //21cc743c
                  if ((r4->bData_69 != 0) && 
                     (Data_21f02164.dwStatus == 0))
                  {
                     func_21ccc430(r4->Data_432);
                     func_21cc686c(33, r4);
                  }
                  else 
                  {
                     //21cc7474
                     func_21cc686c(34, r4);
                  }
                  break;
            }
            //21cc7070
            r4->Data_432 = 0;
            //->48cab8
            break;
            
         case 5:
            //21cc7184 - Bus Reset in Host Mode
            (r4->Func_616)(r4);
            
            if (r4->bSession == 0) 
            {
               if (r4->bData_63 != 0) 
               {
                  r4->bData_63--;
                  
                  func_21cc686c(0, r4);
                  func_21cc6b68(r4, 0);   
               }
               else 
               {
                  //21cc72dc
                  (r4->Func_624)(r4);
                  (r4->Func_620)(r4);
                  
                  func_21cc686c(0, r4);
               }
            }
            //->48cab8
            break;

         case 35: //Tx
         case 36: //Rx
         {
            //21cc7008
            MGC_Endpoint* r5 = List_49a810_get_element(&r4->ep_list/*sp4*/, sp8.bEnd);
            if (r5 != 0) 
            {
               #if 0
               // From linux/usb.h
               #define PIPE_ISOCHRONOUS     0
               #define PIPE_INTERRUPT       1
               #define PIPE_CONTROL         2
               #define PIPE_BULK            3 
               #endif
               
               int r0 = (sp8.bTag == 35)? 
                  r5->bTxTrafficType: 
                  r5->bRxTrafficType;
                  
               switch (r0) 
               {
                  case 2: // Bulk
                  case 3: // Interrupt?
                     //21cc7278
                     {
                        MUSB_Irp* pUrb = sp8.pUrb;
                        if ((pUrb != 0) && (pUrb->pfIrpComplete != 0))
                        {
                           (pUrb->pfIrpComplete)(pUrb->pCompleteParam, pUrb);
                        }
                     }
                     break;
                     
                  case 0: //Control?
                     //21cc731c
                     {
                        MUSB_ControlIrp* pUrb = sp8.pUrb;
                        if ((pUrb != 0) && (pUrb->pfIrpComplete != 0))
                        {
                           (pUrb->pfIrpComplete)(pUrb->pCompleteParam, pUrb);
                        }
                     }
                     break;
                     
                  case 1: //Isochronous?
                     //21cc7278
                     {
                        MUSB_IsochIrp* r1 = sp8.pUrb;
                        if ((r1 != 0) && (r1->pfIrpComplete != 0))
                        {
                           (r1->pfIrpComplete)(r1->pCompleteParam, r1);
                        }
                     }
                     break;   
               }
               //21cc72a0
               if (sp8.bTag == 35) 
               {
                  //21cc734c
                  if ((r5->pCurrentTxUrb == 0) && 
                     (r5->bData_18 == 0)) 
                  {
                     //21cc7364
                     func_21ccf72c(r4, r5, 1);   
                  }
               }
               else 
               {
                  //21cc72a8
                  if (((r5->pCurrentRxUrb == 0) || 
                     (r5->bRxTrafficType == 3)) &&
                     (r5->bData_19 == 0))
                  {
                     //21cc72cc
                     func_21ccf72c(r4, r5, 0);   
                  }
               }
            } //if (r5 != 0)
            //->48cab8
            break;
         }
            
         #if 0
         default:
            //48CAB8
            break;
         #endif
      } //switch (sp8.b)
   } //while (0 != (pOS->pfDequeueBackgroundItem)(pOS->pPrivateData, &sp8))
}


/* 21cc6d64 - todo */
/* v3.8: func_48C1C8 */
int func_21cc6d64(struct MGC* a, 
      MUSB_Device* b,
      MUSB_DeviceDriver* c)
{
   int r13 = 1;
   MUSB_OtgClient* r16 = a->pOtgClient;
   
   if ((b->DeviceDescriptor.idVendor == 0x1a0a)/* &&
      (b->DeviceDescriptor.idProduct == 0xbadd)*/)
   {
      if (b->DeviceDescriptor.idProduct == 0xbadd)
      {
         // Test Device
         if (r16 != 0) 
         {
            (r16->pfOtgError)(r16->pPrivateData, a, 65);
         }
         //48c23c
         r13 = 0;
      }
   }

   if ((a->bData_42 == 0) && 
      (b->DeviceDescriptor.bDeviceClass == USB_CLASS_HUB))
   {
      if (r16 != 0) 
      {
         (r16->pfOtgError)(r16->pPrivateData, a, 66);
      }
      //48c274
      r13 = 0;
   }

   if ((r13 != 0) && (c == 0))
   {
      if (r16 != 0) 
      {
         (r16->pfOtgError)(r16->pPrivateData, a, 65);
      }
      //48c2a4
      r13 = 0;
   }

   if (r13 != 0) 
   {
      if ((a->bData_55 == 0) && (a->Data_432 != 0))
      {
         struct usb_otg_descriptor* r0 = 
            (void*) mgc_search_descriptor(
               a->Data_432->pCurrentConfiguration,
               (a->Data_432->pCurrentConfiguration[3] << 8) | 
                  a->Data_432->pCurrentConfiguration[2],
               USB_DT_OTG, 0);
            
         if ((r0 != 0) && (r0->bmAttributes & USB_OTG_HNP))
         {
            Data_601744.pDevice = a->Data_432;
            Data_601744.pCompleteParam = a;
            
            MUSB_StartControlTransaction(a->b, &Data_601744);
         }
      }
   }
   else 
   {
      //48c330
      if (a->Data_432 == 0)
      {
         func_21cc6cd8(a);
      }
   }
   
   return r13;   
}


/* 21cc6cd8 - todo */
/* v3.8: func_49dda0 */
void func_21cc6cd8(struct MGC* a)
{
   FAPI_SYS_PRINT_MSG("func_21cc6cd8\n");
}


/* v3.8: func_49d82c */
/* 21cc6c24 - todo */
MUSB_BusHandle func_21cc6c24(MUSB_Port* pPort,
      MUSB_FunctionClient* pDevice,
      MUSB_HostClient* pDeviceDriverList,
      MUSB_OtgClient* pOtgClient)
{
   struct usb_otg_descriptor* pOtgDescriptor;
   
   int32_t r0 = 0;
   struct MGC* res = 0;
   struct MGC* r17 = pPort->pPrivateData;
   
   if (r17->bInitialized == 0)
   {
      r17->pDeviceDriverList = pDeviceDriverList;   
      
      if (pDevice != 0) 
      {
         r0 = func_21cc88ac(r17, pDevice);   
      }
      
      if (r0 == 0) 
      {
         r17->bData_70 = 0;  
         
         if (pDevice != 0) 
         {
            pOtgDescriptor = (void*) 
               mgc_search_descriptor(pDevice->pStandardDescriptors,
                  pDevice->wDescriptorBufferLength, USB_DT_OTG, 0);
         }

         if (pOtgDescriptor != 0) 
         {
            r17->bData_70 = (pOtgDescriptor->bmAttributes & USB_OTG_HNP)? 1: 0;
         }

         r17->bData_61 = 4;
         r17->bData_63 = 4;
         
         (r17->Func_624)(r17);
         
         r17->pDevice = pDevice;
         r17->pOtgClient = pOtgClient;
         r17->bInitialized = 1;
         
         func_21cc6b68(r17, 0);
         
         res = r17;
      }
   }
   
   return res;   
}


/* 21cc6b68 - todo */
/* v3.8: func_498b5c */
void func_21cc6b68(struct MGC* a, char b)
{
   a->bData_30 = 0;
   a->bData_33 = 1;
   a->Func_620(a);  
   a->Func_616(a);
   
   if (a->bHostMode == 0) 
   {
      if (a->bData_55 != 0) 
      {
         if (b != 0) 
         {
            func_21cc686c(17, a);   
         }
         else 
         {
            func_21cc686c(20, a);   
         }
      }
      else 
      {
         a->bData_70 = 1;   
         func_21cc686c(34, a);   
      }
   }
}


static void func_48e464(char a, struct MGC* b)
{
   MGC_SetDeviceState(b, MUSB_POWERED);
   
   func_21cc6694(b);
   
   b->address_32 = 0;
   
   (b->Func_620)(b);   
}

static int func_48e4bc(char a, struct MGC* b)
{
   if (a == 36)
   {
      return 0;
   }   

   if (a == 33)
   {
      return 1;
   }
   
   if ((b->bData_68 == 35) ||
         (b->bData_68 == 19))
   {
      return 1;
   }
   
   return 0;
}

/* 21cc686c - todo */
/* v3.8: func_48caf4 */
void func_21cc686c(char a, struct MGC* b)
{
   MUSB_SystemServices* pOS = b->a->pOS;
   
   (b->Func_616)(b);
   
   (pOS->pfCancelTimer)(pOS->pPrivateData, 0);
   
   #if 0
   FAPI_SYS_PRINT_MSG("func_21cc686c: a=%d\n", a);
   #endif

   if ((0 != func_48e4bc(a, b)) || 
         ((b->bData_68 & 0x0F) == 1))
   {
      func_48e464(a, b);   
   }
   
   switch (a) 
   {
      case 0:  
         //21cc69f8
         b->bData_34 = 0;
         b->bData_33 = 0;
         
         (b->Func_620)(b);
         //->21cc69dc
         break;
               
      case 20:
         //21cc6a70
         b->bData_33 = 1;
         
         (b->Func_620)(b);

         if (Data_21f6652c == 0)
         {
            (pOS->pfArmTimer)(pOS->pPrivateData, 0, 5000, 0, func_21cc7ab4);
         }
         //->21cc69dc
         break;
         
      case 34:
         //21cc6ab4
         b->bData_33 = 1;
         
         (b->Func_620)(b);

         if (Data_21f6652c == 0)
         {
            (pOS->pfArmTimer)(pOS->pPrivateData, 0, 8000, 0, func_21cc7a64);
         }
         //->21cc69dc
         break;

      case 18:
         //21cc6a14
         (pOS->pfArmTimer)(pOS->pPrivateData, 0, 200, 0, func_21cc7a0c);
         //->21cc69dc
         break;
      
      case 35:
         //21cc6af8         
#if 0
         FAPI_SYS_PRINT_MSG("21cc6af8: b->bData_68=%d\n", b->bData_68);
#endif
         
         if (b->bData_68 != 36) 
         {
            (pOS->pfArmTimer)(pOS->pPrivateData, 0, 10, 0, func_21cc5fac);
         }
         //->21cc69dc
         break;
         
      case 19:
         //21cc6a38
         b->bData_34 = 0;
         
         (b->Func_620)(b);

         (pOS->pfArmTimer)(pOS->pPrivateData, 0, 100, 0, func_21cc7bbc);
         //->21cc69dc
         break;
         
      case 36:
         //21cc6998
         if (b->Data_432 != 0) 
         {            
            struct usb_otg_descriptor* r0_ = (void*) 
               mgc_search_descriptor(b->Data_432->pCurrentConfiguration, 
                  (b->Data_432->pCurrentConfiguration[3] << 8) | 
                     b->Data_432->pCurrentConfiguration[2], 
                  USB_DT_OTG, 0);
               
            if ((r0_ != 0) && (r0_->bmAttributes & USB_OTG_HNP))
            {
               //21cc6b28
               Data_21f02164.pDevice = b->Data_432;
               Data_21f02164.pCompleteParam = b;
                
               MUSB_StartControlTransaction(b->b, &Data_21f02164);
            }
            else
            {
               //21cc69d4
               func_21cc4d40(b);
            }
         }
         //->21cc69dc
         break;

      case 17:
      case 33:
         //21cc69dc
         break; 
         
      default:
         return;
   }
   //21cc69dc
   b->bData_68 = a;
   
   musb_otg_state_changed(b);
}


/* 21cc6694 - todo */
/* v3.8: 48ce80 - complete */
/* Remove all URB's (tx/rx) from all Endpoints (except Ep0) */
/* Clear Endpoints */
void func_21cc6694(struct MGC* a)
{
   uint32_t i;
   uint16_t j;
   
   void* pUrb;
   uint32_t numURBs;
   uint32_t pBase = a->a->addr2;
   
   uint32_t numEndpoints = List_49a810_get_element_count(&a->ep_list);
   
   for (i = 1; i < numEndpoints; i++) 
   {
      MGC_Endpoint* pEndpoint = List_49a810_get_element(&a->ep_list, i);   
      
      if (pEndpoint != 0) 
      {
         pEndpoint->g = pEndpoint->h = 0;   
         pEndpoint->halted_15 = pEndpoint->bData_19 = 0;
         pEndpoint->pCurrentTxUrb = pEndpoint->pCurrentRxUrb = 0;
         pEndpoint->bData_80 = pEndpoint->bData_81 = 0;
         
         (a->Func_640)(a, pEndpoint, 0x00, 1);
         (a->Func_640)(a, pEndpoint, 0x80, 1);
         
         numURBs = List_49cd8c_get_element_count(&pEndpoint->tx_urb_list);
         
         for (j = 0; j < numURBs; j++) 
         {
            pUrb = List_49cd8c_get_element(&pEndpoint->tx_urb_list, 0);
            List_49cd8c_remove_element(&pEndpoint->tx_urb_list, pUrb);   
         }

         numURBs = List_49cd8c_get_element_count(&pEndpoint->rx_urb_list);
         
         for (j = 0; j < numURBs; j++) 
         {
            pUrb = List_49cd8c_get_element(&pEndpoint->rx_urb_list, 0);
            List_49cd8c_remove_element(&pEndpoint->rx_urb_list, pUrb);   
         }
      }
   }
   
   MGC_SelectEnd(pBase, 0);
}


/* 21cc67d8 - complete */
int func_21cc67d8(struct MGC* a)
{
//   FAPI_SYS_PRINT_MSG("func_21cc67d8\n");

   Struct_49d2fc* r15 = a->a;
   
   (r15->pOS->pfCancelTimer)(r15->pOS->pPrivateData, 0);

   a->bData_33 = 0;
   a->address_32 = 0;
   
   (a->Func_620)(a);
   (r15->pfDisableInterrupts)(r15);
   (r15->pOS->pfFlushBackgroundQueue)(r15->pOS->pPrivateData);
   
   func_21cc6694(a);
   
   (r15->pfEnableInterrupts)(r15);
   (a->Func_616)(a);
   
   a->bData_29 = 0;
   a->bDeviceState = MUSB_POWER_OFF;
   a->bData_34 = 0;
   a->bData_68 = 0;
   
   musb_otg_state_changed(a);
   
   return 0;
}


/* 21cc6144 - todo */
/* v3.8: func_48CDB4 */
void func_21cc6144(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
#if 0
   int r15;
   unsigned short r16;
   struct MGC* r13 = a->Data_20;
   
   r13->bData_31 = 0;
   
   (r13->Func_620)(r13);
   
   r15 = List_49cd8c_get_element_count(&r13->List_524);
   
   for (r16 = 0; r16 < r15; r16++) 
   {
      struct
      {
         int a; //0
         void* b; //4   
      }* r0 = List_49cd8c_get_element(&r13->List_524, r16);   
      if (r0 != 0)
      {
         struct 
         {
            void* a; //0   
         }* r0_ = r0->b;
         if (r0_ != 0) 
         {
            struct 
            {
               int a; //0
               int b; //4
               int c; //8
               int d; //12
               int e; //16
               int f; //20
               void (*g)(int a, struct MGC* b); //24   
            }* r1 = r0_->a;
            if (r1->f != 0) 
            {
               (r1->g)(r1->a, r13);   
            }
         }
      }
   }
#else
   FAPI_SYS_PRINT_MSG("21cc6144\n");
#endif
}


/* 21cc6100 - todo */
void func_21cc6100(void)
{
   FAPI_SYS_PRINT_MSG("func_21cc6100\n");
}


/* v3.8: func_48bc60 */
/* 21cc6084 - complete */
void musb_otg_state_changed(struct MGC* a)
{     
   MUSB_OtgClient* pOtgClient = a->pOtgClient;

   if (pOtgClient != 0)
   {
      (pOtgClient->pfOtgState)(pOtgClient->pPrivateData, a, a->bData_68);
   }
}


/* 21cc5fac - complete */
/* v3.8: func_495134 */
void func_21cc5fac(Struct_49d2fc* a, unsigned short b, MGC_Timer* c)
{
#if 0
   FAPI_SYS_PRINT_MSG("func_21cc5fac\n");
#endif
   
   struct MGC* r0 = a->Data_20;
   
   r0->bData_29 = 1;
   r0->bData_30 = 0;
   
   (r0->Func_620)(r0);
   
   (a->pOS->pfArmTimer)(a->pOS->pPrivateData, 0, 60, 0, func_21cc7bbc);
}


/* 21cc5974 - todo */
int MGC_HsDmaProgramChannel(MGC_DmaChannel* pChannel, 
                        unsigned short wPacketSize, char bMode,
                        const char* pBuffer, 
                        int dwLength)
{
   return 0;
}


/* 21cc57c8 - todo */
MGC_HsDmaChannel* MGC_HsDmaAllocateChannel(void* pPrivateData, 
      char bLocalEnd, 
      char bTransmit, 
      char bProtocol, 
      unsigned short wMaxPacketSize)
{   
   if ((MGC_pHsDmaController == 0) ||
      ((MGC_HsDmaController*)pPrivateData != MGC_pHsDmaController) ||
      (bLocalEnd > 3))
   {
      return 0;
   }
   
   if (MGC_pHsDmaController->aChannel[0].Channel.f == 0)
   {         
      return 0;
   }
   
   MGC_HsDmaChannel* pImplChannel = &MGC_pHsDmaController->aChannel[0];
   
   pImplChannel->Channel.g = 0;
   pImplChannel->dwStartAddress = 0;
   pImplChannel->dwCount = 0;
   pImplChannel->bEnd = bLocalEnd;
   pImplChannel->bTransmit = bTransmit;
   pImplChannel->wMaxPacketSize = wMaxPacketSize;
   pImplChannel->k = 0;
   pImplChannel->l = 0;
   pImplChannel->nNumPackets = 0;
   pImplChannel->nPacketCount = 0;
   pImplChannel->Channel.pPrivateData = pImplChannel;
   pImplChannel->Channel.dwMaxLength = 0x100000;
   pImplChannel->Channel.dwActualLength = 0;
   pImplChannel->Channel.d = 1;
   pImplChannel->Channel.bDesiredMode = 0;    
      
   if (2 == MGC_HsDmaGetChannelStatus(&pImplChannel->Channel))
   {
      FAPI_SYS_PRINT_MSG("[MUSB] Cancel not finished DMA request.\n");
                  
      FAPI_DMA_CancelRequest(pImplChannel->Channel.f, 
            pImplChannel->Channel.f->feature);
   }
   
   return pImplChannel;
}


/* 21cc5784 - complete */
int MGC_HsDmaGetChannelStatus(MGC_DmaChannel* pChannel)
{
   if (pChannel != 0)
   {
      MGC_HsDmaChannel* pImplChannel = (MGC_HsDmaChannel*)pChannel->pPrivateData;
      
      if ((pImplChannel != 0) &&
            (pImplChannel->Channel.f != 0))
      {
         if ((FAPI_DMA_GetRequestStatus(pImplChannel->Channel.f) - 2) < 3)
         {
            return 2;
         }

         return 1;
#if 0
         switch (fd_dma_get_channel_state(r0)) 
         {
            case 0:
            case 5:
            case 6:
               return 1;   
               
            default:
               return 2;
         }
#endif
      }
   }

   return 0;
}


/* 21cc5674 - todo */
MGC_DmaController* MGC_HsNewDmaController(int (*pfDmaChannelStatusChanged)(void* pPrivateData, char bLocalEnd, char bTransmit),
      struct MGC* pDmaPrivate,
      char* (*c)(int, char* b),
      unsigned d,
      unsigned addr1,
      unsigned addr2)
{   
   if (MGC_pHsDmaController == 0) 
   {
      MGC_pHsDmaController = (MGC_HsDmaController*)
         FAPI_SYS_MALLOC(sizeof(MGC_HsDmaController));
         
      if (MGC_pHsDmaController != 0) 
      {
         musb_memset(MGC_pHsDmaController, 0, sizeof(MGC_HsDmaController));

         MGC_pHsDmaController->pfDmaChannelStatusChanged = pfDmaChannelStatusChanged;
         MGC_pHsDmaController->pDmaPrivate = pDmaPrivate;
         MGC_pHsDmaController->Data_328 = c;
         MGC_pHsDmaController->Data_332 = d;
         MGC_pHsDmaController->Data_336 = addr1;
         MGC_pHsDmaController->Data_340 = addr2;

         MGC_pHsDmaController->Controller.pPrivateData = MGC_pHsDmaController;
         MGC_pHsDmaController->Controller.pfDmaStartController = MGC_HsDmaStartController;
         MGC_pHsDmaController->Controller.pfDmaStopController = MGC_HsDmaStopController;
         MGC_pHsDmaController->Controller.pfDmaAllocateChannel = MGC_HsDmaAllocateChannel;
         MGC_pHsDmaController->Controller.pfDmaReleaseChannel = MGC_HsDmaReleaseChannel;
         MGC_pHsDmaController->Controller.pfDmaAllocateBuffer = MGC_HsDmaAllocateBuffer;
         MGC_pHsDmaController->Controller.pfDmaReleaseBuffer = MGC_HsDmaReleaseBuffer;
         MGC_pHsDmaController->Controller.pfDmaProgramChannel = MGC_HsDmaProgramChannel;
         MGC_pHsDmaController->Controller.pfDmaGetChannelStatus = MGC_HsDmaGetChannelStatus;
         MGC_pHsDmaController->Controller.pfDmaControllerIsr = 0;         

         MGC_pHsDmaController->aChannel[0].Channel.f = 
            FAPI_DMA_AllocateRequest(FAPI_DMA_FEATURE_USB_COPY, 0);
         
         return &MGC_pHsDmaController->Controller;
      }
   }
   
   return 0;
}


/* 21cc5640 - todo */
void MGC_HsDmaReleaseChannel(MGC_DmaChannel* pChannel)
{
   if (pChannel != 0)
   {
      MGC_HsDmaChannel* pImplChannel = (MGC_HsDmaChannel*) pChannel->pPrivateData;

      if (pImplChannel != 0)
      {
         FAPI_DMA_RequestT* request = pImplChannel->Channel.f;
         
         musb_memset(pImplChannel, 0, sizeof(MGC_HsDmaChannel));
         
         pImplChannel->Channel.f = request;
      }
   }
}


/* 21cc55e0 - complete */
void MGC_HsDestroyDmaController(MGC_DmaController* pController)
{
   MGC_pHsDmaController = (MGC_HsDmaController*) pController->pPrivateData;   
   
   if (MGC_pHsDmaController != 0) 
   {      
      if (MGC_pHsDmaController->aChannel[0].Channel.f != 0)
      {
         FAPI_DMA_CancelRequest(MGC_pHsDmaController->aChannel[0].Channel.f, 
               MGC_pHsDmaController->aChannel[0].Channel.f->feature);
         
         FAPI_DMA_ReleaseRequest(MGC_pHsDmaController->aChannel[0].Channel.f);
      }
      
      MGC_pHsDmaController->Controller.pPrivateData = 0;
      
      (FAPI_SYS_Services.freeFunc)(MGC_pHsDmaController);
      
      MGC_pHsDmaController = 0;
   }
}


/* 21cc553c - complete */
int MGC_HsDmaStartController(void* pPrivateData)
{
   /* nothing to do */
   return 1;   
}


/* 21cc5544 - complete */
int MGC_HsDmaStopController(void* pPrivateData)
{
   /* nothing to do */
   return 1;   
}


/* 21cc554c - complete */
void* MGC_HsDmaAllocateBuffer(void* a, int size)
{
   return FAPI_SYS_MALLOC(size);   
}


/* 21cc5574 - todo */
int MGC_HsDmaReleaseBuffer(void* a, void* p)
{
   (FAPI_SYS_Services.freeFunc)(p);
   
   return 1;   
}


/* 21cc5594 - complete */
int MGC_SetDmaControllerFactory(int a)
{
   Data_21f7a058.a1 = 0x200;
   Data_21f7a058.pfNewDmaController = MGC_HsNewDmaController;
   Data_21f7a058.pfDestroyDmaController = MGC_HsDestroyDmaController;
   
   MGC_SetExternalDmaControllerFactory(&Data_21f7a058);
   
   Data_21f7a050 = a;
   
   return 1;
}


/* 48bd78 - todo */
static void func_48bd78(Struct_49d2fc* a, int addr1, int addr2)
{
   a->addr1 = addr1;
   a->addr2 = addr2;
   a->Data_20 = &a->Data_88;   
   
   List_49cd8c_clear(&a->Data_88.List_524);
   List_49cd8c_clear(&a->Data_88.Data_536.b);

   a->Data_88.b = &a->Data_88.usbPort;
   a->Data_48.wVersion = 1;
   a->Data_48.pPrivateData = a;

   a->Data_88.usbPort.pPrivateData = &a->Data_88;
   a->Data_88.usbPort.bcdCdiRev = 0x100;

   a->Data_88.a = a;
}


/* 48c1c0 - complete */
static int func_48c1c0(int a)
{
   return 0;
}


/* 21cc4eac - todo */
/* v3.8: func_49d2fc */
void* func_21cc4eac(MUSB_SystemUtils* r8,
      unsigned short r9, int addr1, int addr2)
{
   struct Struct_49d2fc_t* r6;
   int r0 = 0;
   void* r14 = 0; 
   
   r6 = FAPI_SYS_MALLOC(sizeof(struct Struct_49d2fc_t));
         
   if (r6 != 0)
   {
      musb_memset(r6, 0, sizeof(struct Struct_49d2fc_t));
         
      /* Add an element in the list and set its data */      
      if (0 == List_49cd8c_add_element(&Data_21f7a044, r6, 0))
      {
         //21cc4f70
         (FAPI_SYS_Services.freeFunc)(r6);
         r6 = 0; //return 0;
      }
      else
      {
         wData_21f7a040++;
      }
   }
   
   if (r6 != 0)
   {
      r6->pSystemUtils = r8;
      //memcpy((void*)&r6->pSystemUtils, (void*)r8, sizeof(MUSB_SystemUtils));
      
      func_48bd78(r6, addr1, addr2);
                  
      if (r9 == 0)
      {
         r9 = func_48c1c0(addr2);
      }
      
      if (r9 == 3)
      {
         r0 = musb_log_release_info(&r6->Data_88); 
         
         r6->Data_88.bData_28 = 1;
         r6->Data_88.bData_42 = 1;

         r6->Data_48.wQueueLength = 80;
         r6->Data_48.wQueueItemSize = 8;
         r6->Data_48.wTimerCount = 1;
         r6->Data_48.adwTimerResolutions = Data_21f02130;
         r6->Data_48.wLockCount = 17;
         r6->Data_48.pfIsr = mgc_isr;
         r6->Data_48.pIsrParam = &r6->Data_48;
         r6->Data_48.pfBsr = mgc_handle_messages;
         r6->Data_48.pBsrParam = &r6->Data_88.usbPort;
         
         r6->pDmaControllerFactory = mgc_pDmaControllerFactory;
         r6->pfEnableInterrupts = mgc_set_interrupt_enables;
         r6->pfDisableInterrupts = mgc_set_interrupt_disables;
         r6->Data_36 = func_21ccb9ec;
         r6->Data_40 = func_21cc6100;
                  
         r6->Data_88.Func_616 = func_21cc97ec;
         r6->Data_88.Func_620 = func_21cc96e8;
         r6->Data_88.Func_624 = func_21cc67d8;
         r6->Data_88.Func_628 = func_21cceaac;
         r6->Data_88.pfReceiveOnEp = mgc_receive_on_endpoint;
         r6->Data_88.pfSendOnEp = mgc_send_on_endpoint;
         r6->Data_88.Func_640 = func_21cc993c;
         r6->Data_88.Func_644 = func_21ccb504;
         r6->Data_88.Func_648 = func_21ccabf0;
         r6->Data_88.pfDefaultEndHandler = MGC_HdrcServiceDefaultEnd;
         r6->Data_88.pfServiceRxReady = MGC_HdrcServiceRxReady;
         r6->Data_88.pfServiceTxAvail = MGC_HdrcServiceTxAvail;
         r6->Data_88.pfLoadFifo = MGC_HdrcLoadFifo;
         r6->Data_88.pfUnloadFifo = MGC_HdrcUnloadFifo;
         r6->Data_88.Func_672 = func_21cc6d64;
         r6->Data_88.pfDmaChannelStatusChanged = func_21ccb018;
         r6->Data_88.Func_680 = func_21cca940;
         r6->Data_88.Func_684 = func_21cc9530;

         r6->Data_88.usbPort.Type = MUSB_PORT_TYPE_OTG; //r9;
         r6->Data_88.usbPort.Speed = MUSB_PORT_SPEED_HIGH; //r9;
      }      
   }
   
   if (r0 != 0)
   {
      char i;
      for (i = 0; i < r6->Data_88.bEndCount; i++)
      {
         MGC_Endpoint* pEnd = List_49a810_get_element(&r6->Data_88.ep_list, i);
         
         List_49cd8c_clear(&pEnd->tx_urb_list);
         List_49cd8c_clear(&pEnd->rx_urb_list);
      }
      r14 = &r6->Data_48;
   }
      
   return r14;
}


/* 21cc4dc8 - complete */
/* v3.8: func_49c1d4 */
MUSB_Port* func_21cc4dc8(unsigned short a)
{
   MUSB_Port* res = 0;
   
   if (a < wData_21f7a040)
   {
      Struct_49d2fc* r0 = List_49cd8c_get_element(&Data_21f7a044, a);      
      if (r0 != 0)
      {
         res = &r0->Data_88.usbPort;
      }
   }
   
   return res;
}


/* 21cc4d40 - todo */
void func_21cc4d40(struct MGC* a)
{
   printf("func_21cc4d40");
}


/* 21cc41c4 - complete */
int func_21cc41c4(MUSB_Controller* a, MUSB_SystemServices* pOS)
{
   Struct_49d2fc* r14 = 0;
   
   if (a != 0) 
   {
      r14 = a->pPrivateData;
   }
   
   if (r14 != 0) 
   {
      if (r14->pDmaControllerFactory != 0) 
      {
         r14->Data_20->pDmaController =
            (r14->pDmaControllerFactory->pfNewDmaController)(
               r14->Data_20->pfDmaChannelStatusChanged, 
               /*pDmaPrivate*/r14->Data_20, 
               pOS->pfSystemToBusAddress,
               pOS->pPrivateData,
               r14->addr1,
               r14->addr2);      
      }
      
      if (r14->Data_20->pDmaController != 0) 
      {
         (r14->Data_20->pDmaController->pfDmaStartController)(
            r14->Data_20->pDmaController->pPrivateData);   
      }
      
      r14->pOS = pOS;
      
      return (r14->pfEnableInterrupts)(r14);
   }
   
   return 132;
}


/* 21cc417c - complete */
void MGC_SetExternalDmaControllerFactory(MGC_DmaControllerFactory* a)
{
   mgc_pDmaControllerFactory = a;
}


/* 21cc3e28 - todo */
void func_21cc3e28(char* a, unsigned short b, unsigned c, unsigned char d, unsigned e)
{
//   printf("func_21cc3e28");
   
   sprintf(a, "0x%x", c);
}


/* 21cc3b54 - todo */
void func_21cc3b54(char* a, unsigned short b, const char* c)
{
//   printf("func_21cc3b54");
   
   strcpy(&a[Data_21cc3b54], c);
   
   Data_21cc3b54 += strlen(c);
}


/* 49b7d8 - nearly complete */
void* mgc_find_interface_descriptor(char* pConfig, int b, int c) 
{
   char* r6;
   char r5;
   char* pInterface = 0;
   signed short r7 = -1;
   
   if ((pConfig != 0) &&
      ((pConfig[1]/*bDescriptorType*/ == USB_DT_CONFIG) ||
         (pConfig[1]/*bDescriptorType*/ == USB_DT_OTHER_SPEED_CONFIG)))
   {
      unsigned short wTotalLength = (pConfig[3] << 8) | pConfig[2];      
      unsigned short wLength = pConfig[0]/*bLength*/;
      
      while ((wLength < wTotalLength) && (r7 != (signed short)b)) 
      {
         r6 = &pConfig[wLength];
            
         r5 = r6[0];
         if (r5 <= 0) return 0;
         wLength += r5;
         
         if (r6[1] == USB_DT_INTERFACE) 
         {
            pInterface = r6;  
            r7 = pInterface[2]/*bInterfaceNumber*/;                
         }
      }

      if ((wLength == wTotalLength) && (r7 == (signed short)b) && 
         (pInterface[3]/*bAlternateSetting*/ == c))
      {
         return pInterface;   
      }

      if (wLength < wTotalLength) 
      {
         r7 = -1;
         
         if (c == 0) return pInterface;
         
         while ((wLength < wTotalLength) && (r7 != (signed short)c)) 
         {
            r6 = &pConfig[wLength];   

            r5 = r6[0];
            if (r5 <= 0) return 0;
            wLength += r5;

            if (r6[1] == USB_DT_INTERFACE) 
            {
               pInterface = r6;  
               r7 = pInterface[3]/*bAlternateSetting*/;
            }
         }

         if ((wLength < wTotalLength) && (pInterface[2] == b))
         {
            return pInterface;   
         } 
      }
   }

   return 0;   
}


/* 49b6b0 - nearly complete */
void* mgc_find_descriptor_in_interface(struct usb_config_descriptor* pConfig, 
                  char* pInterface, int c, int d)
{
   signed short r7 = -1;
   char* r4 = pInterface;
   
   if ((pConfig != 0) &&
      ((pConfig->bDescriptorType == USB_DT_CONFIG) 
         || (pConfig->bDescriptorType == USB_DT_OTHER_SPEED_CONFIG)) &&
      (pInterface != 0))
   {
      char r10 = r4[1]; //bDescriptorType
      int r5_ = r4[0];
      unsigned short r6 = r5_;
      r4 = &pInterface[r6];
      unsigned short r5 = pInterface - (char*) pConfig;
      unsigned short r9 = pConfig->wTotalLength - r5;       
      while ((r6 < r9) && (r10 != r4[1]) && (r7 < (signed short)d))
      {
         r4 = &pInterface[r6];

         char r5_ = r4[0];
         if (r5_ <= 0) return 0;   
         r6 += r5_;

         if (c == r4[1]) 
         {
            r7++;   
         }
      }

      if ((r6 < r9) && (r10 != r4[1]))
      {
         return r4;   
      }

      if ((r6 == r9) && (r7 == d) && (c == r4[1])) 
      {
         return r4;        
      }
   }

   return 0;
}


/* 49b7b0 - complete */
void* mgc_find_endpoint_descriptor(void* pConfig, void* pInterface, int c)
{
   return mgc_find_descriptor_in_interface(pConfig, pInterface, USB_DT_ENDPOINT, c);
}


typedef struct
{
   int a; //0 
   int b; //4
   int c; //8
   int d; //12
   int e; //16
   int f; //20
   unsigned g; //24
   unsigned h; //28
   //32     
} Struct_601854;

//0x601954
Struct_601854 Data_601854 = //601854 -256 
{
   1000000,
   1000,
   66060, 
   66107, 
   0,
   0,
   6767,
   900000,   
}; 

Struct_601854 Data_601874 = //601874 -224
{
   1000000,
   1000,
   9107, 
   9107, 
   7268,
   6265,
   836,
   900000,   
}; 

Struct_601854 Data_601894 = //601894 -192
{
   125000,
   1000,
   917, 
   917, 
   634,
   634,
   21,
   100000,   
}; 


/* 48bcb4 - complete */
int func_48bcb4(Struct_601854* a, char transfer_type, int endpoint_dir, unsigned short d)
{
   int r13 = a->b;
   
   r13 += ((d * 177352) / 6000) * a->g/10;
   
   switch (transfer_type) 
   {
      case 1:      
         if (endpoint_dir != 0) 
         {
            r13 += a->e;   
         }
         else 
         {
            r13 += a->f;   
         }
         break;
         
      default:
         if (endpoint_dir != 0) 
         {
            r13 += a->c;   
         }
         else 
         {
            r13 += a->d;   
         }
         break;
   }
   
   return r13;
}


/* 48e610 - complete */
List_49cd8c* func_48e610(Struct_49d2fc_Inner1_5* a, 
                  Struct_601854* b, 
                  unsigned short c, 
                  unsigned short d, 
                  int e)
{
   List_49cd8c* r13;
   
   if (a->c == 0) 
   {
      r13 = FAPI_SYS_MALLOC(sizeof(List_49cd8c));
      if (r13 != 0) 
      {
         musb_memset(r13, 0, sizeof(List_49cd8c));
         
         List_49cd8c_clear(r13);
         
         if (0 != List_49cd8c_add_element(&a->b, r13, 0)) 
         {
            a->c = 1;   
         }
         else 
         {
            FAPI_SYS_FREE(r13);
            r13 = 0;   
         }
      }
   }
   else 
   {
      r13 = List_49cd8c_get_element(&a->b, 0);         
   }
   
   if (r13 != 0) 
   {      
      if ((a->a + e) <= b->h) 
      {
         a->a += e;   
      }
      else 
      {
         r13 = 0;   
      }
   }
   
   return r13;
}


/* 48b728 - complete */
int func_48b728(struct MGC* a, MGC_Pipe* b, MUSB_DeviceEndpoint* c)
{
   Struct_49d2fc_Inner1_5* r14_;
   
   int r19;
   unsigned short wMaxPacketSize;
   Struct_601854* r23;
   Struct_601854* r16 = 0;
   unsigned short r17 = 1;
   int res = 0;
   void* r3_ = c->pDevice;
   MUSB_Device* r24 = r3_;
   char r1 = c->UsbDescriptor.bInterval & 0x0F;
   char transfer_type = c->UsbDescriptor.bmAttributes & USB_ENDPOINT_XFERTYPE_MASK;
   int endpoint_dir = (c->UsbDescriptor.bEndpointAddress & USB_ENDPOINT_DIR_MASK)? 1: 0;
   unsigned short r22 = r1;
   
   if (a->bRootSpeed != 0) 
   {
      r23 = &Data_601894;   
   }
   else if (a->bData_49 != 0)
   {
      r23 = &Data_601854;
   }
   else   
   {
      r23 = &Data_601874;
   }
   
   switch (((MUSB_Device*)r3_)->ConnectionSpeed)
   {
      case 1:
         //48b804
         r16 = &Data_601854;
         break;

      case 3:
         //48b80c
         r16 = &Data_601894;
         
         r17 = (((((char*)&c->UsbDescriptor.wMaxPacketSize)[1] << 8) |
            ((char*)&c->UsbDescriptor.wMaxPacketSize)[0]) & 0x1800) >> 11;
            
         if (r17 == 0) 
         {
            r17 = 1;
         }
         
         switch (transfer_type) 
         {
            case 1:
            case 3:
               r22 = (1 << (r1-1));   
               break;
         }
         break;
         
      case 2:
         //48b860
         r16 = &Data_601874;
         if (transfer_type == 1) 
         {
            r22 = (1 << (r1-1));   
         }
         break;
   }
   //48b88c
   wMaxPacketSize = (((((char*)&c->UsbDescriptor.wMaxPacketSize)[1] << 8) |
      ((char*)&c->UsbDescriptor.wMaxPacketSize)[0]) & 0x7FF) * r17;
   
   r19 = func_48bcb4(r23, transfer_type, endpoint_dir, wMaxPacketSize);
   func_48bcb4(r16, transfer_type, endpoint_dir, wMaxPacketSize);
   
   switch (transfer_type) 
   {
      case 1:
      case 3:
         //48b8e0 
         {
            List_49cd8c* r13_ = func_48e610(&a->Data_536, r23, r22, r17, r19);
            if (r13_ != 0) 
            {
               res = 1;
               if (r24->pParentUsbDevice != 0)
               {
                  void* r20 = ((void**)r24->pParentUsbDevice)[1];
                  r14_ = ((void**)r20)[1];               
                  if (r14_ == 0) 
                  {
                     r14_ = FAPI_SYS_MALLOC(sizeof(Struct_49d2fc_Inner1_5));  
                     if (r14_ != 0) 
                     {
                        ((void**)r20)[1] = r14_;
                     
                        musb_memset(r14_, 0, sizeof(Struct_49d2fc_Inner1_5));
                        List_49cd8c_clear(&r14_->b);   
                     }
                  }
                  //48b95c
                  if (r14_ != 0) 
                  {
                     List_49cd8c* r0 = func_48e610(r14_, r16, r22, r17, r19);
                     if (r0 != 0) 
                     {
                        res = List_49cd8c_add_element(r0, b, 0);   
                     }
                     else 
                     {
                        res = 0;   
                     }
                  }
                  else 
                  {
                     res = 0;
                  }
               }
               //48b99c
               if (res != 0) 
               {
                  res = List_49cd8c_add_element(r13_, b, 0);   
                  b->Data_12 = r13_;
               }
            }
            //if (r13_ != 0)
         }
         //->48b9cc
         break;  

      default:
         //48b9c8
         res = 1;
         break;         
   }
   //48b9cc
   if (res != 0) 
   {
      b->Data_24 = r19;   
      b->bTransferType = transfer_type;
      b->wMaxPacketSize = wMaxPacketSize;
   }

   return res;
}


/* 21ccc5a8 - todo */
/* v3.8: 49d604 - complete */
MGC_Pipe* func_21ccc5a8(struct MGC* a, 
      MUSB_DeviceEndpoint* b, void* c)
{
   struct 
   {
      int a; //0
      int b; //4
      int c; //8   
   } fp12;   
   
   int r13;
   MGC_Endpoint* ep;
   MGC_Pipe* r20;
   
   int r16 = 0;
   MGC_Pipe* r22 = 0;
   
   struct MGC* r19 = a;
   MUSB_Device* r15 = b->pDevice;
   Struct_49d2fc_Inner1_3_2* r23 = r15->pPrivateData;
   MUSB_SystemServices* pOS = r19->a->pOS;
   
   (pOS->pfLock)(pOS->pPrivateData, 0);
   
   (r19->Func_616)(r19);
   
   r20 = FAPI_SYS_MALLOC(sizeof(MGC_Pipe));
   if (r20 != 0) 
   {
      musb_memset(r20, 0, sizeof(MGC_Pipe));
   
      r20->Data_0 = a;
      r20->Data_8 = r19;

      if (b->UsbDescriptor.bEndpointAddress & USB_ENDPOINT_DIR_MASK)
      {
         r20->Data_28 = 0x01;
         r13 = 0;   
      }
      else 
      {
         r20->Data_28 = 0x81;
         r13 = 1;   
      }
   
      r20->Data_16 = r15;

      if (c == 0) 
      {
         fp12.b = 0;
         fp12.a = b->UsbDescriptor.wMaxPacketSize;
         c = &fp12;   
      }

      ep = (r19->Func_628)(r19, b, c, 1);   
      if (ep != 0) 
      {
         r16 = func_48b728(r19, r20, b);   
      }

      if ((ep != 0) && (r16 != 0) &&
         (0 != List_49cd8c_add_element(&r23->c, r20, 0))) 
      {
         r20->pEndpoint = ep;
         r22 = r20;  
      }
      else if (ep != 0)
      {
         if (r13) 
         {
            ep->g = 0;   
         }
         else
         {
            ep->h = 0;   
         }
      
         (r19->Func_640)(r19, ep, 
            b->UsbDescriptor.bEndpointAddress & USB_ENDPOINT_DIR_MASK, 0);

         FAPI_SYS_FREE(r20);   
      }
      else 
      {
         FAPI_SYS_FREE(r20);   
      }
   }

   (pOS->pfUnlock)(pOS->pPrivateData, 0);
      
   return r22;
}


/* 21ccc110 - todo */
/* v3.8: 49af40 - complete */
/* Cleanup */
int func_21ccc110(MGC_Pipe* a)
{
   int res;
   int r19;
   char r2;
   Struct_49d2fc_Inner1_3_2* r15 = a->Data_16->pPrivateData;
   struct MGC* r14 = a->Data_0;
   MGC_Endpoint* pEndpoint = a->pEndpoint;
   MUSB_SystemServices* pOS = r14->a->pOS;
   
   (pOS->pfLock)(pOS->pPrivateData, 0);
   
   a->Data_28 |= 0x02;
   
   r19 = (a->Data_28 & 0x80)? 1:0; 
   
   if (a->Data_28 & 0x01)
   {
      r2 = (a->Data_28 & 0x80)? 0: 0x80;  
   }
   else 
   {
      r2 = (a->Data_28 & 0x80)? 0x80: 0;
   }

   res = (r14->Func_640)(r14, pEndpoint, r2, 1);
     
   if (a->Data_12 != 0) 
   {
      List_49cd8c_remove_element(a->Data_12, a);   
   }
   
   List_49cd8c_remove_element(&r15->c, a);
   
   musb_memset(a, 0, sizeof(MGC_Pipe));
   FAPI_SYS_FREE(a);
   
   if (r19 != 0) 
   {
      pEndpoint->g = 0;   
   }
   else
   {
      pEndpoint->h = 0;   
   }
   
   (pOS->pfUnlock)(pOS->pPrivateData, 0);
   
   return res;
}


/* 21ccc2b4 - todo */
/* v3.8: 49d950 - complete */
/* Cleanup */
void func_21ccc2b4(struct MGC* a, MUSB_Device* b)
{
   FAPI_SYS_PRINT_MSG("TODO: func_21ccc2b4\n");
   
#if 0
   Struct_49d2fc_Inner1_3_2* r13 = b->b;
   MUSB_SystemServices* pOS = a->a->pOS;
   
   if ((r13 != 0) && (r13->b != 0))
   {
      func_48e704(r13->b);
      
      RTOS_MemoryRelease(r13->b);
      
      r13->b = 0;      
   }

   if (b == a->Data_432) 
   {
      func_48e704(&a->Data_536);      
   }
   
   MGC_ReleaseDeviceAddress(&a->Data_156, b->bDeviceAddress);
   
   (pOS->pfLock)(pOS->pPrivateData, 0);
   
   if (r13 != 0) 
   {
      int r20 = List_49cd8c_get_element_count(&r13->c);
      for (unsigned short i = 0; i < r20; i++) 
      {
         MGC_Pipe* r1 = List_49cd8c_get_element(&r13->c, 0);   
         
         if (r1->Data_12 != 0) 
         {
            List_49cd8c_remove_element(r1->Data_12, r1);   
         }
      }
   }
   //49da54
   List_49cd8c_remove_element(&a->List_524, b);
   
   if (b->pConfigurationOffsets != 0) 
   {
      RTOS_MemoryRelease(b->pConfigurationOffsets);   
      b->pConfigurationOffsets = 0;
   }
   
   if (b->pConfigurations != 0) 
   {
      RTOS_MemoryRelease(b->pConfigurations);   
      b->pConfigurations = 0;
   }
   
   b->b = 0;
   
   if (r13 != 0) 
   {
      RTOS_MemoryRelease(r13);   
   }

   (pOS->pfUnlock)(pOS->pPrivateData, 0);
#endif
}


/* 21ccfc9c - todo */
/* v3.8: 49e470 - complete */
uint32_t MUSB_StartTransfer(MUSB_Irp* a)
{   
   /*void*/MUSB_Irp* sl;
   uint32_t res = 0;
 
   MGC_Pipe* pPipe = a->hPipe;
   struct MGC* r14 = pPipe->Data_0;
   MUSB_SystemServices* pOS = r14->a->pOS;
   MGC_Endpoint* pEndpoint = pPipe->pEndpoint;
   
   char bTx = (pPipe->Data_28 & 0x80)? 1: 0;

   char bTrafficType = (bTx != 0)? 
      pEndpoint->bTxTrafficType:
      pEndpoint->bRxTrafficType;
   
   char fp = r14->bHostMode;

#if 0
   FAPI_SYS_PRINT_MSG("MUSB_StartBulkOrInterruptTransfer: bTx=%d bTrafficType=%d\n",
         bTx, bTrafficType);
#endif

   if (fp != 0) 
   {
      (pOS->pfLock)(pOS->pPrivateData, pEndpoint->bEnd + 1);
   }
   
   switch (bTrafficType)
   {
      case 0:
      case 1:
         //21ccfd98
         res = 164;
         break;
                 
      case 2:
      case 3:
         //21ccfd24
         sl = (bTx != 0)? 
               pEndpoint->pCurrentTxUrb: 
               pEndpoint->pCurrentRxUrb;
         
         if (sl != 0)
         {
            //21ccfd38
            if (bTrafficType == 3)
            {
               //21ccfdb8
               res = 133;
            }
            else 
            {
               if (bTx == 0) 
               {
                  //21ccfd48
#if 0
                  if (0 == List_49cd8c_add_element(&pEndpoint->rx_urb_list, a, 0))
                  {
                     res = 166;
                  }
#else
                  res = (0 == List_49cd8c_add_element(&pEndpoint->rx_urb_list, a, 0))?
                        166: 0;
#endif
               }
               else
               {
                  //21ccfdf8
#if 1
                  if (0 == List_49cd8c_add_element(&pEndpoint->tx_urb_list, a, 0)) 
                  {
                     res = 166;
                  }
#else
                  res = (0 == List_49cd8c_add_element(&pEndpoint->tx_urb_list, a, 0))?
                        166: 0;
#endif
               }
            }

            if (bTx != 0)
            {        
               if ((pEndpoint->pCurrentTxUrb == 0) &&
                     (a == List_49cd8c_get_element(&pEndpoint->tx_urb_list, 0)))
               {
                  func_21ccf72c(r14, pEndpoint, 1);                  
               }                   
            }
            else
            {        
               if ((pEndpoint->pCurrentRxUrb == 0) &&
                     (a == List_49cd8c_get_element(&pEndpoint->rx_urb_list, 0)))
               {
                  func_21ccf72c(r14, pEndpoint, 0);
               }
            } 
         } //if ((bTx != 0)? pEndpoint->pCurrentTxUrb: pEndpoint->Data_48)
         else
         {
            //21ccfe18
            if (bTx != 0)
            {
               //21ccfe20
               if (pEndpoint->halted_15 == 0)
               {
                  //21ccfe34
                  (r14->pfSendOnEp)(r14, pEndpoint, 
                     a->pBuffer, a->dwLength, a);
               }
               else
               {
                  pEndpoint->pCurrentTxUrb = a;
               }
            }
            else
            {
               //21ccfe54
               (r14->pfReceiveOnEp)(r14, pEndpoint, 
                  a->pBuffer, a->dwLength, a, a->bAllowDma);
            }
         }
         break;
   } //switch (bTrafficType)

   if (fp != 0)
   {
      (pOS->pfUnlock)(pOS->pPrivateData, pEndpoint->bEnd + 1);
   } 
   
   return res;
}


/* 21ccf514 - todo */
/* v3.8: 49b90c - complete */
int func_21ccf514(MGC_Pipe* a)
{
   struct MGC* r1 = a->Data_0;
   int r2 = (a->Data_28 & 0x80)? 1: 0;
   
   if (r1->bHostMode) 
   {
      r2 = !r2;   
   }
   
   if (r1 != 0) 
   {      
      return (r1->Func_640)(r1, a->pEndpoint, r2? 0x80: 0, 0);   
   }
   
   return 168;
}
























