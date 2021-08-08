

#include <fapi/sys_services.h>
#include <fapi/drv_dma_priv.h>


static FAPI_DMA_SearchReplaceParamsT dmaSrParams; //21f21ed0

static void dmaSearchReplacePreEnable(FAPI_DMA_RequestT* damRequestPtr);
static void dmaSearchReplaceFinish(FAPI_DMA_RequestT* damRequestPtr);


/* 21c19ca8 - complete */
FAPI_DMA_RequestT* FAPI_DMA_StartSearchReplace(
                        volatile uint32_t*             buffer,
                        uint32_t                       bytes,
                        FAPI_DMA_SearchReplaceParamsT* srParams,
                        FAPI_DMA_OptCallbackT          hitCallback,
                        FAPI_DMA_OptCallbackT          finishCallback )
{
   uint32_t feature = FAPI_DMA_FEATURE_SEARCH_REPLACE;
   
   FAPI_DMA_RequestT* request;
   FAPI_DMA_RequestT* request2;

   bytes = (bytes + 3) & ~3;
   
   request = FAPI_DMA_AllocateRequest(feature, 1);
   
   if (request != NULL)
   {
      dmaSrParams.searchPattern = srParams->searchPattern;
      dmaSrParams.searchMask = srParams->searchMask;
      dmaSrParams.searchAlignment = srParams->searchAlignment;
      dmaSrParams.replacePattern = srParams->replacePattern;
      dmaSrParams.replaceMask = srParams->replaceMask;
      dmaSrParams.replaceEnable = srParams->replaceEnable;
      dmaSrParams.request2 = 0;
      
      request->preEnableCallback = dmaSearchReplacePreEnable;
      request->postEnableCallback = NULL;
      request->postFinishCallback = dmaSearchReplaceFinish;
      request->optHandle = 0;
      request->optData = &dmaSrParams;
      request->optFinishCallback = finishCallback;
      request->optChangeCallback = hitCallback;
      
      request->channelConfig.chConfig = 0;
      request->channelConfig.chLength = bytes + 8;
      request->channelConfig.chLladdr = 0;
      //request->channelConfig.chRdaddr_EndianSwap = 0;
      request->channelConfig.chRdaddr_PeripheralAddress = 0;
      request->channelConfig.chRdaddr_OffsetAddress = (uint32_t) buffer;
      request->channelConfig.chRdline_Lines = 0x3ff;
      //request->channelConfig.chRdline_LoopIncrement = 0;
      request->channelConfig.chRdinc_LineLength = 0xFFF;
      request->channelConfig.chRdinc_LineIncrement = 0;
      request->channelConfig.chRdlpaddr_LoopAddress = 0;
      //request->channelConfig.chWraddr_EndianSwap = 0;
      request->channelConfig.chWraddr_PeripheralAddress = 4;
      request->channelConfig.chWraddr_OffsetAddress = 0;
      request->channelConfig.chWrline_Lines = 0x3FF;
      //request->channelConfig.chWrline_LoopIncrement = 0;
      request->channelConfig.chWrinc_LineLength = 0xFFF;
      request->channelConfig.chWrinc_LineIncrement = 0;
      request->channelConfig.chWrlpaddr_LoopAddress = 0;
      
      if (srParams->replaceEnable == 1)
      {
         request2 = FAPI_DMA_AllocateRequest(feature|FAPI_DMA_FEATURE_SDRAM_COPY, 1);
         
         if (request2 != NULL)
         {
            dmaSrParams.request2 = request2;
            
            request->postFinishCallback = NULL;
            request->optFinishCallback = NULL;
            
            request2->preEnableCallback = NULL;
            request2->postEnableCallback = NULL;
            request2->postFinishCallback = dmaSearchReplaceFinish;
            request2->optHandle = 0;
            request2->optData = &dmaSrParams;
            request2->optFinishCallback = finishCallback;
            request2->optChangeCallback = NULL;

            request2->channelConfig.chConfig = 0;
            request2->channelConfig.chLength = bytes;
            request2->channelConfig.chLladdr = 0;
            //request2->channelConfig.chRdaddr_EndianSwap = 0;
            request2->channelConfig.chRdaddr_PeripheralAddress = 4;
            request2->channelConfig.chRdaddr_OffsetAddress = 0;
            request2->channelConfig.chRdline_Lines = 0x3ff;
            //request2->channelConfig.chRdline_LoopIncrement = 0;
            request2->channelConfig.chRdinc_LineLength = 0xFFF;
            request2->channelConfig.chRdinc_LineIncrement = 0;
            request2->channelConfig.chRdlpaddr_LoopAddress = 0;
            //request2->channelConfig.chWraddr_EndianSwap = 0;
            request2->channelConfig.chWraddr_PeripheralAddress = 0;
            request2->channelConfig.chWraddr_OffsetAddress = (uint32_t) buffer;
            request2->channelConfig.chWrline_Lines = 0x3FF;
            //request2->channelConfig.chWrline_LoopIncrement = 0;
            request2->channelConfig.chWrinc_LineLength = 0xFFF;
            request2->channelConfig.chWrinc_LineIncrement = 0;
            request2->channelConfig.chWrlpaddr_LoopAddress = 0;
         }
         
         if (0 != FAPI_DMA_SendRequest2(request, request2))
         {
            FAPI_DMA_ReleaseRequest(request);
            FAPI_DMA_ReleaseRequest(request2);
            
            request = NULL;
         }
      }
      else
      {
         if (0 != FAPI_DMA_SendRequest(request))
         {
            FAPI_DMA_ReleaseRequest(request);
            
            request = NULL;
         }
      }
   }

   return request;
}


