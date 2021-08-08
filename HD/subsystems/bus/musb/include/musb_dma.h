#ifndef MUSB_DMA_H_
#define MUSB_DMA_H_



struct MGC;


typedef struct
{
  void* pPrivateData; //0
  unsigned dwMaxLength; //4
  unsigned dwActualLength; //8
  int d; //12
  char bDesiredMode; //16
  FAPI_DMA_RequestT* f; //20
  char g; //24
} MGC_DmaChannel;

typedef struct
{
  MGC_DmaChannel Channel; //0
  int dwStartAddress; //28
  unsigned dwCount; //32
  unsigned short wMaxPacketSize; //36
  char bEnd; //38
  char bTransmit; //39
  char k; //40
  int l; //44
  unsigned nNumPackets; //48
  unsigned nPacketCount; //52
  //56
} MGC_HsDmaChannel;

typedef int (*MGC_pfDmaStartController)(void* pPrivateData);
typedef int (*MGC_pfDmaStopController)(void* pPrivateData);
typedef MGC_HsDmaChannel* (*MGC_pfDmaAllocateChannel)(void* pPrivateData, 
    char bLocalEnd, 
    char bTransmit, 
    char bProtocol, 
    unsigned short wMaxPacketSize);
typedef void (*MGC_pfDmaReleaseChannel)(MGC_DmaChannel* pChannel);
typedef void* (*MGC_pfDmaAllocateBuffer)(void* pChannel, int dwLength);
typedef int (*MGC_pfDmaReleaseBuffer)(void* pChannel, void* pBuffer);
typedef int (*MGC_pfDmaProgramChannel)(MGC_DmaChannel* pChannel, 
                        unsigned short wPacketSize, char bMode,
                        const char* pBuffer, 
                        int dwLength);
typedef int (*MGC_pfDmaGetChannelStatus)(MGC_DmaChannel* pChannel);
typedef int (*MGC_pfDmaControllerIsr)(void* pPrivateData);

typedef struct MGC_DmaController
{
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
} MGC_DmaController;

typedef struct MGC_HsDmaController_t
{
   MGC_DmaController Controller; //0
   MGC_HsDmaChannel aChannel[1]; //40
   int (*pfDmaChannelStatusChanged)(void* pPrivateData, char bLocalEnd, char bTransmit); //96    
   struct MGC* pDmaPrivate; //100    
   char* (*Data_328)(int, char* b); //104    
   unsigned Data_332; //108
   unsigned Data_336; //112    
   unsigned Data_340; //116
   //120    
} MGC_HsDmaController;

typedef struct MGC_DmaControllerFactory
{
   unsigned short a1; //0
   MGC_DmaController* (*pfNewDmaController)(
              int (*a)(void* pPrivateData, char bLocalEnd, char bTransmit),
              struct MGC* b,
              char* (*c)(int, char* b),
              int d,
              int addr1,
              int addr2); //4
   void (*pfDestroyDmaController)(MGC_DmaController* pController); //8  
} MGC_DmaControllerFactory;


extern MGC_DmaControllerFactory MGC_HdrcDmaControllerFactory; //21f02190 

#endif /*MUSB_DMA_H_*/
