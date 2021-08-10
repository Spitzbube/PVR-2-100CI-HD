#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_


struct EventHeader
{
   uint32_t tag; //0
   uint16_t size; //4
   //8
};

struct Event
{
   struct EventHeader header; //0
#ifdef MB86H30
   union
   {
      int fill_8[14]; //8
   } payload; //8
   //64
#else
   unsigned short wData_8; //8
   int Data_12; //12
   int Data_16; //16
   union
   {
      int fill_20[27]; //20
   } payload; //20
   //128
#endif
};


struct Event_23
{
   struct EventHeader header; //0
   uint16_t wData_8; //8
   uint32_t Data_12; //12
   uint32_t Data_16; //16
   //20
};

struct Event_35
{
   struct EventHeader header; //0
   int Data_8; //8
   int Data_12; //12
   int Data_16; //16
   int Data_20; //20
   int Data_24; //24
   //28
};

struct Event_50
{
   struct EventHeader header; //0
   //8
};

struct Event_59
{
   struct EventHeader header; //0
#if 1
   USB_CONNECT_STATUS_S status; //8
#else
   int insertFlag; //8
   int Data_12; //12
   int Data_16; //16
#endif
   //20
};

extern int EVT_Init(void);
extern int EVT_Send(void* a, const char* name);
extern int EVT_Get(void* a, int b);



#endif /*EVENT_QUEUE_H_*/
