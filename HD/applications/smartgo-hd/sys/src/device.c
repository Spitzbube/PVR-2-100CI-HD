
#include <string.h>
#include <fatfs/fatfs.h>
#include <fapi/sys_services.h>
#include <fapi/drv_timer.h>
#include "rtos/rtos.h"
#include "sys.h"
#include "sys_api.h"
#include "event_queue.h"
#include "device.h"



struct Struct_21b88d20
{
   uint32_t Data_0; //0
   uint32_t Data_4; //4
   char Data_8[72]; //8
   //80
};


struct Struct_21f029f0 
{
   void* semaphore; //0
   struct Struct_21f029f0_0
   {
      unsigned index; //0
      struct deviceParams Data_8; //4
#if 0
      int fill_28[23]; //24
#else
      struct Struct_21b88d20 Data_28; //28
      int fill_108; //108
      int fill_112; //112
      int fill_116; //116
#endif
      int Data_120; //116
      unsigned Data_124; //120
      unsigned Data_128; //124
      unsigned short useCount; //128
      int Data_136; //132
      int Data_140; //136
      int Data_144; //140
      char bData_148; //144
      int Data_152; //148
      //152
   } Data_0[4]; //4
   struct Struct_21f029f0_612
   {
      unsigned Data_0; //0
      char bData_4; //4
      int Data_8; //8
      int Data_12; //12
      //16
   } Data_612[10];
};

struct Struct_21f029f0* Data_21f029f0; //21f029f0



void device_thread(int);
int32_t func_21b86d54(uint16_t* yearPtr, 
                        uint16_t* monthPtr, 
                        uint16_t* dayPtr);
int32_t func_21b86cc4(uint16_t* hoursPtr, 
                     uint16_t* minutesPtr, 
                     uint16_t* secondsPtr);
static uint32_t getTaskId(void);
int func_21b887c0(struct Struct_21f029f0*, struct Struct_21f029f0_0*);
int func_21b882a8(struct Struct_21f029f0*, struct Struct_21f029f0_0*, signed char);



/* 21b898a4 - todo */
int device_init(void)
{
   FATFS_InitParamsT sp8;
   
   unsigned i, j = 0;
   int res = 0;
   
   if (Data_21f029f0 != 0)
   {
      FAPI_SYS_PRINT_MSG("ASSERTION FAILED at %s, l.%i\n",
            "sys/src/device.c", 192);
      
      rtos_halt(-1);      
   }
   
   Data_21f029f0 = SYS_MemoryAllocate(sizeof(struct Struct_21f029f0));
   
   if (Data_21f029f0 == 0)
   {
      return -10000002; //0xff67697e;
   }
   
   memset(Data_21f029f0, 0, sizeof(struct Struct_21f029f0));
   
   Data_21f029f0->semaphore = rtos_create_semaphore(1);
   
   if (Data_21f029f0->semaphore == 0)
   {
      return -10001010; //0xff67658e;
   }
      
   for (i = 0; i < 4; i++)
   {            
      struct Struct_21f029f0_0* p = &Data_21f029f0->Data_0[j++];
      
      Data_21f029f0->Data_0[i].Data_136 = -1;
      Data_21f029f0->Data_0[i].Data_120 = 0;
      Data_21f029f0->Data_0[i].Data_124 = 0;
      Data_21f029f0->Data_0[i].Data_128 = 0;
      Data_21f029f0->Data_0[i].Data_140 = 0;
      Data_21f029f0->Data_0[i].Data_144 = 0;      
      p->Data_152 = 0;      
   }
   
   for (i = 0; i < 10; i++)
   {
      Data_21f029f0->Data_612[i].Data_0 = 3;
      Data_21f029f0->Data_612[i].bData_4 = -1;      
      Data_21f029f0->Data_612[i].Data_8 = 0;
      Data_21f029f0->Data_612[i].Data_12 = 0;         
   }
   
   sp8.getDateFct = func_21b86d54;
   sp8.getTimeFct = func_21b86cc4;
   sp8.getTaskIdFct = getTaskId;
   
   res = FATFS_Init(&sp8);
   
   if (res != 0)
   {
      FAPI_SYS_PRINT_MSG("[HDD] FATFS_Init failed %ld\n", res);
      
      return res;
   }
   
   res = SYS_ThreadCreate(device_thread, 
         (int)Data_21f029f0, 35, 0x2000, "DeviceOp", 0);
   
   return res;
}


