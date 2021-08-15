#ifndef MUSB_H_
#define MUSB_H_


struct Struct_49d2fc_t;
struct MGC_DmaControllerFactory;
struct MGC_DmaController;
struct MGC;
struct MGC_Pipe;
struct Struct_48b494_Inner1_t;
struct Struct_49d604;
struct Struct_49d2fc_Inner1_3_2_t;


typedef struct Struct_49d2fc_Inner1_3_2_t
{
   MUSB_DeviceDriver* pDeviceDriver; //0
   void* b; //4
   MUSB_LinkedList c; //8
   MUSB_Device d; //20
   int fill_84; //84
   char bData_88; //88
   //92    
} Struct_49d2fc_Inner1_3_2;

typedef struct
{
   void* pUrb; //0  
   char bTag; //4
   char bEnd; //5
   char bStatus; //6   
} MGC_Message;



typedef struct 
{
   char bEnd; //0 -96
   char bIsSharedFifo; //1 -95 
   unsigned short wMaxPacketSizeTx; //2 -94
   unsigned short wMaxPacketSizeRx; //4 -92
   char bData_6; //6 -90
   char bTxRemoteEnd; //7 -89
   char bData_8; //8 -88
   char bTxTrafficType; //9 -87
   char bData_10; //10 -86
   char bRxRemoteEnd; //11 -85
   char bData_12; //12 -84
   char bRxTrafficType; //13 -83  
   char bData_14; //14 -82  
   uint8_t halted_15; //15 -81
   char g; //16 -80
   char h; //17 -79
   char bData_18; //18 -78
   char bData_19; //19 -77
   unsigned short wTxPacketSize; //20
   unsigned short wRxPacketSize; //22
   int dwRequestSize; //24
   /*void*/MUSB_Irp* pCurrentTxUrb; //28
   MUSB_LinkedList tx_urb_list; //32
   int dwOffset; //44
   /*void*/MUSB_Irp* pCurrentRxUrb; //48
   MUSB_LinkedList rx_urb_list; //52
   int Data_64; //64
   MGC_DmaChannel* pTxDmaChannel; //68
   MGC_DmaChannel* pRxDmaChannel; //72
   int fill_76; //76
   char bData_80; //80
   char bData_81; //81
   int fill_84; //84
   int Data_88; //88
   unsigned short fill_92; //92
   char bData_94; //94
   char bData_95; //95
   //96
} MGC_Endpoint;


typedef struct MGC_Pipe
{
   struct MGC* Data_0; //0  
   MGC_Endpoint* pEndpoint; //4
   struct MGC* Data_8; //8 
   MUSB_LinkedList* Data_12; //12
   MUSB_Device* Data_16; //16
   unsigned short wMaxPacketSize; //20
   int Data_24; //24
   int Data_28; //28
   char bTransferType; //32
   //36
} MGC_Pipe;


typedef struct
{
   void (*Func_76)(void* a, void* b); //0 76
   MUSB_Device* Data_80; //4 80
   MUSB_DeviceRequest device_request; //84
   char descriptor_buffer[256]; //16 92
   char arDeviceAddressBitmap[16]; //272 348
   MUSB_ControlIrp control_urb; // 364
   int dwTotalLengthSum; //332 408
   unsigned short fill_412; //336 412
   char bDeviceAddress; //338 414
   char bStep; //339 415
   char iConfiguration; //340 416
   char bNumConfigurations; //340 417
   char bRetry; //342 418
   char bData_419; //343 419
   //344
} Struct_49d2fc_Inner1_3;

typedef void (*MUSB_pfHubEnumerationComplete)(
        MUSB_Device*, //<1486>
        MUSB_Device* //<1486>
        ); /*Line 825*/

typedef struct //Struct_49d2fc_Inner1_3
{
    MUSB_pfHubEnumerationComplete pfEnumerationComplete; //0
    MUSB_Device* pDevice; //4 /*Line 786*/
    uint8_t aSetupTx[8]; //8
    uint8_t aSetupRx[256]; //16
    uint8_t abmAddress[16]; //272
    MUSB_ControlIrp Irp; //288 +44
    uint32_t dwData; //332 <1a6>
    uint16_t wData; //336 <187>
    uint8_t bAddress; //338
    uint8_t bState;
    uint8_t bIndex;
    uint8_t bCount;
    uint8_t bRetries;
    uint8_t bFatalRetries; //343
    //344
} MGC_EnumerationData;

