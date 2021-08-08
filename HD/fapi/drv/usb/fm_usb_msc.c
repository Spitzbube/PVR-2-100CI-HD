
#include <fapi/sys_services.h>
#include <fapi/drv_usb_func_msc.h>
#include <fapi/drv_usb_func_vendor.h>
#include "musb/mu_bot.h"
#include "musb/mu_scsi.h"


extern void func_21c0ab7c(void); //drv_ata

#define MUSB_MemAlloc FAPI_SYS_MALLOC
#define MUSB_MemFree FAPI_SYS_FREE
#define MUSB_MemSet musb_memset

typedef struct
{
   uint8_t bmRequestType; //0 ?
   uint8_t bRequest; //1 ?

} MUSB_DeviceRequest;

#define MUSB_TYPE_MASK 0x60
#define MUSB_TYPE_CLASS 0x20


typedef struct
{
   uint8_t bLength; //0
   uint8_t bDescriptorType; //1
   uint16_t bcdUSB; //2
   uint8_t bDeviceClass; //4
   uint8_t bDeviceSubClass; //5
   uint8_t bDeviceProtocol; //6
   uint8_t bMaxPacketSize0; //7
   uint16_t idVendor; //8
   uint16_t idProduct; //10
   uint16_t bcdDevice; //12
   uint8_t iManufacturer; //14
   uint8_t iProduct; //15
   uint8_t iSerialNumber; //16
   uint8_t bNumConfigurations; //17
   //18
} MUSB_DeviceDescriptor;


typedef struct
{
   uint8_t bLength;
   uint8_t bDescriptorType;
   uint16_t bcdUSB;
   uint8_t bDeviceClass;
   uint8_t bDeviceSubClass;
   uint8_t bDeviceProtocol;
   uint8_t bMaxPacketSize0;
   uint8_t bNumConfigurations;
   uint8_t bRESERVED;

} MUSB_QualifierDescriptor;

typedef struct
{
   uint8_t bLength; //0
   uint8_t bDescriptorType; //1
   uint16_t wTotalLength; //2
   uint8_t bNumInterfaces; //4
   uint8_t bConfigurationValue; //5
   uint8_t iConfiguration; //6
   uint8_t bmAttributes; //7
   uint8_t bMaxPower; //8
   //9
} MUSB_ConfigurationDescriptor;

typedef struct
{
   uint8_t bLength; //0
   uint8_t bDescriptorType; //1
   uint8_t bInterfaceNumber; //2
   uint8_t bAlternateSetting; //3
   uint8_t bNumEndpoints; //4
   uint8_t bInterfaceClass; //5
   uint8_t bInterfaceSubClass; //6
   uint8_t bInterfaceProtocol; //7
   uint8_t iInterface; //8
   //9
} MUSB_InterfaceDescriptor;

typedef struct
{
   uint8_t bLength; //0
   uint8_t bDescriptorType; //1
   uint8_t bEndpointAddress; //2
   uint8_t bmAttributes; //3
   uint16_t wMaxPacketSize; //4
   uint8_t bInterval; //6
   //7
} MUSB_EndpointDescriptor;

typedef struct
{
   int fill_0; //0
   uint32_t dCswTag; //4
   uint32_t dCswDataResidue; //8
   uint8_t bCswStatus; //12

} MGC_MsdCsw;

typedef struct
{
   uint32_t dCbwSignature; //0
   uint32_t dCbwTag; //4
   uint32_t dCbwDataTransferLength; //8
   uint8_t bmCbwFlags; //12
   uint8_t bCbwLun; //13
   uint8_t bCbwCbLength; //14
   uint8_t aCbwCb[16]; //15
} MGC_MsdCbw;

extern int32_t MUSB_StartTransfer(MUSB_Irp*);
extern void MUSB_CancelTransfer(MUSB_Irp*);
extern void MUSB_SetPipeHalt(void* hPipe, uint32_t);
extern uint32_t MUSB_GetPipeHalt(void* hPipe);

#define FERR int32_t
#define FBOOL uint32_t
#define FFALSE 0
#define FTRUE 1
#define FALSE 0

#define MUSB_STATUS_OK 0

#define USB_MSC_CONTROL_BUFFER_LENGTH 256

// MODE DATA
#define RBC_MODE_LENGTH        17
#define RBC_MODE_PAGE_CODE   0x06
#define RBC_MODE_PAGE_LENGTH 0x0b
#define RBC_MODE_PAGE_ALL    0x3F
#define RBC_MODE_LENGTH        17

#define RBC_SENSE_ASC_LUN_NOT_SUPPORTED     0x25

#define RBC_INQUIRY_DATA_LEN           0x24


#define EP_BULK_IN  0
#define EP_BULK_OUT 1

#define IRP_DATA_IN  0
#define IRP_DATA_OUT 1
#define IRP_CBW      2
#define IRP_CSW      3

typedef struct
{
    uint8_t  dataLength;              //!< Length of the following data bytes.
    uint8_t  mediumType;              //!< The medium type (always 0 for RBC devices).
    uint8_t  deviceParameter;         //!< The device parameter (always 0 for RBC devices).
    uint8_t  blockDescriptorLength;   //!< The block descriptor length (always 0 for RBC devices).
    uint8_t  pageCode;                //!< The page code (always 0x86 for RBC devices).
    uint8_t  pageLength;              //!< The page length (always 0x0B for this mode page).
    uint8_t  wcd;                     //!< Write cache disable bit.
    uint8_t  blockLengthHighByte;     //!< The high byte of the block size of the logical unit.
    uint8_t  blockLengthLowByte;      //!< The low byte of the block size of the logical unit.
    uint8_t  blockCount[5];           //!< The block capacity of the user data area.
    uint8_t  powerPerformance;        //!< Power vs. performance level.
    uint8_t  flags;                   //!< READ,WRITE,FORMAT and LOCK disable  flags.
    uint8_t  reserved;                //!< A reserved byte.
} FM_USB_MSC_RBC_MODE_S;

typedef struct
{
    uint8_t  responseCode;      //!< Response code (current errors=0x70), plus VALID.
    uint8_t  obsolete;          //!< Not used.
    uint8_t  senseKey; //2         //!< Sense key, plus FILEMARK, EOM, and ILI.
    uint8_t  info_31_24;        //!< Information bit 31 to 24 (LBA for direct-access devices).
    uint8_t  info_23_16;        //!< Information bit 23 to 16 (LBA for direct-access devices).
    uint8_t  info_15_8;         //!< Information bit 15 to  8 (LBA for direct-access devices).
    uint8_t  info_7_0;          //!< Information bit  7 to  0 (LBA for direct-access devices).
    uint8_t  additionalLength;  //!< Length of additional data (everything else from here).
    uint8_t  commandInfo_31_24; //!< Command-specific information (bit 31 to 24).
    uint8_t  commandInfo_23_16; //!< Command-specific information (bit 23 to 16).
    uint8_t  commandInfo_15_8;  //!< Command-specific information (bit 15 to  8).
    uint8_t  commandInfo_7_0;   //!< Command-specific information (bit  7 to  0).
    uint8_t  asc; //12              //!< Additional sense code.
    uint8_t  ascq;              //!< Additional sense code qualifier.
    uint8_t  fruCode;           //!< Field-Replaceable Unit code.
    uint8_t  keyFlags_0;        //!< Sense key specific data plus SKSV.
    uint8_t  keyFlags_1;        //!< Sense key specific data.
    uint8_t  keyFlags_2;        //!< Sense key specific data.
} FM_USB_MSC_RBC_SENSE_S;