/* 21b86bb0 - complete */
int device_add(unsigned a, struct deviceParams* b)
{   
   unsigned lr;
   char r1;
   
   if (a >= 4)
   {
      return -10001005; //0xff676593;      
   }
   
   if (a == 3)
   {
      return -10000001; //0xff67697f;      
   }
   
   struct Struct_21f029f0_0* r7 = &Data_21f029f0->Data_0[a];
   
   if (r7->Data_120 != 0)
   {
      return -10000001; //0xff67697f;      
   }
      
   memset(r7, 0, sizeof(struct Struct_21f029f0_0));
   
//   memcpy(&r7->Data_8, b, sizeof(struct deviceParams));     
   r7->Data_8 = *b;
   r7->index = a;
   r7->Data_120 = 1;
   r7->Data_124 = 0;
   r7->Data_128 = 0;
   r7->Data_136 = -1;
      
   for (lr = 0, r1 = 0; lr < 10; lr++)
   {   
      if (r1 > 3) break;

      if (Data_21f029f0->Data_612[lr].Data_0 == 3)
      {
         Data_21f029f0->Data_612[lr].Data_0 = a;
         Data_21f029f0->Data_612[lr].bData_4 = r1++;         
         Data_21f029f0->Data_612[lr].Data_8 = 0;
         Data_21f029f0->Data_612[lr].Data_12 = 0;         
      }      
   }
   
   return 0;
}


/* 21b8961c - complete */
int func_21b8961c(void)
{   
   if (Data_21f029f0 == 0)
   {
      FAPI_SYS_PRINT_MSG("ASSERTION FAILED at %s, l.%i\n",
            "sys/src/device.c", 394);
      
      rtos_halt(-1);
   } 
   
   return func_21c9de38();
}


/* 21b89678 - todo */
void device_thread(int a)
{
   struct Event_35 sp8;
   
   struct Struct_21f029f0* r6 = (void*) a;
   
   func_21b8961c();
   
   sp8.header.tag = 35;
   sp8.header.size = sizeof(sp8);
      
   while (1)
   {
      //21b8969c
      unsigned r7;
      for (r7 = 0; r7 < 4; r7++)
      {
         //21b896c4
         struct Struct_21f029f0_0* sp = &r6->Data_0[r7];
         unsigned fp = 0;
         char sp4 = 0;
            
         rtos_request_semaphore(r6->semaphore, -1);
         
         if (sp->Data_152 != 0)
         {
            fp = sp->Data_144;
            sp4 = sp->bData_148;
         }
         
         rtos_release_semaphore(r6->semaphore, 0);
         
         if (sp->Data_152 != 0)
         {
            device_set_use(sp->index, 1);
            
            int r8 = sp->Data_128;
            
            switch (fp)
            {
            case 1:
               //21b89844
               r8 = func_21b882a8(r6, sp, sp4);
               
               FAPI_SYS_PRINT_MSG("[DEV] Mount dev %i, new mount status: %i\n",
                     sp->index, r8);
               
               if (sp->index == 1)
               {
                  if (r8 == 5)
                  {
                     func_21bc6c8c();
                  }
               }
               //->21b8977c
               break;
               
            case 2:
               //21b8980c
               func_21bc2464();
               
               r8 = func_21b887c0(r6, sp);
               
               FAPI_SYS_PRINT_MSG("[DEV] Format dev %i, new mount status: %i\n", 
                     sp->index, r8);
               //->21b8977c
               break;
            }
            //21b8977c
            device_set_use(sp->index, 0);
   
            if (r8 == sp->Data_128)
            {
               sp->Data_152 = 0;
               //->21b89804
            }         
            else
            {
               //21b897bc
               sp->Data_152 = 0;
   
               sp8.Data_8 = sp->index;
               sp8.Data_12 = sp->Data_124;
               sp8.Data_16 = r8;
               sp8.Data_20 = (sp->useCount != 0)? 1: 0;
                           
               EVT_Send(&sp8, "DevStat");
            }
         }
         //21b89804
      } //for (r7 = 0; r7 < 4; r7++)

      FAPI_SYS_SLEEP(200);
      //->21b8969c      
   } //while (1)
}