typedef struct
{
  int a; //0
  MUSB_LinkedList b; //4
  unsigned short c; //16
  //20
} Struct_49d2fc_Inner1_5;

typedef struct //Struct_49d2fc_Inner1_5
{
    uint32_t dwTotalTime; //0
    MUSB_LinkedList ScheduleSlots; //4
    uint16_t wSlotCount; //16
    //20
} MGC_Schedule;


struct MGC
{
   struct Struct_49d2fc_t* a; //0
   MUSB_Port* b; //4
   void* c; //8
   MUSB_Array ep_list; //12
   char bData_28; //28
   char bData_29; //29
   char bData_30; //30
   char bData_31; //31
   char address_32; //32
   char bData_33; //33
   char bData_34; //34
   char bTestMode; //35
   char bTestModeValue; //36
   uint8_t bInitialized; //37
   char bBulkSplit; //38
   char bBulkCombine; //39
   char bData_40; //40
   char bData_41; //41
   char bData_42; //42
   char bEndCount; //43
   int Data_44; //44
   char bRootSpeed; //48
   char bData_49; //49
   char bReset; //50
   char bSuspend; //51
   char bData_52; //52
   char bSession; //53
   char bHostMode; //54
   char bData_55; //55
   char bData_56; //56
   char bData_57; //57
   unsigned short wEndMask; //58
   char bData_60; //60
   char bData_61; //61
   char bData_62; //62
   char bData_63; //63
   char bData_64; //64
   char fill_65; //65
   char fill_66; //66
   char fill_67; //67
   /*MUSB_OtgState*/uint8_t bData_68; //68
   char bData_69; //69
   char bData_70; //70
   MUSB_OtgClient* pOtgClient; //72
   Struct_49d2fc_Inner1_3 Data_156; //76 +344
   MUSB_HostClient* pDeviceDriverList; //420
   int fill_424[2]; //424
   MUSB_Device* Data_432; //432
   MUSB_LinkedList List_524; //436 +12
   Struct_49d2fc_Inner1_5 Data_536; //448
   int fill_556; //468
   MUSB_FunctionClient* pDevice; //472
   int fill_476[3]; //476   
   /*MUSB_State*/uint8_t bDeviceState; //488
   char bDeviceNumTotalEndpoints; //489
   char bDevRemoteWakeup_578; //490
   char bDeviceHasAddress; //491
   char bDeviceCurrentConfigurationValue; //492
   char bData_581; //493
   unsigned short wDeviceBufferOffset; //494
   unsigned short fill_584; //496
   unsigned short wDeviceTxSize; //498
   uint8_t* pDeviceTransferBuffer; //500
   uint32_t dwDeviceSequenceNum; //504
   char arEndpointOffsets[16/*iInterface*/]; //508
   struct MGC_DmaController* pDmaController; //524
   int (*Func_616)(struct MGC* a); //528
   int (*Func_620)(struct MGC* a); //532
   int (*Func_624)(struct MGC* a); //536
   MGC_Endpoint* (*Func_628)(struct MGC* a, 
                             /*Struct_497028*/struct Struct_49d604* b, 
                             void* c, 
                             int d); //540
   int (*pfReceiveOnEp)(struct MGC* a, 
                    MGC_Endpoint* b, 
                    char* c, int d, void* e, char f); //544
   int (*pfSendOnEp)(struct MGC* a, 
                    MGC_Endpoint* b, 
                    char* c, unsigned d, void* e); //548
   int (*Func_640)(struct MGC* a, 
                    MGC_Endpoint* b, 
                    char c, int d); //552
   int (*Func_644)(struct MGC* a, 
                    MGC_Endpoint* b, 
                    char c, int d); //556
   int (*Func_648)(struct MGC* a, 
                    int b); //560
   int (*pfDefaultEndHandler)(struct MGC* a, MGC_Message* b); //564
   int (*pfServiceTxAvail)(struct MGC* a, unsigned short wEnd, MGC_Message* c); //568
   int (*pfServiceRxReady)(struct MGC* a, unsigned short wEnd, MGC_Message* c); //572
   int (*pfLoadFifo)(struct MGC* a, char bEnd, 
                   unsigned wCount, const char* pSource); //576
   int (*pfUnloadFifo)(struct MGC* a, char bEnd, 
                   unsigned short wCount, char* pDest); //580
   int (*Func_672)(struct MGC* a, MUSB_Device* b, MUSB_DeviceDriver* c); //584
   int (*pfDmaChannelStatusChanged)(void* pPrivateData, char bLocalEnd, char bTransmit); //588
   void (*Func_680)(struct MGC* a, int b); //592
   void (*Func_684)(void); //596
   MUSB_Port usbPort; //600 +16
   //616
};

