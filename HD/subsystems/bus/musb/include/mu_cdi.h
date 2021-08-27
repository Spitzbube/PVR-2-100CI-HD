#ifndef MU_CDI_H_
#define MU_CDI_H_

#define  MUSB_BusHandle void* //todo

/* USB_DT_DEVICE: Device descriptor */
struct usb_device_descriptor
{
    char  bLength; //0
    char  bDescriptorType; //1

    unsigned short bcdUSB; //2
    char  bDeviceClass; //4
    char  bDeviceSubClass; //5
    char  bDeviceProtocol; //6
    char  bMaxPacketSize0; //7
    unsigned short idVendor; //8
    unsigned short idProduct; //10
    unsigned short bcdDevice; //12
    char  iManufacturer; //14
    char  iProduct; //15
    char  iSerialNumber; //16
    char  bNumConfigurations; //17
};



struct Struct_49d2fc_t;

typedef struct MGC_Timer_t
{
   int count; //0
   int recount; //4
   void (*func)(struct Struct_49d2fc_t* a, unsigned short b); //8
   //12
} MGC_Timer;


typedef int (*MUSB_pfControllerIsr)(void*); //:t(38,1)=(7,22)
typedef void (*MUSB_pfControllerBsr)(void*); //:t(38,2)=(38,3)=*(38,4)=f(0,19)
typedef void (*MUSB_pfTimerExpired)(); //:t(38,5)=(38,6)=*(38,7)=f(0,19)

typedef struct
{
   uint16_t wVersion; //0
   void* pPrivateData; //4
   uint16_t wQueueLength; //8
   uint16_t wQueueItemSize; //10
   uint16_t wTimerCount; //12
   uint32_t* adwTimerResolutions; //16
   uint16_t wLockCount; //20
   MUSB_pfControllerIsr pfIsr; //24  //:(38,1),192,32;
   void* pIsrParam; //28
   MUSB_pfControllerBsr pfBsr; //32 //:(38,2),256,32;
   void* pBsrParam; //36
   //40
} MUSB_Controller; //:t(38,8)=(38,9)=s40;


typedef void (*MUSB_pfMessageString)(); //:t(38,11)=(38,12)=*(38,13)=f(16,4)
typedef void (*MUSB_pfMessageNumber)(); //:t(38,14)=(38,15)=*(38,16)=f(16,4)
typedef void (*MUSB_pfGetTime)(); //:t(38,17)=(38,18)=*(38,19)=f(16,6)

typedef struct
{
   uint16_t wVersion; //0
   MUSB_pfMessageString pfMessageString; //4 //:(38,11),32,32;
   MUSB_pfMessageNumber pfMessageNumber; //8 //:(38,14),64,32;
   MUSB_pfGetTime pfGetTime; //12 //:(38,17),96,32;;
   //16
} MUSB_SystemUtils; //:t(38,20)=(38,21)=s16;

typedef char* (*MUSB_pfSystemToBusAddress)(void*, char*); //:t(38,22)=(38,23)=*(38,24)=f(6,2)
typedef int (*MUSB_pfQueueBackgroundItem)(void*, void*); //:t(38,25)=(38,26)=*(38,27)=f(16,4)
typedef uint8_t (*MUSB_pfDequeueBackgroundItem)(void*, void*); //:t(38,28)=(38,29)=*(38,30)=f(16,4)
typedef void (*MUSB_pfFlushBackgroundQueue)(void*); //:t(38,31)=(38,3)
typedef int (*MUSB_pfArmTimer)(void*, int, int, int,
      void (*)(struct Struct_49d2fc_t* a,
                unsigned short b, MGC_Timer* c)); //:t(38,32)=(38,33)=*(38,34)=f(16,4)
typedef int (*MUSB_pfCancelTimer)(void*, int); //:t(38,35)=(38,36)=*(38,37)=f(16,4)
typedef int (*MUSB_pfLock)(void*, unsigned short); //:t(38,38)=(38,36)
typedef int (*MUSB_pfUnlock)(void*, unsigned short); //:t(38,39)=(38,36)
typedef int (*MUSB_pfPrintDiag)(void*, char*); //:t(38,40)=(38,41)=*(38,42)=f(16,4)
typedef void (*MUSB_pfNewPowerLoad)(); //:t(38,43)=(38,44)=*(38,45)=f(16,4)
typedef void (*MUSB_pfRemovePowerLoad)(); //:t(38,46)=(38,44)

