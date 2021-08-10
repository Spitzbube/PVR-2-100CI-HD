

#include <fapi/sys_services.h>
#if 0
#include <assert.h>
#include "rtos.h"
#include "fapi_adapter.h"
#endif
#include "app_main.h"
#include "device.h"
#include "event_queue.h"
#include "sys.h"

extern struct appData sysConfig; //21f7be58


int SYS_HandleEvent(int a, void* b);


/* 21b8fd08 - complete */
void ApplicationThread(int a)
{
   int retVal;
   struct Event event;
   
   func_21b8961c(); //->device.c
   
   if (sysConfig.init != 0)
   {
      retVal = (sysConfig.init)();
      
      FAPI_SYS_PRINT_MSG("\n %s %d retVal=%x  ",
            "sys/src/main.c", 1641, retVal);
      
      if (retVal != 0)
      {
         FAPI_SYS_PRINT_MSG("Application initialisation failed: %d!\n",
               retVal);
         
         rtos_halt(-1);
      }
   }

   // The main application loop
   while (1)
   {
      if (0 != EVT_Get(&event, 1))
      {
         FAPI_SYS_PRINT_MSG("EVT_Get: ERROR \n");
      }
      else
      {
         if (0 != SYS_HandleEvent(event.header.tag, &event))
         {
            if (sysConfig.handleEvent != 0)
            {
               (sysConfig.handleEvent)(event.header.tag, &event);
            }
         }
      }
   }
}


/* 21b8fa28 - todo */
int SYS_HandleEvent(int a, void* b)
{
#if 1
    if (a != 10)
    {
        FAPI_SYS_PRINT_MSG("SYS_HandleEvent() a=%d\n", a);
    }
#endif
   
   switch (a)
   {
   case EVT_USB_CONNECT_STATUS: //59: //"USB"
      //21b8fc00
      USB_ConnectStatusHandling(b);
      //->21b8fb48
      return 1;
      //break;
      
   default:
      //->21b8fb44
      break;
   }
   //21b8fb44
   return 1;
}

