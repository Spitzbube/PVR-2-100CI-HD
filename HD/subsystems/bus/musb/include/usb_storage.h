#ifndef USB_STORAGE_H_
#define USB_STORAGE_H_

#include "mu_hfi.h"


extern int MGC_FillStorageClassPeripheralList(uint8_t*, unsigned short);
extern void* MUSB_GetStorageClassDriver(void);




#define USB_BULK_CB_SIGN        0x43425355  /*spells out USBC */


/* Bulk-only class specific requests */
#define USB_BULK_RESET_REQUEST      0xff
#define USB_BULK_GET_MAX_LUN_REQUEST    0xfe 


struct CBW
{
    int dSignature; //0 
    int dTag; //4 
    int dDataTransferLength; //8 
    char bmFlags; //12
    char bLUN; //13
    char bCBLength; //14    
    char CB[16]; //15
    //31
};

struct CSW
{
    int dSignature; //0
    int dTag; //4 
    int dDataResidue; //8
    char bStatus; //12
    //13
}; 


typedef struct _usb_volume //:T(0,20)=s324 //Struct_b75bb8_t
{
   MUSB_HfiDevice* pUsbDevice; //0
   MUSB_HfiDeviceInfo usbDeviceInfo; //4 //:(5,10),32,2240;
   void* transferCompleted; //284 //:(4,7),2272,32;
   void* transferEnabled; //288 //:(4,7),2304,32;
   uint32_t transBlockCount; //292 //:(1,4),2336,32
#if 0
   int b; //296
#else
   uint32_t isMediumInserted: 1; //296 //:(1,4),2368,1;
   uint32_t error: 1; //296 //:(1,4),2369,1;
#endif
   /*tFDRV_Device*/void* partitions[4]; //300 //:(0,22)=ar(0,1);0;3;(9,7),2400,128;
   uint8_t* mbr; //316 //:(13,11),2528,32;
   struct _usb_volume* pMasterVolume; //320 //:(0,23)=*(0,20),2560,32;;
   //324
} FDRV_USB_VOLUME_S; //:t(0,24)=(0,20)
//Struct_b75bb8


struct Struct_48b494_Inner1_t;
struct Struct_48b494_Inner3;


typedef struct USB_StorageBulkOnly 
{
   struct USB_StorageBulkOnly* Data_0; //0
   int (*pfGetMaxLun)(struct USB_StorageBulkOnly* a, MUSB_Device* b); //4
   void (*Func_8)(void); //8
   int (*pfSendCBW)(struct USB_StorageBulkOnly* a1, 
                  void* c, 
                  int lun, void* pCB, int bCBLength, 
                  void* pDataBuffer, int dwDataLength, int bHostTx, 
                  void (*i)(void* a, void*, int c,
                            int d, int e, int f, int g), 
                  char j, int k); //12
   int (*Func_16)(struct USB_StorageBulkOnly* a, char lun, void* c); //16
   int (*Func_20)(struct USB_StorageBulkOnly* a, char lun, void* c); //20
   void (*Func_24)(struct USB_StorageBulkOnly* a, int b, int c); //24
   void (*Func_28)(struct USB_StorageBulkOnly* a, char lun, int c); //28
   struct USB_StorageBulkOnly* Data_32; //32
   MUSB_ControlIrp ControlUrb; //36
   char arDeviceRequestBuffer[8]; //80
   char arDeviceResponseBuffer[32]; //88
   MUSB_Device* Data_120; //120
   struct MGC* Data_124; //124
   struct usb_interface_descriptor* pInterfaceDescriptor; //128
   struct usb_endpoint_descriptor* pBulkInEpDescriptor; //132
   struct usb_endpoint_descriptor* pBulkOutEpDescriptor; //136
   struct Struct_48b494_Inner3* Data_140; //140
   int dCBWTag; //144
   MGC_Pipe* pBulkIn; //148
   MGC_Pipe* pBulkOut; //152
   struct CSW CSW; //156
   MUSB_Irp BulkStatus; //172 +32
   struct Struct_48b494_Inner1_t* Data_204; //204
   struct Struct_48b494_Inner1_t* arLUN; //208
   int Data_212; //212
   char bGetMaxLunActive; //216
   char bNumLUNs; //217
   char bData_218; //218
   char bData_219; //219
   char bData_220; //220
   //224   
} USB_StorageBulkOnly;


