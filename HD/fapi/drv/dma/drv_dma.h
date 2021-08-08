/*!
********************************************************************************
**
** \file        ./fapi/drv/dma/src/drv_dma.h
**
** \brief       DMA Driver.
**
** This file contains the declaration of the DMA driver API.
**
** \attention   THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU MICROELECTRONICS
**              ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**              OMMISSIONS.
**
** (C) Copyright 2006-2009 by Fujitsu Microelectronics Europe GmbH
**
********************************************************************************
*/
#ifndef FAPI_DMA_H
    #define FAPI_DMA_H

    #ifndef __LINUX__
    #define FAPI_DMA_CALLS_USB_BSR
    #endif
    #include <fapi/sys_driver.h>

    /*!
    ********************************************************************************
    ** Standard error codes for the driver
    ********************************************************************************
    */

    /*! Bad parameter passed. */
    #define FAPI_DMA_ERR_BAD_PARAMETER         (FAPI_DMA_MODULE_BASE +\
                                                FAPI_ERR_BAD_PARAMETER)
    /*! Memory allocation failed. */
    #define FAPI_DMA_ERR_OUT_OF_MEMORY         (FAPI_DMA_MODULE_BASE +\
                                                FAPI_ERR_OUT_OF_MEMORY)
    /*! Device already initialised. */
    #define FAPI_DMA_ERR_ALREADY_INITIALIZED   (FAPI_DMA_MODULE_BASE +\
                                                FAPI_ERR_ALREADY_INITIALIZED)
    /*! Device not initialised. */
    #define FAPI_DMA_ERR_NOT_INITIALIZED       (FAPI_DMA_MODULE_BASE +\
                                                FAPI_ERR_NOT_INITIALIZED)
    /*! Feature or function is not available. */
    #define FAPI_DMA_ERR_FEATURE_NOT_SUPPORTED (FAPI_DMA_MODULE_BASE +\
                                                FAPI_ERR_FEATURE_NOT_SUPPORTED)
    /*! Timeout occured. */
    #define FAPI_DMA_ERR_TIMEOUT               (FAPI_DMA_MODULE_BASE +\
                                                FAPI_ERR_TIMEOUT)
    /*! The device is busy, try again later. */
    #define FAPI_DMA_ERR_DEVICE_BUSY           (FAPI_DMA_MODULE_BASE +\
                                                FAPI_ERR_DEVICE_BUSY)
    /*! Invalid handle was passed. */
    #define FAPI_DMA_ERR_INVALID_HANDLE        (FAPI_DMA_MODULE_BASE +\
                                                FAPI_ERR_INVALID_HANDLE)
    /*! Semaphore could not be created. */
    #define FAPI_DMA_ERR_SEMAPHORE_CREATE      (FAPI_DMA_MODULE_BASE +\
                                                FAPI_ERR_SEMAPHORE_CREATE)
    /*! The driver's used version is not supported. */
    #define FAPI_DMA_ERR_UNSUPPORTED_VERSION   (FAPI_DMA_MODULE_BASE +\
                                                FAPI_ERR_UNSUPPORTED_VERSION)

    /*!
    ****************************************************************************
    ** Additional driver error codes (reserved from -100 to -999)
    ** Each error code shall be decreased in steps of 1.
    ****************************************************************************
    */
    #define FAPI_DMA_ERR_BASE            FAPI_DMA_MODULE_BASE
    #define FAPI_DMA_ERR_INIT_FAILED     FAPI_DMA_ERR_BASE-101 //!< initialization failed
    #define FAPI_DMA_ERR_TOO_MANY_OPEN   FAPI_DMA_ERR_BASE-102 //!< too many open handles
    #define FAPI_DMA_ERR_CONFIG_OVERFLOW FAPI_DMA_ERR_BASE-103 //!< too many feature configurations
    #define FAPI_DMA_ERR_FIFO_OVERFLOW   FAPI_DMA_ERR_BASE-104 //!< too many fifo entries
    #define FAPI_DMA_ERR_BAD_FEATURE     FAPI_DMA_ERR_BASE-105 //!< wrong feature id
    #define FAPI_DMA_ERR_BAD_REQUEST     FAPI_DMA_ERR_BASE-106 //!< wrong fifo request
    #define FAPI_DMA_ERR_STATUS          FAPI_DMA_ERR_BASE-107 //!< bad status from DMA controller
    #define FAPI_DMA_ERR_CCIR_OVERFLOW   FAPI_DMA_ERR_BASE-108 //!< CCIR capture reported overflow
    #define FAPI_DMA_ERR_AUDIO_OVERFLOW  FAPI_DMA_ERR_BASE-109 //!< AUDIO capture reported overflow
    #define FAPI_DMA_ERR_CANCELED        FAPI_DMA_ERR_BASE-110 //!< cancel by the application

    /*
    ****************************************************************************
    **
    ** \brief Internal DMA feature index
    **
    ** This enumeration block describes all available DMA features
    ** supported by the embedded DMA controller.
    **
    ****************************************************************************
    */

    /*
    ** The SDRAM/SDRAM copy feature index.
    */
    #define FAPI_DMA_FEATURE_INDEX_SDRAM_COPY       0
    /*
    ** The REGISTER/SDRAM copy feature index.
    */
    #define FAPI_DMA_FEATURE_INDEX_REGISTER_COPY    1
    /*
    ** The CCIR-656 capture feature index.
    */
    #define FAPI_DMA_FEATURE_INDEX_CCIR656_CAPTURE  2
    /*
    ** The color conversion feature index.
    */
    #define FAPI_DMA_FEATURE_INDEX_COLOR_CONVERSION 3
    /*
    ** The search'n'replace feature index.
    */
    #define FAPI_DMA_FEATURE_INDEX_SEARCH_REPLACE   4
    /*
    ** The Audio capture feature index.
    */
    #define FAPI_DMA_FEATURE_INDEX_AUDIO_CAPTURE    5
    /*
    ** The 3DES encryption/decryption feature index.
    */
    #define FAPI_DMA_FEATURE_INDEX_3DES_CRYPTO      6
    /*
    ** The USB data transfer feature index.
    */
    #define FAPI_DMA_FEATURE_INDEX_USB_COPY         7
    /*
    ** The ATA data transfer feature index.
    */
    #define FAPI_DMA_FEATURE_INDEX_ATA_COPY         FAPI_DMA_FEATURE_INDEX_USB_COPY
    /*
    ** The max. number of DMA features.
    */
    #define FAPI_DMA_FEATURE_COUNT                  8

    /*!
    ****************************************************************************
    **
    ** \brief DMA feature enumeration
    **
    ** The following feature macros define the bitmasks for all DMA
    ** features, some can be or'd with others, like:
    ** - FAPI_DMA_FEATURE_USB_COPY | FAPI_DMA_FEATURE_SDRAM_COPY
    **   to transfer data between USB and SDRAM
    **
    ** - FAPI_DMA_FEATURE_ATA_COPY | FAPI_DMA_FEATURE_SDRAM_COPY
    **   to transfer data between ATA and SDRAM
    **
    ** - FAPI_DMA_FEATURE_ATA_COPY | FAPI_DMA_FEATURE_USB_COPY
    **   to transfer data between USB and ATA
    **
    ****************************************************************************
    */

    /*!
    ** The unknown feature qualifier.
    */
    #define FAPI_DMA_FEATURE_UNDEFINED        0
    /*!
    ** The SDRAM/SDRAM copy feature qualifier.
    */
    #define FAPI_DMA_FEATURE_SDRAM_COPY       (1<<FAPI_DMA_FEATURE_INDEX_SDRAM_COPY)
    /*!
    ** The REGISTER/SDRAM copy feature qualifier.
    */
    #define FAPI_DMA_FEATURE_REGISTER_COPY    (1<<FAPI_DMA_FEATURE_INDEX_REGISTER_COPY)
    /*!
    ** The CCIR-656 capture feature qualifier.
    */
    #define FAPI_DMA_FEATURE_CCIR656_CAPTURE  (1<<FAPI_DMA_FEATURE_INDEX_CCIR656_CAPTURE)
    /*!
    ** The color conversion feature qualifier.
    */
    #define FAPI_DMA_FEATURE_COLOR_CONVERSION (1<<FAPI_DMA_FEATURE_INDEX_COLOR_CONVERSION)
    /*!
    ** The search'n'replace feature qualifier.
    */
    #define FAPI_DMA_FEATURE_SEARCH_REPLACE   (1<<FAPI_DMA_FEATURE_INDEX_SEARCH_REPLACE)
    /*!
    ** The Audio capture feature qualifier.
    */
    #define FAPI_DMA_FEATURE_AUDIO_CAPTURE    (1<<FAPI_DMA_FEATURE_INDEX_AUDIO_CAPTURE)
    /*!
    ** The 3DES encryption/decryption feature qualifier.
    */
    #define FAPI_DMA_FEATURE_3DES_CRYPTO      (1<<FAPI_DMA_FEATURE_INDEX_3DES_CRYPTO)
    /*!
    ** The USB data transfer feature qualifier.
    */
    #define FAPI_DMA_FEATURE_USB_COPY         (1<<FAPI_DMA_FEATURE_INDEX_USB_COPY)
    /*!
    ** The ATA data transfer feature qualifier.
    */
    #define FAPI_DMA_FEATURE_ATA_COPY         (1<<FAPI_DMA_FEATURE_INDEX_ATA_COPY)

    /*!
    ****************************************************************************
    **
    ** \brief DMA status types
    **
    ** This type block describes all available DMA status values a DMA
    ** request block may have.
    **
    ****************************************************************************
    */
    #define FAPI_DMA_REQUEST_STATUS_UNDEFINED 0 //!< the undefined indicator
    #define FAPI_DMA_REQUEST_STATUS_ALLOCATED 1 //!< request is allocated (in use)
    #define FAPI_DMA_REQUEST_STATUS_QUEUED    2 //!< request is queued in request fifo
    #define FAPI_DMA_REQUEST_STATUS_CHANNELED 3 //!< request is on channel list but not yet running
    #define FAPI_DMA_REQUEST_STATUS_ACTIVE    4 //!< request on channel is running (active)
    #define FAPI_DMA_REQUEST_STATUS_FINISHED  5 //!< request has finished with ok status
    #define FAPI_DMA_REQUEST_STATUS_ABORTED   6 //!< request has terminated with error status

    /*!
    ****************************************************************************
    ** \brief Version of the DMA driver.
    **
    ** This number consists of a \b major version number stored in bits
    ** 16 to 31 (upper word) and a \b minor version number stored in bits
    ** 0 to 15 (lower word).
    ** - The \b major number has to be increased when a parameter change
    **   occurs for the existing driver's API after its first release.
    ** - The \b minor number has to be increased when functions are added to
    **   the existing driver's API after its first release.
    **
    ** \note This value has to be assigend to the \b version field of the
    **       FAPI_DMA_OpenParamsT structure.
    **
    ****************************************************************************
    */
    typedef enum {
        FAPI_DMA_VERSION = (int32_t)0x00020000   //!< The current driver version
    } FAPI_DMA_VersionEnumT;

    /*!
    ****************************************************************************
    **
    ** \brief DMA channel configuration parameter structure
    **
    ** This structure contains all settings required to configure a
    ** single DMA channel for a specific feature, this structure is
    ** used within the DMA request data structure.
    **
    ****************************************************************************
    */
    typedef struct
    {
        uint32_t chConfig;
        uint32_t chLength;
        uint32_t chLladdr;
        uint32_t chRdaddr_EndianSwap;
        uint32_t chRdaddr_PeripheralAddress;
        uint32_t chRdaddr_OffsetAddress;
        uint32_t chRdline_Lines;
        uint32_t chRdline_LoopIncrement;
        uint32_t chRdinc_LineLength;
        uint32_t chRdinc_LineIncrement;
        uint32_t chRdlpaddr_LoopAddress;
        uint32_t chWraddr_EndianSwap;
        uint32_t chWraddr_PeripheralAddress;
        uint32_t chWraddr_OffsetAddress;
        uint32_t chWrline_Lines;
        uint32_t chWrline_LoopIncrement;
        uint32_t chWrinc_LineLength;
        uint32_t chWrinc_LineIncrement;
        uint32_t chWrlpaddr_LoopAddress;
    }
    FAPI_DMA_ChannelConfigT;

    /*!
    ****************************************************************************
    **
    ** \brief DMA callback function signature
    **
    ** This is the callback function signature required by the DMA driver
    ** for internally used pre and post functions.
    **
    ** \param request The request
    **
    ** \note functions of this signature will be called in interrupt
    **       context
    **
    ****************************************************************************
    */
    struct FAPI_DMA_RequestS;
    typedef struct FAPI_DMA_RequestS FAPI_DMA_RequestT;
    typedef void (*FAPI_DMA_CallbackT)(FAPI_DMA_RequestT* damRequestPtr);

    /*!
    ****************************************************************************
    **
    ** \brief DMA callback function signature for user callbacks
    **
    ** This is the callback function signature required by the DMA
    ** driver used for user specific optional completion callbacks.
    **
    ** \param status  The FD status of the current feature operation
    ** \param bytes   The real number of bytes transfered by the channel
    **
    ** \note functions of this signature will be called in interrupt
    **       context
    **
    ****************************************************************************
    */
    typedef void (*FAPI_DMA_OptCallbackT)(int32_t status,uint32_t bytes);

    /*!
    ****************************************************************************
    **
    ** \brief DMA request parameter structure
    **
    ** This structure contains all settings required for a single DMA
    ** request.
    **
    ****************************************************************************
    */
    typedef struct FAPI_DMA_RequestS
    {
        // /*
        // ** The version of the driver.
        // ** \ref FAPI_DMA_VersionEnumT "FAPI_DMA_VERSION".
        // */
        // FAPI_DMA_VersionEnumT version;

        /*!
        ** The DMA feature to be used, this field will be set by the
        ** function FAPI_DMA_RequestAllocate().
        **
        ** \note You should not modify this field manually.
        */
        uint32_t feature;

        /*!
        ** The status of the current DMA request, this field will be set
        ** by the DMA interrupt service routine.
        **
        ** \note You should not modify this field manually.
        */
        uint32_t status;

        /*!
        ** The current DMA channel the request is running on
        **
        ** \note You should not modify this field manually.
        */
        uint32_t channel;

        /*!
        ** The specific result status of the DMA operation
        ** will be set to FAPI_OK if ok or FAPI_DMA_ERR_xxx in case of
        ** error, this field will be set by the DMA interrupt service
        ** routine.
        **
        ** \note You should not modify this field manually.
        */
        int32_t result;

        /*!
        ** This flag decides whether to destroy the current request
        ** structure automatically or not, if FFALSE the caller should
        ** destroy the request structure somewhere in the optFinshCallback
        ** function.
        **
        ** \note You should not modify this field manually.
        */
        uint32_t selfDestroy;

        /*!
        ** The DMA channel configuration data.
        */
        FAPI_DMA_ChannelConfigT channelConfig;

        /*!
        ** the callback function to be called just before the DMA channel
        ** will be enabled by the DMA fifo, in case of the ATA feature
        ** this may be a pointer to a function preparing the ATA block
        ** for DMA access and to send the required ATA command to the
        ** IDE command register.
        **
        ** \note This function is called in interrupt context.
        */
        FAPI_DMA_CallbackT preEnableCallback;

        /*!
        ** the callback function to be called just after the DMA channel
        ** has been enabled by the DMA fifo, in case of the ATA feature
        ** this may be a pointer to a function preparing the ATA block
        ** for DMA access and to send the required ATA command to the
        ** IDE command register.
        **
        ** \note This function is called in interrupt context.
        */
        FAPI_DMA_CallbackT postEnableCallback;

        /*!
        ** the callback function to be called after the DMA channel
        ** has finished its transfer, in case of the ATA feature
        ** this may be a pointer to a function disabling the ATA block
        ** from DMA access.
        **
        ** \note This function is called in interrupt context.
        */
        FAPI_DMA_CallbackT postFinishCallback;

        /*!
        ** A generic handle to be passed to all callback functions.
        */
        FAPI_SYS_HandleT optHandle;

        /*!
        ** A void pointer to transport any addition optional data, as DMA
        ** request structure will be passed to each callback function, the
        ** optional data pointer can be used for any kind of feature specific
        ** data.
        */
        void* optData;

        /*!
        ** The callback function to be called if the transfer is completed,
        ** either if done OK or because of error.
        **
        ** \note This function is called in interrupt context.
        */
        FAPI_DMA_OptCallbackT optFinishCallback;

        /*!
        ** the callback function to be called if a change is detected
        ** in case of the search-and-replace engine this is the SearchHit
        ** callback in case of the CCIR engine this is the SyncChange callback.
        **
        ** \note This function is called in interrupt context.
        */
        FAPI_DMA_OptCallbackT optChangeCallback;

        /*!
        ** This flag decides whether to automatically restart the DMA
        ** channel after having received the finish condition, or not,
        ** this feature is useful only for the DMA capture features
        ** #FAPI_DMA_FEATURE_CCIR656_CAPTURE and #FAPI_DMA_FEATURE_AUDIO_CAPTURE,
        ** internally this requires also the use of DMA channel#0 as this is
        ** the only channel capable for the auto-restart feature.
        **
        ** \note You should not modify this field manually, this flag should
        **       be set only by the function dmaRequestSetAutoRestart
        **       which is called internally by the CCIR and AUDIO capture
        **       prepare functions.
        */
        uint32_t autoRestart;

        /*!
        ** This number is used to split a DMA buffer into several parts.
        ** Each split buffer is used for DAM in a linked-list operation.
        */
        uint32_t optSplitCount;

        /*!
        ** The number of split buffers which finished DMA transfer.
        **
        ** \note You should not modify this field manually.
        */
        uint32_t completeSplitCount;

        /*!
        ** the callback function to be called if the transfer to a splited
        ** buffer is completed.
        **
        ** \note This function is called in interrupt context
        */
        FAPI_DMA_OptCallbackT optSplitCallback;
    }
    FAPI_DMA_RequestS;

    //**************************************************************************
    //**************************************************************************
    //** Global Data
    //**************************************************************************
    //**************************************************************************
    extern FAPI_SYS_DriverT FAPI_DMA_Driver;

    /*
    ****************************************************************************
    ****************************************************************************
    **
    ** Generic DMA specific functions and data types
    **
    ****************************************************************************
    ****************************************************************************
    */
    #ifdef __cplusplus
    extern "C" {
    #endif

    int32_t            FAPI_DMA_Init( void );
    void               FAPI_DMA_Exit( void );
    uint32_t           FAPI_DMA_Isr( void );
    FAPI_DMA_RequestT* FAPI_DMA_AllocateRequest( uint32_t feature, uint32_t selfDestroy );
    void               FAPI_DMA_ReleaseRequest( FAPI_DMA_RequestT* dmaRequestPtr );
    int32_t            FAPI_DMA_CancelRequest( FAPI_DMA_RequestT* dmaRequestPtr, uint32_t feature );
    int32_t            FAPI_DMA_SendRequest( FAPI_DMA_RequestT* dmaRequestP );
    int32_t            FAPI_DMA_SendRequest2( FAPI_DMA_RequestT* dmaRequestP, FAPI_DMA_RequestT* dmaRequest2Ptr );
    uint32_t           FAPI_DMA_GetRequestStatus( FAPI_DMA_RequestT* dmaRequestP );
    uint32_t           FAPI_DMA_GetRequestList( uint32_t feature, FAPI_DMA_RequestS* dmaRequestPtrArray[], uint32_t requestArraySize );

    #ifdef FAPI_DMA_CALLS_USB_BSR
    void               FAPI_DMA_Bsr(uint32_t privData);
    #endif

    #ifdef __cplusplus
    }
    #endif

    /*
    ****************************************************************************
    ****************************************************************************
    **
    ** ATA feature specific functions and data types
    **
    ****************************************************************************
    ****************************************************************************
    */
    #ifdef __cplusplus
    extern "C" {
    #endif

    FAPI_DMA_RequestT* FAPI_DMA_ATA_ReadData(
                           FAPI_SYS_HandleT      ideHandle,
                           uint32_t              lba,
                           uint32_t              sectors,
                           uint32_t              sectorBytes,
                           volatile uint32_t*    buffer,
                           FAPI_DMA_OptCallbackT finishCallback );

    FAPI_DMA_RequestT* FAPI_DMA_ATA_WriteData(
                           FAPI_SYS_HandleT      ideHandle,
                           uint32_t              lba,
                           uint32_t              sectors,
                           uint32_t              sectorBytes,
                           volatile uint32_t*    buffer,
                           FAPI_DMA_OptCallbackT finishCallback );

    #ifdef __cplusplus
    }
    #endif

    /*
    ****************************************************************************
    ****************************************************************************
    **
    ** SDRAM feature specific functions and data types
    **
    ****************************************************************************
    ****************************************************************************
    */
    #ifdef __cplusplus
    extern "C" {
    #endif

    FAPI_DMA_RequestT* FAPI_DMA_StartMemcopy(
                           volatile uint32_t*    target,
                           volatile uint32_t*    source,
                           uint32_t              bytes,
                           FAPI_DMA_OptCallbackT finishCallback );

    FAPI_DMA_RequestT* FAPI_DMA_StartPFlashcopy(
                           volatile uint32_t*    target,
                           volatile uint32_t*    source,
                           uint32_t              bytes,
                           FAPI_DMA_OptCallbackT finishCallback );

    #ifdef __cplusplus
    }
    #endif

    /*!
    ****************************************************************************
    **
    ** \brief memory fill specific request parameters
    **
    ** This structure contains all settings required for a single DMA
    ** memory fill request with offsets.
    **
    ****************************************************************************
    */
    typedef struct
    {
        //! count of words that will be written per burst
        uint32_t  lineWords;

        //! count of lines/bursts to perform
        uint32_t  lineCount;

        //! count of words that will be ommitted between the bursts
        uint32_t  skipWords;

        //! the value that will be written to each word
        uint32_t  fillValue;
    }
    FAPI_DMA_MemoryFillParamsT;

    #ifdef __cplusplus
    extern "C" {
    #endif

    FAPI_DMA_RequestT* FAPI_DMA_StartMemfill(
                           volatile uint32_t*          targetPtr,
                           FAPI_DMA_MemoryFillParamsT* fillParamsPtr,
                           FAPI_DMA_OptCallbackT       finishCallback);

    #ifdef __cplusplus
    }
    #endif

    /*
    ****************************************************************************
    ****************************************************************************
    **
    ** Search'n'Replace feature specific functions and data types
    **
    ****************************************************************************
    ****************************************************************************
    */
    #define FAPI_DMA_SR_ALIGN_8  0x2 //!< byte alignment for the search pattern
    #define FAPI_DMA_SR_ALIGN_16 0x1 //!< word alignment for the search pattern
    #define FAPI_DMA_SR_ALIGN_32 0x0 //!< long word alignment for the search pattern

    /*!
    ****************************************************************************
    **
    ** \brief Search'n'replace request parameters
    **
    ** This structure contains all settings required for a single DMA
    ** search'n'replace request.
    **
    ****************************************************************************
    */
    typedef struct
    {
        /*!
        ** Pattern to search in buffer
        */
        uint32_t searchPattern;

        /*!
        ** A bit mask for the search pattern, 0-bits disable
        ** the compare alogrithm for specific bits
        */
        uint32_t searchMask;

        /*!
        ** The type of data alignment, only the values
        ** #FAPI_DMA_SR_ALIGN_8, #FAPI_DMA_SR_ALIGN_16 and
        ** #FAPI_DMA_SR_ALIGN_32 are allowed
        */
        uint32_t searchAlignment;

        /*!
        ** The value to replace
        */
        uint32_t replacePattern;

        /*!
        ** A bit mask for the search pattern, 0-values disable
        ** the replace alogrithm for specific bits
        */
        uint32_t replaceMask;

        /*!
        * Enable replacement if FTRUE, search-only if FFALSE.
        */
        uint32_t replaceEnable;

        FAPI_DMA_RequestT* request2;
    }
    FAPI_DMA_SearchReplaceParamsT;

    #ifdef __cplusplus
    extern "C" {
    #endif

    FAPI_DMA_RequestT* FAPI_DMA_StartSearchReplace(
                           volatile uint32_t*             buffer,
                           uint32_t                       bytes,
                           FAPI_DMA_SearchReplaceParamsT* srParams,
                           FAPI_DMA_OptCallbackT          hitCallback,
                           FAPI_DMA_OptCallbackT          finishCallback );

    #ifdef __cplusplus
    }
    #endif

    /*
    ****************************************************************************
    ****************************************************************************
    **
    ** CCIR656 feature specific functions and data types
    **
    ****************************************************************************
    ****************************************************************************
    */
    #define FAPI_DMA_CCIR656_EXT_TS2_INPUT   FTRUE  //!< select the external TS2 input
    #define FAPI_DMA_CCIR656_INT_CCIR_OUTPUT FFALSE //!< select the internal CCIR output

    /*!
    ****************************************************************************
    **
    ** \brief CCIR656 capture request parameters
    **
    ** This structure contains all settings required for a single DMA
    ** CCIR656 capture request.
    **
    ****************************************************************************
    */
    typedef struct
    {
        /*!
        ** Select the raw mode enable signal in external input mode
        ** FTRUE means TS2EN is the enable signal
        ** FFALSE means ignore enable
        */
        uint32_t useRawMode;

        /*!
        ** Selects the external input to be used, only the values
        ** #FAPI_DMA_CCIR656_EXT_TS2_INPUT and #FAPI_DMA_CCIR656_INT_CCIR_OUTPUT
        ** are allowed
        */
        uint32_t dataSource;

        /*!
        ** Inverts the input clock if FTRUE
        */
        uint32_t invertClock;

        /*!
        ** Enables capturing of field0 if FTRUE
        */
        uint32_t useField0;

        /*!
        ** Enables capturing of field1 if FTRUE
        */
        uint32_t useField1;

        /*!
        ** Horizontal start position for CCIR capture, the value given
        ** here specifies the the pixel pos inside active video where
        ** capturing starts
        */
        uint32_t absColumnStart;

        /*!
        ** Horizontal end position for CCIR capture, the value given
        ** here specifies the the pixel pos inside active video where
        ** capturing ends  (after having captured this pixel)
        */
        uint32_t absColumnEnd;

        /*!
        ** Vertical start position for CCIR capture, the value given
        ** here specifies absolute line number inside the frame
        ** where capturing starts
        */
        uint32_t absLineStart;

        /*!
        ** Vertical end position for CCIR capture, the value given
        ** here specifies the absolute line number inside the frame
        ** where capturing stops (after having captured this line)
        */
        uint32_t absLineEnd;

        /*!
        ** Enables auto restart feature for capturing
        */
        uint32_t enableAutoRestart;
    }
    FAPI_DMA_Ccir656CaptureParamsT;

    #ifdef __cplusplus
    extern "C" {
    #endif

    FAPI_DMA_RequestT* FAPI_DMA_StartCcir656Capture(
                           volatile uint32_t*              targetBuffer,
                           uint32_t                        bufferBytes,
                           uint32_t                        bufferLineInc,
                           FAPI_DMA_Ccir656CaptureParamsT* captureParams,
                           FAPI_DMA_OptCallbackT           syncCallback,
                           FAPI_DMA_OptCallbackT           finishCallback );
    FAPI_DMA_RequestT* FAPI_DMA_StartCcir656CaptureHalfMode(
                           volatile uint32_t*              targetBuffer,
                           uint32_t                        bufferBytes,
                           uint32_t                        bufferLineInc,
                           FAPI_DMA_Ccir656CaptureParamsT* captureParams,
                           FAPI_DMA_OptCallbackT           syncCallback,
                           FAPI_DMA_OptCallbackT           finishCallback );
    int32_t FAPI_DMA_StopCcir656Capture( FAPI_DMA_RequestT* dmaRequest );

    #ifdef __cplusplus
    }
    #endif

    /*
    ****************************************************************************
    ****************************************************************************
    **
    ** AUDIO capture feature specific functions and data types
    **
    ****************************************************************************
    ****************************************************************************
    */

    /*!
    *************************************************************************
    ** \brief Sample Frequency.
    **
    ** This enumeration contains the <b>Sample Frequencies</b> supported by
    ** the audio capture module.
    **
    *************************************************************************
    */
    typedef enum {
        FAPI_DMA_AUDIO_UNDEF_HZ = (int32_t)-1, //!< Sample frequency undefined.
        FAPI_DMA_AUDIO_8000_HZ  =  8000,       //!< Sample frequency 8 kHz.
        FAPI_DMA_AUDIO_12000_HZ = 12000,       //!< Sample frequency 12 kHz.
        FAPI_DMA_AUDIO_16000_HZ = 16000,       //!< Sample frequency 16 kHz.
        FAPI_DMA_AUDIO_22050_HZ = 22050,       //!< Sample frequency 22.05 kHz.
        FAPI_DMA_AUDIO_24000_HZ = 24000,       //!< Sample frequency 24 kHz.
        FAPI_DMA_AUDIO_32000_HZ = 32000,       //!< Sample frequency 32 kHz.
        FAPI_DMA_AUDIO_44100_HZ = 44100,       //!< Sample frequency 44.1 kHz.
        FAPI_DMA_AUDIO_48000_HZ = 48000,       //!< Sample frequency 48 kHz.
        FAPI_DMA_AUDIO_96000_HZ = 96000        //!< Sample frequency 96 kHz.
    } FAPI_DMA_AUDIO_SampleFreqEnumT;

    /*!
    *************************************************************************
    ** \brief Audio Data Format.
    **
    ** This enumeration contains the various <b>Data Formats</b> supported
    ** by the audio capture module.
    **
    *************************************************************************
    */
    typedef enum {
        FAPI_DMA_AUDIO_FORMAT_I2S           = (int32_t)0, //!< I2S.
        FAPI_DMA_AUDIO_FORMAT_LEFT_ALIGNED  = 1,          //!< Left aligned.
        FAPI_DMA_AUDIO_FORMAT_RIGHT_ALIGNED = 2           //!< Right aligned.
    } FAPI_DMA_AUDIO_FormatEnumT;

    /*!
    *************************************************************************
    ** \brief Audio Bit Clock Selection.
    **
    ** This enumeration contains the various settings for the
    ** <b>Audio Bit Clock</b> supported by the audio capture module.
    **
    *************************************************************************
    */
    typedef enum {
        FAPI_DMA_AUDIO_BCLK_64_FS     = (int32_t)0, //!< Audio Bit Clock is 64 * sample frequency.
        FAPI_DMA_AUDIO_BCLK_RESERVED1 = 1,          //!< Reserved
        FAPI_DMA_AUDIO_BCLK_32_FS     = 2,          //!< Audio Bit Clock is 32 * sample frequency.
    } FAPI_DMA_AUDIO_BClkEnumT;

    /*!
    ****************************************************************************
    **
    ** \brief audio capture specific request parameters
    **
    ** This structure contains all settings required for a single DMA
    ** AUDIO capture request.
    **
    ****************************************************************************
    */
    typedef struct
    {
        /*!
        ** For MSB-first case, position of LSB counted in clocks
        ** after change of Left/Right (word) clock.
        ** For LSB-first case, position of MSB counted in clocks
        ** after change of Left/Right (word) clock.
        */
        uint32_t endPosition;

        /*!
        ** Inverts the bit clock
        */
        uint32_t invertBitClock;

        /*!
        ** Inverts the Left/Right (word) clock
        */
        uint32_t invertWordClock;

        /*!
        ** Inverts data
        */
        uint32_t invertData;

        /*!
        ** Enables capturing of Left channel data
        */
        uint32_t enableLeftChannel;

        /*!
        ** Enables capturing of Right channel data
        */
        uint32_t enableRightChannel;

        /*!
        ** Reverses MSB position
        */
        uint32_t msbFirst;

        /*!
        ** Specifies sample Frequency
        ** Needed for workaround(Application Note chp. 2.2)
        */
        FAPI_DMA_AUDIO_SampleFreqEnumT fs;

        /*!
        ** Specifies format type(I2S, right alined, left aligned)
        ** Needed for workaround(Application Note chp. 2.2)
        */
        FAPI_DMA_AUDIO_FormatEnumT fmtType;

        /*!
        ** Specifies bit clock(34*fs, 64*fs)
        ** Needed for workaround(Application Note chp. 2.2)
        */
        FAPI_DMA_AUDIO_BClkEnumT bclkType;

        /*!
        ** GPIO handle to read inputLeft/Right (word) clock
        ** Needed for workaround(Application Note chp. 2.2)
        */
        FAPI_SYS_HandleT gpio_handle_lrclk;
    }
    FAPI_DMA_AudioCaptureParamsT;

    #ifdef __cplusplus
    extern "C" {
    #endif

    FAPI_DMA_RequestT* FAPI_DMA_StartAudioCapture(
                           volatile uint32_t*            targetBuffer,
                           uint32_t                      bufferBytes,
                           FAPI_DMA_AudioCaptureParamsT* captureParams,
                           FAPI_DMA_OptCallbackT         finishCallback,
                           FAPI_DMA_OptCallbackT         splitCallback,
                           uint32_t                      numberOfSplit );
    int32_t FAPI_DMA_StopAudioCapture( FAPI_DMA_RequestT* dmaRequest );

    #ifdef __cplusplus
    }
    #endif

    /*
    ****************************************************************************
    ****************************************************************************
    **
    ** Crypto feature specific functions and data types
    **
    ****************************************************************************
    ****************************************************************************
    */
    #define FAPI_DMA_CRYPTO_K_KEY       0 //!< key from the DMA_CRYPTO_K register
    #define FAPI_DMA_CRYPTO_VENDOR_KEY1 1 //!< CA vendor key 1
    #define FAPI_DMA_CRYPTO_VENDOR_KEY2 2 //!< CA vendor key 2
    #define FAPI_DMA_CRYPTO_SESSION_KEY 3 //!< session key, not supported!

    /*!
    ****************************************************************************
    **
    ** \brief 3des crypto request parameters
    **
    ** This structure contains all settings required for a single DMA
    ** 3des crypto request.
    **
    ****************************************************************************
    */
    typedef struct
    {
        /*!
        ** Controls key generation
        ** FTRUE means generate session key
        ** FFALSE means means do not generate session key
        ** this entry is currently not supported and will be ignored
        */
        uint32_t genSessionKey;

        /*!
        ** Selects which key to be used, supported values are
        ** #FAPI_DMA_CRYPTO_K_KEY, #FAPI_DMA_CRYPTO_VENDOR_KEY1,
        ** #FAPI_DMA_CRYPTO_VENDOR_KEY2 and #FAPI_DMA_CRYPTO_SESSION_KEY
        */
        uint32_t keySelection;

        /*!
        ** Controls endianness
        ** FTRUE means little endian
        ** FFALSE means big endian
        */
        uint32_t littleEndian;

        /*!
        ** Selects the TDES mode
        ** FTRUE means CBC
        ** FFALSE means ECB (without initial vector)
        */
        uint32_t cbc;

        /*!
        ** Selects the encryption/decryption mode
        ** FTRUE means decrypt
        ** FFALSE means encrypt
        */
        uint32_t decrypt;
    }
    FAPI_DMA_CryptoParamsT;

    typedef struct
    {
        // keys and initialization vectors
        uint32_t key1Msb;  // Key 1 55:28 (31:25)(23:17)(15:9)(7:1)
        uint32_t key1Lsb;  // Key 1 27:0  (31:25)(23:17)(15:9)(7:1)
        uint32_t key2Msb;  // Key 2 55:28 (31:25)(23:17)(15:9)(7:1)
        uint32_t key2Lsb;  // Key 2 27:0  (31:25)(23:17)(15:9)(7:1)
        uint32_t key3Msb;  // Key 3 55:28 (31:25)(23:17)(15:9)(7:1)
        uint32_t key3Lsb;  // Key 3 27:0  (31:25)(23:17)(15:9)(7:1)
        uint32_t ivMsb;    // Initialization vector 63:32
        uint32_t ivLsb;    // Initialization vector 31:0
    }
    FAPI_DMA_CryptoKeysT;

    #ifdef __cplusplus
    extern "C" {
    #endif

    FAPI_DMA_RequestT* FAPI_DMA_StartCrypto(
                           volatile uint32_t*      sourceBuffer,
                           volatile uint32_t*      targetBuffer,
                           uint32_t                bufferBytes,
                           FAPI_DMA_CryptoParamsT* cryptoParams,
                           FAPI_DMA_CryptoKeysT*   cryptoKeys,
                           FAPI_DMA_OptCallbackT   finishCallback );

    #ifdef __cplusplus
    }
    #endif

    /*
    ****************************************************************************
    ****************************************************************************
    **
    ** Color space conversion feature specific functions and data types
    **
    ****************************************************************************
    ****************************************************************************
    */
    #define FAPI_DMA_CS_INPUT_MODE_ARGB     0 //!< Alpha  Red   Green Blue
    #define FAPI_DMA_CS_INPUT_MODE_BGRA     1 //!< Blue   Green Red   Alpha
    #define FAPI_DMA_CS_INPUT_MODE_RGBA     2 //!< Red    Green Blue  Alpha
    #define FAPI_DMA_CS_INPUT_MODE_ABGR     3 //!< Alpha  Blue  Green Red
    #define FAPI_DMA_CS_INPUT_MODE_RGB      4 //!< Red    Green Blue  unused
    #define FAPI_DMA_CS_INPUT_MODE_BGR      5 //!< unused Blue  Green Red

    #define FAPI_DMA_CS_CONVERSION_MODE_444 1 //!< conv. mode CCIR 4:4:4 (2 pixel/uint32_t)
    #define FAPI_DMA_CS_CONVERSION_MODE_422 0 //!< conv. mode CCIR 4:2:2 (4 pixel/uint32_t)

    typedef struct
    {
        /*!
        ** Selects the input color mode, supported values are
        ** #FAPI_DMA_CS_INPUT_MODE_ARGB, #FAPI_DMA_CS_INPUT_MODE_BGRA,
        ** #FAPI_DMA_CS_INPUT_MODE_RGBA, #FAPI_DMA_CS_INPUT_MODE_ABGR,
        ** #FAPI_DMA_CS_INPUT_MODE_RGB and #FAPI_DMA_CS_INPUT_MODE_BGR
        */
        uint32_t colorInputMode;

        /*!
        ** Controls the manual alpha value mode
        ** FTRUE means enable the manual alpha mode
        ** FFALSE means disable the manual alpha mode
        ** and ignore the manualAlphaValue
        */
        uint32_t manualAlphaEnable;

        /*!
        ** the manual alpha valueto replcae the input RGB alpha
        ** value with
        */
        uint32_t manualAlphaValue;

        /*!
        ** Selects the output color mode, supported values are
        ** #FAPI_DMA_CS_CONVERSION_MODE_444 and #FAPI_DMA_CS_CONVERSION_MODE_422
        */
        uint32_t conversionMode;
    }
    FAPI_DMA_CsConversionParamsT;

    typedef struct
    {
        uint32_t Y_red;     //!< red coefficient for the Y part in the conversion formula
        uint32_t Y_green;   //!< green coefficient for the Y part in the conversion formula
        uint32_t Y_blue;    //!< blue coefficient for the Y part in the conversion formula
        uint32_t Cr_red;    //!< red coefficient for the Cr part in the conversion formula
        uint32_t Cr_green;  //!< green coefficient for the Cr part in the conversion formula
        uint32_t Cr_blue;   //!< blue coefficient for the Cr part in the conversion formula
        uint32_t Cb_red;    //!< red coefficient for the Cb part in the conversion formula
        uint32_t Cb_green;  //!< green coefficient for the Cb part in the conversion formula
        uint32_t Cb_blue;   //!< blue coefficient for the Cb part in the conversion formula
        uint32_t Y_offset;  //!< offset value for Y
        uint32_t Cr_offset; //!< offset value for Cr
        uint32_t Cb_offset; //!< offset value for Cb
    }
    FAPI_DMA_CsConversionCoeffT;

    #ifdef __cplusplus
    extern "C" {
    #endif

    FAPI_DMA_RequestT* FAPI_DMA_StartCsConversion(
                           volatile uint32_t*            sourceBuffer,
                           volatile uint32_t*            targetBuffer,
                           uint32_t                      sourceBytes,
                           FAPI_DMA_CsConversionParamsT* csParams,
                           FAPI_DMA_CsConversionCoeffT*  csCoeff,
                           FAPI_DMA_OptCallbackT         finishCallback );

    #ifdef __cplusplus
    }
    #endif

#endif