typedef struct
{
   uint16_t wVersion; //0
   void* pPrivateData; //4
   MUSB_pfSystemToBusAddress pfSystemToBusAddress; //8
   MUSB_pfQueueBackgroundItem pfQueueBackgroundItem; //12
   MUSB_pfDequeueBackgroundItem pfDequeueBackgroundItem; //16
   MUSB_pfFlushBackgroundQueue pfFlushBackgroundQueue; //20
   MUSB_pfArmTimer pfArmTimer; //24
   MUSB_pfCancelTimer pfCancelTimer; //28
   MUSB_pfLock pfLock; //32
   MUSB_pfUnlock pfUnlock; //36
   MUSB_pfPrintDiag pfPrintDiag; //40
   MUSB_pfNewPowerLoad pfNewPowerLoad; //44
   MUSB_pfRemovePowerLoad pfRemovePowerLoad; //48
   //52
} MUSB_SystemServices;

typedef enum
{
   MUSB_PORT_UNKNOWN = 0,
   MUSB_PORT_TYPE_FUNCTION = 1,
   MUSB_PORT_TYPE_HOST = 2,
   MUSB_PORT_TYPE_OTG = 3,
} MUSB_PortType;

typedef enum
{
   MUSB_PORT_SPEED_UNKNOWN = 0,
   MUSB_PORT_SPEED_LOW = 1,
   MUSB_PORT_SPEED_FULL = 2,
   MUSB_PORT_SPEED_HIGH = 3,
} MUSB_PortSpeed;

typedef enum
{
   MUSB_POWER_OFF = 0,
   MUSB_POWERED = 1,
   MUSB_DEFAULT = 2,
   MUSB_ADDRESS = 3,
   MUSB_CONFIGURED = 4,
   MUSB_SUSPENDED = 5,
} MUSB_State;

typedef enum
{
   MUSB_AB_IDLE = 0,
   MUSB_WAIT_VRISE = 1,
   MUSB_B_PERIPHERAL = 17,
   MUSB_B_WAIT_ACON = 18,
   MUSB_B_HOST = 19,
   MUSB_B_SRP_INIT = 20,
   MUSB_A_PERIPHERAL = 33,
   MUSB_A_WAIT_BCON = 34,
   MUSB_A_HOST = 35,
   MUSB_A_SUSPEND = 36,
   MUSB_S_TRANSITION = 48,
} MUSB_OtgState;

typedef enum
{
   MUSB_CONNECTION_SPEED_UNKNOWN = 0,
   MUSB_CONNECTION_SPEED_LOW = 1,
   MUSB_CONNECTION_SPEED_FULL = 2,
   MUSB_CONNECTION_SPEED_HIGH = 3,
} MUSB_ConnectionSpeed;

typedef enum
{
   MUSB_TARGET_UNKNOWN = 0,
   MUSB_TARGET_VID = 1,
   MUSB_TARGET_PID = 2,
   MUSB_TARGET_DEVICE_BCD = 3,
   MUSB_TARGET_CONFIG = 4,
   MUSB_TARGET_INTERFACE = 5,
   MUSB_TARGET_CLASS = 6,
   MUSB_TARGET_SUBCLASS = 7,
   MUSB_TARGET_PROTOCOL = 8,
   MUSB_TARGET_ACCEPT = 9,
   MUSB_TARGET_REJECT = 10,
} MUSB_TargetListCode;

typedef struct
{
   void* pPrivateData; //0
   uint16_t bcdCdiRev; //4
   MUSB_PortType Type; //8
   MUSB_PortSpeed Speed; //12
   //16
} MUSB_Port;

