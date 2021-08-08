#ifndef MU_HFI_H_
#define MU_HFI_H_

typedef enum //:T(5,1)=e
{
   MUSB_HFI_MEDIA_UNKNOWN = 0,
   MUSB_HFI_MEDIA_FIXED = 1,
   MUSB_HFI_MEDIA_REMOVABLE = 2,
} MUSB_HfiMediaType; //:t(5,2)=(5,1)

typedef enum //:T(5,3)=e
{
   MUSB_HFI_ACCESS_RANDOM = 0,
   MUSB_HFI_ACCESS_RANDOM_WRITE_ONCE = 1,
   MUSB_HFI_ACCESS_RANDOM_READ = 2,
} MUSB_HfiAccessType; //:t(5,4)=(5,3);

typedef enum //:T(5,5)=e
{
   MUSB_HFI_SUCCESS = 0,
   MUSB_HFI_ERROR_UNSUPPORTED_BLOCKSIZE = 1,
   MUSB_HFI_ERROR_MEDIA_PROTECTED = 2,
   MUSB_HFI_ERROR_MEDIA_REMOVED = 3,
   MUSB_HFI_ERROR_DEVICE_REMOVED = 4,
   MUSB_HFI_ERROR_TRANSFER = 5,
   MUSB_HFI_ERROR_VERIFY = 6,
   MUSB_HFI_ERROR_BUSY = 7,
   MUSB_HFI_NO_MEMORY = 8,
} MUSB_HfiStatus; //:t(5,6)=(5,5)

typedef struct
{
   MUSB_HfiAccessType AccessType; //0 //:(5,4),0,32;
   uint32_t dwBlockSize; //4 //:(7,6),32,32;
   uint32_t dwBlockCountLo; //8 //:(7,6),64,32;
   uint32_t dwBlockCountHi; //12 //:(7,6),96,32;
   uint16_t awSerialNumber[32]; //16 //:(5,9)=ar(0,1);0;31;(7,5),128,512;;
   //80
} MUSB_HfiMediumInfo; //:t(5,7)=(5,8)=s80
//Struct_b75bb8_Inner2

typedef struct
{
   MUSB_HfiMediaType MediaType; //0 //:(5,2),0,32;
   MUSB_HfiMediumInfo InitialMedium; //4 //:(5,7),32,640;
   uint16_t bmAccessType; //84 //:(7,5),672,16;
   uint8_t bCanFormat; //86 //:(7,4),688,8;
   uint8_t bHasCache; //87 //:(7,4),696,8;
   uint32_t dwBlockSize; //88 //:(7,6),704,32;
   uint16_t awSerialNumber[32]; //92
   //unsigned short k; //154
   uint16_t wVendorId; //156
   uint16_t wProductId; //158
   uint16_t bcdDevice; //160
   uint8_t bBusAddress; //162
   uint8_t bPad[3]; //163
   uint16_t awDiskVendor[16]; //166
   uint16_t awDiskProduct[32]; //198
   uint16_t awDiskRevision[8]; //262
   //280
} MUSB_HfiDeviceInfo; //:t(5,10)=(5,11)=s28
//Struct_b75bb8_Inner1;



struct Struct_b75bb8_t;

typedef int (*MUSB_pfHfiMountVolume)(/*Struct_48b494_Inner1*/void* a,
      void (*b)(struct Struct_b75bb8_t* a, int b)); //:t(5,25)=(5,26)=*(5,27)=f(5,6)
typedef int (*MUSB_pfHfiGetMediumInfo)(/*Struct_48b494_Inner1*/void* a,
      void* b); //:t(5,28)=(5,29)=*(5,30)=f(5,6)
typedef int (*MUSB_pfHfiReadDevice)(/*Struct_48b494_Inner1*/void* a,
      int lba, int c, int numBlocks, void* e,
      void (*complete)(struct Struct_b75bb8_t* a, unsigned short b),
      char g); //:t(5,31)=(5,32)=*(5,33)=f(5,6)
typedef int (*MUSB_pfHfiWriteDevice)(/*Struct_48b494_Inner1*/void* a,
      int b, int c, int d, void* e, int f,
      void (*g)(struct Struct_b75bb8_t* a, unsigned short b),
      char h); //:t(5,34)=(5,35)=*(5,36)=f(5,6)
typedef int (*MUSB_pfHfiFlushDevice)(/*Struct_48b494_Inner1*/void* a); //:t(5,37)=(5,38)=*(5,39)=f(5,6)
typedef int (*MUSB_pfHfiFormatMedium)(/*Struct_48b494_Inner1*/void* a,
      int b, char* c); //:t(5,40)=(5,41)=*(5,42)=f(5,6)
typedef int (*MUSB_pfHfiGetFormatProgress)(/*Struct_48b494_Inner1*/void* a); //:t(5,43)=(5,44)=*(5,45)=f(7,4)
typedef int (*MUSB_pfHfiAbortFormat)(/*Struct_48b494_Inner1*/void* a); //:t(5,46)=(5,38)
typedef int (*MUSB_pfHfiUnmountVolume)(/*Struct_48b494_Inner1*/void* a); //:t(5,47)=(5,38)
typedef int (*MUSB_pfHfiCheckMedium)(/*Struct_48b494_Inner1*/void* a); //:t(5,48)=(5,38)
typedef int (*MUSB_pfHfiCheckMediumNotify)(/*Struct_48b494_Inner1*/void* a,
      void (*b)(struct Struct_b75bb8_t*)); //:t(5,49)=(5,50)=*(5,51)=f(5,6)

typedef struct
{
   /*Struct_48b494_Inner1*/void* pPrivateData; //0
   MUSB_pfHfiMountVolume pfMountVolume; //4 //:(5,25),32,32;
   MUSB_pfHfiGetMediumInfo pfGetMediumInfo; //8 //:(5,28),64,32;
   MUSB_pfHfiReadDevice pfReadDevice; //12 //:(5,31),96,32;
   MUSB_pfHfiWriteDevice pfWriteDevice; //16 //:(5,34),128,32;
   MUSB_pfHfiFlushDevice pfFlushDevice; //20 //:(5,37),160,32;
   MUSB_pfHfiFormatMedium pfFormatMedium; //24 //:(5,40),192,32;
   MUSB_pfHfiGetFormatProgress pfGetFormatProgress; //28 //:(5,43),224,32;();
   MUSB_pfHfiAbortFormat pfAbortFormat; //32 //:(5,46),256,32;
   MUSB_pfHfiUnmountVolume pfUnmountVolume; //36 //:(5,47),288,32;
   MUSB_pfHfiCheckMedium pfCheckMedium; //40 //:(5,48),320,32;
   MUSB_pfHfiCheckMediumNotify pfCheckMediumNotify; //44 //:(5,49),352,32;
   //48
} MUSB_HfiDevice; //:t(5,52)=(5,53)=s48




#endif /* MU_HFI_H_ */