typedef struct /*230*/
{
#if 1 //TODO
    void* pPrivateData; //0
    MGC_pfDmaStartController pfDmaStartController; //4
    MGC_pfDmaStopController pfDmaStopController; //8
    MGC_pfDmaAllocateChannel pfDmaAllocateChannel; //12
    MGC_pfDmaReleaseChannel pfDmaReleaseChannel; //16
    MGC_pfDmaAllocateBuffer pfDmaAllocateBuffer; //20
    MGC_pfDmaReleaseBuffer pfDmaReleaseBuffer; //24
    MGC_pfDmaProgramChannel pfDmaProgramChannel; //28
    MGC_pfDmaGetChannelStatus pfDmaGetChannelStatus; //32
    MGC_pfDmaControllerIsr pfDmaControllerIsr; //36
#endif
    //40?
} MUSB_DmaController; /*Line 241*/

//typedef void* (*MUSB_pfSystemToBusAddress)(void*, char*); /*Line 225*/

typedef uint8_t (*MUSB_pfDmaChannelStatusChanged)(void*, uint8_t, uint8_t); /*Line 257 */

typedef MUSB_DmaController* (*MUSB_pfNewDmaController)(
        MUSB_pfDmaChannelStatusChanged,
        void*,
        MUSB_pfSystemToBusAddress,
        void*,
        uint8_t*,
        uint8_t*
        ); /*Line 290*/

typedef void (*MUSB_pfDestroyDmaController)(MUSB_DmaController*); /*Line 296*/

typedef struct
{
    /*TODO*/
    int dummy;

} MUSB_DmaChannel;

typedef struct
{
    uint8_t bLocalEnd; //0 -96
    char bIsFifoShared; //1 -95
    uint16_t wMaxPacketSizeTx; //2 -94
    uint16_t wMaxPacketSizeRx; //4 -92
    uint8_t bBusAddress; //6 -90
    uint8_t bBusEnd; //7 -89
    uint8_t bInterval; //8 -88
    uint8_t bTrafficType; //9 -87
    uint8_t bRxBusAddress; //10 -86
    uint8_t bRxBusEnd; //11 -85
    uint8_t bRxInterval; //12 -84
    uint8_t bRxTrafficType; //13 -83
    uint8_t bIsTx; //14 -82
    uint8_t bIsHalted; //15 -81
    uint8_t bIsClaimed; //16 -80
    uint8_t bRxClaimed; //17 -79
    uint8_t bStopTx; //18 -78
    uint8_t bIsRxHalted; //19 -77
    uint16_t wPacketSize; //20
    uint16_t wRxPacketSize; //22
    uint32_t dwTxSize; //24
    /*void*/MUSB_Irp* pTxIrp; //28
    MUSB_LinkedList TxIrpList; //32
    uint32_t dwTxOffset; //44
    /*void*/MUSB_Irp* pRxIrp; //48
    MUSB_LinkedList RxIrpList; //52
    uint32_t dwRxOffset; //64
    MUSB_DmaChannel* pDmaChannel; //68
    MUSB_DmaChannel* pRxDmaChannel; //72
    uint16_t wDmaBufferCount; //76
    uint16_t wRxDmaBufferCount; //78
    uint8_t bDmaTx; //80
    uint8_t bDmaResidual; //81
    void* pPrivateData; //84
    uint32_t dwWaitFrameCount; //88
    uint8_t bTxFifoSize; //92
    uint8_t bRxFifoSize; //93
    uint8_t bIsrResidual; //94
    uint8_t bBsrResidual; //95
    //96
} MGC_EndpointResource;


typedef struct
{
    int dummy;
    //TODO
} MGC_BsrItem;


typedef struct
{
    uint16_t wCoreRegistersExtent; //0
    MUSB_pfNewDmaController pfNewDmaController; //4
    MUSB_pfDestroyDmaController pfDestroyDmaController; //8 /*Line 316*/
    //12
} MUSB_DmaControllerFactory;

