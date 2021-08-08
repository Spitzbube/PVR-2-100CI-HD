
#include <string.h>
#include <fapi/sys_services.h>
#include <fapi/drv_intr.h>

#define FAPI_DMA_PRIV_C
#include <fapi/drv_dma_priv.h>
#include <fapi/drv_usb.h>




FAPI_SYS_DriverT FAPI_DMA_Driver = //21efa6bc 
{
      "DMA",
      FAPI_INTR_DMA,
      FAPI_SYS_BSR_EXEC_THREAD,
      FAPI_DMA_Init, 
      FAPI_DMA_Exit,
      FAPI_DMA_Isr,
      FAPI_DMA_Bsr,
      0x1000,
      0x100,
};



/* 21c18fb8 - complete */
int32_t FAPI_DMA_Init(void)
{
   uint32_t i = 0;
   
   if (fapi_dma_initialized == 1)
   {
      return FAPI_DMA_ERR_ALREADY_INITIALIZED;
   }
   
   fapi_dma_request = FAPI_SYS_MALLOC(FAPI_DMA_REQUEST_MAX * sizeof(FAPI_DMA_RequestT));
   
   if (fapi_dma_request == 0)
   {
      return FAPI_DMA_ERR_OUT_OF_MEMORY;
   }
   
   for (i = 0; i < FAPI_DMA_REQUEST_MAX; i++)
   {      
      FAPI_DMA_RequestT* h = &fapi_dma_request[i];
      
      memset(h, 0, sizeof(FAPI_DMA_RequestT));
      
      h->feature = FAPI_DMA_FEATURE_UNDEFINED;
   }
   
   fapi_dma_request_usage = 0;
   
   fapi_dma_fifo = FAPI_SYS_MALLOC(FAPI_DMA_FIFO_MAX * 4);
         
   if (fapi_dma_fifo == 0)
   {
      FAPI_SYS_FREE(fapi_dma_request);
      
      return FAPI_DMA_ERR_OUT_OF_MEMORY;
   }

   for (i = 0; i < FAPI_DMA_FIFO_MAX; i++)
   {
      fapi_dma_fifo[i] = 0;
   }
   
   fapi_dma_fifo_usage = 0;
   
   fapi_dma_channel = FAPI_SYS_MALLOC(FAPI_DMA_CHANNEL_MAX * 4);
         
   if (fapi_dma_channel == 0)
   {
      FAPI_SYS_FREE(fapi_dma_request);

      FAPI_SYS_FREE(fapi_dma_channel);
      
      return FAPI_DMA_ERR_OUT_OF_MEMORY;
   }

   for (i = 0; i < FAPI_DMA_CHANNEL_MAX; i++)
   {
      fapi_dma_channel[i] = 0;
   }
   
   fapi_dma_channel_usage = 0;
   
   fapi_dma_feature_allow[FAPI_DMA_FEATURE_INDEX_SDRAM_COPY] = 
      FAPI_DMA_FEATURE_REGISTER_COPY|FAPI_DMA_FEATURE_CCIR656_CAPTURE;
   
   fapi_dma_feature_allow[FAPI_DMA_FEATURE_INDEX_REGISTER_COPY] = 
      FAPI_DMA_FEATURE_SDRAM_COPY;
   
   fapi_dma_feature_allow[FAPI_DMA_FEATURE_INDEX_CCIR656_CAPTURE] = 
      FAPI_DMA_FEATURE_SDRAM_COPY;
   
   fapi_dma_feature_allow[FAPI_DMA_FEATURE_INDEX_COLOR_CONVERSION] = 
      FAPI_DMA_FEATURE_REGISTER_COPY;
   
   fapi_dma_feature_allow[FAPI_DMA_FEATURE_INDEX_SEARCH_REPLACE] = 
      FAPI_DMA_FEATURE_REGISTER_COPY;
   
   fapi_dma_feature_allow[FAPI_DMA_FEATURE_INDEX_AUDIO_CAPTURE] = 
      FAPI_DMA_FEATURE_SDRAM_COPY;
   
   fapi_dma_feature_allow[FAPI_DMA_FEATURE_INDEX_3DES_CRYPTO] = 
      FAPI_DMA_FEATURE_REGISTER_COPY;
   
   fapi_dma_feature_allow[FAPI_DMA_FEATURE_INDEX_USB_COPY] = 
      FAPI_DMA_FEATURE_SDRAM_COPY;
         
   FREG_DMA_SetIntMask(0);   
   FREG_DMA_SetIntClear(0x3ff);
   
   fapi_dma_initialized = 1;
   
   return 0;
}


/* 21c18eb4 - complete */
void FAPI_DMA_Exit(void)
{
   int cpu_sr = FAPI_SYS_DISABLE_IRQ;
         
   if (fapi_dma_initialized == 1)
   {
      FREG_DMA_SetIntMask(0x3ff);
      FREG_DMA_SetIntClear(0x3ff);

      FAPI_SYS_FREE(fapi_dma_request);            
      FAPI_SYS_FREE(fapi_dma_channel);      
      FAPI_SYS_FREE(fapi_dma_fifo);
      
      fapi_dma_request = 0;
      fapi_dma_channel = 0;
      fapi_dma_fifo = 0;
   }

   fapi_dma_initialized = 0;
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);
}