typedef struct
{
   USB_StorageBulkOnly* Data_0; //0
   void (*Func_4)(int a, int b); //4
   int Data_8; //8
   char arData_12[512]; //12
   char arCommandBlock[16]; //524
   char arInquiryData[36]; //540
   char arData_576[20]; //20
   char arRequestSenseData[20]; //596
   char arCapacityList[252]; //616
   struct
   {
      char arLastBlockAddress[4]; //0
      char arBlockLength[4]; //4
   } rReadCapacityData; //868
   char fill_876[64]; //876
   unsigned Data_940; //940
   unsigned Data_944; //944
   char bReadFormatCapacitiesTry; //948
   int nStep; //952
   char bRequestSense; //956
   char bData_957; //957
   char bData_958; //958
   char bFirstLUN; //959
   char bNumLUNs; //960 
   char bMoreLUNs; //961 
   char bCurrentLUN; //962 
   char bData_963; //963
} Struct_499b0c;


typedef struct Struct_48b494_Inner1_t
{
    USB_StorageBulkOnly* a; //0
    Struct_499b0c* b; //4
    void (*pfBulkDataTransferComplete)(void* a, 
                    void*, int c,
                    int bUsbProtocolError, 
                    int bCSWStatus, 
                    int f, int g); //8
    void* Data_12; //12
    struct CBW CBW; //16
    MUSB_Irp BulkCommand; //48    +32
    MUSB_Irp BulkDataTransfer; //80   +32
    /*Struct_b75bb8_Inner1*/struct Struct_21c402a8 Data_112; //112
    MUSB_HfiMediumInfo Data_392; //392 +80
    MUSB_HfiDevice Data_472; //472 +48
    void (*Func_520)(FDRV_USB_VOLUME_S* a, int b); //520
    void (*ReadWriteBlocksComplete)(FDRV_USB_VOLUME_S* a, unsigned short b); //524
    void (*Data_528)(FDRV_USB_VOLUME_S*); //528
    FDRV_USB_VOLUME_S* Data_532; //532
    char* Data_536; //536
    unsigned Data_540; //540
    char bLun; //544
    char bData_545; //545
    char bActive; //546
    char bData_547; //547
    char bUsbProtocolError; //548
    char bData_549; //549
    char bData_550; //550
    uint8_t bData_551; //551
    char bData_552; //552
    //556
} Struct_48b494_Inner1;


typedef struct Struct_48b494_Inner3
{
    Struct_499b0c* (*a)(int a); //0
    void (*b)(Struct_499b0c* a); //4
    void (*SendInquiry)(Struct_499b0c* a, 
              USB_StorageBulkOnly* b, 
              char lun); //8
    int (*d)(Struct_499b0c* a, 
             void (*b)(Struct_48b494_Inner1* a, int b), 
             Struct_48b494_Inner1* c, 
             USB_StorageBulkOnly* d, 
             char e); //12
    int (*e)(Struct_499b0c* a, 
             USB_StorageBulkOnly* b, 
             char c); //16
    int (*CreateRead10CommandBlock)(Struct_499b0c* a,
             char* CB,
             int,
             unsigned lba, 
             int,
             int length,
             char); //20
    int (*g)(Struct_499b0c* a, 
             void* b,
             int c,
             int d, 
             int e,
             int f,
             char g); //24
    int (*h)(Struct_499b0c* a, 
             USB_StorageBulkOnly* b, 
             char d); //28
    int (*i)(Struct_499b0c* a, 
             USB_StorageBulkOnly* b, 
             int c, 
             char d); //32
    int (*j)(Struct_499b0c* a, 
             USB_StorageBulkOnly* b, 
             char c,
             Struct_499b0c* d); //36
    void (*k)(int a); //40
    int (*l)(Struct_499b0c* a, 
             USB_StorageBulkOnly* b, 
             char d); //44
} Struct_48b494_Inner3;


typedef struct Struct_498a94
{
   int fill_0; //0
   int fill_4; //4
   struct MGC* Data_8; //8
   MUSB_Device* Data_12; //12
   MUSB_DeviceDriver* pDeviceDriver; //16
   int fill_20; //20
   MUSB_DeviceRequest device_request; //24
   MUSB_ControlIrp control_urb; //32
   int fill_76; //76
   int fill_80; //80
   int fill_84; //84
   int fill_88; //88
   USB_StorageBulkOnly* Data_92; //92  
   //96
} Struct_498a94;




extern void func_48b494(USB_StorageBulkOnly* a, int b, int c);



#endif /*USB_STORAGE_H_*/
