

#include <fapi/sys_services.h>
#include <fapi/drv_timer.h>
#include "sys.h"
#include "sys_priv.h"
#include "flash.h"
#include "sys_time.h"

#if 0
typedef struct //__attribute((packed))
{
   uint8_t hour; //0
   uint8_t min; //1
   uint8_t sec; //2
   //3   
} Time;

typedef struct 
{
   unsigned short year; //0
   char mon; //2
   char day; //3
   int32_t weekday; //4
   //8
} Date;
#endif

#define TIME_SINGLETIMERS_MAX  8
#define TIME_PERIODTIMERS_MAX  2

typedef struct 
{
   FAPI_SYS_HandleT sysTimeHandle; //0
   FAPI_SYS_HandleT timerHandle[4]; //4
   FAPI_SYS_HandleT arData_24[2]; //0x14
#if 0
   int syncTimestamp; //32
   int needSync; //36
   int Data_40; //40
   int Data_44; //44
   int Data_48; //48
#endif
   int Data_0x1c; //44
   uint32_t Data_0x20; //48
   int Data_0x24; //52
   unsigned short milliSeconds; //56
   Time time; //60
//   unsigned short fill_58; //58
   Date date; //64
   int Data_68; //0x38
   int Data_0x3c; //76
   Time Data_0x40; // __attribute((aligned (2))); //0x40
   Date Data_0x44; //0x44
   unsigned short wData_0x4c; //0x4c
   //0x50
} TIME_DATA_S;


//0x5f914c
static TIME_DATA_S* /*volatile*/ timeDat = 0; //21f02e44
int Data_21f02e48; //21f02e48


/* 21ba2970 - todo */
void func_21823938(uint64_t value, uint32_t optData)
{
   struct
   {
      struct
      {
         int Data_0; //0
         unsigned short wData_4; //4
      } header;
      //unsigned short fill_6; //6
      union
      {
         struct
         {
            Time time; //8
            Date date; //12
         }s;
      }u;
   } event;
   
   struct
   {
      int Data_0; //0
      unsigned short wData_4; //4
      unsigned short fill_6; //6
      unsigned short wData_8; //8
   } event1;
   
   //volatile Time fp24;   
   //Time fp28;   
   //Date fp36;
   
   uint32_t days = 0;
   
   /*volatile*/ Time fp24 = timeDat->time;
   
   timeDat->Data_68 += 2;
   timeDat->milliSeconds += 200;
   
   while (timeDat->milliSeconds >= 1000)
   {
      timeDat->time.sec++;
      
      timeDat->milliSeconds -= 1000;
      
      while (timeDat->time.sec >= 60)
      {
         timeDat->time.min++;
         
         timeDat->time.sec -= 60;
         
         while (timeDat->time.min >= 60)
         {
            timeDat->time.hour++;
            
            timeDat->time.min -= 60;
         }
      }
   }
   
   if (timeDat->time.hour >= 24)
   {
      timeDat->time.hour -= 24;
      
      timeDat->date.weekday++;
      
      if (timeDat->date.weekday > 7)
      {
         timeDat->date.weekday = 1;
      } 
      
      timeDat->date.day++;
      
      days = time_get_days_per_month(timeDat->date.mon,
                        timeDat->date.year);
                        
      if (timeDat->date.day > days)
      {
         timeDat->date.day -= days;
         
         timeDat->date.mon++;
         
         if (timeDat->date.mon > 12)
         {
            timeDat->date.mon -= 12;
            
            timeDat->date.year++;
         }
      }
   }
   
   if ((timeDat->Data_0x3c != 0) &&
         (0 == time_compare(&timeDat->Data_0x40, &timeDat->Data_0x44,
               &timeDat->time, &timeDat->date)))
   {
      event1.Data_0 = 11;
      event1.wData_4 = 12;
      event1.wData_8 = timeDat->wData_0x4c;
      
      EVT_Send(&event1, "Timezone update");

      timeDat->Data_0x1c/*Data_40*/ = 1;
      timeDat->Data_0x3c = 0;
   }
  
   if ((timeDat->Data_0x1c/*Data_40*/ == 1) ||
      (fp24.sec != timeDat->time.sec) ||
      (fp24.min != timeDat->time.min) ||
      (fp24.hour != timeDat->time.hour))
   {
      //36e384
      Time fp28 = timeDat->time;
      Date fp36 = timeDat->date;
      
      func_2182384c(&fp28, &fp36);
      
      event.header.Data_0 = 10;
      event.header.wData_4 = sizeof(event.header) + sizeof(event.u.s); // 20;
      //memcpy(&event.u.s.time, &fp28, sizeof(Time));
      event.u.s.time = fp28;      
      //memcpy(&event.u.s.date, &fp36, sizeof(Date));
      event.u.s.date = fp36;
      
      EVT_Send(&event, "Time update");
      
      timeDat->Data_0x1c/*Data_40*/ = 0;
   }

#if 0
   if (timeDat->needSync == 0)
   {
      if (SYS_Diff(timeDat->syncTimestamp, FD_TIMER_ReadTimerStamp()) >
         120000)
      {
         timeDat->needSync = 1;
      } 
   }
#endif
}