/* 21c191a4 - complete */
uint32_t FAPI_DMA_Isr(void)
{
   FAPI_DMA_RequestT* req;
   unsigned r6;
   uint32_t j;
   uint32_t i = 0;
   uint32_t res = 0;
   
   if (0 != FREG_DMA_GetIntStatus_CcirSyncChange())
   {
      for (i = 0; i < FAPI_DMA_CHANNEL_MAX; i++)
      {
         req = fapi_dma_channel[i];
         
         if ((req != 0) && 
               (req->feature & FAPI_DMA_FEATURE_CCIR656_CAPTURE))
         {
            req->status = FAPI_DMA_REQUEST_STATUS_ACTIVE;
            req->result = 0;
            
            dmaChangeChannelCallback(req);

            break;
         }
      }

      FREG_DMA_SetIntClear(0x200);
   }

   if (0 != FREG_DMA_GetIntStatus_CcirOverflow())
   {
      for (i = 0; i < FAPI_DMA_CHANNEL_MAX; i++)
      {
         req = fapi_dma_channel[i];
         
         if ((req != 0) && 
               (req->feature & FAPI_DMA_FEATURE_CCIR656_CAPTURE))
         {
            if (req->autoRestart != 2)
            {
               req->status = FAPI_DMA_REQUEST_STATUS_ABORTED;
               req->result = FAPI_DMA_ERR_CCIR_OVERFLOW;
               req->autoRestart = 0;
               
               dmaFinalizeChannelRequest(req);
               dmaRemoveChannelRequest(req);
               dmaSelfDestroyRequest(req);
            }
            else
            {
               func_21c19534(req);
            }

            break;
         }
      }

      FREG_DMA_SetIntClear(0x40);
   }

   if (0 != FREG_DMA_GetIntStatus_AudioOverflow())
   {
      for (i = 0; i < FAPI_DMA_CHANNEL_MAX; i++)
      {
         req = fapi_dma_channel[i];
         
         if ((req != 0) && 
               (req->feature & FAPI_DMA_FEATURE_AUDIO_CAPTURE))
         {
            req->status = FAPI_DMA_REQUEST_STATUS_ABORTED;
            req->result = FAPI_DMA_ERR_AUDIO_OVERFLOW;
            req->autoRestart = 0;
            
            dmaFinalizeChannelRequest(req);
            dmaRemoveChannelRequest(req);
            dmaSelfDestroyRequest(req);

            break;
         }
      }

      FREG_DMA_SetIntClear(0x80);
   }

   if (0 != FREG_DMA_GetIntStatus_SearchHit())
   {
      for (i = 0; i < FAPI_DMA_CHANNEL_MAX; i++)
      {
         req = fapi_dma_channel[i];
         
         if ((req != 0) && 
               (req->feature & FAPI_DMA_FEATURE_SEARCH_REPLACE))
         {
            req->status = FAPI_DMA_REQUEST_STATUS_ACTIVE;
            req->result = 0;
            
            dmaChangeChannelCallback(req);

            break;
         }
      }

      FREG_DMA_SetIntClear(0x100);
   }

   r6 = FREG_DMA_GetIntStatus_ChannelComplete();
   
   if (r6 != 0)
   {      
      for (i = 0; i < FAPI_DMA_CHANNEL_MAX; i++)
      {
         if ((r6 >> i) & 1)
         {
            req = fapi_dma_channel[i];
            
            if (req != 0)
            {
               if (req->channelConfig.chWraddr_PeripheralAddress == 0)
               {
                  FAPI_SYS_InvalidateDataCache(req->channelConfig.chWraddr_OffsetAddress, 
                        req->channelConfig.chLength);
               }

               req->result = 0;
               
               if ((req->channelConfig.chRdaddr_PeripheralAddress == 7) ||
                     (req->channelConfig.chWraddr_PeripheralAddress == 7))
               {
                  if (req->feature & FAPI_DMA_FEATURE_USB_COPY)
                  {
                     res = 0x80;
                  }
               }

               if ((req->feature & FAPI_DMA_FEATURE_CCIR656_CAPTURE) &&
                     (req->autoRestart == 2))
               {
                  if (req->postFinishCallback != 0)
                  {
                     (req->postFinishCallback)(req);
                  }

                  FREG_DMA_SetIntClear(1 << i);

                  break;
               }
               else if (req->autoRestart == 0)
               {
                  req->status = FAPI_DMA_REQUEST_STATUS_FINISHED;
                  
                  dmaRemoveChannelRequest(req);
                  dmaFinalizeChannelRequest(req);
                  dmaSelfDestroyRequest(req);
               }
               else
               {
                  req->status = FAPI_DMA_REQUEST_STATUS_ACTIVE;
                  
                  if (req->optSplitCount != 0)
                  {
                     if (req->completeSplitCount < req->optSplitCount)
                     {
                        req->completeSplitCount++;
                     }
                     else
                     {
                        req->completeSplitCount = 0;
                     }
                     
                     if (req->optSplitCallback != 0)
                     {
                        (req->optSplitCallback)(req->status,
                              req->channelConfig.chLength);
                     }
                  }
               }
            }

            FREG_DMA_SetIntClear(1 << i);
         }
      }      
   }

   i = 0;
   for (j = 0; j < FAPI_DMA_FIFO_MAX; j++)
   {
      if (fapi_dma_fifo[i] == 0) 
      {
         break;
      }
      
      req = fapi_dma_fifo[i];
            
      if (0 != dmaCheckChannelFeatureUsage(req->feature))
      {
         req = dmaAddChannelRequest(req);
         
         if (req == 0)
         {
            break;
         }
         
         dmaRemoveFifoRequest(req);
         dmaStartChannelRequest(req);
      }
      else
      {
         i++;
      }
   }

   return res;
}


/* 21c18ea4 - complete */
void FAPI_DMA_Bsr(uint32_t a)
{
   if (a & 0x80)
   {
      FAPI_USB_Bsr0(2);
   }
}


/* 21c18dc0 - complete */
FAPI_DMA_RequestT* FAPI_DMA_AllocateRequest( uint32_t feature, uint32_t selfDestroy )
{
   int cpu_sr;
   FAPI_DMA_RequestT* h = 0;
   uint32_t i = 0;
   
   if (fapi_dma_initialized == 0)
   {
      h = 0;
   }
   else
   {
      cpu_sr = FAPI_SYS_DISABLE_IRQ;

      for (i = 0; i < FAPI_DMA_REQUEST_MAX; i++)
      {
         h = &fapi_dma_request[i];
         
         if (h->feature == 0)
         {
            memset(h, 0, sizeof(FAPI_DMA_RequestT));
            
            h->feature = feature;
            h->status = FAPI_DMA_REQUEST_STATUS_ALLOCATED;
            h->result = 0;
            h->selfDestroy = selfDestroy;
            
            fapi_dma_request_usage++;
            break;
         }
      }
            
      FAPI_SYS_ENABLE_IRQ(cpu_sr);
      
      if (i == FAPI_DMA_REQUEST_MAX) 
      {
         h = 0;
      }
   }
   
   return h;
}


/* 21c18bd4 - complete */
void FAPI_DMA_ReleaseRequest( FAPI_DMA_RequestT* dmaRequestPtr )
{   
   if (fapi_dma_initialized != 0)
   {
      int cpu_sr = FAPI_SYS_DISABLE_IRQ;
      
      if (dmaRequestPtr != 0)
      {
         fapi_dma_request_usage--;
         
         memset(dmaRequestPtr, 0, sizeof(FAPI_DMA_RequestT));
         
         dmaRequestPtr->feature = FAPI_DMA_FEATURE_UNDEFINED;
         dmaRequestPtr->status = FAPI_DMA_REQUEST_STATUS_UNDEFINED;
      } 
      
      FAPI_SYS_ENABLE_IRQ(cpu_sr);
   }
}


/* 21c18b54 - complete */
int32_t FAPI_DMA_SendRequest(FAPI_DMA_RequestT* dmaRequestP)
{
   int32_t res = FAPI_OK;
   
   if (fapi_dma_initialized)
   {
      if ((dmaRequestP != NULL) && 
            (dmaRequestP->feature != FAPI_DMA_FEATURE_UNDEFINED))
      {
         if (0 != dmaAddChannelRequest(dmaRequestP))
         {
            dmaStartChannelRequest(dmaRequestP);
         }
         else if (0 == dmaAddFifoRequest(dmaRequestP))
         {
            res = FAPI_DMA_ERR_FIFO_OVERFLOW;
         }
      }
      else
      {
         res = FAPI_DMA_ERR_INVALID_HANDLE;
      }
   }
   else   
   {
      res = FAPI_DMA_ERR_NOT_INITIALIZED;
   }
   
   return res;
}


/* 21c18a58 - complete */
int32_t FAPI_DMA_SendRequest2( FAPI_DMA_RequestT* dmaRequestP, FAPI_DMA_RequestT* dmaRequest2Ptr )
{
   int32_t res = FAPI_OK;
   
   if (fapi_dma_initialized)
   {
      if (dmaRequestP == NULL)
      {
         res = FAPI_DMA_ERR_INVALID_HANDLE;
      }
      else if (dmaRequestP->feature == FAPI_DMA_FEATURE_UNDEFINED)
      {
         res = FAPI_DMA_ERR_INVALID_HANDLE;
      }
      else if (dmaRequest2Ptr == NULL)
      {
         res = FAPI_DMA_ERR_INVALID_HANDLE;
      }
      else if (dmaRequest2Ptr->feature == FAPI_DMA_FEATURE_UNDEFINED)
      {
         res = FAPI_DMA_ERR_INVALID_HANDLE;
      }
      else
      {
         if (0 != dmaAddChannelRequest(dmaRequestP))
         {
            dmaStartChannelRequest(dmaRequestP);
          
            if (0 != dmaAddChannelRequest(dmaRequest2Ptr))
            {
               dmaStartChannelRequest(dmaRequest2Ptr);
            }
            else
            {
               if (0 == dmaAddFifoRequest(dmaRequest2Ptr))
               {
                  dmaRemoveChannelRequest(dmaRequestP);
                  dmaRemoveFifoRequest(dmaRequestP);
                  
                  res = FAPI_DMA_ERR_FIFO_OVERFLOW;
               }
            }
         }
         else
         {
            if ((0 == dmaAddFifoRequest(dmaRequestP)) ||
                  (0 == dmaAddFifoRequest(dmaRequest2Ptr)))
            {
               dmaRemoveFifoRequest(dmaRequestP);
               dmaRemoveFifoRequest(dmaRequest2Ptr);
               
               res = FAPI_DMA_ERR_FIFO_OVERFLOW;
            }
         }
      }
   }
   else
   {
      res = FAPI_DMA_ERR_NOT_INITIALIZED;
   }
      
   return res;
}


