
#define DEBUG 0

#include <fapi/sys_services.h>
#include <fapi/drv_mmu.h>
#include <fapi/drv_mmu_heap.h>
#if 0
#include <fapi/drv_ir.h>
#include <fapi/drv_ir_rc5.h>
#endif
#include <fapi/drv_timer.h>
#include <fapi/drv_uart.h>
#include "sys.h"
#include "sys_api.h"
#if 0
#include "rtos.h"
#include "device.h"
#endif
#include "event_queue.h"
#include "input.h"


/* 21b90968 - todo */
void func_21b90968(void)
{
   printf("21b90968");   
}


/* 21bbc518 - todo */
void func_21bbc518(void)
{
   printf("21bbc518");
}


/* 21b0c17c - todo */
void func_21b0c17c(void)
{
//   printf("func_21b0c17c");
}


/* 21ba5714 - todo */
int func_21ba5714(void* a)
{
//   printf("func_21ba5714");
   return 0;
}


/* 21ba6930 - todo */
void func_21ba6930(void)
{
   printf("21ba6930");
}


void input_thread(int a);
void input_rc(FAPI_SYS_HandleT, uint32_t*);
int input_uart(void);


int Data_21f02a00; //21f02a00
unsigned short wData_21f02a04; //21f02a04
unsigned short wData_21f02a06; //21f02a06
unsigned short wData_21f02a08; //21f02a08

int Data_21f03030; //21f03030

uint64_t input_timeStamp; //21f7ba18
void* input_uart_handle; //21f7ba20




/* 21b8e0f8 - complete */
int input_init(void)
{
#if 0
   FAPI_IR_OpenParamsT sp8;
#endif
   FAPI_UART_OpenParamsT uart;
   int32_t res;
      
   input_timeStamp = 0;
   
   Data_21f02a00 = 1;
   wData_21f02a04 = 200;
   wData_21f02a06 = 100;
   wData_21f02a08 = 0;
   
#if 0
   sp8.version = FAPI_IR_VERSION;
   sp8.swProtocol = FAPI_IR_RC5_Mode;
   sp8.rcmNotifyFunction = input_rc;
#endif
   
#if 0
   res = 0;
   
   FAPI_IR_Open(&sp8, &res);
#endif
   
   res = SYS_ThreadCreate(input_thread,
      0, 46, 0x2000, "Input Poller", 0);
   
   uart.version = FAPI_UART_VERSION;
   uart.index = 0;
   
   input_uart_handle = FAPI_UART_Open(&uart, &res);
      
   return res;
}


/* 21b8dfbc - complete */
void input_thread(int a)
{
   int r13;
   struct Event_23 sp;
   struct Event_50 sp20;
   int sp28;

   sp.header.tag = 23; //14;
   sp.header.size = sizeof(struct Event_23);
   
   func_21b8961c(); //->device.c
   
   while (1)
   {
      rtos_wait(125);
      
#if 0
      if (Data_21f02a00 == 1)
      {
         r13 = func_21ba5714(&sp28) & 0xFFFF; //func_36f294(&sp28) & 0xFFFF; //->vfd.c
         
         if ((r13 == 0x80) && (sp28 != 0))
         {
            sp20.header.tag = 50; //32;
            sp20.header.size = sizeof(struct Event_50);
            
            EVT_Send(&sp20, "Factory Default");
         }
         else if (r13 != 0)
         {
            if (Data_21f03030 == 1)
            {
               func_21bbc518();
               func_21ba6930();
            }
            else
            {
               func_21bbc518();
               
               sp.wData_8 = r13;
               sp.Data_16 = 1;
               sp.Data_12 = 1;

               EVT_Send(&sp, "Frontpad input");
            }
         }
      }
#endif

      r13 = input_uart();
      if (r13 != 0)
      {
         if (Data_21f03030 == 1)
         {
            func_21bbc518();
            func_21ba6930();
         }
         else
         {
            func_21bbc518();
            
            sp.Data_16 = 2;
            sp.wData_8 = r13;
            sp.Data_12 = 1;

            EVT_Send(&sp, "Console input");
         }
      }

      func_21b0c17c(); //func_2dda20(); //->app_items.c
   } 
}