typedef struct
{
    FAPI_USB_FUNC_MSC_LunEnumT type; //0
    uint32_t     firstLba; //4    //!< The index of the first logical block of the device.
    uint32_t     lbaCapacity; //8 //!< Total number of logical blocks for the device.
    uint32_t     blockLength; //12 //!< Length of logical block in bytes.
    uint32_t     bufferLength; //16 //!< Length of current data buffer for non-DMA LUN, otherwise 0.
    uint32_t     rwAddr; //20      //!< Read/write address of the device. This is either a RAM-buffer address or the DMA device address.
    FAPI_SYS_HandleT ataHandle; //24       //!< #FTRUE for hardware controlled DMA device otherwise #FFALSE;
    void (*Data_28)(void); //28       //!< #FTRUE if device is ready for data transfer otherwise #FFALSE.
    uint32_t Data_32; //32    //!< #FTRUE if the device is write protected otherwise #FFALSE
    uint8_t bData_36; //36
    uint8_t      inquiryData[RBC_INQUIRY_DATA_LEN]; //37
    //76
} FM_USB_MSC_DEVICE_S;

static void usbMscProcessCbw(MGC_MsdCbw* pCBW);

static void usbMscRbcTestUnitReady(uint8_t lun, uint8_t* pCmd);

static uint8_t cbDeviceRequest( void* pPrivateData,
                           MUSB_BusHandle busHandle,
                           uint32_t sequenceNum,
                           const uint8_t* pRequestData,
                           uint16_t length
                         );
static uint8_t cbDeviceConfigSelected( void* pPrivateData,
                                  MUSB_BusHandle busHandle,
                                  uint8_t configValue,
                                  MUSB_Pipe* pPipes
                                );
static uint8_t cbInterfaceSet( void* pPrivateData,
                          MUSB_BusHandle busHandle,
                          uint8_t interfaceNumber,
                          uint8_t alternateSetting,
                          MUSB_Pipe* pPipes
                        );
static void cbNewUsbState( void* pPrivateData,
                           MUSB_BusHandle busHandle,
                           MUSB_State state
                         );

static FERR getDescriptors( FBOOL hsFlag,
                            uint16_t   vendorID,
                            uint16_t   productID,
                            const char* pVendorName,
                            const char* pProductName,
                            const char* pSerialNumber,
                            uint8_t** ppDescriptors,
                            uint16_t* pLength
                          );

static void cbTxDataTransferComplete(void* pCompleteParam, MUSB_Irp* pIrp);
static void cbRxDataTransferComplete(void* pCompleteParam, MUSB_Irp* pIrp);
static void cbTxCswTransferComplete(void* pCompleteParam, MUSB_Irp* pIrp);
static void cbRxCbwTransferComplete(void* pCompleteParam, MUSB_Irp* pIrp);

static void (*usbMscNotifyFct)(uint32_t) = NULL; //21f66a8c