/* 21c18790 - complete */
void dmaStartChannelRequest(FAPI_DMA_RequestT* a)
{
   uint32_t i = 0;
   uint32_t cpu_sr;
   
   cpu_sr = FAPI_SYS_DISABLE_IRQ;
   
   for (i = 0; i < FAPI_DMA_CHANNEL_MAX; i++)
   {
      if (a == fapi_dma_channel[i])
      {
         break;
      }
   } 
   
   if (i == FAPI_DMA_CHANNEL_MAX)
   {
      FAPI_SYS_ENABLE_IRQ(cpu_sr);
   }
   else
   {
      FAPI_DMA_ChannelConfigT* conf = &a->channelConfig;
      
      FREG_DMA_SetChLength(i, conf->chLength);
      FREG_DMA_SetChLladdr(i, conf->chLladdr); 
      FREG_DMA_SetChConfig_EndianSwapRead(i, conf->chRdaddr_EndianSwap);       
      FREG_DMA_SetChConfig_PeripheralAddressRead(i, conf->chRdaddr_PeripheralAddress);
      FREG_DMA_SetChRdline_Lines(i, conf->chRdline_Lines);
      FREG_DMA_SetChRdline_LoopIncrement(i, conf->chRdline_LoopIncrement);
      FREG_DMA_SetChRdinc_LineLength(i, conf->chRdinc_LineLength);
      FREG_DMA_SetChRdinc_LineIncrement(i, conf->chRdinc_LineIncrement);
      FREG_DMA_SetChConfig_EndianSwapWrite(i, conf->chWraddr_EndianSwap);
      FREG_DMA_SetChConfig_PeripheralAddressWrite(i, conf->chWraddr_PeripheralAddress);
      FREG_DMA_SetChWrline_Lines(i, conf->chWrline_Lines);
      FREG_DMA_SetChWrline_LoopIncrement(i, conf->chWrline_LoopIncrement);
      FREG_DMA_SetChWrinc_LineLength(i, conf->chWrinc_LineLength);
      FREG_DMA_SetChWrinc_LineIncrement(i, conf->chWrinc_LineIncrement);

      FREG_DMA_SetChRdaddr(i, FAPI_SYS_GET_PHYSICAL_ADDRESS(conf->chRdaddr_OffsetAddress));
      FREG_DMA_SetChRdlpaddr(i, FAPI_SYS_GET_PHYSICAL_ADDRESS(conf->chRdlpaddr_LoopAddress));
      FREG_DMA_SetChWraddr(i, FAPI_SYS_GET_PHYSICAL_ADDRESS(conf->chWraddr_OffsetAddress));      
      FREG_DMA_SetChWrlpaddr(i, FAPI_SYS_GET_PHYSICAL_ADDRESS(conf->chWrlpaddr_LoopAddress));
      
      FREG_DMA_SetIntMask(0x3ff);
      
      FAPI_SYS_ENABLE_IRQ(cpu_sr);

      if (conf->chRdaddr_PeripheralAddress == 0)
      {
         if ((conf->chRdlpaddr_LoopAddress != 0) && 
               (conf->chRdlpaddr_LoopAddress == 
                  (int) &a->optData))
         {
            FAPI_SYS_FlushDataCache(conf->chRdaddr_OffsetAddress, 4);
         }
         else
         {
            FAPI_SYS_FlushDataCache(conf->chRdaddr_OffsetAddress, 
                  conf->chLength);            
         }
      }

      if (conf->chWraddr_PeripheralAddress == 0)
      {
         FAPI_SYS_FlushDataCache(conf->chWraddr_OffsetAddress, 
               conf->chLength);
      }

      if (a->preEnableCallback != 0)
      {
         (a->preEnableCallback)(a);
      } 
      
      if ((a->autoRestart == 1) &&
            (a->optSplitCount == 0))
      {
         FREG_DMA_SetChConfig_AutoRestartEnable(i, 1);
      } 

      a->status = FAPI_DMA_REQUEST_STATUS_ACTIVE;
      
      if (a->optSplitCount != 0)
      {
         FREG_DMA_SetChConfig(i, FREG_DMA_GetChConfig(i) | 0x0C);
      }
      else
      {
         FREG_DMA_SetChConfig_Enable(i, 1);         
      }

      FREG_DMA_SetIntMask(0);         
      
      if (a->postEnableCallback != 0)
      {
         (a->postEnableCallback)(a);
      }       
   } 
}


/* 21c18740 - complete */
void dmaFinalizeChannelRequest(FAPI_DMA_RequestT* a)
{
   if (a != 0)
   {
      if (a->channel < FAPI_DMA_CHANNEL_MAX)
      {
         FREG_DMA_SetChConfig(a->channel, 0);
         
         if (a->postFinishCallback != 0)
         {
            (a->postFinishCallback)(a);
         } 
         
         if (a->optFinishCallback != 0)
         {
            (a->optFinishCallback)(a->result, 
                  a->channelConfig.chLength);
         }       
      } 
   }
}


/* 21c186f4 - complete */
void dmaChangeChannelCallback(FAPI_DMA_RequestT* a)
{
   int b = 0;
   if (a != 0)
   {
      
      if (a->optChangeCallback != 0)
      {
         if (a->feature == FAPI_DMA_FEATURE_SEARCH_REPLACE)
         {
            b = func_219316b4(a);
         } 

         (a->optChangeCallback)(a->result, b);
      } 
   }
}


/* 21c18088 - complete */
uint32_t FAPI_DMA_GetRequestStatus( FAPI_DMA_RequestT* dmaRequestP )
{
   uint32_t res = 0;
   
   if (dmaRequestP != 0)
   {
      if (dmaRequestP->feature != 0)
      {
         res = dmaRequestP->status;
      }
   }
   
   return res;
}


/* 21c180a8 - complete */
uint32_t FAPI_DMA_GetRequestList(uint32_t feature, FAPI_DMA_RequestS* dmaRequestPtrArray[], uint32_t requestArraySize)
{
   uint32_t i = 0;
   uint32_t count = 0;
   
   if (fapi_dma_initialized)
   {
      uint32_t irqFlags = FAPI_SYS_DISABLE_IRQ;
      
      for (i = 0; (i < FAPI_DMA_REQUEST_MAX) && (count < requestArraySize); i++)
      {
         FAPI_DMA_RequestT* r1 = &fapi_dma_request[i];
         
         if (r1->feature == feature)
         {
            *dmaRequestPtrArray++ = r1;
            count++;
         }
      }
      
      FAPI_SYS_ENABLE_IRQ(irqFlags);
   }
   
   return count;
}