/* 21b8db84 - todo */
/* v3.8: func_35bad4 */
int input_uart(void)
{
   int r14 = 0;
   
   if (1 == FAPI_UART_CheckRxFifo(input_uart_handle))
   {
      int r13 = FAPI_UART_ReadByte(input_uart_handle);
      switch (r13 & 0xFF)
      {
         case '0':
            //21b8de14
            r14 = 48;
            break;

         case '1':
            r14 = 49;
            break;

         case '2':
            r14 = 50;
            break;

         case '3':
            r14 = 51;
            break;

         case '4':
            r14 = 52;
            break;

         case '5':
            r14 = 53;
            break;

         case '6':
            r14 = 54;
            break;

         case '7':
            r14 = 55;
            break;

         case '8':
            r14 = 56;
            break;

         case '9':
            //21b8de5c
            r14 = 57;
            break;
            
         case 'a': //97: 
            //21b8defc
            r14 = 131;
            break;
            
         case 'b': //98:
            //21b8df04
            r14 = 8; //back
            break;
            
         case 'c': //99:
            //21b8df0c
            r14 = 149;
            break;
            
         case 'h': //104:
            //21b8df30
            r14 = 133;                      
            break;
            
         case 'i': //105: 
            //21b8df38
            r14 = 138;
            break;
            
         case 'm': //109:
            //21b8df40
            r14 = 132; //menu?
            break;
            
         case 'n': //110:
            //21b8df48
            r14 = 134;
            break;
            
         case 'e': //101:
            //21b8df20
            r14 = 129; //exit?
            break;
            
         case 't': //116:
            //21b8df98
            r14 = 130;
            break;
            
         case 'v': //118:
            //21b8dfa0
            r14 = 135;
            break;

         case '+': //43:
            //21b8de04
            r14 = 136;
            break;
            
         case '-': //45:
            //21b8de0c
            r14 = 137;
            break;
            
         case '>': //62:
            //21b8de6c
            r14 = 141;
            break;
            
         case '<': //60:
            //21b8de64
            r14 = 142;
            break;
            
         case 'p': //112:
            //21b8df58   
            r14 = 148;
            break; 
            
         case 's': //115:
            //21b8df90
            r14 = 150;
            break;
            
         case 'o': //111:
            //21b8df50
            r14 = 151;
            break;
            
         case 'q': //113:
            //21b8df60
            r14 = 128;
            break;
            
         case '{': //123:
            //21b8dd9c
            r14 = 152;
            break;
            
         case '}': //125:
            //21b8ddfc
            r14 = 153;
            break;
            
         case 27: 
            //21b8dda4
            if ('[' == FAPI_UART_ReadByte(input_uart_handle))
            {
               unsigned short r13 = FAPI_UART_ReadByte(input_uart_handle);
               if (r13 == 'A') r14 = 146;
               else if (r13 == 'B') r14 = 147;
               else if (r13 == 'C') r14 = 144;
               else if (r13 == 'D') r14 = 145;
            }
            break;
            
         case 13:
            //21b8dd94
            r14 = 13; //ok
            break;
            
         case 'd': //100:
            //21b8df14
            famos_print_report();
            
#if 0
            func_21bc311c(); //->dev_usb.c //rtos_get_thread_status();
#endif
            break;
            
         case 'r': //114:
            //21b8df68
            FAPI_MMU_ReportMemory(FAPI_MMU_HeapHandleSys0, 0); 
            FAPI_MMU_ReportMemory(FAPI_MMU_HeapHandleSys1, 0);
            break;
            
         case 'x': //120:
            //21b8dd84
            func_21ba3e64(); //func_3595d8(); //->352dfc.c (EPG events)
            func_21b90968();
            break;
            
#if 0
         case '*': //42:
            //35BE18
            func_3941a8(); //->dev_usb.c
            break;
#endif
         
         case 'g': //103:
            //21b8df28
            r14 = 185; //func_2fed54();
            break;   
            
         case '(': //40:
            //21b8ddec
            r14 = 178;
            break;
            
         case ')': //41:
            //21b8ddf4
            r14 = 179;
            break;  
            
         case 'A': //65:
            //21b8de74
            r14 = 173;
            break;
            
         case 'B': //66:
            //21b8de7c
            r14 = 157;
            break;
            
         case 'C': //67:
            //21b8de84
            r14 = 183;
            break;
            
         case 'F': //70:
            //21b8de8c
            r14 = 169;
            break;
            
         case 'G': //71:
            //21b8de94
            r14 = 155;
            break;
            
         case 'J': //74:
            //21b8de9c
            r14 = 177;
            break;
            
         case 'L': //76:
            //21b8dea4
            r14 = 170;
            break;
            
         case 'M': //77:
            //21b8deac
            r14 = 140;
            break;
            
         case 'P': //80:
            //21b8deb4
            r14 = 184;
            break;
            
         case 'R': //82:
            //21b8debc
            r14 = 154;
            break;
            
         case 'S': //83:
            //21b8dec4
            r14 = 180;
            break;

         case 'T': //84:
            //21b8decc
            r14 = 181;
            break;
            
         case 'V': //86:
            //21b8ded4
            r14 = 182;
            break;
            
         case 'Y': //89:
            //21b8dedc
            r14 = 156;
            break;
            
         case '[': //91:
            //21b8dee4
            r14 = 167;
            break;
            
         case ']': //93:
            //21b8deec
            r14 = 168;
            break;
            
         case 96: //'´'
            //21b8def4
            r14 = 161;
            break;
      }
   }
   
   return r14;
}