static FBOOL             isInitialized = FFALSE; //21f66a90
static uint8_t                usbMscSelfPowered = FFALSE; //21f66ac8
static uint8_t                usbMscControlBuffer[USB_MSC_CONTROL_BUFFER_LENGTH]; //21f66aec
static uint8_t                usbMscMaxLun    = 0; //21f66a98
static FM_USB_MSC_DEVICE_S* pUsbMscDevices  = NULL; //21f66a94
static uint8_t                usbMscRxBuffer[512]; //21f66bec
static uint8_t                usbMscTxBuffer[512]; //21f66dec
static MGC_MsdCbw        usbMscCBW = { //21f66acc
                             0,0,0,0,0,0,
                              {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
                         };
static MGC_MsdCsw usbMscCSW = { MGC_MSD_BOT_CSW_SIGNATURE, 0, 0, 0 }; //21efc738

/*static*/ FM_USB_MSC_RBC_MODE_S usbMscRbcMode = //21efc690
      { RBC_MODE_LENGTH-1,
        0, 0, 0,
        0x80 | RBC_MODE_PAGE_CODE,
        RBC_MODE_PAGE_LENGTH,
        0,          // write cache disable
        0, 0,       // block size
        {0,0,0,0,0},// block capacity
        0xff,       // full performance
        0x00,       // read enabled/write disabled/format enabled
        0
      };

static FM_USB_MSC_RBC_SENSE_S usbMscRbcSense = { //21efc6a4
      0xF0,0,0,0,0,0,0,0x0A,0,0,0,0,0,0,0,0,0,0 };

static MUSB_FunctionClient mscFunctionClient = { //21f66a9c
    NULL,     // pPrivateData
    NULL,     // pStandardDescriptors
    0,        // wDescriptorBufferLength
    0,        // wStringDescriptorCount
    NULL,     // pHighSpeedDescriptors
    0,        // wHighSpeedDescriptorLength
    0,        // wControlBufferLength
    NULL,     // pControlBuffer
    NULL,     // pbSelfPowered
    NULL,     // pfDeviceRequest
    NULL,     // pfDeviceConfigSelected
    NULL,     // pfInterfaceSet
    NULL      // pfUsbState
};

/*static*/ MUSB_Irp usbMscDataIrp[4] = { //21efc6b8
    {NULL,                          // hPipe (set with confguration)
     usbMscTxBuffer,                // pBuffer
     sizeof(usbMscTxBuffer),        // dwLength
     0,                             // dwStatus
     0,                             // dwActualLength
     cbTxDataTransferComplete,      // pfIrpComplete
     NULL,                          // pCompleteParam
     FFALSE,                        // bAllowShortTransfer
     FFALSE,                        // bIsrCallback
     FFALSE                         // bAllowDma
    },
    {NULL,                          // hPipe (set with confguration)
     usbMscRxBuffer,                // pBuffer
     sizeof(usbMscRxBuffer),        // dwLength
     0,                             // dwStatus
     0,                             // dwActualLength
     cbRxDataTransferComplete,      // pfIrpComplete
     NULL,                          // pCompleteParam
     FFALSE,                        // bAllowShortTransfer
     FFALSE,                        // bIsrCallback
     FFALSE                         // bAllowDma
    },
    {NULL,                          // will be set hPipe (set with confguration)
     (uint8_t*)(&usbMscCBW),             // pBuffer
     MGC_MSD_BOT_CBW_LENGTH,        // dwLength
     0,                             // dwStatus
     0,                             // dwActualLength
     cbRxCbwTransferComplete,       // pfIrpComplete
     NULL,                          // pCompleteParam
     FFALSE,                        // bAllowShortTransfer
     FFALSE,                        // bIsrCallback
     FFALSE                         // bAllowDma
    },
    {NULL,                          // hPipe (set with confguration)
     (uint8_t*)(&usbMscCSW),             // pBuffer
     MGC_MSD_BOT_CSW_LENGTH,        // dwLength
     0,                             // dwStatus
     0,                             // dwActualLength
     cbTxCswTransferComplete,       // pfIrpComplete
     NULL,                          // pCompleteParam
     FFALSE,                        // bAllowShortTransfer
     FFALSE,                        // bIsrCallback
     FFALSE                         // bAllowDma
    }
};


typedef struct
{
    uint16_t    vendorID;          //!< The Vendor ID to transmit with the USB device descriptor. If NULL #FM_USB_VENDOR_ID will be assigned.
    uint16_t    productID;         //!< The product ID to transmit with the USB device descriptor. If NULL #FM_USB_MSC_PRODUCT_ID will be assigned.
    char*  pVendorName;       //!< The optional name of the vendor to transmit with the USB device descriptor. NULL if not needed.
    char*  pProductName;      //!< The optional name of the product to transmit with the USB device descriptor. NULL if not needed.
    char*  pSerialNumber;     //!< The optional serial number string to transmit with the USB device descriptor. NULL if not needed.
    char** ppDeviceNames;     //!< Pointer to NULL terminated array of IDE device names to be accessed via USB. For example {"ide0", NULL}.
    FBOOL  readOnly;          //!< #FTRUE if the driver should prevent write access to the device(s).
} FM_USB_MSC_INIT_PARAMS_S;



/* 21c40e4c - todo */
//FERR FM_USB_MSC_Init( FM_USB_MSC_INIT_PARAMS_S* paramsP )
int32_t FAPI_USB_FUNC_MSC_Init(FAPI_USB_FUNC_MSC_InitParamsT* paramsP)
{
    FERR err = 0; //FD_OK;
//    FDRV_S* pDriver;
    /*uint8_t*/uint32_t ii;
    /*uint8_t*/uint32_t lun;

    if ( !paramsP )
        return( -27001 ); //FD_ERR_BAD_PARAMETER );

    if ( !paramsP->lunCount || !paramsP->lunList )
        return( -27001 ); //FD_ERR_BAD_PARAMETER );

    if ( isInitialized )
        return( -27003 ); //FD_ERR_ALREADY_INITIALIZED );

    //---------------------------------
    // Check vendor and product ID
    //---------------------------------
    if ( !paramsP->vendorID  ) { // use Fujitsu vendor and product settings
        paramsP->vendorID     = FAPI_USB_FUNC_VENDOR_ID;
        paramsP->productID    = FAPI_USB_FUNC_MSC_PRODUCT_ID;
        paramsP->vendorNamePtr  = FAPI_USB_FUNC_VENDOR_NAME;
        paramsP->productNamePtr = FAPI_USB_FUNC_MSC_PRODUCT_NAME;
        paramsP->serialNumberPtr= FAPI_USB_FUNC_MSC_SERIAL_NUMBER;
    }

    if ( paramsP->vendorID == FAPI_USB_FUNC_VENDOR_ID ) {
        if ( paramsP->productID != FAPI_USB_FUNC_MSC_PRODUCT_ID ||
            (paramsP->vendorNamePtr && strcmp( paramsP->vendorNamePtr, FAPI_USB_FUNC_VENDOR_NAME )) ||
            (paramsP->productNamePtr && strcmp( paramsP->productNamePtr, FAPI_USB_FUNC_MSC_PRODUCT_NAME )) ||
            (paramsP->serialNumberPtr && strcmp( paramsP->serialNumberPtr, FAPI_USB_FUNC_MSC_SERIAL_NUMBER ))
        ) return( -1 ); //FD_ERR_BAD_PARAMETER );
    }

    if (paramsP->lunCount > 1)
    {
       if ((paramsP->serialNumberPtr == 0) ||
             (paramsP->serialNumberPtr[0] == 0))
       {
          FAPI_SYS_PRINT_MSG("WARNING: no serial number given for multi LUN device, maybe Windows doesn't work correctly.\n");
       }
    }
    //21c40f10
    usbMscRbcMode.dataLength = RBC_MODE_LENGTH-1;
    usbMscRbcMode.mediumType = 0;
    usbMscRbcMode.deviceParameter = 0;
    usbMscRbcMode.blockDescriptorLength = 0;
    usbMscRbcMode.pageCode = 0x80 | RBC_MODE_PAGE_CODE;
    usbMscRbcMode.pageLength = RBC_MODE_PAGE_LENGTH;
    usbMscRbcMode.wcd = 0;
    usbMscRbcMode.blockLengthHighByte = 0;
    usbMscRbcMode.blockLengthLowByte = 0;
    musb_memset(usbMscRbcMode.blockCount, 0, 5);
    usbMscRbcMode.powerPerformance = 0xff;
    usbMscRbcMode.flags = 0x00;
    usbMscRbcMode.reserved = 0;

    usbMscRbcSense.responseCode = 0xF0;
    usbMscRbcSense.obsolete = 0;
    usbMscRbcSense.senseKey = 0;
    usbMscRbcSense.info_31_24 = 0;
    usbMscRbcSense.info_23_16 = 0;
    usbMscRbcSense.info_15_8 = 0;
    usbMscRbcSense.info_7_0 = 0;
    usbMscRbcSense.additionalLength = 10;
    usbMscRbcSense.commandInfo_31_24 = 0;
    usbMscRbcSense.commandInfo_23_16 = 0;
    usbMscRbcSense.commandInfo_15_8 = 0;
    usbMscRbcSense.commandInfo_7_0 = 0;
    usbMscRbcSense.asc = 0;
    usbMscRbcSense.ascq = 0;
    usbMscRbcSense.fruCode = 0;
    usbMscRbcSense.keyFlags_0 = 0;
    usbMscRbcSense.keyFlags_1 = 0;
    usbMscRbcSense.keyFlags_2 = 0;

    //---------------------------------
    // Initialize clients registration data
    //---------------------------------
    usbMscSelfPowered = FTRUE;

    mscFunctionClient.pPrivateData            = NULL;

    err = getDescriptors( FFALSE,
                          paramsP->vendorID,
                          paramsP->productID,
                          paramsP->vendorNamePtr,
                          paramsP->productNamePtr,
                          paramsP->serialNumberPtr,
                          (uint8_t**)&mscFunctionClient.pStandardDescriptors,
                          &mscFunctionClient.wDescriptorBufferLength
                        );

    if ( err != /*FD_OK*/0 ) return( err );

    if ( strlen(paramsP->vendorNamePtr)>0 )
        mscFunctionClient.wStringDescriptorCount++;
    if ( strlen(paramsP->productNamePtr)>0 )
        mscFunctionClient.wStringDescriptorCount++;
    if ( strlen(paramsP->serialNumberPtr)>0 )
        mscFunctionClient.wStringDescriptorCount++;

    err = getDescriptors( FTRUE,
                          0,
                          0,
                          NULL,
                          NULL,
                          NULL,
                          (uint8_t**)&mscFunctionClient.pHighSpeedDescriptors,
                          &mscFunctionClient.wHighSpeedDescriptorLength
                        );
    if ( err != /*FD_OK*/0 ) return( err );

    mscFunctionClient.wControlBufferLength       = USB_MSC_CONTROL_BUFFER_LENGTH;
    mscFunctionClient.pControlBuffer             = usbMscControlBuffer;
    mscFunctionClient.pbSelfPowered              = &usbMscSelfPowered;
    mscFunctionClient.pfDeviceRequest            = cbDeviceRequest;
    mscFunctionClient.pfDeviceConfigSelected     = cbDeviceConfigSelected;
    mscFunctionClient.pfInterfaceSet             = cbInterfaceSet;
    mscFunctionClient.pfUsbState                 = cbNewUsbState;

    //------------------------------
    // Create device list
    //------------------------------
#if 0
    lun=0;
    while( paramsP->ppDeviceNames[lun] ) lun++;
#endif

    pUsbMscDevices = (FM_USB_MSC_DEVICE_S*)MUSB_MemAlloc( paramsP->lunCount*sizeof(FM_USB_MSC_DEVICE_S) );
    if ( !pUsbMscDevices) return( -2 ); //FD_ERR_OUT_OF_MEMORY );
    MUSB_MemSet( pUsbMscDevices, 0, paramsP->lunCount*sizeof(FM_USB_MSC_DEVICE_S));

    usbMscMaxLun = 0;

    for (lun = 0; lun < paramsP->lunCount; lun++) {

       if ((paramsP->lunList[lun].type < FAPI_USB_FUNC_MSC_LUN_RAM) ||
             (paramsP->lunList[lun].type > FAPI_USB_FUNC_MSC_LUN_HDD))
       {
       }
       else if ((paramsP->lunList[lun].type == FAPI_USB_FUNC_MSC_LUN_HDD) &&
                (paramsP->lunList[lun].ataHandle == NULL))
       {
          //->21c41130
//          continue;
       }
          //21c41160
       else if (paramsP->lunList[lun].lbaCapacity == 0)
       {
          //->21c41130
//          continue;
       }
       else if (paramsP->lunList[lun].blockLength == 0)
       {
          //->21c41130
//          continue;
       }
       else
       {
          if (paramsP->lunList[lun].type == FAPI_USB_FUNC_MSC_LUN_RAM)
          {
             //21c41564
             pUsbMscDevices[usbMscMaxLun].type = paramsP->lunList[lun].type;
             pUsbMscDevices[usbMscMaxLun].firstLba = 0;
             pUsbMscDevices[usbMscMaxLun].bufferLength = paramsP->lunList[lun].startAddrOrLba;
             pUsbMscDevices[usbMscMaxLun].bData_36 |= (1 << 0);
          }
          //21c41180
          if (paramsP->lunList[lun].type == FAPI_USB_FUNC_MSC_LUN_HDD)
          {
             //21c414fc
             pUsbMscDevices[usbMscMaxLun].type = paramsP->lunList[lun].type;
             pUsbMscDevices[usbMscMaxLun].firstLba = paramsP->lunList[lun].startAddrOrLba;
             pUsbMscDevices[usbMscMaxLun].rwAddr = paramsP->lunList[lun].blockLength;
             pUsbMscDevices[usbMscMaxLun].ataHandle = paramsP->lunList[lun].ataHandle;
             pUsbMscDevices[usbMscMaxLun].Data_28 = func_21c0ab7c;
             pUsbMscDevices[usbMscMaxLun].bData_36 |= (1 << 0);
          }
          //21c41188
          pUsbMscDevices[usbMscMaxLun].lbaCapacity = paramsP->lunList[lun].lbaCapacity;
          pUsbMscDevices[usbMscMaxLun].blockLength = paramsP->lunList[lun].blockLength;
          pUsbMscDevices[usbMscMaxLun].bData_36 =
                (pUsbMscDevices[usbMscMaxLun].bData_36 & ~(1 << 2)) |
                ((paramsP->lunList[lun].readOnly & 1) << 2);
          pUsbMscDevices[usbMscMaxLun].Data_32 = 0;
          pUsbMscDevices[usbMscMaxLun].bData_36 |= (1 << 1);
          pUsbMscDevices[usbMscMaxLun].bData_36 &= ~(1 << 3);
          pUsbMscDevices[usbMscMaxLun].inquiryData[0] = 0;
          pUsbMscDevices[usbMscMaxLun].inquiryData[1] = 0x80;
          pUsbMscDevices[usbMscMaxLun].inquiryData[2] = 0;
          pUsbMscDevices[usbMscMaxLun].inquiryData[3] = 0;
          pUsbMscDevices[usbMscMaxLun].inquiryData[4] = 0x1F;
          pUsbMscDevices[usbMscMaxLun].inquiryData[5] = 0;
          pUsbMscDevices[usbMscMaxLun].inquiryData[6] = 0;
          pUsbMscDevices[usbMscMaxLun].inquiryData[7] = 0;

          if (paramsP->lunList[lun].namePtr != 0)
          {
             strncpy( (char*)(&pUsbMscDevices[usbMscMaxLun].inquiryData[8]),
                   (const char*)paramsP->lunList[lun].namePtr, 24 );
          }
          for( ii=8+strlen((const char*)(&pUsbMscDevices[usbMscMaxLun].inquiryData[8]));ii<=31;ii++ ) {
              pUsbMscDevices[usbMscMaxLun].inquiryData[ii] = ' ';
          }

          pUsbMscDevices[usbMscMaxLun].inquiryData[32] = 0;
          pUsbMscDevices[usbMscMaxLun].inquiryData[33] = 0x30;
          pUsbMscDevices[usbMscMaxLun].inquiryData[34] = 0;
          pUsbMscDevices[usbMscMaxLun].inquiryData[35] = 0x30 + usbMscMaxLun;

          usbMscMaxLun++;
       }
        //21c41130
//        lun++;
    }

    //21c414a4
    if ( !usbMscMaxLun ) {
      /*MUSB_MemFree*/FAPI_SYS_Services.freeFunc( pUsbMscDevices );
      #ifdef FM_USB_MSC_DEBUG
          FM_Printf("ERROR: No disk drive detected.\n");
      #endif
        return( -1 ); //FD_ERR_BAD_PARAMETER );
    } else {
        #ifdef FM_USB_MSC_DEBUG
            FM_Printf("%d disk drive(s) detected.\n", usbMscMaxLun);
        #endif
        usbMscMaxLun -= 1;
    }

    isInitialized = FTRUE;

    return( 0 ); //FD_OK );
}


/* 21c405a8 - complete */
//MUSB_FunctionClient* FM_USB_MSC_GetFunctionClient(void)
MUSB_FunctionClient* FAPI_USB_FUNC_MSC_GetFunctionClient(void)
{
    if ( !isInitialized  )
        return( NULL );

    return(&mscFunctionClient);
}


/* 21c405c8 - complete */
uint32_t FAPI_USB_FUNC_MSC_Lock(void)
{
   uint32_t res = 1;
   uint8_t i = 0;

   FAPI_SYS_LOCK_SCHEDULER;

   if (isInitialized)
   {
      for (i = 0; i <= usbMscMaxLun; i++)
      {
         uint8_t r0 = pUsbMscDevices[i].bData_36 & (1 << 3);

         if (r0)
         {
            res = 0;
            break;
         }
      }

      if (res)
      {
         for (i = 0; i <= usbMscMaxLun; i++)
         {
            pUsbMscDevices[i].Data_32++;
         }
      }
   }
   else
   {
      res = 0;
   }

   FAPI_SYS_UNLOCK_SCHEDULER;

   return res;
}


/* 21c406b4 - complete */
void FAPI_USB_FUNC_MSC_Unlock(void)
{
   uint8_t i = 0;

   FAPI_SYS_LOCK_SCHEDULER;

   if (isInitialized)
   {
      for (i = 0; i <= usbMscMaxLun; i++)
      {
         if (pUsbMscDevices[i].Data_32 != 0)
         {
            pUsbMscDevices[i].Data_32--;
         }
      }
   }

   FAPI_SYS_UNLOCK_SCHEDULER;
}


/* 21c40750 - complete */
void FAPI_USB_FUNC_MSC_SetHostNotifyFct(FAPI_USB_FUNC_MSC_HostNotifyFuncT notifyFct)
{
   usbMscNotifyFct = notifyFct;
}


/* 21c40788 - complete */
static uint8_t cbInterfaceSet( void* pPrivateData,
                          MUSB_BusHandle busHandle,
                          uint8_t interfaceNumber,
                          uint8_t alternateSetting,
                          MUSB_Pipe* pPipes
                        )
{
    #ifdef FM_USB_MSC_DEBUG
        FM_PrCStrHex8("SET_INTERFACE request received for interface ", interfaceNumber);
        FM_PrStr( ".\n" );
    #endif
    return( FTRUE );
}


/* 21c40790 - complete */
static void cbNewUsbState( void* pPrivateData,
                           MUSB_BusHandle busHandle,
                           MUSB_State state
)
{
    uint8_t i = 0;

    switch( state ) {
        case 0: //MUSB_POWER_OFF:
             //#ifdef FM_USB_MSC_DEBUG
                 FAPI_SYS_PRINT_MSG( "[usb] entering POWER_OFF state!\n" );
             //#endif
             break;
        case 1: //MUSB_POWERED:
             //#ifdef FM_USB_MSC_DEBUG
                 FAPI_SYS_PRINT_MSG( "[usb] entering POWERED state!\n" );
             //#endif
             break;
        case 2: //MUSB_DEFAULT:
             //#ifdef FM_USB_MSC_DEBUG
                 FAPI_SYS_PRINT_MSG( "[usb] entering DEFAULT state!\n" );

                 for (i = 0; i <= usbMscMaxLun; i++)
                 {
                    pUsbMscDevices[i].bData_36 &= ~8;
                 }

                 if (usbMscNotifyFct)
                 {
                    (usbMscNotifyFct)(0);
                 }
             //#endif
             break;
        case 3: //MUSB_ADDRESS:
             //#ifdef FM_USB_MSC_DEBUG
                 FAPI_SYS_PRINT_MSG( "[usb] entering ADDRESS state!\n" );
             //#endif
             break;
        case 4: //MUSB_CONFIGURED:
             //#ifdef FM_USB_MSC_DEBUG
                 FAPI_SYS_PRINT_MSG( "[usb] entering CONFIGURED state!\n" );
             //#endif
             break;
        case 5: //MUSB_SUSPENDED:
             //#ifdef FM_USB_MSC_DEBUG
                 FAPI_SYS_PRINT_MSG( "[usb] entering SUSPEND state!\n" );
             //#endif
             break;
        default:
             //#ifdef FM_USB_MSC_DEBUG
           FAPI_SYS_PRINT_MSG( "[usb] entering UNKNOWN state %d\n", state );
             //#endif
             ;
    }
}


#define MUSB_DT_DEVICE 0x01
#define MUSB_DT_CONFIG 0x02
#define MUSB_DT_STRING 0x03
#define MUSB_DT_INTERFACE 0x04
#define MUSB_DT_ENDPOINT 0x05
#define MUSB_DT_DEVICE_QUALIFIER 0x06
#define MUSB_DT_OTG 0x09

#define MUSB_DT_OTG_SIZE 3
#define MUSB_DT_CONFIG_SIZE 9
#define MUSB_DT_OTHER_SPEED 7
#define MUSB_DT_INTERFACE_SIZE 9
#define MUSB_DT_ENDPOINT_SIZE 7
#define MUSB_DT_DEVICE_QUALIFIER_SIZE 10
#define MUSB_DT_DEVICE_SIZE 18
#define MUSB_OTG_SRP (1 << 0)
#define MUSB_OTG_HNP (1 << 1)


/* 21c40910 - complete */
static FERR getDescriptors( FBOOL hsFlag,
                            uint16_t   vendorID,
                            uint16_t   productID,
                            const char* pVendorName,
                            const char* pProductName,
                            const char* pSerialNumber,
                            uint8_t** ppDescriptors,
                            uint16_t* pLength
                          )
{
    MUSB_DeviceDescriptor        device; //sp4
    MUSB_QualifierDescriptor     devqual; //sp64
    MUSB_ConfigurationDescriptor config; //sp52
    MUSB_InterfaceDescriptor     interface; //sp40
    MUSB_EndpointDescriptor      ep[2]; //sp24
    uint8_t string0[4] =  {
                       4,
                       MUSB_DT_STRING,
                       0x09, // Language code: English
                       0x04  // Sub code: US
                     };
    uint8_t* pDescriptor = NULL;
    uint8_t* pData = NULL;
    uint16_t length = 0;
    uint8_t  strIndex = 0;
    uint16_t ii;

    *ppDescriptors = NULL;
    *pLength =  0;

    //-------------------------------
    // Fill descriptors
    //-------------------------------
    length = MUSB_DT_OTG_SIZE;

    //---------------
    // Configuration
    //---------------
    length += MUSB_DT_CONFIG_SIZE;
    config.bLength             = MUSB_DT_CONFIG_SIZE;
    config.bDescriptorType     = hsFlag==FTRUE ? MUSB_DT_OTHER_SPEED : MUSB_DT_CONFIG;
    config.wTotalLength        = MUSB_DT_CONFIG_SIZE        +
                                 MUSB_DT_INTERFACE_SIZE     +
                                 2* MUSB_DT_ENDPOINT_SIZE   +
                                 MUSB_DT_OTG_SIZE;
    config.bNumInterfaces      = 1;
    config.bConfigurationValue = 1;
    config.iConfiguration      = 0;
    config.bmAttributes        = 0xc0; // own power supply
    config.bMaxPower           = 0x02; // 1mA default ?????

    //---------------
    // Interface
    //---------------
    length += MUSB_DT_INTERFACE_SIZE;
    interface.bLength            = MUSB_DT_INTERFACE_SIZE;
    interface.bDescriptorType    = MUSB_DT_INTERFACE;
    interface.bInterfaceNumber   = 0;
    interface.bAlternateSetting  = 0;
    interface.bNumEndpoints      = 2;
    interface.bInterfaceClass    = 0x08; // Mass storage class
    interface.bInterfaceSubClass = 0x06; // SCSI commands
    interface.bInterfaceProtocol = 0x50; // Bulk-only protocol
    interface.iInterface         = 0;


    //---------------
    // Endpoint
    //---------------
    length += MUSB_DT_ENDPOINT_SIZE;
    ep[EP_BULK_IN].bLength          = MUSB_DT_ENDPOINT_SIZE;
    ep[EP_BULK_IN].bDescriptorType  = MUSB_DT_ENDPOINT;
#if 0
    ep[EP_BULK_IN].bEndpointAddress = hsFlag==FTRUE ? 0x83 : 0x81; // Bulk-IN
#else
    ep[EP_BULK_IN].bEndpointAddress = 0x81; // Bulk-IN
#endif
    ep[EP_BULK_IN].bmAttributes     = 0x02; // Bulk-Transfer
    ep[EP_BULK_IN].wMaxPacketSize   = hsFlag==FTRUE ? 512 : 64;
    ep[EP_BULK_IN].bInterval        = 0;    // ??????

    length += MUSB_DT_ENDPOINT_SIZE;
    ep[EP_BULK_OUT].bLength          = MUSB_DT_ENDPOINT_SIZE;
    ep[EP_BULK_OUT].bDescriptorType  = MUSB_DT_ENDPOINT;
#if 0
    ep[EP_BULK_OUT].bEndpointAddress = hsFlag==FTRUE ? 0x03 : 0x01; // Bulk-OUT
#else
    ep[EP_BULK_OUT].bEndpointAddress = 0x01; // Bulk-OUT
#endif
    ep[EP_BULK_OUT].bmAttributes     = 0x02; // Bulk-Transfer
    ep[EP_BULK_OUT].wMaxPacketSize   = hsFlag==FTRUE ? 512 : 64;
    ep[EP_BULK_OUT].bInterval        = 0;    // ??????


    if ( hsFlag == FTRUE ) {

        //---------------
        // Device qualifier
        //---------------
        length += MUSB_DT_DEVICE_QUALIFIER_SIZE;
        devqual.bLength            = MUSB_DT_DEVICE_QUALIFIER_SIZE;
        devqual.bDescriptorType    = MUSB_DT_DEVICE_QUALIFIER;
        devqual.bcdUSB             = 0x0200;
        devqual.bDeviceClass       = 0;
        devqual.bDeviceSubClass    = 0;
        devqual.bDeviceProtocol    = 0;
        devqual.bMaxPacketSize0    = 0x40;
        devqual.bNumConfigurations = 1;
        devqual.bRESERVED          = 0;

    } else {

        //---------------
        // Device
        //---------------
        length += MUSB_DT_DEVICE_SIZE;
        device.bLength              = MUSB_DT_DEVICE_SIZE;
        device.bDescriptorType      = MUSB_DT_DEVICE;
        device.bcdUSB               = 0x0200;
        device.bDeviceClass         = 0;
        device.bDeviceSubClass      = 0;
        device.bDeviceProtocol      = 0;
        device.bMaxPacketSize0      = 0x40;
        device.idVendor             = vendorID;
        device.idProduct            = productID;
        device.bcdDevice            = 0x0100;
        if ( pVendorName && *pVendorName ) {
            device.iManufacturer    = ++strIndex;
            length += strlen(pVendorName)*2 + 2;
        } else {
            device.iManufacturer    = 0;
        }
        if ( pProductName && *pProductName ) {
            device.iProduct         = ++strIndex;
            length += strlen(pProductName)*2 + 2;
        } else {
            device.iProduct         = 0;
        }
        if ( pSerialNumber && *pSerialNumber ) {
            device.iSerialNumber    = ++strIndex;
            length += strlen(pSerialNumber)*2 + 2;
        } else {
            device.iSerialNumber    = 0;
        }
        device.bNumConfigurations   = 1;
    }

    if ( strIndex > 0 ) length += sizeof(string0);

    //-------------------------------
    // Allocate descriptor buffer
    //-------------------------------
    pDescriptor = (uint8_t*)MUSB_MemAlloc( length );
    if ( pDescriptor == NULL ) return(-2); //FD_ERR_OUT_OF_MEMORY);
    *ppDescriptors = pDescriptor;
    *pLength = length;

    //-------------------------------
    // Copy descriptors to buffer
    //-------------------------------

    if ( hsFlag == FTRUE ) {

        //---------------
        // Device qualifier
        //---------------
        pData = (uint8_t*)&devqual;
        for( ii=0; ii<MUSB_DT_DEVICE_QUALIFIER_SIZE; ii++ ) {
            *pDescriptor++ = *pData++;
        }

    } else {

        //---------------
        // Device
        //---------------
        pData = (uint8_t*)&device;
        for( ii=0; ii<MUSB_DT_DEVICE_SIZE; ii++ ) {
            *pDescriptor++ = *pData++;
        }

        //---------------
        // String
        //---------------
        if ( strIndex > 0 ) {
            for( ii=0; ii<4; ii++ ) {
                *pDescriptor++ = string0[ii];
            }
            if ( pVendorName && *pVendorName ) {
                *pDescriptor++ = strlen(pVendorName)*2 + 2;
                *pDescriptor++ = MUSB_DT_STRING;
                for( ii=0; ii<strlen(pVendorName); ii++) {
                    *pDescriptor++ = pVendorName[ii];
                    *pDescriptor++ = 0;
                }
            }
            if ( pProductName && *pProductName ) {
                *pDescriptor++ = strlen(pProductName)*2 + 2;
                *pDescriptor++ = MUSB_DT_STRING;
                for( ii=0; ii<strlen(pProductName); ii++) {
                    *pDescriptor++ = pProductName[ii];
                    *pDescriptor++ = 0;
                }
            }
            if ( pSerialNumber && *pSerialNumber ) {
                *pDescriptor++ = strlen(pSerialNumber)*2 + 2;
                *pDescriptor++ = MUSB_DT_STRING;
                for( ii=0; ii<strlen(pSerialNumber); ii++) {
                    *pDescriptor++ = pSerialNumber[ii];
                    *pDescriptor++ = 0;
                }
            }
        }
    }

    //---------------
    // Configuration
    //---------------
    pData = (uint8_t*)&config;
    for( ii=0; ii<MUSB_DT_CONFIG_SIZE; ii++ ) {
        *pDescriptor++ = *pData++;
    }

    //---------------
    // Interface
    //---------------
    pData = (uint8_t*)&interface;
    for( ii=0; ii<MUSB_DT_INTERFACE_SIZE; ii++ ) {
        *pDescriptor++ = *pData++;
    }

    //---------------
    // Endpoint
    //---------------
    pData = (uint8_t*)&ep[EP_BULK_IN];
    for( ii=0; ii<MUSB_DT_ENDPOINT_SIZE; ii++ ) {
        *pDescriptor++ = *pData++;
    }

    pData = (uint8_t*)&ep[EP_BULK_OUT];
    for( ii=0; ii<MUSB_DT_ENDPOINT_SIZE; ii++ ) {
        *pDescriptor++ = *pData++;
    }

    //---------------
    // OTG
    //---------------
    *pDescriptor++ = MUSB_DT_OTG_SIZE;
    *pDescriptor++ = MUSB_DT_OTG;
    *pDescriptor++ = (MUSB_OTG_SRP | MUSB_OTG_HNP);
    return( 0 ); //FD_OK );
}


static void usbMscProcessCbw(MGC_MsdCbw* pCBW)
{
    uint8_t    cmdLength = pCBW->bCbwCbLength & 0x1f;
    uint8_t    lun       = pCBW->bCbwLun & 0x0f;
    uint8_t*   pCmd      = pCBW->aCbwCb;
    uint8_t    opCode    = pCmd[0];
    FBOOL inFlag    = pCBW->bmCbwFlags>>7;

    //---------------------------
    // Prepare status
    //---------------------------
    usbMscCSW.dCswTag         = pCBW->dCbwTag;
    usbMscCSW.dCswDataResidue = pCBW->dCbwDataTransferLength;
    usbMscCSW.bCswStatus      = MGC_MSD_BOT_COMMAND_PASSED;

    if ( opCode != MGC_SCSI_REQUEST_SENSE ) {
        usbMscRbcSense.senseKey   = MGC_SCSI_SC_NONE;
        usbMscRbcSense.asc        = 0;
        usbMscRbcSense.ascq       = 0;
    }

    //---------------------------
    // Check CBW content
    //---------------------------
    if ( lun > usbMscMaxLun         ||
         pCBW->bCbwLun & ~0x0f      ||
         pCBW->bCbwCbLength & ~0x1f
    ) {
        usbMscRbcSense.senseKey = MGC_SCSI_SC_ILLEGAL_REQUEST;
        usbMscRbcSense.asc      = RBC_SENSE_ASC_LUN_NOT_SUPPORTED;
        usbMscCSW.bCswStatus    = MGC_MSD_BOT_PHASE_ERROR;
    }

    //---------------------------
    // Decode RBC command
    //---------------------------
    switch( opCode ) {

        case 0x12: //MGC_SCSI_INQUIRY:
             if ( cmdLength >= 6 )
                 usbMscRbcInquiry(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 0x15: //MGC_SCSI_MODE_SELECT:
             if ( cmdLength >= 6 )
                 usbMscRbcModeSelect(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 0x1a: //MGC_SCSI_MODE_SENSE:
             if ( cmdLength >= 6 )
                 usbMscRbcModeSense6(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 0x5a: //MGC_SCSI_MODE_SENSE10:
             if ( cmdLength >= 10 )
                 usbMscRbcModeSense10(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 0x28: //MGC_SCSI_READ10:
             if ( cmdLength >= 10 )
                 usbMscRbcRead(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 0x25: //MGC_SCSI_READ_CAPACITY:
             if ( cmdLength >= 10 )
                 usbMscRbcReadCapacity(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 0x23: //MGC_SCSI_RD_FMT_CAPC:
             if ( cmdLength >= 10 )
                 usbMscRbcReadFormatCapacities(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 0x1b: //MGC_SCSI_START_STOP_UNIT:
             if ( cmdLength >= 6 )
                 usbMscRbcStartStopUnit(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 0: //MGC_SCSI_TEST_UNIT_READY:
             //21c41d28
             if ( cmdLength >= 6 )
                 usbMscRbcTestUnitReady(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 0x2f: //MGC_SCSI_VERIFY:
             if ( cmdLength >= 10 )
                 usbMscRbcVerify(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 0x2a: //MGC_SCSI_WRITE10:
             if ( cmdLength >= 10 )
                 usbMscRbcWrite(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 0x3b: //MGC_SCSI_WRITE_BUFFER:
             if ( cmdLength >= 10 )
                 usbMscRbcWriteBuffer(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 3: //MGC_SCSI_REQUEST_SENSE:
             if ( cmdLength >= 6 )
                 usbMscRbcRequestSense(lun, pCmd);
             else
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
             break;

        case 4: //MGC_SCSI_FORMAT_UNIT:
             if ( cmdLength < 6 ) {
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
                 break;
             }

        case 0x1e: //MGC_SCSI_PREVENT_ALLOW_MED_REMOVE:
             if ( cmdLength < 6 ) {
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
                 break;
             }

        case 0x17: //MGC_SCSI_RELEASE:
             if ( cmdLength != 6 ) {
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
                 break;
             }

        case 0x16: //MGC_SCSI_RESERVE:
             if ( cmdLength < 6 ) {
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
                 break;
             }

        case 0x35: //MGC_SCSI_SYNC_CACHE:
             if ( cmdLength < 10 ) {
                 usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
                 break;
             }
             usbMscRbcOptionalCmd(lun, pCmd);
             break;

        default:
             #ifdef FM_USB_MSC_DEBUG
                 FM_PrCStrHex8("Unknown RBC command 0x", opCode);
                 FM_PrStr("!!!\n");
             #endif
             usbMscCSW.bCswStatus = MGC_MSD_BOT_PHASE_ERROR;
    }

    if ( pCBW->dCbwDataTransferLength && usbMscCSW.bCswStatus == MGC_MSD_BOT_COMMAND_PASSED ) {
        // Start data transfer

        if ( inFlag ) {
            usbMscDataIrp[IRP_DATA_IN].dwActualLength = 0;
            MUSB_StartTransfer(&usbMscDataIrp[IRP_DATA_IN]);
        } else {
            usbMscDataIrp[IRP_DATA_OUT].dwActualLength = 0;
            MUSB_StartTransfer(&usbMscDataIrp[IRP_DATA_OUT]);
        }
    } else {
        if ( usbMscCSW.bCswStatus != MGC_MSD_BOT_COMMAND_PASSED && pCBW->dCbwDataTransferLength ) {
            if ( inFlag ) { // case 7
                MUSB_SetPipeHalt(usbMscDataIrp[IRP_DATA_IN].hPipe, FTRUE);
                usbMscDataIrp[IRP_CSW].dwActualLength = 0;
                MUSB_StartTransfer(&usbMscDataIrp[IRP_CSW]);

            } else { // case 13
                // prematurely stall the Bulk-OUT pipe
                MUSB_SetPipeHalt(usbMscDataIrp[IRP_DATA_OUT].hPipe, FTRUE);
                usbMscDataIrp[IRP_CSW].dwActualLength = 0;
                MUSB_StartTransfer(&usbMscDataIrp[IRP_CSW]);
            }
        } else {

            // No data, so start CSW transfer immediately
            usbMscDataIrp[IRP_CSW].dwActualLength = 0;
            MUSB_StartTransfer(&usbMscDataIrp[IRP_CSW]);
        }
    }
}


static void usbMscRbcTestUnitReady(uint8_t lun, uint8_t* pCmd)
{
    #ifdef FM_USB_MSC_DEBUG
        FM_PrCStrHex2("Received TEST UNIT READY command for unit ", lun);
        FM_PrStr("\n");
    #endif

//    if ( pUsbMscDevices[lun].ready ) {
    if ( pUsbMscDevices[lun].bData_36 & 2 ) {
        usbMscCSW.bCswStatus = MGC_MSD_BOT_COMMAND_PASSED;
    } else {
        usbMscRbcSense.senseKey = MGC_SCSI_SC_NOT_READY;
        usbMscRbcSense.asc      = MGC_SCSI_ASC_UNIT_NOT_READY;
        usbMscCSW.bCswStatus    = MGC_MSD_BOT_COMMAND_FAILED;
    }
}


/* v3.8: 413100 - complete */
/* 21c4257c - complete */
/*static*/ uint8_t cbDeviceRequest( void* pPrivateData,
                           MUSB_BusHandle busHandle,
                           uint32_t sequenceNum,
                           const uint8_t* pRequestData,
                           uint16_t length
                         )
{
    uint32_t stat = 0;

    MUSB_DeviceRequest* pDevRequest = (MUSB_DeviceRequest*)pRequestData;
    #ifdef FM_USB_MSC_DEBUG
        FM_PrStr("SETUP request received!\n");
    #endif

    if ( (pDevRequest->bmRequestType & MUSB_TYPE_MASK) == MUSB_TYPE_CLASS ) {
        #ifdef FM_USB_MSC_DEBUG
            FM_PrCStrHex2( "Class Request: ", pDevRequest->bRequest );
            FM_PrStr("\n");
        #endif
        switch ( pDevRequest->bRequest ) {
            case 0xfe: //FM_USB_MSC_REQ_GET_MAX_LUN:
                stat = MUSB_DeviceResponse( busHandle,
                                            sequenceNum,
                                            &usbMscMaxLun,
                                            1,
                                            FFALSE
                                          );
                break;
            case 0xff: //FM_USB_MSC_REQ_RESET:
                // cancel pending transfers
                MUSB_CancelTransfer(&usbMscDataIrp[IRP_DATA_IN]);
                MUSB_CancelTransfer(&usbMscDataIrp[IRP_DATA_OUT]);
                MUSB_CancelTransfer(&usbMscDataIrp[IRP_CBW]);
                MUSB_CancelTransfer(&usbMscDataIrp[IRP_CSW]);

                // recover previously halted pipes
                if ( MUSB_GetPipeHalt(usbMscDataIrp[IRP_DATA_IN].hPipe) )
                    MUSB_SetPipeHalt(usbMscDataIrp[IRP_DATA_IN].hPipe, FFALSE);
                if ( MUSB_GetPipeHalt(usbMscDataIrp[IRP_DATA_OUT].hPipe) )
                    MUSB_SetPipeHalt(usbMscDataIrp[IRP_DATA_OUT].hPipe, FFALSE);

                usbMscDataIrp[IRP_DATA_IN].pBuffer   = usbMscTxBuffer;
                usbMscDataIrp[IRP_DATA_IN].dwLength  = sizeof(usbMscTxBuffer);
                usbMscDataIrp[IRP_DATA_IN].bAllowDma = FFALSE;
                usbMscDataIrp[IRP_DATA_OUT].pBuffer  = usbMscRxBuffer;
                usbMscDataIrp[IRP_DATA_OUT].dwLength = sizeof(usbMscRxBuffer);
                usbMscDataIrp[IRP_DATA_OUT].bAllowDma= FFALSE;
                usbMscDataIrp[IRP_CBW].dwActualLength = 0;
                usbMscCBW.dCbwSignature = 0;
                if ( MUSB_StartTransfer(&usbMscDataIrp[IRP_CBW]) == MUSB_STATUS_OK ) {
                    MUSB_DeviceResponse( busHandle,
                                         sequenceNum,
                                         NULL,
                                         0,
                                         FALSE
                                       );
                } else {
                    return( FFALSE );
                }
                break;
            default:
                #ifdef FM_USB_MSC_DEBUG
                    FM_PrCStrHex2( "BAD Request: ", pDevRequest->bRequest );
                    FM_PrStr("\n");
                #endif
                return( FFALSE );

        }
        return( FTRUE );
    } else {
        return( FFALSE );
    }
}


//extern char* MGC_pMsgBuf;

/* 21c4166c - complete */
/*static*/ uint8_t cbDeviceConfigSelected( void* pPrivateData,
                                  MUSB_BusHandle busHandle,
                                  uint8_t configValue,
                                  MUSB_Pipe* pPipes
                                )
{
    uint8_t i = 0;

    #ifdef FM_USB_MSC_DEBUG
        FM_PrStr("SET_CONFIGURATION request received for configuration " );
        FM_PrU32( configValue);
        FM_PrStr( ".\n" );
    #endif

    if ( configValue == 1 ) {
        usbMscDataIrp[IRP_DATA_IN].hPipe     = pPipes[EP_BULK_IN];
        usbMscDataIrp[IRP_CSW].hPipe         = pPipes[EP_BULK_IN];
        usbMscDataIrp[IRP_DATA_OUT].hPipe    = pPipes[EP_BULK_OUT];
        usbMscDataIrp[IRP_CBW].hPipe         = pPipes[EP_BULK_OUT];
        usbMscDataIrp[IRP_DATA_IN].pBuffer   = usbMscTxBuffer;
        usbMscDataIrp[IRP_DATA_IN].dwLength  = sizeof(usbMscTxBuffer);
        usbMscDataIrp[IRP_DATA_IN].bAllowDma = FFALSE;
        usbMscDataIrp[IRP_DATA_OUT].pBuffer  = usbMscRxBuffer;
        usbMscDataIrp[IRP_DATA_OUT].dwLength = sizeof(usbMscRxBuffer);
        usbMscDataIrp[IRP_DATA_OUT].bAllowDma= FFALSE;

        // prepare for bulk request from host
        usbMscDataIrp[IRP_CBW].dwActualLength = 0;
        usbMscCBW.dCbwSignature = 0;
        if ( MUSB_StartTransfer(&usbMscDataIrp[IRP_CBW]) == MUSB_STATUS_OK ) {

           for (i = 0; i <= usbMscMaxLun; i++)
           {
              pUsbMscDevices[i].bData_36 |= 8;
           }

           if (usbMscNotifyFct)
           {
              (usbMscNotifyFct)(1);
           }

           return( FTRUE );
        } else  {
            return( FFALSE );
        }
    } else if ( configValue == 0 ) {
        usbMscDataIrp[IRP_DATA_IN].hPipe     = NULL;
        usbMscDataIrp[IRP_CSW].hPipe         = NULL;
        usbMscDataIrp[IRP_DATA_OUT].hPipe    = NULL;
        usbMscDataIrp[IRP_CBW].hPipe         = NULL;

        for (i = 0; i <= usbMscMaxLun; i++)
        {
           pUsbMscDevices[i].bData_36 &= ~8;
        }

        if (usbMscNotifyFct)
        {
           (usbMscNotifyFct)(0);
        }

        return( FTRUE );
    }
    return( FFALSE );
}


/* 21c418b8 - complete */
static void cbTxDataTransferComplete(void* pCompleteParam, MUSB_Irp* pIrp)
{
    #ifdef FM_USB_MSC_DEBUG
         if ( pIrp->bAllowDma ) FM_PrStr("DMA ");
         FM_PrStr( "BULK IN complete. Sent bytes=" );
         FM_PrU32(pIrp->dwActualLength);
         FM_PrStr( " status=" );
         FM_PrU32(pIrp->dwStatus);
         FM_PrStr( ".\n" );
    #endif

    if ( !pIrp->dwStatus ) {

        // check for command error condition
        if ( usbMscCSW.bCswStatus != MGC_MSD_BOT_COMMAND_PASSED ) {
            MUSB_SetPipeHalt(usbMscDataIrp[IRP_DATA_IN].hPipe, FTRUE);
        }

        // prepare for CSW
        usbMscCSW.dCswDataResidue -= pIrp->dwActualLength;
        usbMscDataIrp[IRP_CSW].dwActualLength = 0;
        MUSB_StartTransfer(&usbMscDataIrp[IRP_CSW]);
    }
}


/* 21c41800 - complete */
static void cbTxCswTransferComplete(void* pCompleteParam, MUSB_Irp* pIrp)
{
    #ifdef FM_USB_MSC_DEBUG
         FM_PrStr( "CSW IN complete. Sent bytes=" );
         FM_PrU32(pIrp->dwActualLength);
         FM_PrStr( " status=" );
         FM_PrU32(pIrp->dwStatus);
         FM_PrStr( ".\n" );
    #endif

    if ( !pIrp->dwStatus && pIrp->dwActualLength == MGC_MSD_BOT_CSW_LENGTH ) {

        // prepare for next CBW
        if ( !MUSB_GetPipeHalt(usbMscDataIrp[IRP_CBW].hPipe) ) {
            usbMscCBW.dCbwSignature = 0;
            usbMscDataIrp[IRP_CBW].dwActualLength = 0;
            MUSB_StartTransfer(&usbMscDataIrp[IRP_CBW]);
        }
    }
}


/* 21c41850 - complete */
static void cbRxDataTransferComplete(void* pCompleteParam, MUSB_Irp* pIrp)
{
    #ifdef FM_USB_MSC_DEBUG
         if ( pIrp->bAllowDma ) FM_PrStr("DMA ");
         FM_PrStr( "BULK OUT data complete. Received bytes=" );
         FM_PrU32(pIrp->dwActualLength);
         FM_PrStr( " status=" );
         FM_PrU32(pIrp->dwStatus);
         FM_PrStr( ".\n" );
    #endif

    if ( !pIrp->dwStatus ) {

        // check for command error condition
        if ( usbMscCSW.bCswStatus != MGC_MSD_BOT_COMMAND_PASSED ) {
            MUSB_SetPipeHalt(usbMscDataIrp[IRP_DATA_OUT].hPipe, FTRUE);
        }

        // prepare for CSW
        usbMscCSW.dCswDataResidue -= pIrp->dwActualLength;
        usbMscDataIrp[IRP_CSW].dwActualLength = 0;
        MUSB_StartTransfer(&usbMscDataIrp[IRP_CSW]);
    }
}


/* 21c41920 - todo */
static void cbRxCbwTransferComplete(void* pCompleteParam, MUSB_Irp* pIrp)
{
    MGC_MsdCbw* pCBW;

    #ifdef FM_USB_MSC_DEBUG
         FM_PrStr( "\nCBW OUT complete. Received bytes=" );
         FM_PrU32(pIrp->dwActualLength);
         FM_PrStr( " status=" );
         FM_PrU32(pIrp->dwStatus);
         FM_PrStr( ".\n" );
    #endif

    pCBW = (MGC_MsdCbw*)pIrp->pBuffer;

    if ( !pIrp->dwStatus && pIrp->dwActualLength == MGC_MSD_BOT_CBW_LENGTH ) {

        if ( pCBW->dCbwSignature == MGC_MSD_BOT_CBW_SIGNATURE ) {
            usbMscProcessCbw( pCBW );
            return;
        }
    }

    // Invalid CBW - stall the endpoints
    MUSB_SetPipeHalt(usbMscDataIrp[IRP_DATA_IN].hPipe,  FTRUE);
    MUSB_SetPipeHalt(usbMscDataIrp[IRP_DATA_OUT].hPipe, FTRUE);

    //FM_PrStr(MGC_pMsgBuf);
}