/* 21c18c74 - complete */
int32_t FAPI_DMA_CancelRequest( FAPI_DMA_RequestT* dmaRequestPtr, uint32_t feature )
{
   uint32_t j;
   int found = 0;
   uint32_t i = 0;
   int32_t res = FAPI_DMA_ERR_BAD_PARAMETER;
   
   if (fapi_dma_initialized)
   {
      uint32_t irqFlags = FAPI_SYS_DISABLE_IRQ;

      for (i = 0; i < FAPI_DMA_FIFO_MAX; i++)
      {
         FAPI_DMA_RequestT* r1 = fapi_dma_fifo[i];
         
         if ((r1 == dmaRequestPtr) &&
               (r1->feature == feature))
         {
            found = 1;
            break;
         }
      }
      
      if (found == 1)
      {
         dmaRequestPtr->status = FAPI_DMA_REQUEST_STATUS_ABORTED;
         dmaRequestPtr->result = FAPI_DMA_ERR_CANCELED; 
         dmaRequestPtr->autoRestart = 0;
         
         if (dmaRequestPtr->optFinishCallback != 0)
         {
            (dmaRequestPtr->optFinishCallback)(dmaRequestPtr->result, 
                  dmaRequestPtr->channelConfig.chLength);
         }
         
         dmaRemoveFifoRequest(dmaRequestPtr);
         dmaSelfDestroyRequest(dmaRequestPtr);
         
         res = FAPI_OK;
      }
      else
      {
         for (j = 0; j < FAPI_DMA_CHANNEL_MAX; j++)
         {
            FAPI_DMA_RequestT* r1 = fapi_dma_channel[j];
            
            if ((r1 == dmaRequestPtr) &&
                  (r1->feature == feature))
            {
               found = 1;
               break;
            }
         }
         
         if (found == 1)
         {
            dmaRequestPtr->status = FAPI_DMA_REQUEST_STATUS_ABORTED;
            dmaRequestPtr->result = FAPI_DMA_ERR_CANCELED; 
            dmaRequestPtr->autoRestart = 0;

            dmaRemoveChannelRequest(dmaRequestPtr);
            dmaFinalizeChannelRequest(dmaRequestPtr);
            dmaSelfDestroyRequest(dmaRequestPtr);
            
            res = FAPI_OK;
         }
      }
      
      FAPI_SYS_ENABLE_IRQ(irqFlags);
   }
   else
   {
      res = FAPI_DMA_ERR_NOT_INITIALIZED;
   }
   
   return res;
}


/* 21c186c8 - complete */
int32_t dmaSetRequestAutoRestart(FAPI_DMA_RequestT* requestPtr, uint32_t restart)
{
   int32_t res = 0;
   
   if (requestPtr != NULL)
   {
      if (requestPtr->feature & (FAPI_DMA_FEATURE_CCIR656_CAPTURE|FAPI_DMA_FEATURE_AUDIO_CAPTURE))
      {
         requestPtr->autoRestart = restart;
      }
      else
      {
         res = FAPI_DMA_ERR_BAD_FEATURE;
      }
   }
   else
   {
      res = FAPI_DMA_ERR_INVALID_HANDLE;
   }
   
   return res;
}


/* 21c18674 - complete */
int32_t dmaSetRequestSplitTransfer(FAPI_DMA_RequestT* requestPtr,
                                   uint32_t numberOfSplit,
                                   FAPI_DMA_OptCallbackT splitCallback)
{
   int32_t res = 0;

   if (requestPtr != NULL)
   {
      if (requestPtr->feature & FAPI_DMA_FEATURE_AUDIO_CAPTURE)
      {
         if (requestPtr->autoRestart == 1)
         {
            if (requestPtr->optSplitCount == 0)
            {
               requestPtr->optSplitCount = numberOfSplit;
               requestPtr->completeSplitCount = 0;
               requestPtr->optSplitCallback = splitCallback;
            }
            else
            {
               res = FAPI_DMA_ERR_BAD_FEATURE;
            }
         }
         else
         {
            res = FAPI_DMA_ERR_BAD_FEATURE;
         }
      }
      else
      {
         res = FAPI_DMA_ERR_BAD_FEATURE;
      }
   }
   else
   {
      res = FAPI_DMA_ERR_INVALID_HANDLE;
   }

   return res;
}


/* 21c1814c - complete */
void dmaReportUsage(const char_t* title)
{
   uint32_t i = 0;

   FAPI_SYS_PRINT_MSG("%s\n", (title != 0)? title: "");

   FAPI_SYS_PRINT_MSG("fifo usage: %d/%d\n", fapi_dma_fifo_usage, FAPI_DMA_FIFO_MAX);

   for (i = 0; i < FAPI_DMA_FIFO_MAX; i++)
   {
      if (fapi_dma_fifo[i] == NULL)
      {
         break;
      }

      FAPI_SYS_PRINT_MSG("fifo#%d:\n", i);
      FAPI_SYS_PRINT_MSG("  request...: 0x%08x\n", fapi_dma_fifo[i]);
      FAPI_SYS_PRINT_MSG("  feature...: %d\n", fapi_dma_fifo[i]->feature);
      FAPI_SYS_PRINT_MSG("  status....: %d\n", fapi_dma_fifo[i]->status);
   }

   FAPI_SYS_PRINT_MSG("channel usage: %d/%d\n", fapi_dma_channel_usage, FAPI_DMA_CHANNEL_MAX);

   for (i = 0; i < FAPI_DMA_CHANNEL_MAX; i++)
   {
      if (fapi_dma_channel[i] != NULL)
      {
         FAPI_SYS_PRINT_MSG("channel#%d:\n", i);
         FAPI_SYS_PRINT_MSG("  request...: 0x%08x\n", fapi_dma_channel[i]);
         FAPI_SYS_PRINT_MSG("  feature...: %d\n", fapi_dma_channel[i]->feature);
         FAPI_SYS_PRINT_MSG("  status....: %d\n", fapi_dma_channel[i]->status);
      }
   }
}


/* 21c184a8 - complete */
FAPI_DMA_RequestT* dmaAddChannelRequest(FAPI_DMA_RequestT* a)
{
   int32_t i;
   FAPI_DMA_RequestT* r = 0;

   if (a != 0)
   {
      int cpu_sr = FAPI_SYS_DISABLE_IRQ;

      if (a->autoRestart == 1)
      {
         i = 0;

         if ((fapi_dma_channel[i] == 0) &&
            (0 != dmaCheckChannelFeatureUsage(a->feature)))
         {
            fapi_dma_channel[i] = a;
            fapi_dma_channel_usage++;
            r = a;
            a->status = FAPI_DMA_REQUEST_STATUS_CHANNELED;
            a->channel = i & 0x0F;
         }
      }
      else
      {
         for (i = 5; i >= 0; i--)
         {
            if (fapi_dma_channel[i] == 0)
            {
               if (0 != dmaCheckChannelFeatureUsage(a->feature))
               {
                  fapi_dma_channel[i] = a;
                  fapi_dma_channel_usage++;
                  r = a;
                  a->status = FAPI_DMA_REQUEST_STATUS_CHANNELED;
                  a->channel = i & 0x0F;
               }
               break;
            }
         }
      }

      FAPI_SYS_ENABLE_IRQ(cpu_sr);
   }

   return r;
}


/* 21c18358 - complete */
FAPI_DMA_RequestT* dmaRemoveChannelRequest(FAPI_DMA_RequestT* a)
{
   FAPI_DMA_RequestT* ip = 0;

   if (fapi_dma_channel[a->channel] == a)
   {
      ip = a;
      fapi_dma_channel[a->channel] = 0;
      fapi_dma_channel_usage--;
   }

   return ip;
}