struct _MGC_Port;
struct _MGC_Controller;

typedef uint32_t (*MGC_pfProgramStartController)(struct _MGC_Controller*); //Line 435
typedef uint32_t (*MGC_pfProgramStopController)(struct _MGC_Controller*); //Line 444
typedef uint32_t (*MGC_pfDestroyController)(struct _MGC_Controller*); //Line 452
typedef uint32_t (*MGC_pfSetControllerHostPower)(struct _MGC_Controller*); //Line 460

typedef struct _MGC_Controller
{
    void* pControllerAddressIsr; //0
    void* pControllerAddressIst; //4
    MUSB_SystemUtils* pUtils; //8
    MUSB_SystemServices* pSystemServices; //12
    void* pPrivateData; //16
    struct _MGC_Port* pPort; //20
    MUSB_DmaControllerFactory* pDmaControllerFactory; //24 /*Line 519*/
    MGC_pfProgramStartController pfProgramStartController; //28
    MGC_pfProgramStopController pfProgramStopController; //32
    MGC_pfDestroyController pfDestroyController; //36
    MGC_pfSetControllerHostPower pfSetControllerHostPower; //40 /*Line 525*/
#if 0
    pfDumpControllerState; /*Line 528*/
    pfDumpPipeState;
    pfDumpLocalEndState;
#endif
    uint16_t wControllerType; //44 /*Line 532*/
    //48
} MGC_Controller;


typedef enum
{
    MUSB_HSET_PORT_NONE = 0,
    MUSB_HSET_PORT_TEST_J = 1,
    MUSB_HSET_PORT_TEST_K = 2,
    MUSB_HSET_PORT_TEST_SE0_NAK = 3,
    MUSB_HSET_PORT_TEST_PACKET = 4,
    MUSB_HSET_PORT_RESET = 5,
    MUSB_HSET_PORT_SUSPEND = 6,
    MUSB_HSET_PORT_RESUME = 7,
    MUSB_HSET_PORT_SETUP_START = 8,
    MUSB_HSET_PORT_SETUP_IN = 9,
    MUSB_HSET_PORT_SETUP_STATUS = 10
}  MUSB_HsetPortMode;


typedef uint32_t (*MGC_pfReadBusState)(struct _MGC_Port*); /*Line 547 */
typedef uint32_t (*MGC_pfProgramBusState)(struct _MGC_Port*); /*Line 556*/
typedef uint32_t (*MGC_pfResetPort)(struct _MGC_Port*); /*Line 565*/
typedef MGC_EndpointResource* (*MGC_pfBindEndpoint)(struct _MGC_Port*, /*Line 581*/
        const MUSB_DeviceEndpoint*,
        MUSB_EndpointResource*,
        uint8_t);
typedef uint32_t (*MGC_pfProgramStartReceive)(struct _MGC_Port*, //<2d55> /*Line 600*/
        MGC_EndpointResource*, //<2491>
        uint8_t*, //<f1a>
        uint32_t, //<1a6>
        void*, //<f3>
        uint8_t //<178>
        );
typedef uint32_t (*MGC_pfProgramStartTransmit)(struct _MGC_Port*, //<2d55> /*Line 614*/
        MGC_EndpointResource*, //<2491>
        const uint8_t*, //<659>
        uint32_t, //<1a6>
        void* //<f3>
        );
typedef uint32_t (*MGC_pfProgramFlushEndpoint)(struct _MGC_Port*, //<2d55> /*Line 629*/
        MGC_EndpointResource*, //<2491>
        uint8_t, //<178>
        uint8_t //<178>
        );
typedef uint32_t (*MGC_pfProgramHaltEndpoint)(struct _MGC_Port*, //<2d55> /*Line 646*/
        MGC_EndpointResource*, //<2491>
        uint8_t, //<178>
        uint8_t //<178>
        );
typedef uint32_t (*MGC_pfDefaultEndResponse)(struct _MGC_Port*, //<2d55> /*Line 656*/
        uint8_t //<178>
        );
typedef uint8_t (*MGC_pfServiceDefaultEnd)(struct _MGC_Port*, //<2d55> /*Line 665*/
        MGC_BsrItem* //<3411>
        );
typedef uint8_t (*MGC_pfServiceTransmitAvail)(struct _MGC_Port*, //<2d55> /*Line 675*/
        uint16_t, //<187>
        MGC_BsrItem* //<3411>
        );