/* 21c19bf8 - complete */
uint32_t func_219316b4(FAPI_DMA_RequestT* a)
{      
   FAPI_DMA_SearchReplaceParamsT* params = a->optData;
   uint32_t searchPattern = params->searchPattern;
   uint32_t searchMask = params->searchMask;
   uint32_t offset = a->channelConfig.chRdaddr_OffsetAddress;   
   uint32_t address = FREG_DMA_GetSrAddress();   
   uint32_t data = *((uint32_t*) (offset + address));
   
   if (params->searchAlignment == FAPI_DMA_SR_ALIGN_8)
   {
      if ((searchPattern & searchMask) == (data & searchMask))
      {
      }
      else if (((searchPattern << 8) & (searchMask << 8)) == (data & (searchMask << 8)))
      {
         address++;
      }
      else if (((searchPattern << 16) & (searchMask << 16)) == (data & (searchMask << 16)))
      {
         address += 2;
      }
      else if (((searchPattern << 24) & (searchMask << 24)) == (data & (searchMask << 24)))
      {
         address += 3;
      }
   }
   else if (params->searchAlignment == FAPI_DMA_SR_ALIGN_16)
   {
      if ((searchPattern & searchMask) == (data & searchMask))
      {
      }
      else if (((searchPattern << 16) & (searchMask << 16)) == (data & (searchMask << 16)))
      {
         address += 2;
      }
   }
   
   return address;
}


/* 21c19e9c - complete */
void dmaSearchReplacePreEnable(FAPI_DMA_RequestT* damRequestPtr)
{
   FAPI_DMA_SearchReplaceParamsT* srParams = damRequestPtr->optData;
   
   FREG_DMA_SetSrConfig(0);
   FREG_DMA_SetSrConfig_Alignment(srParams->searchAlignment);
   FREG_DMA_SetSrConfig_ReplacementEnable(srParams->replaceEnable);
   FREG_DMA_SetSrCompare(srParams->searchPattern);
   FREG_DMA_SetSrMask(srParams->searchMask);
   FREG_DMA_SetSrReplace(srParams->replacePattern);
   FREG_DMA_SetSrReplaceMask(srParams->replaceMask);
   FREG_DMA_SetSrConfig_Enable(1);
}


/* 21c19e94 - complete */
void dmaSearchReplaceFinish(FAPI_DMA_RequestT* damRequestPtr)
{
   FREG_DMA_SetSrConfig(0);      
}