/* 21c185b4 - complete */
FAPI_DMA_RequestT* dmaAddFifoRequest(FAPI_DMA_RequestT* a)
{
   FAPI_DMA_RequestT* p = 0;
   uint32_t i = 0;

   for (i = 0; i < FAPI_DMA_FIFO_MAX; i++)
   {
      if (fapi_dma_fifo[i] == 0)
      {
         fapi_dma_fifo[i] = a;
         a->status = FAPI_DMA_REQUEST_STATUS_QUEUED;
         p = a;
         fapi_dma_fifo_usage++;
         break;
      }
   }

   return p;
}


/* 21c18618 - complete */
FAPI_DMA_RequestT* dmaRemoveFifoRequest(FAPI_DMA_RequestT* a)
{
   FAPI_DMA_RequestT* p = 0;
   uint32_t i = 0;

   for (i = 0; i < FAPI_DMA_FIFO_MAX; i++)
   {
      if (fapi_dma_fifo[i] == a)
      {
         p = a;
         fapi_dma_fifo_usage--;
      }
      else if (p != 0)
      {
         fapi_dma_fifo[i-1] = fapi_dma_fifo[i];
      }
   }

   return p;
}


/* 21c18c5c - complete */
void dmaSelfDestroyRequest(FAPI_DMA_RequestT* a)
{
   if (a != 0)
   {
      if (a->selfDestroy != 0)
      {
         FAPI_DMA_ReleaseRequest(a);
      }
   }
}


/* 21c1839c - complete */
uint32_t dmaCheckChannelFeatureUsage(uint16_t feature)
{
   unsigned i = 0;
   uint32_t res = 1;
   unsigned iChannel = 0;
   unsigned count = 0;

   int cpu_sr = FAPI_SYS_DISABLE_IRQ;

   if (feature == 1)
   {
      count = 0;
      for (iChannel = 0; iChannel < FAPI_DMA_CHANNEL_MAX; iChannel++)
      {
         if ((fapi_dma_channel[iChannel] != 0) &&
            (fapi_dma_channel[iChannel]->feature == feature))
         {
            count++;
         }
      }

      if (count >= fapi_dma_feature_allow[0])
      {
         res = 0;
      }
   }
   else
   {
      for (i = 1; i < FAPI_DMA_FEATURE_COUNT; i++)
      {
         count = 0;
         if (1 == ((feature >> i) & 1))
         {
            for (iChannel = 0; iChannel < FAPI_DMA_CHANNEL_MAX; iChannel++)
            {
               if ((fapi_dma_channel[iChannel] != 0) &&
                  (((fapi_dma_channel[iChannel]->feature >> i) & 1) == 1))
               {
                  count++;
               }
            }

            if (count >= fapi_dma_feature_allow[i])
            {
               res = 0;
               break;
            }

#if 0
            //Only in Megasat / Evolve Apollo
            if (i == 6)
            {
               for (iChannel = 0; iChannel < FAPI_DMA_CHANNEL_MAX; iChannel++)
               {
                  if ((fapi_dma_channel[iChannel] != 0) &&
                     (((fapi_dma_channel[iChannel]->feature >> 7) & 1) == 1))
                  {
                     res = 0;
                     break;
                  }
               }
            }
            else if (i == 7)
            {
               for (iChannel = 0; iChannel < FAPI_DMA_CHANNEL_MAX; iChannel++)
               {
                  if ((fapi_dma_channel[iChannel] != 0) &&
                     (((fapi_dma_channel[iChannel]->feature >> 6) & 1) == 1))
                  {
                     res = 0;
                     break;
                  }
               }
            }
#endif
         }
      }
   }

   FAPI_SYS_ENABLE_IRQ(cpu_sr);

   return res;
}




#if 0
#include <string.h>
#include <fapi/sys_services.h>
#include <fapi/reg_dma.h>
#include <fapi/drv_dma.h>
#include <fapi/drv_usb.h>



void dmaCloseRequest(FAPI_DMA_RequestT*);
void dmaSetParams(FAPI_DMA_RequestT* a);
void dmaFinishRequest(FAPI_DMA_RequestT* a);
void dmaChangeCallback(FAPI_DMA_RequestT* a);
FAPI_DMA_RequestT* dmaDequeueRequest(FAPI_DMA_RequestT*);
FAPI_DMA_RequestT* dmaQueueRequest(FAPI_DMA_RequestT*);
FAPI_DMA_RequestT* dmaChannelRequest(FAPI_DMA_RequestT* a);
int func_21c1839c(unsigned short a);
FAPI_DMA_RequestT* dmaUnchannelRequest(FAPI_DMA_RequestT* a);


FAPI_SYS_DriverT FAPI_DMA_Driver = //21efa6bc 
{
      "DMA",
      20,
      1,
      FAPI_DMA_Init, 
      FAPI_DMA_Exit,
      FAPI_DMA_Isr,
      FAPI_DMA_Bsr,
      0x1000,
      0x100,
};


static int dmaInitialized; //21f21e58
static FAPI_DMA_RequestT* fapi_dma_handles; //21f21e60
static int dmaNumRequests; //21f21e64
static FAPI_DMA_RequestT** fapi_dma_queued_transfers; //21f21e68
static int dmaNumQueuedRequests; //21f21e6c
static FAPI_DMA_RequestT** dmaChannelledRequests; //21f21e70
static unsigned dmaNumChannelledRequests; //21f21e74
static unsigned Data_21f21e78[8]; //21f21e78



/* 21c19bf8 - todo */
int func_21c19bf8(FAPI_DMA_RequestT* a)
{
   printf("func_21c19bf8");
   return 0;
}


/* 21c19534 - todo */
int func_21c19534(FAPI_DMA_RequestT* a)
{
   printf("func_21c19534");
   return 0;
}


