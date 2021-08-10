
#include <string.h>
#include <fapi/sys_services.h>
#if 0
#include "fapi_adapter.h"
#include "rtos.h"
#endif
#include "sys.h"
#include "sys_api.h"
#include "event_queue.h"


#define MAX_EVENTS 96


struct event_queue
{
   struct Event* queue; //0   
   void* mbox; //4   
   signed short writePos; //8 
   signed short readPos; //10
   unsigned short size; //12
#ifdef MB86H30
   int Data_16; //16
   int Data_20; //20
   //24
#endif
   //16
};


static struct event_queue* event_queue; //21f029fc


/* 21b8d3c0 - complete */
int EVT_Init(void)
{
   if (event_queue != 0)
   {
      FAPI_SYS_PRINT_MSG("ASSERTION FAILED at %s, l.%i\n",
         "sys/src/event_queue.c",
         75);
         
      rtos_halt(-1);
   } 
   
   event_queue = SYS_MemoryAllocate(sizeof(struct event_queue));
   if (event_queue == 0)
   {
      return 0xff67697e;
   } 
   
   event_queue->queue = 0;
   event_queue->mbox = 0;
   event_queue->readPos = 0;
   event_queue->writePos = 0;
   event_queue->size = 0;
#ifdef MB86H30
   event_queue->Data_16 = 0;
   event_queue->Data_20 = 0;
#endif
   
   event_queue->queue = 
      SYS_MemoryAllocate(sizeof(struct Event) * MAX_EVENTS); //0x3000
   
   if (event_queue->queue == 0)
   {
      return 0xff67697e;
   }
   
   event_queue->mbox = rtos_mailbox_create(-1);
   if (event_queue->mbox == 0)
   {
      return 0xff67658e;
   } 
   
   return 0;   
}


/* 3598c0 - nearly complete */
int EVT_Send(void* a, const char* name)
{
   unsigned irqFlags;
   struct Event* event = a;
   int res = 0;
   
   if (event_queue == 0)
   {
      //21b8d324
      FAPI_SYS_PRINT_MSG("ASSERTION FAILED at %s, l.%i\n",
         "sys/src/event_queue.c",
         109);
         
      rtos_halt(-1);         
      
      if (event == 0)
      {
         //21b8d354
         return 0xff67697f;
      }
   }
   
   if (event == 0)
   {
      //21b8d354
      return 0xff67697f;
   }
   
   //21b8d160
   if (event->header.size > sizeof(struct Event))
   {
      //21b8d35c
      FAPI_SYS_PRINT_MSG("[SYS] Event %i: size too large (%i > %i)\n",
         event->header.tag, 
         event->header.size, 
         sizeof(struct Event));
      
      return 0xff67697f;
   } 

#ifdef MB86H30
   if (((event_queue->Data_20 != 0) || (event_queue->Data_16 != 0)) &&
      ((event->header.tag == 14) || (event->header.tag == 8)))
   {
      return 0;
   }
#endif
   
   irqFlags = rtos_save_flags_and_cli(); 
      
   if (event_queue->size < MAX_EVENTS)
   {
      //21b8d1f0
      if (event->header.tag == 23)
      {
         unsigned i;         
         for (i = 0; i < event_queue->size; i++)
         {
            struct Event* e;            
            e = &event_queue->queue[(event_queue->readPos + i) % MAX_EVENTS];
            
            if ((e->header.tag == 23) &&
                  (event->wData_8 == e->wData_8) &&
                  (event->Data_12 == e->Data_12) &&
                  (event->Data_16 == e->Data_16))
            {
               //21b8d2fc
               rtos_restore_flags(irqFlags);
               
               FAPI_SYS_PRINT_MSG("Event key exist!\n");
               
               return 0xff67697f;
            }
            //21b8d290
         }
      }
      //21b8d1fc
      memcpy(&event_queue->queue[event_queue->writePos],
            event, event->header.size);
         
      event_queue->writePos = (event_queue->writePos + 1) % MAX_EVENTS;      
      event_queue->size++;
   }
   else 
   {
      //21b8d184
      if (name != 0)
      {
         //21b8d18c
         FAPI_SYS_PRINT_MSG("[");
         FAPI_SYS_PRINT_MSG(name);
         FAPI_SYS_PRINT_MSG("] ");
      }
      //21b8d384
      FAPI_SYS_PRINT_MSG("Event queue overflow!\n");
            
      res = 0xff676593;
      //rtos_restore_flags(irqFlags);
      //return 0xff676593;
   }
   
   rtos_restore_flags(irqFlags); 
   
   if (res == 0)
   {
#ifdef MB86H30
      rtos_mailbox_send(event_queue->mbox, 1);
#else
      int sp;
      rtos_mailbox_send(event_queue->mbox, &sp, 0);
#endif
   }
   
   return res;
}


/* 21b8cfec - complete */
int EVT_Get(void* a, int b)
{
   struct Event* event;
   struct
   {
      int a; //0
   }* r8 = a;
   
   int res = 0;
   unsigned irqFlags = rtos_save_flags_and_cli();
   
   while (1)
   {
      if (event_queue->size <= 0)
      {
         if (b == 1)
         {
            rtos_restore_flags(irqFlags);

#ifdef MB86H30
            if (0 == rtos_mailbox_receive(event_queue->mbox, 1))
            {
               res = ((104 << 16) | 11);
            }
#else
            int sp;
            if (0 == rtos_mailbox_receive(event_queue->mbox, &sp, -1))
            {
               res = 0xff67658b;
            }
#endif

            irqFlags = rtos_save_flags_and_cli();
         } 
         else
         {
            res = 0xff676592;
            break;
         } 
      } 
      else
      {
         break;
      }       
   } 

   if (res == 0)
   {
      event = &event_queue->queue[event_queue->readPos];
      
      if (event->header.size > sizeof(struct Event))
      {
         FAPI_SYS_PRINT_MSG("ASSERTION FAILED at %s, l.%i\n",
            "sys/src/event_queue.c",
            227);
            
         rtos_halt(-1);
      } 
      
      memcpy(r8, event, event->header.size);
      
      event_queue->readPos = (event_queue->readPos + 1) % MAX_EVENTS;
      event_queue->size--;
   } 

   rtos_restore_flags(irqFlags);
   
   return res;   
}

