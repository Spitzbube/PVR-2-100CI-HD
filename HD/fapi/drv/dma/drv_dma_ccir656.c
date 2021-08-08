
#include <fapi/sys_services.h>
#include <fapi/drv_timer.h>
#include <fapi/drv_dma_priv.h>


static uint32_t Data_21bd5780 = 0; //21f21e98
static uint32_t Data_21bd5784 = 0; //21f21e9c
static uint64_t Data_21bd5788 = 0; //21bd5788

static FAPI_DMA_Ccir656CaptureParamsT Data_21ca6340; //21ca6340


static void dmaCcir656PreEnable(FAPI_DMA_RequestT* damRequestPtr);
static void dmaCcir656PostFinish(FAPI_DMA_RequestT* damRequestPtr);



/* 21c199a0 - todo */
FAPI_DMA_RequestT* FAPI_DMA_StartCcir656Capture(
                           volatile uint32_t*              targetBuffer,
                           uint32_t                        bufferBytes,
                           uint32_t                        bufferLineInc,
                           FAPI_DMA_Ccir656CaptureParamsT* captureParams,
                           FAPI_DMA_OptCallbackT           syncCallback,
                           FAPI_DMA_OptCallbackT           finishCallback )
{
   uint32_t columns;
   uint32_t lines;
   uint32_t r2;
   int32_t increment;
   uint32_t feature = FAPI_DMA_FEATURE_CCIR656_CAPTURE|FAPI_DMA_FEATURE_SDRAM_COPY;
   FAPI_DMA_RequestT* request = 0;
   
   Data_21bd5780 = 0;
   Data_21bd5784 = 0;

   Data_21ca6340.useRawMode = captureParams->useRawMode;
   Data_21ca6340.dataSource = captureParams->dataSource;
   Data_21ca6340.invertClock = captureParams->invertClock;
   Data_21ca6340.useField0 = captureParams->useField0;
   Data_21ca6340.useField1 = captureParams->useField1;
   Data_21ca6340.absColumnStart = captureParams->absColumnStart / 4; //>> 2;
   Data_21ca6340.absColumnStart *= 4; //<<= 2; 
   Data_21ca6340.absColumnEnd = (captureParams->absColumnEnd + 2) / 4; //>> 2;
   Data_21ca6340.absColumnEnd *= 4; //<<= 2;
   Data_21ca6340.absColumnEnd -= 2;
   Data_21ca6340.absLineStart = captureParams->absLineStart;
   Data_21ca6340.absLineEnd = captureParams->absLineEnd;
   
   lines = Data_21ca6340.absLineEnd - Data_21ca6340.absLineStart + 1;
   
   columns = Data_21ca6340.absColumnEnd - Data_21ca6340.absColumnStart + 2;
   
   r2 = 4 * columns * lines;
 
   increment = bufferLineInc - columns * 2;
   increment = bufferLineInc - increment/2;

   if ((Data_21ca6340.absColumnStart < Data_21ca6340.absColumnEnd) &&
         (Data_21ca6340.absLineEnd >= Data_21ca6340.absLineStart) &&
         (bufferBytes >= r2))
   {
      request = FAPI_DMA_AllocateRequest(feature, 1);
      
      if (request != NULL)
      {
         request->preEnableCallback = dmaCcir656PreEnable;
         request->postEnableCallback = NULL;
         request->postFinishCallback = dmaCcir656PostFinish;
         request->optHandle = 0;
         request->optData = &Data_21ca6340;
         request->optFinishCallback = finishCallback;
         request->optChangeCallback = syncCallback;
         
         request->channelConfig.chConfig = 0;
         request->channelConfig.chLength = columns * lines * 2;
         request->channelConfig.chLladdr = 0;
         //request->channnelConfig.chRdaddr_EndianSwap = 0;
         request->channelConfig.chRdaddr_PeripheralAddress = 2;
         request->channelConfig.chRdaddr_OffsetAddress = 0;
         request->channelConfig.chRdline_Lines = 0;
         //request->channelConfig.chRdline_LoopIncrement = 0;
         request->channelConfig.chRdinc_LineLength = 4;
         request->channelConfig.chRdinc_LineIncrement = 0;
         request->channelConfig.chRdlpaddr_LoopAddress = 0;
         //request->channelConfig.chWraddr_EndianSwap = 0;
         request->channelConfig.chWraddr_PeripheralAddress = 0;
         request->channelConfig.chWraddr_OffsetAddress = (uint32_t) targetBuffer;
         request->channelConfig.chWrline_Lines = lines;
         request->channelConfig.chWrline_LoopIncrement = increment;
         request->channelConfig.chWrinc_LineLength = columns * 2;
         request->channelConfig.chWrinc_LineIncrement = bufferLineInc;
         request->channelConfig.chWrlpaddr_LoopAddress = 0;
         
         if (captureParams->enableAutoRestart == 1)
         {
            dmaSetRequestAutoRestart(request, 1);
         }
         else if (captureParams->enableAutoRestart == 2)
         {
            dmaSetRequestAutoRestart(request, 2);
         }
         else
         {
            dmaSetRequestAutoRestart(request, 0);
         }
         
         if (0 != FAPI_DMA_SendRequest(request))
         {
            FAPI_DMA_ReleaseRequest(request);
            
            request = NULL;
         }
      }
   }
   
   return request;
}