/* 21b86d54 - todo */
int32_t func_21b86d54(uint16_t* yearPtr, 
                     uint16_t* monthPtr, 
                     uint16_t* dayPtr)
{
   printf("21b86d54\n");
   
   return 0;
}


/* 21b86cc4 - todo */
int32_t func_21b86cc4(uint16_t* hoursPtr, 
                     uint16_t* minutesPtr, 
                     uint16_t* secondsPtr)
{
   printf("21b86cc4\n");   
   
   return 0;
}


/* 21b86bac - complete */
uint32_t getTaskId(void)
{
//   printf("getTaskId\n");
   
   return rtos_get_current_thread();
}


/* 21b867ec - complete */
struct Struct_21f029f0_0* getDevice(unsigned a, int* b)
{
   struct Struct_21f029f0_0* p = 0;
   int res = 0;
   
   if (a > 3)
   {
      res = -10000001; //0xff67697f;      
   }
   else
   {
      if (Data_21f029f0->Data_0[a].Data_120 == 0)
      {
         res = -10001001; //0xff676597;
      }
      else
      {
         p = &Data_21f029f0->Data_0[a];
      }
      
   }
   
   if (b != 0)
   {
      *b = res;
   }
   
   return p;
}


/* 21b87f0c - complete */
int device_set_use(unsigned a, unsigned b)
{
   int res = 0;
   
   struct Struct_21f029f0_0* r4 = getDevice(a, &res);
   
   if (r4 != 0)
   {
      rtos_request_semaphore(Data_21f029f0->semaphore, -1);
      
      if (r4->Data_124 != 1)
      {
         res = -10001001; //0xff676597;
      }
      else
      {
         if (b != 0)
         {
            r4->useCount++;
         }
         else if (r4->useCount > 0)
         {
            r4->useCount--;
         }
      }
      
      rtos_release_semaphore(Data_21f029f0->semaphore, 0);
   }

   return res;
}


/* func_21b887c0 - todo */
int func_21b887c0(struct Struct_21f029f0* a, struct Struct_21f029f0_0* b)
{
   printf("func_21b887c0\n");
   
   return 0;
}


/* 21b882a8 - todo */
int func_21b882a8(struct Struct_21f029f0* a, struct Struct_21f029f0_0* b, signed char c)
{
   printf("func_21b882a8\n");
   
   return 0;
}


/* 21b868f8 - todo */
void func_21b868f8(struct Struct_21f029f0_0* a, struct Struct_21b8818c* b)
{
//   printf("func_21b868f8\n");
   
   b->Data_0 = a->Data_124;
   b->Data_4 = a->Data_128;   
   b->Data_8 = (a->useCount != 0)? 1: 0;
}


/* 21b8818c - complete */
int func_21b8818c(unsigned a, struct Struct_21b8818c* b)
{
   int res = 0;
   
   struct Struct_21f029f0_0* r5 = getDevice(a, &res);

   if (r5 != 0)
   {
      rtos_request_semaphore(Data_21f029f0->semaphore, -1);

      func_21b868f8(r5, b);
      
      rtos_release_semaphore(Data_21f029f0->semaphore, 0);
   }
   
   return res;
}


/* 21b86858 - complete */
int func_21b86858(unsigned a)
{
   int res = 0;
   
   struct Struct_21f029f0_0* r0 = getDevice(a, &res);
   
   if (r0 != 0)
   {
      if (r0->Data_8.Data_12 != 0)
      {
         res = (r0->Data_8.Data_12)(r0->index);
      }
   }

   return res;
}