typedef uint8_t (*MGC_pfServiceReceiveReady)(struct _MGC_Port*, //<2d55> /*Line 685*/
        uint16_t, //<187>
        MGC_BsrItem* //<3411>
        );
typedef uint8_t (*MGC_pfLoadFifo)(struct _MGC_Port*, //<2d55> /*Line 695*/
        uint8_t, //<178>
        uint32_t, //<1a6>
        const uint8_t* //<659>
        );
typedef uint8_t (*MGC_pfUnloadFifo)(struct _MGC_Port*, //<2d55> /*Line 705*/
        uint8_t, //<178>
        uint32_t, //<1a6>
        uint8_t* //<f1a>
        );
typedef uint8_t (*MGC_pfAcceptDevice)(struct _MGC_Port*, //<2d55> /*Line 718*/
        MUSB_Device*, //<1486>
        MUSB_DeviceDriver* //<169a>
        );
typedef void (*MGC_pfSetPortTestMode)(struct _MGC_Port*, //<2d55> /*Line 725*/
        MUSB_HsetPortMode //<1a14>
        );
typedef void (*MGC_pfDynamicFifoLocation)(struct _MGC_Port*, //<2d55> /*Line 737*/
        uint8_t, //<178>
        uint8_t, //<178>
        uint8_t, //<178>
        uint32_t*, //<119c>
        uint8_t*, //<f1a>
        uint8_t* //<f1a>
        );

typedef struct _MGC_Port
{
    MGC_Controller* pController; //0
    MUSB_Port* pInterfacePort; //4 /*Line 867*/
    void* pPrivateData; //8
    MUSB_Array LocalEnds; //12
    uint8_t bWantHighSpeed; //28
    uint8_t bWantReset; //29
    uint8_t bWantSuspend; //30
    uint8_t bWantResume; //31
    uint8_t bFuncAddr; //32
    uint8_t bWantSession; //33
    uint8_t bWantHost; //34
    uint8_t bWantTestMode; //35
    uint8_t bTestMode; //36
    uint8_t bSessionActive; //37
    uint8_t bHasBulkSplit; //38
    uint8_t bHasBulkCombine; //39
    uint8_t bHasHbIsoTx; //40
    uint8_t bHasHbIsoRx; //41
    uint8_t bIsMultipoint; //42
    uint8_t bEndCount; //43 / debug:51
    uint32_t dwFrame; //44 / debug:52
    uint8_t bIsHighSpeed; //48
    uint8_t bIsLowSpeed; //49
    uint8_t bIsReset; //50
    uint8_t bIsSuspend; //51
    uint8_t bIsResume; //52
    uint8_t bIsSession; //53
    uint8_t bIsHost; //54
    uint8_t bConnectorId; //55
    uint8_t bBusVoltage; //56
    uint8_t bEnd0Status; //57
    uint16_t wEndMask; //58 / debug: 66
    uint8_t bDmaCompleted;
    uint8_t bVbusErrorRetries;
    uint8_t bHasDisablePing; //62
    uint8_t bBabbleRetries;
    uint8_t bHasRqPktCount; //64
    uint8_t bPad1;
    uint8_t bPad2;
    uint8_t bPad3;
    uint8_t bOtgState; //68 / debug: 76
    uint8_t bRelinquish;
    uint8_t bIsHnpSupported;
    uint8_t bIsHnpAllowed;
    MUSB_OtgClient* pOtgClient; //72 /*922*/
    MGC_EnumerationData EnumerationData; // /*926*/
    MUSB_HostClient* pHostClient; //420
    int/*TODO*/ pfDriverTimerExpired;
    int/*TODO*/ pDriverTimerData;
    MUSB_Device* pRootDevice;
    MUSB_LinkedList ConnectedDeviceList; //List_524 //436 +12 /*931*/
    MGC_Schedule Schedule; //Data_536; //448 /*932*/
    uint16_t wHostMaxPower; // /*933*/
    uint16_t wHostUsedPower;
    int/*TODO*/ pFunctionClient;
    int/*TODO*/ apConfigDescriptors[1]; //476
    int/*TODO*/ pCurrentConfig;
    int/*TODO*/ apPipe[1];
    uint8_t bUsbState; //488
    uint8_t bMaxFunctionEnds;
    uint8_t bCanWakeup;
    uint8_t bSetAddress;
    uint8_t bConfigValue;
    uint8_t bParse;
    uint16_t wSetupDataIndex; //494 /*Line 948*/
    uint16_t wSetupRxIndex;
    uint16_t wSetupDataSize;
    int/*TODO*/ pSetupData; //500
    int/*TODO*/ dwSequenceNumber;
    char/*TODO*/ abIfaceOffsets[16]; //508
    MUSB_DmaController* pDmaController; //524 /*Line 969*/
    MGC_pfReadBusState pfReadBusState; //528 Func_616 /*Line 972*/
    MGC_pfProgramBusState pfProgramBusState; //Func_620) //532
    MGC_pfResetPort pfResetPort;
    MGC_pfBindEndpoint pfBindEndpoint;
    MGC_pfProgramStartReceive pfProgramStartReceive; //544
    MGC_pfProgramStartTransmit pfProgramStartTransmit;
    MGC_pfProgramFlushEndpoint pfProgramFlushEndpoint; //552
    MGC_pfProgramHaltEndpoint pfProgramHaltEndpoint; //Func_644
    MGC_pfDefaultEndResponse pfDefaultEndResponse;
    MGC_pfServiceDefaultEnd pfServiceDefaultEnd;
    MGC_pfServiceTransmitAvail pfServiceTransmitAvail; //pfServiceTxAvail   //568
    MGC_pfServiceReceiveReady pfServiceReceiveReady;
    MGC_pfLoadFifo pfLoadFifo; //576
    MGC_pfUnloadFifo pfUnloadFifo;
    MGC_pfAcceptDevice pfAcceptDevice;
    MUSB_pfDmaChannelStatusChanged pfDmaChannelStatusChanged;
    MGC_pfSetPortTestMode pfSetPortTestMode; //Func_680 //592
    MGC_pfDynamicFifoLocation pfDynamicFifoLocation;
    //TODO
} MGC_Port; /*Line 990*/


