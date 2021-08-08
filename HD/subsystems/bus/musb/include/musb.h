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


struct MUSB_DeviceDriverCandidate
{
   MUSB_DeviceDriver deviceDriver; //0
   char peripheralList[52]; //28
   unsigned listLength; //80
   struct MUSB_DeviceDriverCandidate* next; //84
};


typedef struct Struct_49d2fc_Inner1_3_2_t
{
   MUSB_DeviceDriver* pDeviceDriver; //0
   void* b; //4
   List_49cd8c c; //8
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
   unsigned short elem_size; //0 
   unsigned short count; //2  
   void* data; //4
   unsigned short num_elements; //8
   unsigned short e; //10
   void* more_data; //12
   //16
} List_49a810;


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
   List_49cd8c tx_urb_list; //32  
   int dwOffset; //44
   /*void*/MUSB_Irp* pCurrentRxUrb; //48
   List_49cd8c rx_urb_list; //52  
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
   List_49cd8c* Data_12; //12
   MUSB_Device* Data_16; //16
   unsigned short wMaxPacketSize; //20
   int Data_24; //24
   int Data_28; //28
   char bTransferType; //32
   //36
} MGC_Pipe;

typedef struct
{
  int a; //0
  List_49cd8c b; //4
  unsigned short c; //16   
  //20
} Struct_49d2fc_Inner1_5;


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


struct MGC
{
   struct Struct_49d2fc_t* a; //0
   MUSB_Port* b; //4
   void* c; //8
   List_49a810 ep_list; //12
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
   List_49cd8c List_524; //436 +12   
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


typedef struct Struct_49d2fc_t
{
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
   MUSB_Controller Data_48; //48 +40
   struct MGC Data_88; //88 +616
   //704
} Struct_49d2fc;


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
extern void* func_21cc4eac(MUSB_SystemUtils*, unsigned short, int, int);
extern MUSB_Port* func_21cc4dc8(unsigned short);
extern int func_21cc41c4(MUSB_Controller*, MUSB_SystemServices*);
extern int MGC_HdrcUlpiVbusControl(struct MGC*, char, char);
extern MUSB_BusHandle func_21cc6c24(MUSB_Port*, MUSB_FunctionClient*, MUSB_HostClient*, MUSB_OtgClient*);
extern void func_21cc8328(char*);
extern void func_21cc3b54(char*, unsigned short, const char*);
extern void func_21cc3e28(char*, unsigned short, unsigned, unsigned char, unsigned);
extern int MGC_SetDmaControllerFactory(int);

extern void musb_memcpy(void*, void*, int);


#endif /*MUSB_H_*/
