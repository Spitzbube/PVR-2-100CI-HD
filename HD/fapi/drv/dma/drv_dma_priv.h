/*
********************************************************************************
**
** \file      ./fapi/drv/dma/src/drv_dma_priv.h
**
** \brief     DMA driver
**
** \attention   THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU MICROELECTRONICS
**              ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**              OMMISSIONS.
**
** (C) Copyright 2006-2009 by Fujitsu Microelectronics Europe GmbH
**
********************************************************************************
*/
#ifndef FAPI_DMA_PRIV_H
#define FAPI_DMA_PRIV_H

#include <stdchar.h>
#include <fapi/reg_dma.h>
#include "drv_dma.h"

/*
********************************************************************************
**
** IDE handle data structure.
**
********************************************************************************
*/
#define FAPI_DMA_CHANNEL_MAX 6
#define FAPI_DMA_REQUEST_MAX 16
#define FAPI_DMA_FIFO_MAX    FAPI_DMA_REQUEST_MAX

/*
********************************************************************************
**
** DMA controller specific internal variables.
**
********************************************************************************
*/
#ifdef FAPI_DMA_PRIV_C
    uint32_t                  fapi_dma_initialized      = 0; //21f21e58
    FAPI_SYS_HandleT          fapi_dma_interrupt_handle = 0;
    FAPI_DMA_RequestT*        fapi_dma_request          = 0; //21f21e60
    uint32_t                  fapi_dma_request_usage    = 0; //21f21e64
    FAPI_DMA_RequestT**       fapi_dma_fifo             = 0; //21f21e68
    uint32_t                  fapi_dma_fifo_usage       = 0; //21f21e6c
    FAPI_DMA_RequestT**       fapi_dma_channel          = 0; //21f21e70
    uint32_t                  fapi_dma_channel_usage    = 0; //21f21e74
    uint32_t                  fapi_dma_feature_allow[FAPI_DMA_FEATURE_COUNT] = { 0 }; //21f21e78
#else
    extern uint32_t            fapi_dma_initialized; //21f21e58
    extern FAPI_SYS_HandleT    fapi_dma_interrupt_handle; //
    extern FAPI_DMA_RequestT*  fapi_dma_request; //21f21e60
    extern uint32_t            fapi_dma_request_usage; //21f21e64
    extern FAPI_DMA_RequestT** fapi_dma_fifo; //21f21e68
    extern uint32_t            fapi_dma_fifo_usage; //21f21e6c
    extern FAPI_DMA_RequestT** fapi_dma_channel; //21f21e70
    extern uint32_t            fapi_dma_channel_usage; //21f21e74
    extern uint32_t            fapi_dma_feature_allow[FAPI_DMA_FEATURE_COUNT]; //21f21e78
#endif

/*
********************************************************************************
**
** Function prototypes
**
********************************************************************************
*/
#ifdef __cplusplus
extern "C" {
#endif

int32_t            dmaSetRequestAutoRestart     ( FAPI_DMA_RequestT* requestPtr, uint32_t restart );
int32_t            dmaSetRequestSplitTransfer   ( FAPI_DMA_RequestT* dmaRequestPtr, uint32_t numberOfSplit,
                                                  FAPI_DMA_OptCallbackT splitCallback );
void               dmaOverflowHandler           ( FAPI_DMA_RequestT* dmaRequestPtr );
void               dmaReportUsage               ( const char_t* title );
FAPI_DMA_RequestT* dmaAddChannelRequest         ( FAPI_DMA_RequestT* requestPtr );
FAPI_DMA_RequestT* dmaRemoveChannelRequest      ( FAPI_DMA_RequestT* requestPtr );
void               dmaStartChannelRequest       ( FAPI_DMA_RequestT* requestPtr );
void               dmaFinalizeChannelRequest    ( FAPI_DMA_RequestT* requestPtr );
FAPI_DMA_RequestT* dmaAddFifoRequest            ( FAPI_DMA_RequestT* requestPtr );
void               dmaChangeChannelCallback     ( FAPI_DMA_RequestT* requestPtr );
FAPI_DMA_RequestT* dmaRemoveFifoRequest         ( FAPI_DMA_RequestT* requestPtr );
void               dmaSelfDestroyRequest        ( FAPI_DMA_RequestT* requestPtr );
uint32_t           dmaCheckChannelFeatureUsage  ( uint16_t feature );
uint32_t           dmaGetSearchReplaceHitAddress( FAPI_DMA_RequestT* requestPtr );

#ifdef __cplusplus
}
#endif

#endif