/* 21c191a4 - complete */
uint32_t FAPI_DMA_Isr(void)
{
   int i, j;
   FAPI_DMA_RequestT* req;
   unsigned r6;
   uint32_t res = 0;
   
   if (0 != FREG_DMA_GetIntStatus_CcirSyncChange())
   {
      for (i = 0; i < 6; i++)
      {
         req = dmaChannelledRequests[i];
         
         if ((req != 0) && 
               (req->feature & FAPI_DMA_FEATURE_CCIR656_CAPTURE))
         {
            req->status = FAPI_DMA_REQUEST_STATUS_ACTIVE;
            req->result = 0;
            
            dmaChangeCallback(req);

            break;
         }
      }

      FREG_DMA_SetIntClear(0x200);
   }

   if (0 != FREG_DMA_GetIntStatus_CcirOverflow())
   {
      for (i = 0; i < 6; i++)
      {
         req = dmaChannelledRequests[i];
         
         if ((req != 0) && 
               (req->feature & FAPI_DMA_FEATURE_CCIR656_CAPTURE))
         {
            if (req->autoRestart != 2)
            {
               req->status = FAPI_DMA_REQUEST_STATUS_ABORTED;
               req->result = FAPI_DMA_ERR_CCIR_OVERFLOW;
               req->autoRestart = 0;
               
               dmaFinishRequest(req);
               dmaUnchannelRequest(req);
               dmaCloseRequest(req);
            }
            else
            {
               func_21c19534(req);
            }

            break;
         }
      }

      FREG_DMA_SetIntClear(0x40);
   }

   if (0 != FREG_DMA_GetIntStatus_AudioOverflow())
   {
      for (i = 0; i < 6; i++)
      {
         req = dmaChannelledRequests[i];
         
         if ((req != 0) && 
               (req->feature & FAPI_DMA_FEATURE_AUDIO_CAPTURE))
         {
            req->status = FAPI_DMA_REQUEST_STATUS_ABORTED;
            req->result = FAPI_DMA_ERR_AUDIO_OVERFLOW;
            req->autoRestart = 0;
            
            dmaFinishRequest(req);
            dmaUnchannelRequest(req);
            dmaCloseRequest(req);

            break;
         }
      }

      FREG_DMA_SetIntClear(0x80);
   }

   if (0 != FREG_DMA_GetIntStatus_SearchHit())
   {
      for (i = 0; i < 6; i++)
      {
         req = dmaChannelledRequests[i];
         
         if ((req != 0) && 
               (req->feature & FAPI_DMA_FEATURE_SEARCH_REPLACE))
         {
            req->status = FAPI_DMA_REQUEST_STATUS_ACTIVE;
            req->result = 0;
            
            dmaChangeCallback(req);

            break;
         }
      }

      FREG_DMA_SetIntClear(0x100);
   }

   r6 = FREG_DMA_GetIntStatus_ChannelComplete();
   
   if (r6 != 0)
   {      
      for (i = 0; i < 6; i++)
      {
         if ((r6 >> i) & 1)
         {
            req = dmaChannelledRequests[i];
            
            if (req != 0)
            {
               if (req->channelConfig.chWraddr_PeripheralAddress == 0)
               {
                  FAPI_SYS_InvalidateDataCache(req->channelConfig.chWraddr_OffsetAddress, 
                        req->channelConfig.chLength);
               }

               req->result = 0;
               
               if ((req->channelConfig.chRdaddr_PeripheralAddress == 7) ||
                     (req->channelConfig.chWraddr_PeripheralAddress == 7))
               {
                  if (req->feature & FAPI_DMA_FEATURE_USB_COPY)
                  {
                     res = 0x80;
                  }
               }

               if ((req->feature & FAPI_DMA_FEATURE_CCIR656_CAPTURE) &&
                     (req->autoRestart == 2))
               {
                  if (req->postFinishCallback != 0)
                  {
                     (req->postFinishCallback)(req);
                  }

                  FREG_DMA_SetIntClear(1 << i);

                  break;
               }
               else if (req->autoRestart == 0)
               {
                  req->status = FAPI_DMA_REQUEST_STATUS_FINISHED;
                  
                  dmaUnchannelRequest(req);
                  dmaFinishRequest(req);
                  dmaCloseRequest(req);
               }
               else
               {
                  req->status = FAPI_DMA_REQUEST_STATUS_ACTIVE;
                  
                  if (req->optSplitCount != 0)
                  {
                     if (req->optSplitCount > req->completeSplitCount)
                     {
                        req->completeSplitCount++;
                     }
                     else
                     {
                        req->completeSplitCount = 0;
                     }
                     
                     if (req->optSplitCallback != 0)
                     {
                        (req->optSplitCallback)(req->status,
                              req->channelConfig.chLength);
                     }
                  }
               }
            }

            FREG_DMA_SetIntClear(1 << i);
         }
      }      
   }

   for (i = j = 0; (j < 16); j++)
   {
      if (fapi_dma_queued_transfers[i] == 0) 
      {
         break;
      }
      
      req = fapi_dma_queued_transfers[i];
            
      if (0 != func_21c1839c(req->feature))
      {
         req = dmaChannelRequest(req);
         
         if (req == 0)
         {
            break;
         }
         
         dmaDequeueRequest(req);
         dmaSetParams(req);
      }
      else
      {
         i++;
      }
   }

   return res;
}


/* 21c18fb8 - todo */
int32_t FAPI_DMA_Init(void)
{
   unsigned i, j, k;
   
   if (dmaInitialized == 1)
   {
      return 0xffffe0bd;
   }
   
   fapi_dma_handles = FAPI_SYS_MALLOC(16 * sizeof(FAPI_DMA_RequestT));
   
   if (fapi_dma_handles == 0)
   {
      return 0xffffe0be;
   }
   
   for (i = 0; i < 16; i++)
   {      
      FAPI_DMA_RequestT* h = &fapi_dma_handles[i];
      
      memset(h, 0, sizeof(FAPI_DMA_RequestT));
      
      h->feature = FAPI_DMA_FEATURE_UNDEFINED;
   }
   
   dmaNumRequests = 0;
   
   fapi_dma_queued_transfers = FAPI_SYS_MALLOC(16 * 4);
         
   if (fapi_dma_queued_transfers == 0)
   {
      FAPI_SYS_FREE(fapi_dma_handles);
      
      return 0xffffe0be;
   }

   for (k = 0; k < 16; k++)
   {
      fapi_dma_queued_transfers[k] = 0;
   }
   
   dmaNumQueuedRequests = 0;
   
   dmaChannelledRequests = FAPI_SYS_MALLOC(6 * 4);
         
   if (dmaChannelledRequests == 0)
   {
      FAPI_SYS_FREE(fapi_dma_handles);

      FAPI_SYS_FREE(dmaChannelledRequests);
      
      return 0xffffe0be;
   }

   for (j = 0; j < 6; j++)
   {
      dmaChannelledRequests[j] = 0;
   }
   
   dmaNumChannelledRequests = 0;
   
   Data_21f21e78[0] = 6;
   Data_21f21e78[1] = 1;
   Data_21f21e78[2] = 1;
   Data_21f21e78[3] = 2;
   Data_21f21e78[4] = 2;
   Data_21f21e78[5] = 1;
   Data_21f21e78[6] = 2;
   Data_21f21e78[7] = 1;
         
   FREG_DMA_SetIntMask(0);   
   FREG_DMA_SetIntClear(0x3ff);
   
   dmaInitialized = 1;
   
   return 0;
}


/* 21c18eb4 - complete */
void FAPI_DMA_Exit(void)
{
   int cpu_sr = FAPI_SYS_DISABLE_IRQ;
         
   if (dmaInitialized == 1)
   {
      FREG_DMA_SetIntMask(0x3ff);
      FREG_DMA_SetIntClear(0x3ff);

      FAPI_SYS_FREE(fapi_dma_handles);            
      FAPI_SYS_FREE(dmaChannelledRequests);      
      FAPI_SYS_FREE(fapi_dma_queued_transfers);
      
      fapi_dma_handles = 0;
      dmaChannelledRequests = 0;
      fapi_dma_queued_transfers = 0;
   }

   dmaInitialized = 0;
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);
}


/* 21c18ea4 - complete */
void FAPI_DMA_Bsr(uint32_t a)
{
   if (a & 0x80)
   {
      FAPI_USB_Bsr0(2);
   }
}


/* 21c18dc0 - complete */
FAPI_DMA_RequestT* FAPI_DMA_AllocateRequest( uint32_t feature, uint32_t selfDestroy )
{
   int cpu_sr;
   int i;
   FAPI_DMA_RequestT* h;
   
   if (dmaInitialized == 0)
   {
      h = 0;
   }
   else
   {
      cpu_sr = FAPI_SYS_DISABLE_IRQ;

      for (i = 0; i < 16; i++)
      {
         h = &fapi_dma_handles[i];
         
         if (h->feature == 0)
         {
            memset(h, 0, sizeof(FAPI_DMA_RequestT));
            
            h->feature = feature;
            h->status = FAPI_DMA_REQUEST_STATUS_ALLOCATED;
            h->result = 0;
            h->selfDestroy = selfDestroy;
            
            dmaNumRequests++;
            break;
         }
      }
            
      FAPI_SYS_ENABLE_IRQ(cpu_sr);
      
      if (i == 16) h = 0;
   }
   
   return h;
}


