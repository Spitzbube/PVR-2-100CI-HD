


typedef struct
{
   void* pBuf; //0
   int bufSize; //4
   int flashDeviceType; //8
   int flashStartAddr; //12
   int32_t flashDeviceSize; //16
   int32_t flashSectorCount; //20
   uint16_t flashUsrStartSector; //24
   int powerOn; //28
   int Data_32; //32
   void* mutexId; //36
   struct
   {
      int inUse; //0
      int (*reqCb)(char); //4
      //8
   } arData_40[3];
   int fill_64[64]; //64
   //320
} SYS_DATA_S;

extern SYS_DATA_S* sysData; //21f02a10

#if 0 //Moved to sys_api.h

typedef uint32_t    SYS_EVTTYPE_E;

enum
{
    /* Core system messages */
    EVT_SERVICE_SELECTED = 0, //!< A new service has been selected.
    EVT_AUDIOSTREAM_CHANGED,  //!< A new audio stream has been selected.
    EVT_SPDIFSTREAM_CHANGED,  //!< A new SPDIF stream has been selected.
    EVT_IN_SYS_CHANGE,        //!< Input TV Standard has changed.
    EVT_OUT_SYS_CHANGE,       //!< Output TV Standard has changed.
    EVT_PIC_FORMAT_CHANGE,    //!< Input video frame format has changed.
    EVT_LOCK_STATUS,          //!< Frontend lock status has changed.
    EVT_PMT_RECEIVED,         //!< A PMT has been received.
    EVT_SDT_RECEIVED,         //!< A SDT has been received (live-TV only).
    EVT_NIT_RECEIVED,         //!< A NIT has been received (live-TV only).
    EVT_CAT_RECEIVED,         //!< A CAT has been received.
    EVT_TIME_UPDATE/*10*/,          //!< 10 New time and date is sent.
    EVT_TIMEZONE_CHANGE/*11*/,      //!< Timezone has changed (triggered by TOT).
    EVT_TIMESYNC_TIMEOUT,     //!< Requests the timer Menu to set Time manually
    EVT_TIMER_EXPIRED/*15*/,        //!< User timer has expired.
    EVT_SCAN_SERVICE_ADDED,   //!< A service has been found during scan.
    EVT_SCAN_CARRIER_ADDED,   //!< A carrier has been found and added to a list.
    EVT_SCAN_PROGRESS,        //!< Actual scanning progress is reported.
    EVT_EIT_UPDATE,           //!< New EIT current/next event is available.
    EVT_EIT_SCHED_UPDATE,     //!< New EIT schedule section received.
    EVT_SERVICE_RUNSTAT,      //!< Running status of curr. live service changed.
    EVT_SERVICE_NO_DECODING,  //!< 20 Timeout for start of decoding is expired
                              //!< (AV_SERVICE_DECODING_TIMEOUT).
    EVT_HDMI_CONNECTION,      //!< HDMI connection event occured.
    EVT_HDMI_HDCP_STATE,      //!< HDCP authenicate state of HDMI

    /* Configuration-dependant messages */
    EVT_KEY,                  //!< A remote control key has been pressed.

    EVT_PVR_PLAYBACK,         //!< PVR playback status has changed.
    EVT_PVR_RECORD,           //!< PVR record status has changed.
    EVT_PVR_PLAYEVENT,        //!< An event occurred during playback.
    EVT_PVR_RECEVENT,         //!< An event occurred during record.

    EVT_RECTIMER_LISTCHG,     //!< Record timer list has changed: a timer has
                              //!< been removed by system.
    EVT_RECTIMER_STATUS,      //!< Record timer has started or stopped.
    EVT_SLEEPTIMER_EXPIRED,   //!< Sleep timer has expired.

    EVT_FILL_32,
    EVT_FILL_33,
    EVT_FILL_34,

    EVT_CISYS_STATUSCHG, //35     //!< 30 Status of a CI slot has changed.
    EVT_CISYS_MMI, //36           //!< An MMI-related event occured.

    EVT_UPDATE_STATUS, //37       //!< Status of software update.

    EVT_FILEOP_STATUS,        //!< Actual file operation status/progress.

    EVT_DEVICE_STATUS,        //!< A device has been connected or removed.

    EVT_JPEG_FILEINFO,        //!< Information on JPEG file being decoded.

    EVT_JPEG_OSDINFO,         //!< information of osd layer for JPEG display
    EVT_JPEG_DECODEINFO,      //!< Information of picture decoding result.
    EVT_MP3_STATUS,           //!< Status of current MP3 playback.

    EVT_AFD_WSS_CHANGE_NOTIFY,   //!< (sys internal)

    EVT_MHEG_STATUS,          //!< 40 State change
    EVT_MHEG_OPEN,           //!< (sys internal)
    EVT_MHEG_CLOSE,           //!< (sys internal)
    EVT_MHEG_START,           //!< (sys internal)
    EVT_MHEG_STOP,            //!< (sys internal)
    EVT_MHEG_SHOW,            //!< (sys internal)
    EVT_MHEG_TUNE,            //!< (sys internal)
    EVT_MHEG_APPMMI_MSG,      //!< (sys internal)

    EVT_PCTRL_NEEDAUTH,       //!< Authorization requested by parental control
    EVT_PCTRL_NOMOREAUTH,     //!< Parental control authorization not needed
                              //!< (anymore).

    EVT_USB_CONNECT_STATUS = 59,   //!< Connection status of a usb device has
                              //!< changed.

    EVT_DEV_FORMAT_FINISH,    //!< Notify the application that the format
                              //!< process is finsihed

    EVT_MOTOR_MOVES,          //!< Notify the application that motor dish is moving.

    EVT_ETH_LINK,
    EVT_ETH_DHCP,
    EVT_ETH_DATA,

    EVT_TABLE_UPDATE,         //!<
    EVT_NUMS                  //!< end of system event area
};

typedef struct
{
    SYS_EVTTYPE_E       type;
    uint16_t            length;

} SYS_EVTHDR_S;

#endif

typedef struct
{
   void (*Func_0)(void); //0
   void* lock; //4
   void* request; //8
   void* response; //12
   void* Data_16; //16
   unsigned Data_20; //20
   int Data_24; //24
   //28
} Struct_35e444;


typedef struct
{
   unsigned a;
   unsigned b;   
} Struct_35e0a8;


struct Struct_21b8f67c;


extern struct appData* SYS_GetConfig(void);
extern int SYS_Start(struct appData* a);
extern void SYS_PrintString(char*);
extern uint64_t SYS_TimeDiff(uint64_t, uint64_t);
extern void* SYS_MemoryAllocate(int a);
extern void SYS_MemoryFree(void* a);
extern int SYS_ThreadCreate(void (*func)(int), 
                     int b, int prio, unsigned stacksize, 
                     const char* thread_name, void** thread);
extern int ITC_Init(Struct_35e444*, int*); //func_21b8ecf8
extern void* SYS_ReceiveRequestMessage(Struct_35e444*);
extern int SYS_SendResponseMessage(Struct_35e444*, void*);
extern int SYS_SendRequestMessage(Struct_35e444*, void*, int*);
extern int SYS_StrNCpy(char_t*, const char_t*, uint32_t);
extern void func_21b8e92c(Struct_35e0a8*, unsigned, unsigned);
extern int SYS_GetPowerOn(void);
extern int SYS_OnOff(int);