#if 0

/* 21b8d9e0 - complete */
void input_rc(FAPI_SYS_HandleT handle, uint32_t* optDataPtr)
{
   uint64_t timeStamp;
   uint32_t rcmCode;
   int32_t deviceId = 0;
   uint32_t repeatFlag = 0;
   uint32_t keyValue = 0;
   struct Event_23 event;

   if (0 != FAPI_IR_GetKey(handle, &deviceId, &keyValue, &rcmCode, &repeatFlag))
   {
      wData_21f02a08 =  0;
      return;
   }

   FAPI_SYS_PRINT_DEBUG(3, "input_rc: deviceId=0x%x, keyValue=0x%x, rcmCode=0x%x, repeatFlag=0x%x\n",
         deviceId, keyValue, rcmCode, repeatFlag);

   if (Data_21f03030 == 1)
   {
      func_21bbc518();
      func_21ba6930();
   }
   else
   {
      func_21bbc518();

      timeStamp = FAPI_TIMER_GetTimeStamp(1000);

      if (repeatFlag == 0)
      {
         input_timeStamp = timeStamp;
         wData_21f02a08 = 0;

         event.header.tag = 23;
         event.header.size = sizeof(struct Event_23);
         event.wData_8 = rcmCode;
         event.Data_12 = 1;
         event.Data_16 = 0;
      }
      else
      {
         uint32_t r2;
         uint64_t maxTimeDiff, timeDiff;

         if (wData_21f02a08 == 0)
         {
            maxTimeDiff = wData_21f02a04;
         }
         else
         {
            maxTimeDiff = wData_21f02a06;
         }

         timeDiff = SYS_TimeDiff(input_timeStamp, timeStamp);

         if ((timeDiff < maxTimeDiff) || (timeDiff > wData_21f02a04*4))
         {
            wData_21f02a08 = 0;
            return;
         }

         input_timeStamp = timeStamp;

         wData_21f02a08++;

         r2 = rcmCode;

         if ((wData_21f02a08 % 2) == 0)
         {
            return;
         }

         event.header.tag = 23;
         event.header.size = sizeof(struct Event_23);
         event.wData_8 = r2; //rcmCode;
         event.Data_12 = 3;
         event.Data_16 = 0;
      }

      EVT_Send(&event, "RC input");
   }
}

#endif