/* 21c18c74 - nearly complete */
int32_t FAPI_DMA_CancelRequest( FAPI_DMA_RequestT* dmaRequestPtr, uint32_t feature )
{
   unsigned i, j;
   int res = 0;
   
   if (dmaInitialized == 0)
   {
      res = -8004;
   }
   else
   {
      int cpu_sr = FAPI_SYS_DISABLE_IRQ;

      for (i = 0; i < 16; i++)
      {
         if ((fapi_dma_queued_transfers[i] == dmaRequestPtr) &&
               (dmaRequestPtr->feature == feature))
         {
            dmaRequestPtr->status = FAPI_DMA_REQUEST_STATUS_ABORTED;
            dmaRequestPtr->result = FAPI_DMA_ERR_CANCELED; //-8110;
            dmaRequestPtr->autoRestart = 0;
            
            if (dmaRequestPtr->optFinishCallback != 0)
            {
               (dmaRequestPtr->optFinishCallback)(dmaRequestPtr->result, 
                     dmaRequestPtr->channelConfig.chLength);
            }
            
            dmaDequeueRequest(dmaRequestPtr);
            dmaCloseRequest(dmaRequestPtr);

            goto end;
         }
      }
      
      for (j = 0; j < 6; j++)
      {
         if ((dmaChannelledRequests[j] == dmaRequestPtr) &&
               (dmaRequestPtr->feature == feature))
         {
            dmaRequestPtr->status = FAPI_DMA_REQUEST_STATUS_ABORTED;
            dmaRequestPtr->result = FAPI_DMA_ERR_CANCELED; //-8110;
            dmaRequestPtr->autoRestart = 0;

            dmaUnchannelRequest(dmaRequestPtr);
            dmaFinishRequest(dmaRequestPtr);
            dmaCloseRequest(dmaRequestPtr);

            goto end;
         }
      }
      
      res = -8001;
end:
      FAPI_SYS_ENABLE_IRQ(cpu_sr);
   }
   
   return res;
}


/* 21c18c5c - complete */
void dmaCloseRequest(FAPI_DMA_RequestT* a)
{
   if (a != 0)
   {
      if (a->selfDestroy != 0)
      {
         FAPI_DMA_ReleaseRequest(a);
      } 
   } 
}


/* 21c18bd4 - complete */
void FAPI_DMA_ReleaseRequest( FAPI_DMA_RequestT* dmaRequestPtr )
{   
   if (dmaInitialized != 0)
   {
      int cpu_sr = FAPI_SYS_DISABLE_IRQ;
      
      if (dmaRequestPtr != 0)
      {
         dmaNumRequests--;
         
         memset(dmaRequestPtr, 0, sizeof(FAPI_DMA_RequestT));
         
         dmaRequestPtr->feature = FAPI_DMA_FEATURE_UNDEFINED;
         dmaRequestPtr->status = FAPI_DMA_REQUEST_STATUS_UNDEFINED;
      } 
      
      FAPI_SYS_ENABLE_IRQ(cpu_sr);
   }
}


/* 21c18b54 - complete */
int32_t FAPI_DMA_SendRequest(FAPI_DMA_RequestT* dmaRequestP)
{
   if (dmaInitialized == 0)
   {
      return FAPI_DMA_ERR_NOT_INITIALIZED;
   }
      
   if ((dmaRequestP == 0) || 
         (dmaRequestP->feature == 0))
   {
      return FAPI_DMA_ERR_INVALID_HANDLE;
   }
   
   if (0 != dmaChannelRequest(dmaRequestP))
   {
      dmaSetParams(dmaRequestP);
   }
   else if (0 == dmaQueueRequest(dmaRequestP))
   {
      return FAPI_DMA_ERR_FIFO_OVERFLOW;
   }
   
   return 0;
}


/* 21c18a58 - complete */
int32_t FAPI_DMA_SendRequest2( FAPI_DMA_RequestT* dmaRequestP, FAPI_DMA_RequestT* dmaRequest2Ptr )
{
   if (dmaInitialized == 0)
   {
      return FAPI_DMA_ERR_NOT_INITIALIZED;
   }
   
   if ((dmaRequestP == 0) || 
         (dmaRequestP->feature == 0))
   {
      return FAPI_DMA_ERR_INVALID_HANDLE;
   }
   
   if ((dmaRequest2Ptr == 0) || 
         (dmaRequest2Ptr->feature == 0))      
   {
      return FAPI_DMA_ERR_INVALID_HANDLE;
   }

   if (0 != dmaChannelRequest(dmaRequestP))
   {
      dmaSetParams(dmaRequestP);
    
      if (0 != dmaChannelRequest(dmaRequest2Ptr))
      {
         dmaSetParams(dmaRequest2Ptr);
      }
      else
      {
         if (0 == dmaQueueRequest(dmaRequest2Ptr))
         {
            dmaUnchannelRequest(dmaRequestP);
            dmaDequeueRequest(dmaRequestP);
            
            return FAPI_DMA_ERR_FIFO_OVERFLOW;
         }
      }
   }
   else
   {
      if ((0 == dmaQueueRequest(dmaRequestP)) ||
            (0 == dmaQueueRequest(dmaRequest2Ptr)))
      {
         dmaDequeueRequest(dmaRequestP);
         dmaDequeueRequest(dmaRequest2Ptr);
         
         return FAPI_DMA_ERR_FIFO_OVERFLOW;
      }
   }
   
   return 0;
}


/* 21c18790 - complete */
void dmaSetParams(FAPI_DMA_RequestT* a)
{
   int i;
   int cpu_sr;
   
   cpu_sr = FAPI_SYS_DISABLE_IRQ;
   
   for (i = 0; i < 6; i++)
   {
      if (a == dmaChannelledRequests[i])
      {
         break;
      }
   } 
   
   if (i == 6)
   {
      FAPI_SYS_ENABLE_IRQ(cpu_sr);
   }
   else
   {
      FREG_DMA_SetChLength(i, a->channelConfig.chLength);
      FREG_DMA_SetChLladdr(i, a->channelConfig.chLladdr); 
      FREG_DMA_SetChConfig_EndianSwapRead(i, a->channelConfig.chRdaddr_EndianSwap);       
      FREG_DMA_SetChConfig_PeripheralAddressRead(i, a->channelConfig.chRdaddr_PeripheralAddress);
      FREG_DMA_SetChRdline_Lines(i, a->channelConfig.chRdline_Lines);
      FREG_DMA_SetChRdline_LoopIncrement(i, a->channelConfig.chRdline_LoopIncrement);
      FREG_DMA_SetChRdinc_LineLength(i, a->channelConfig.chRdinc_LineLength);
      FREG_DMA_SetChRdinc_LineIncrement(i, a->channelConfig.chRdinc_LineIncrement);
      FREG_DMA_SetChConfig_EndianSwapWrite(i, a->channelConfig.chWraddr_EndianSwap);
      FREG_DMA_SetChConfig_PeripheralAddressWrite(i, a->channelConfig.chWraddr_PeripheralAddress);
      FREG_DMA_SetChWrline_Lines(i, a->channelConfig.chWrline_Lines);
      FREG_DMA_SetChWrline_LoopIncrement(i, a->channelConfig.chWrline_LoopIncrement);
      FREG_DMA_SetChWrinc_LineLength(i, a->channelConfig.chWrinc_LineLength);
      FREG_DMA_SetChWrinc_LineIncrement(i, a->channelConfig.chWrinc_LineIncrement);

      FREG_DMA_SetChRdaddr(i, 
            FAPI_SYS_GET_PHYSICAL_ADDRESS(a->channelConfig.chRdaddr_OffsetAddress));

      FREG_DMA_SetChRdlpaddr(i, 
            FAPI_SYS_GET_PHYSICAL_ADDRESS(a->channelConfig.chRdlpaddr_LoopAddress));

      FREG_DMA_SetChWraddr(i, 
            FAPI_SYS_GET_PHYSICAL_ADDRESS(a->channelConfig.chWraddr_OffsetAddress));
      
      FREG_DMA_SetChWrlpaddr(i, 
            FAPI_SYS_GET_PHYSICAL_ADDRESS(a->channelConfig.chWrlpaddr_LoopAddress));
      
      FREG_DMA_SetIntMask(0x3ff);
      
      FAPI_SYS_ENABLE_IRQ(cpu_sr);

      if (a->channelConfig.chRdaddr_PeripheralAddress == 0)
      {
         if ((a->channelConfig.chRdlpaddr_LoopAddress != 0) && 
               (a->channelConfig.chRdlpaddr_LoopAddress == 
                  (int) &a->optData))
         {
            FAPI_SYS_FlushDataCache(a->channelConfig.chRdaddr_OffsetAddress, 4);
         }
         else
         {
            FAPI_SYS_FlushDataCache(a->channelConfig.chRdaddr_OffsetAddress, 
                  a->channelConfig.chLength);            
         }
      }

      if (a->channelConfig.chWraddr_PeripheralAddress == 0)
      {
         FAPI_SYS_FlushDataCache(a->channelConfig.chWraddr_OffsetAddress, 
               a->channelConfig.chLength);
      }

      if (a->preEnableCallback != 0)
      {
         (a->preEnableCallback)(a);
      } 
      
      if ((a->autoRestart == 1) &&
            (a->optSplitCount == 0))
      {
         FREG_DMA_SetChConfig_AutoRestartEnable(i, 1);
      } 

      a->status = FAPI_DMA_REQUEST_STATUS_ACTIVE;
      
      if (a->optSplitCount != 0)
      {
         FREG_DMA_SetChConfig(i, FREG_DMA_GetChConfig(i) | 0x0C);
      }
      else
      {
         FREG_DMA_SetChConfig_Enable(i, 1);         
      }

      FREG_DMA_SetIntMask(0);         
      
      if (a->postEnableCallback != 0)
      {
         (a->postEnableCallback)(a);
      }       
   } 
}