typedef struct _MUSB_Device
{
   MUSB_Port* pPort; //0
   /*struct Struct_49d2fc_Inner1_3_2_t*/void* pPrivateData; //4
   void* pDriverPrivateData; //8
   struct usb_device_descriptor DeviceDescriptor; //12
   char* pCurrentConfiguration; //32
   void* apConfigDescriptors; //36 //pConfigurationOffsets
   void* pDescriptorBuffer; //40 //pConfigurations
   uint16_t wDescriptorBufferLength; //44 //wData_44
   MUSB_ConnectionSpeed ConnectionSpeed; //48 //int dwSpeed
   uint8_t bBusAddress; //52 //char bDeviceAddress
   struct _MUSB_Device* pParentUsbDevice; //56 //void* Data_56
   uint8_t bHubPort; //60
   //64
} MUSB_Device; //Struct_49d2fc_Inner1_3_1;

typedef struct //Struct_49d604
{
   MUSB_Device* pDevice; //0
   struct
   {
      char bLength; //0
      char bDescriptorType; //1
      char bEndpointAddress; //2
      char bmAttributes; //3
      unsigned short wMaxPacketSize; //4
      char bInterval; //6
   } UsbDescriptor; //4
   uint16_t wNakLimit; //12
   //16
} MUSB_DeviceEndpoint; //Struct_49d604;

typedef struct
{
   uint32_t dwBufferSize; //0
   uint32_t bmFlags; //4
   void* pPrivateData; //8
   //12
} MUSB_EndpointResource;

typedef void* MUSB_Pipe; //(13,28)

struct _MUSB_ControlIrp;

typedef void (*MUSB_pfControlIrpComplete)(void*, struct _MUSB_ControlIrp*);

typedef struct _MUSB_ControlIrp //:T(13,32)
{
   MUSB_Device* pDevice; //0
   uint8_t* pOutBuffer; //4
   uint32_t dwOutLength; //8
   uint8_t* pInBuffer; //12
   uint32_t dwInLength; //16
   uint32_t dwStatus; //20
   uint32_t dwActualOutLength; //24
   uint32_t dwActualInLength; //28
   MUSB_pfControlIrpComplete pfIrpComplete; //32
   void* pCompleteParam; //36
   uint8_t bAllowDma; //40
   //44
} MUSB_ControlIrp; //:t(13,34)=(13,32);

struct _MUSB_Irp;

typedef void (*MUSB_pfIrpComplete)(void*, struct _MUSB_Irp*); //:t(13,35)=(13,36)=*(13,37)=f(0,19)

typedef struct _MUSB_Irp //:T(13,38)=s32
{
   MUSB_Pipe hPipe; //0
   uint8_t* pBuffer; //4
   uint32_t dwLength; //8
   uint32_t dwStatus; //12
   uint32_t dwActualLength; //16
   MUSB_pfIrpComplete pfIrpComplete; //20
   void* pCompleteParam; //24
   uint8_t bAllowShortTransfer; //28
   uint8_t bIsrCallback; //29
   uint8_t bAllowDma; //30
   //32
} MUSB_Irp; //:t(13,39)=(13,38);

typedef struct _MUSB_Irp MUSB_BulkIrp; //:t(13,40)=(13,38)
typedef struct _MUSB_Irp MUSB_InterruptIrp; //:t(13,41)=(13,38)

struct _MUSB_IsochIrp;

typedef void (*MUSB_pfIsochIrpComplete)(void*, struct _MUSB_IsochIrp*); //(13,42)=(13,43)=*(13,44)=f(0,19)

typedef struct _MUSB_IsochIrp //:T(13,45)
{
   MUSB_Pipe hPipe; //0
   uint8_t* pBuffer; //4
   uint32_t* adwLength; //8
   uint32_t* adwStatus; //12
   uint32_t* adwActualLength; //16
   MUSB_pfIsochIrpComplete pfIrpComplete; //20
   void* pCompleteParam; //24
   uint16_t wFrameCount; //28
   uint16_t wCurrentFrame; //30
   uint16_t wCallbackInterval; //32
   uint8_t bIsrCallback; //34
   uint8_t bAllowDma; //35
   //36
} MUSB_IsochIrp; //:t(13,47)=(13,45);


typedef void (*MUSB_pfNewUsbState)(void* pPrivateData,
      MUSB_BusHandle busHandle,
      MUSB_State state
      ); //:t(13,48)=(13,49)=*(13,50)=f(0,19)