typedef struct Struct_49d2fc_t
{
#if 0
    MGC_Controller ControllerImpl;
#else
   int addr1; //0
   int addr2; //4
   MUSB_SystemUtils* pSystemUtils; //8
   MUSB_SystemServices* pOS; //12
   int fill_16; //16
   struct MGC* Data_20; //20
   struct MGC_DmaControllerFactory* pDmaControllerFactory; //24
   int (*pfEnableInterrupts)(struct Struct_49d2fc_t*); //28
   void (*pfDisableInterrupts)(struct Struct_49d2fc_t*); //32
   void (*Data_36)(void); //36
   void (*Data_40)(void); //40
   int fill_44; //44
#endif

#if 0
   MUSB_Controller Controller; //48
#else
   MUSB_Controller Data_48; //48 +40
#endif

#if 0
   MGC_Port PortImpl; //88
#else
   struct MGC Data_88; //88 +616
#endif

#if 0
   MUSB_Port Port;
#endif
   //704
} Struct_49d2fc/*MGC_ControllerWrapper*/; /*79*/         //?????


typedef struct
{
   struct MGC_Pipe* a; //0
   void* b; //4
   int* c; //8
   int d; //12
   int e; //16
   int f; //20
   struct Struct_48b494_Inner1_t* g; //24
    unsigned short h; //28
    char i; //30 
   char j; //31
   char k; //32
   char l; //33
   char m; //34
   char n; //35     
} Struct_49e470_for_TransferType_1;



extern int func_21cc5594(int);
extern MUSB_Controller* MUSB_NewController(MUSB_SystemUtils*, uint16_t, void*, void*);
extern MUSB_Port* MUSB_GetPort(unsigned short);
extern uint32_t MUSB_StartController(MUSB_Controller*, MUSB_SystemServices*);
extern int MGC_HdrcUlpiVbusControl(struct MGC*, char, char);
extern MUSB_BusHandle MUSB_RegisterOtgClient(MUSB_Port*, MUSB_FunctionClient*, MUSB_HostClient*, MUSB_OtgClient*);
extern void func_21cc8328(char*);
extern void func_21cc3b54(char*, unsigned short, const char*);
extern void func_21cc3e28(char*, unsigned short, unsigned, unsigned char, unsigned);
extern int MUSB_FAPI_InitDma(int);

extern void musb_memcpy(void*, void*, int);
extern void MGC_FAPI_MemSet(void* ptr, uint8_t c, size_t iSize);


#endif /*MUSB_H_*/