/* 21c197dc - todo */
FAPI_DMA_RequestT* FAPI_DMA_StartCcir656CaptureHalfMode(
                       volatile uint32_t*              targetBuffer,
                       uint32_t                        bufferBytes,
                       uint32_t                        bufferLineInc,
                       FAPI_DMA_Ccir656CaptureParamsT* captureParams,
                       FAPI_DMA_OptCallbackT           syncCallback,
                       FAPI_DMA_OptCallbackT           finishCallback )
{
   uint32_t lines;
   uint32_t columns;
   uint32_t r2;
   uint32_t feature = FAPI_DMA_FEATURE_CCIR656_CAPTURE|FAPI_DMA_FEATURE_SDRAM_COPY;
   FAPI_DMA_RequestT* request = 0;
   
   Data_21bd5780 = 0;
   Data_21bd5784 = 1;

   Data_21ca6340.useRawMode = captureParams->useRawMode;
   Data_21ca6340.dataSource = captureParams->dataSource;
   Data_21ca6340.invertClock = captureParams->invertClock;
   Data_21ca6340.useField0 = captureParams->useField0;
   Data_21ca6340.useField1 = captureParams->useField1;
   Data_21ca6340.absColumnStart = captureParams->absColumnStart / 4; //>> 2;
   Data_21ca6340.absColumnStart *= 4; //<<= 2; 
   Data_21ca6340.absColumnEnd = (captureParams->absColumnEnd + 2) / 4; //>> 2;
   Data_21ca6340.absColumnEnd *= 4; //<<= 2;
   Data_21ca6340.absColumnEnd -= 2;
   Data_21ca6340.absLineStart = captureParams->absLineStart;
   Data_21ca6340.absLineEnd = captureParams->absLineEnd;
   
   lines = Data_21ca6340.absLineEnd - Data_21ca6340.absLineStart + 1;
   
   columns = Data_21ca6340.absColumnEnd - Data_21ca6340.absColumnStart + 2;
   
   r2 = 4 * columns * lines;
 
   if ((Data_21ca6340.absColumnStart < Data_21ca6340.absColumnEnd) &&
         (Data_21ca6340.absLineEnd >= Data_21ca6340.absLineStart) && 
         (bufferBytes >= r2))
   {
      request = FAPI_DMA_AllocateRequest(feature, 1);
      
      if (request != NULL)
      {
         request->preEnableCallback = dmaCcir656PreEnable;
         request->postEnableCallback = NULL;
         request->postFinishCallback = dmaCcir656PostFinish;
         request->optHandle = 0;
         request->optData = &Data_21ca6340;
         request->optFinishCallback = finishCallback;
         request->optChangeCallback = syncCallback;
         
         request->channelConfig.chConfig = 0;
         request->channelConfig.chLength = columns * lines * 2;
         request->channelConfig.chLladdr = 0;
       //request->channelConfig.chRdaddr_EndianSwap = 0;
         request->channelConfig.chRdaddr_PeripheralAddress = 2;
         request->channelConfig.chRdaddr_OffsetAddress = 0;
         request->channelConfig.chRdline_Lines = 0;
       //request->channelConfig.chRdline_LoopIncrement = 0;
         request->channelConfig.chRdinc_LineLength = 4;
         request->channelConfig.chRdinc_LineIncrement = 0;
         request->channelConfig.chRdlpaddr_LoopAddress = 0;
       //request->channelConfig.chWraddr_EndianSwap = 0;
         request->channelConfig.chWraddr_PeripheralAddress = 0;
         request->channelConfig.chWraddr_OffsetAddress = (uint32_t) targetBuffer;
         request->channelConfig.chWrline_Lines = lines;
         request->channelConfig.chWrline_LoopIncrement = 0;
         request->channelConfig.chWrinc_LineLength = columns * 2;
         request->channelConfig.chWrinc_LineIncrement = 0;
         request->channelConfig.chWrlpaddr_LoopAddress = 0;
         
         if (captureParams->enableAutoRestart == 1)
         {
            dmaSetRequestAutoRestart(request, 1);
         }
         else if (captureParams->enableAutoRestart == 2)
         {
            dmaSetRequestAutoRestart(request, 2);
         }
         else
         {
            dmaSetRequestAutoRestart(request, 0);
         }
         
         if (0 != FAPI_DMA_SendRequest(request))
         {
            FAPI_DMA_ReleaseRequest(request);
            
            request = NULL;
         }
      }
   }
   
   return request;
}