/* 21ba3224 - todo */
int32_t TIME_Init(void)
{
   int32_t res;
   int sp0x2c;
   FAPI_TIMER_OpenParamsT timerParams;
   struct 
   {
      int fill_0[2]; //0
      Time time; //8
      Date date; //12
      int fill_1; //20
      int fill_2; //24
      //28
   } sp56;
   
   if (timeDat != 0)
   {
      FAPI_SYS_PRINT_MSG("ASSERTION FAILED at %s, l.%i\n",
         "sys/src/time.c", 144);
         
      rtos_halt(-1);
   } 
   
   timeDat = SYS_MemoryAllocate(sizeof(TIME_DATA_S));
   if (timeDat == 0)
   {
      return -10000002;
   }
   
   memset(timeDat, 0, sizeof(TIME_DATA_S));
   
   timeDat->time.hour = 0;
   timeDat->time.min = 0;
   timeDat->time.sec = 0;
   timeDat->date.year = 2009;
   timeDat->date.mon = 8; //1;
   timeDat->date.day = 1;
   timeDat->date.weekday = 6;
#if 0
   timeDat->syncTimestamp = 0;
   timeDat->needSync = 1;
   timeDat->Data_40 = 1;
   timeDat->Data_44 = 0;
   timeDat->Data_0 = 0;
   timeDat->Data_48 = 0;
#endif

   timeDat->Data_0x1c = 1;
   timeDat->Data_0x20 = 0;
   timeDat->milliSeconds = 0;

   func_2181be04(&sp0x2c);
   
   timeDat->Data_0x24 = sp0x2c;
   timeDat->sysTimeHandle = 0;
   timeDat->Data_0x3c = 0;
   
   memset(&timerParams, 0, sizeof(FAPI_TIMER_OpenParamsT));
   
   timerParams.version = FAPI_TIMER_VERSION;
   timerParams.type_ = FAPI_TIMER_TYPE_SOFT;
   timerParams.mode = FAPI_TIMER_MODE_CONTINUOUS;
   timerParams.resolution = 100000;
   timerParams.multiplier = 2;
   timerParams.callback1 = func_21823938;
   timerParams.exec1InIsr = 0;

   timeDat->sysTimeHandle = FAPI_TIMER_Open(&timerParams, &res);
   
   if (timeDat->sysTimeHandle == 0)
   {
      return -10000001;
   }

   FAPI_TIMER_Start(timeDat->sysTimeHandle);

   if (28 == SYS_FlashReadFcData(&sp56))
   {
      FAPI_SYS_PRINT_MSG(" y=%x m=%x d=%x h=%x m=%x s=%x \n\015",
            sp56.date.year,
            sp56.date.mon,
            sp56.date.day,
            sp56.time.hour,
            sp56.time.min,
            sp56.time.sec);
      
      if ((sp56.date.mon <= 12) &&
            (sp56.time.hour <= 23) &&
            (sp56.time.min <= 59) &&
            (sp56.time.sec <= 59))
      {
         func_21ba2f64(&sp56.time, &sp56.date);
      }
      
      Data_21f02e48 = sp56.fill_1;
   }
   
   return 0;
}