/* 21b8895c - complete */
int SYS_DeviceUnmount(struct Struct_21f029f0* r8, struct Struct_21f029f0_0* r5)
{
   unsigned long long r6_r7, r1_r0;
   int res = 0;
   
   FAPI_SYS_PRINT_MSG("[DEV] SYS_DeviceUnmount: dev %i\n", 
         r5->index);
   
   if (r5->Data_124 != 1)
   {
      return -10001001; //0xff676597;
   }
   
   if ((r5->Data_128 > 1) && (r5->Data_128 < 4))
   {
      rtos_release_semaphore(Data_21f029f0->semaphore, 0);
      
      r6_r7 = FAPI_TIMER_GetTimeStamp(1000);

      while (1)
      {
         if (r5->Data_152 == 0) 
         {
            break;
         }
         
         r1_r0 = SYS_TimeDiff(r6_r7, FAPI_TIMER_GetTimeStamp(1000));
         
         if (r1_r0 >= 15000)
         {
            r5->Data_140 = 1;
            
            rtos_request_semaphore(Data_21f029f0->semaphore, -1);
            
            return 0xff67697a;
         }
         
         FAPI_SYS_SLEEP(100);
      }

      r5->Data_140 = 1;
      
      rtos_request_semaphore(Data_21f029f0->semaphore, -1);
   }

   if (r5->Data_136 < 0)
   {
      return 0;
   }

   rtos_release_semaphore(Data_21f029f0->semaphore, 0);

   func_21bb8290(r5->index);
   
   rtos_request_semaphore(Data_21f029f0->semaphore, -1);
   
   res = func_21ca2d6c(r5->Data_136);
   
   if (r5->index == 1)
   {
      if (res == 0)
      {
         func_21ba53c0(55680, 0);
      }
   }

   if (res == 0)
   {
      r5->Data_128 = 0;
   }
   else
   {
      r5->Data_128 = 1;
      res = -10001002; //0xff676596;
   }

   r8->Data_612[r5->Data_136].Data_12 = 0;
   r5->Data_136 = -1;
      
   return res;
}


/* 21b866e0 - todo */
int SYS_DeviceWakeup(void)
{
   unsigned i, j; 
   
   FAPI_SYS_PRINT_MSG("[DEV] SYS_DeviceWakeup\n");
   
   for (i = 0; i < 4; i++)
   {
      j = i;
            
      if (Data_21f029f0->Data_0[i].Data_8.Data_4 != 0)
      {
//         printf("SYS_DeviceWakeup: i=%d\n", i);
         
         (Data_21f029f0->Data_0[i].Data_8.Data_4)(j);
      }
   }
   
   return 0;
}

/* 21b88d20 - todo */
int SYS_DeviceSetAvailability(uint32_t r5, int r4, int r7, struct Struct_21b88d20* r6)
{
   int res = 0;
   
   FAPI_SYS_PRINT_MSG("[DEV] Set availability: device %i, available %i \n",
         r5, r4);
   
   if (r5 > 3)
   {
      return -10000001; //0xff67697f;
   }

   if ((r4 != 0) && (r6 == 0))
   {
      return -10000001; //0xff67697f;
   }
   
   rtos_request_semaphore(Data_21f029f0->semaphore, -1);

   if (Data_21f029f0->Data_0[r5].Data_120 == 0)
   {
      res = 0xff676597;
   }
   else
   {
      if ((r4 == 0) || 
            ((Data_21f029f0->Data_0[r5].Data_124-1) > 1))
      {
         //21b88df8
         struct Struct_21f029f0_0* sp12 = &Data_21f029f0->Data_0[r5];
         
         if (r4 != 0)
         {
            //21b88ef8
            memcpy(&sp12->Data_28, r6, sizeof(struct Struct_21b88d20));
            
            unsigned r2 = ((sp12->Data_28.Data_0 % 1024) * 
                  sp12->Data_28.Data_4) % 1024;
            
            func_21b8e92c(&sp12->fill_108, 0, r2);
         }
         //21b88e10
      }
      //21b88ef0
      //res = 0;
   }
   //21b88ed8
   rtos_release_semaphore(Data_21f029f0->semaphore, 0);

   return res;
}


/* 21b88b2c - todo */
void SYS_DeviceSetExternalAccess(int a, int b)
{
   FAPI_SYS_PRINT_MSG("SYS_DeviceSetExternalAccess: TODO\n");
}