/* 21930f6c - complete */
void dmaCcir656PreEnable(FAPI_DMA_RequestT* damRequestPtr)
{
   FAPI_DMA_Ccir656CaptureParamsT* params = damRequestPtr->optData;
   
   FREG_DMA_SetCcirConfig(0);
   FREG_DMA_SetCcirHconfig(0);
   FREG_DMA_SetCcirVconfig(0);
   FREG_DMA_SetCcirConfig_RawEnableMode(0);
   FREG_DMA_SetCcirConfig_ExternalInputSelect(params->dataSource);
   FREG_DMA_SetCcirConfig_ClockInvert(params->invertClock);
   FREG_DMA_SetCcirConfig_RawMode(params->useRawMode);
   FREG_DMA_SetCcirConfig_CaptureField0(params->useField0);
   FREG_DMA_SetCcirConfig_CaptureField1(params->useField1);
   FREG_DMA_SetCcirHconfig_HStartPosition(params->absColumnStart / 2);
   FREG_DMA_SetCcirHconfig_HStopPosition(params->absColumnEnd / 2);
   FREG_DMA_SetCcirVconfig_VStartPosition(params->absLineStart);
   FREG_DMA_SetCcirVconfig_VStopPosition(params->absLineEnd);
   FREG_DMA_SetCcirConfig_Enable(1);   
}