/* 21ba1dcc - todo */
int32_t time_get_current(Time* a, Date* b, uint32_t* c)
{
//   int32_t res = 0;
   
   if (timeDat == 0)
   {
      return -10000004;
   }
   
   //*a = timeDat->time;
   memcpy(a, &timeDat->time, sizeof(Time));
   
#if 1
   if (c != 0)
   {
      *c = timeDat->Data_0x20; //Data_44;
   } 
#endif
      
#if 1
   if (b != 0)
   {
      //*b = timeDat->date;
      memcpy(b, &timeDat->date, sizeof(Date));
   }
#endif
   
//   return res;
   return 0;
}


/* 21ba28a4 - complete */
void TIME_TimerCb(uint64_t value, uint32_t optData)
{
    SYS_EVT_TIMER_S     ctrlEvent;

    /* generate event */
    ctrlEvent.hdr.type   = 15; //EVT_TIMER_EXPIRED;
    ctrlEvent.hdr.length = sizeof(ctrlEvent);
    ctrlEvent.type       = (SYS_TIMER_TYPE_E)(optData >> 8);
    ctrlEvent.num        = optData & 0xFF;

    if (ctrlEvent.type == TIMER_SINGLE)
    {
        FAPI_TIMER_Close(timeDat->timerHandle[ctrlEvent.num]);

        timeDat->timerHandle[ctrlEvent.num] = 0;
    }

    (void)EVT_Send(&ctrlEvent, "Timer event");
}


/* 21ba2d48 - complete */
int32_t TIME_SingleTimerLaunch(uint8_t timerNum, uint16_t timeInDeciSecs)
{
    FAPI_TIMER_OpenParamsT  timerParams;
    int32_t                 retVal;

    if( timeDat == NULL )
    {
        return -0x989684; //APPL_SMARTGO_ERR_NOT_INITIALIZED;
    }
    if( timerNum >= TIME_SINGLETIMERS_MAX )
    {
        return -0x989681; //APPL_SMARTGO_ERR_BAD_PARAMETER;
    }

    if(timeDat->timerHandle[timerNum] != NULL)
    {
        (void)TIME_SingleTimerAbort(timerNum);
    }

    memset(&timerParams, 0, sizeof(timerParams));

    timerParams.version    = FAPI_TIMER_VERSION;
    timerParams.type_       = FAPI_TIMER_TYPE_SOFT;
    timerParams.mode       = FAPI_TIMER_MODE_ONESHOT;
    timerParams.resolution = FAPI_TIMER_RESOLUTION_100_MSEC;
    timerParams.multiplier = timeInDeciSecs;
    timerParams.callback1  = TIME_TimerCb;
    timerParams.optData1   = (TIMER_SINGLE << 8) | timerNum;
    timerParams.exec1InIsr = 0;

    timeDat->timerHandle[timerNum] = FAPI_TIMER_Open(&timerParams, &retVal);
    if(timeDat->timerHandle[timerNum] != NULL)
    {
        retVal = FAPI_TIMER_Start(timeDat->timerHandle[timerNum]);
    }
    else
    {
        retVal = FAPI_TIMER_Close(timeDat->timerHandle[timerNum]);
    }

    return (retVal);
}


/* 21ba27b4 - complete */
int32_t TIME_SingleTimerAbort(uint8_t timerNum)
{
    int32_t retVal;

    if(timeDat == NULL)
    {
        return -0x989684; //APPL_SMARTGO_ERR_NOT_INITIALIZED;
    }

    if(timerNum >= TIME_SINGLETIMERS_MAX)
    {
        return -0x989681; //APPL_SMARTGO_ERR_BAD_PARAMETER;
    }

    if(timeDat->timerHandle[timerNum] == NULL)
    {
        return FAPI_OK;
    }
    /* stop timer */
    retVal = FAPI_TIMER_Close(timeDat->timerHandle[timerNum]);
    timeDat->timerHandle[timerNum] = NULL;

    return retVal;
}