/* 21c18740 - complete */
void dmaFinishRequest(FAPI_DMA_RequestT* a)
{
   if (a != 0)
   {
      if (a->channel < 6)
      {
         FREG_DMA_SetChConfig(a->channel, 0);
         
         if (a->postFinishCallback != 0)
         {
            (a->postFinishCallback)(a);
         } 
         
         if (a->optFinishCallback != 0)
         {
            (a->optFinishCallback)(a->result, 
                  a->channelConfig.chLength);
         }       
      } 
   }
}


/* 21c186f4 - complete */
void dmaChangeCallback(FAPI_DMA_RequestT* a)
{
   if (a != 0)
   {
      int b = 0;
      
      if (a->optChangeCallback != 0)
      {
         if (a->feature == FAPI_DMA_FEATURE_SEARCH_REPLACE)
         {
            b = func_21c19bf8(a);
         } 

         (a->optChangeCallback)(a->result, b);
      } 
   }
}


/* 21c185b4 - todo */
FAPI_DMA_RequestT* dmaQueueRequest(FAPI_DMA_RequestT* a)
{
   unsigned i;
   FAPI_DMA_RequestT* p = 0;
   
   for (i = 0; i < 16; i++)
   {
      if (fapi_dma_queued_transfers[i] == 0)
      {
         fapi_dma_queued_transfers[i] = a;
         p = a;
         p->status = FAPI_DMA_REQUEST_STATUS_QUEUED;
         dmaNumQueuedRequests++;
         break;
      }
   }

   return p;
}


/* 21c18618 - complete */
FAPI_DMA_RequestT* dmaDequeueRequest(FAPI_DMA_RequestT* a)
{
   unsigned i;
   FAPI_DMA_RequestT* p = 0;
   
   for (i = 0; i < 16; i++)
   {
      if (fapi_dma_queued_transfers[i] == a)
      {
         p = fapi_dma_queued_transfers[i];
         dmaNumQueuedRequests--;
      } 
      else if (p != 0)
      {
         fapi_dma_queued_transfers[i-1] = fapi_dma_queued_transfers[i];
      } 
   } 
   
   return p;
}


/* 21c184a8 - complete */
FAPI_DMA_RequestT* dmaChannelRequest(FAPI_DMA_RequestT* a)
{
   int i;
   FAPI_DMA_RequestT* r15 = 0;
   
   if (a != 0)
   {
      int cpu_sr = FAPI_SYS_DISABLE_IRQ;
      
      if (a->autoRestart == 1)
      {
         i = 0;
         
         if ((dmaChannelledRequests[i] == 0) &&
            (0 != func_21c1839c(a->feature)))
         {
            dmaChannelledRequests[i] = a;
            dmaNumChannelledRequests++;
            r15 = a;
            a->status = FAPI_DMA_REQUEST_STATUS_CHANNELED;
            a->channel = i;
         } 
      } 
      else
      {
         for (i = 5; i >= 0; i--)
         {
            if (dmaChannelledRequests[i] == 0)
            {
               if (0 != func_21c1839c(a->feature))
               {
                  dmaChannelledRequests[i] = a;
                  dmaNumChannelledRequests++;
                  r15 = a;
                  a->status = FAPI_DMA_REQUEST_STATUS_CHANNELED;
                  a->channel = i & 0x0F;
               } 
               break;
            } 
         } 
      } 
      
      FAPI_SYS_ENABLE_IRQ(cpu_sr);
   }
   
   return r15;   
}


/* 21c1839c - complete */
int func_21c1839c(unsigned short a)
{
   unsigned i;
   unsigned count;
   unsigned j;
   int res = 1;
   
   int cpu_sr = FAPI_SYS_DISABLE_IRQ;

   if (a == 1)
   {
      count = 0;
      for (j = 0; j < 6; j++)
      {
         if ((dmaChannelledRequests[j] != 0) && 
            (dmaChannelledRequests[j]->feature == 1))
         {
            count++;
         } 
      } 
      
      if (count >= Data_21f21e78[0])
      {
         res = 0;
      } 
   }
   else
   {
      for (i = 1; i < 8; i++)
      {
         count = 0;
         if (1 == ((a >> i) & 1))
         {
            for (j = 0; j < 6; j++)
            {
               if ((dmaChannelledRequests[j] != 0) && 
                  (((dmaChannelledRequests[j]->feature >> i) & 1) == 1))
               {
                  count++;
               } 
            } 
            
            if (count >= Data_21f21e78[i])
            {
               res = 0;
               break;
            } 
         } 
      } 
   }
   
   FAPI_SYS_ENABLE_IRQ(cpu_sr);
   
   return res;
}


/* 21c18358 - complete */
FAPI_DMA_RequestT* dmaUnchannelRequest(FAPI_DMA_RequestT* a)
{
   FAPI_DMA_RequestT* ip = 0;
   
   if (dmaChannelledRequests[a->channel] == a)
   {
      ip = dmaChannelledRequests[a->channel];
      dmaChannelledRequests[a->channel] = 0;
      dmaNumChannelledRequests--;
   }

   return ip;
}


/* 21c18088 - complete */
uint32_t FAPI_DMA_GetRequestStatus( FAPI_DMA_RequestT* dmaRequestP )
{
   if (dmaRequestP != 0)
   {
      if (dmaRequestP->feature != 0)
      {
         return dmaRequestP->status;
      }
   }
   
   return 0;
}

#endif