/* 21c19604 - todo */
void dmaCcir656PostFinish(FAPI_DMA_RequestT* damRequestPtr)
{
   uint32_t chWraddr;
   uint32_t r5 = 0;
   
   uint64_t timeStamp = FAPI_TIMER_GetTimeStamp(1000);
   
   if (timeStamp > Data_21bd5788)
   {      
      if ((timeStamp - Data_21bd5788) >= 30)
      {
         r5 = 1;
      }
   }
   
   Data_21bd5788 = timeStamp;
   
   if (damRequestPtr->autoRestart == 0)
   {
      FREG_DMA_SetCcirConfig_Enable(0);
   }

   if (damRequestPtr->autoRestart == 2)
   {
      if (r5 == 1)
      {
         FREG_DMA_SetCcirConfig_Enable(0);
      }
      
      FREG_DMA_SetChConfig_Enable(damRequestPtr->channel, 0);
      
      chWraddr = damRequestPtr->channelConfig.chWraddr_OffsetAddress;
      
      if (Data_21bd5780 == 0)
      {
         Data_21bd5780 = 1;
         
         FREG_DMA_SetCcirConfig_CaptureField0(1);
         FREG_DMA_SetCcirConfig_CaptureField1(0);
         
         if (Data_21bd5784 == 0)
         {
            chWraddr += damRequestPtr->channelConfig.chWrinc_LineLength;
         }
         else
         {
            chWraddr = damRequestPtr->channelConfig.chWrinc_LineLength;
         }
      }
      else
      {
         Data_21bd5780 = 0;
         
         FREG_DMA_SetCcirConfig_CaptureField0(0);
         FREG_DMA_SetCcirConfig_CaptureField1(1);
      }
      
      FREG_DMA_SetChWraddr(damRequestPtr->channel, chWraddr);
      
      if (r5 == 1)
      {
         FREG_DMA_SetCcirConfig_Enable(1);
      }
      
      FREG_DMA_SetChConfig_Enable(damRequestPtr->channel, 1);
   }   
}


/* 21c1975c - complete */
int32_t FAPI_DMA_StopCcir656Capture(FAPI_DMA_RequestT* requestPtr)
{
   FAPI_DMA_RequestT* p;
   int32_t res = FAPI_DMA_ERR_INVALID_HANDLE;
   
   if (requestPtr == NULL)
   {      
      p = fapi_dma_channel[0];
      if (p->feature & FAPI_DMA_FEATURE_CCIR656_CAPTURE)
      {
         requestPtr = p;
      }
   }
   
   if (requestPtr != NULL)
   {
      res = dmaSetRequestAutoRestart(requestPtr, 0);
      
      FREG_DMA_SetChConfig_Enable(requestPtr->channel, 0);
      
      dmaRemoveChannelRequest(requestPtr);
      
      FAPI_DMA_ReleaseRequest(requestPtr);            
   }

   FREG_DMA_SetCcirConfig_Enable(0);
   
   return res;
}


/* 21c19534 - complete */
void func_21c19534(FAPI_DMA_RequestT* requestPtr)
{
   uint32_t chWraddr;

   FREG_DMA_SetCcirConfig_Enable(0);
   
   FREG_DMA_SetChConfig_Enable(requestPtr->channel, 0);
   
   chWraddr = requestPtr->channelConfig.chWraddr_OffsetAddress;
   
   if (Data_21bd5780 == 0)
   {
      Data_21bd5780 = 1;
      
      FREG_DMA_SetCcirConfig_CaptureField0(1);
      FREG_DMA_SetCcirConfig_CaptureField1(0);
      
      if (Data_21bd5784 == 0)
      {
         chWraddr += requestPtr->channelConfig.chWrinc_LineLength;
      }
      else
      {
         chWraddr = requestPtr->channelConfig.chWrinc_LineLength;
      }
   }
   else
   {
      Data_21bd5780 = 0;
      
      FREG_DMA_SetCcirConfig_CaptureField0(0);
      FREG_DMA_SetCcirConfig_CaptureField1(1);
   }
   
   FREG_DMA_SetChWraddr(requestPtr->channel, chWraddr);
   
   FREG_DMA_SetCcirConfig_Enable(1);
   
   FREG_DMA_SetChConfig_Enable(requestPtr->channel, 1);

   Data_21bd5788 = FAPI_TIMER_GetTimeStamp(1000) + 20000;
}