typedef uint8_t (*MUSB_pfDeviceRequest)(void* pPrivateData,
      MUSB_BusHandle busHandle,
      uint32_t sequenceNum,
      const uint8_t* pRequestData,
      uint16_t length
      ); //:t(13,51)=(13,52)=*(13,53)=f(16,4)

typedef uint8_t (*MUSB_pfDeviceConfigSelected)(void* pPrivateData,
      MUSB_BusHandle busHandle,
      uint8_t configValue,
      MUSB_Pipe* pPipes
      ); //:t(13,54)=(13,55)=*(13,56)=f(16,4)

typedef uint8_t (*MUSB_pfInterfaceSet)(void* pPrivateData,
      MUSB_BusHandle busHandle,
      uint8_t interfaceNumber,
      uint8_t alternateSetting,
      MUSB_Pipe* pPipes
      ); //:t(13,57)=(13,58)=*(13,59)=f(16,4)

typedef struct
{
    void* pPrivateData; //0
    uint8_t* pStandardDescriptors; //4
    uint16_t wDescriptorBufferLength; //8
    uint16_t wStringDescriptorCount; //10
    uint8_t* pHighSpeedDescriptors; //12
    uint16_t wHighSpeedDescriptorLength; //16
    uint16_t wControlBufferLength; //18
    uint8_t* pControlBuffer; //20
    uint8_t* pbSelfPowered; //24
    MUSB_pfDeviceRequest pfDeviceRequest; //28
    MUSB_pfDeviceConfigSelected pfDeviceConfigSelected; //32
    MUSB_pfInterfaceSet pfInterfaceSet; //36
    MUSB_pfNewUsbState pfUsbState; //40
    //44
} MUSB_FunctionClient;




struct MGC;

typedef int (*MUSB_pfDeviceConnected)(void* a,
      struct MGC* b,
      MUSB_Device* c,
      char* d
      ); //:t(13,62)=(13,63)=*(13,64)=f(16,4)

typedef void (*MUSB_pfDeviceDisconnected)(void* a,
      struct MGC* b,
      MUSB_Device* c
      ); //:t(13,65)=(13,66)=*(13,67)=f(0,19)

typedef void (*MUSB_pfDriverTimerExpired)(); //:t(13,68)=(13,69)=*(13,70)=f(0,19)

typedef void (*MUSB_pfBusSuspended)(void); //:t(13,71)=(13,69)

typedef void (*MUSB_pfBusResumed)(); //:t(13,72)=(13,69)

typedef struct _MUSB_DeviceDriver //:T(13,73)
{
   void* pPrivateData; //0
   uint8_t bTimerCount; //4
   uint32_t* adwTimerResolutions; //8
   MUSB_pfDeviceConnected pfDeviceConnected; //12
   MUSB_pfDeviceDisconnected pfDeviceDisconnected; //16
   MUSB_pfBusSuspended pfBusSuspended; //20
   MUSB_pfBusResumed pfBusResumed; //24
   //28
} MUSB_DeviceDriver; //:t(13,74)=(13,73)

typedef struct
{
   uint8_t* pPeripheralList; //0
   uint16_t wPeripheralListLength; //4
   MUSB_DeviceDriver* aDeviceDriverList; //8
   uint8_t bDeviceDriverListLength; //12
   //16
} MUSB_HostClient; //:t(13,75)=(13,76)=s16;

typedef void (*MUSB_pfNewOtgState)(void*, struct MGC*, uint32_t); //:t(13,78)=(13,79)=*(13,80)=f(0,19)

typedef void (*MUSB_pfOtgError)(void*, void*, uint32_t); //:t(13,81)=(13,82)=*(13,83)=f(0,19)

typedef struct
{
   void* pPrivateData; //0
   uint8_t* pbDesireHostRole; //4
   MUSB_pfNewOtgState pfOtgState; //8
   MUSB_pfOtgError pfOtgError; //12
   //16
} MUSB_OtgClient; //:t(13,84)=(13,85)=s16;


//typedef void (*MUSB_pfHubEnumerationComplete)(); //:t(13,86)=(13,87)=*(13,88)=f(0,19)

#endif /*MU_CDI_H_*/
