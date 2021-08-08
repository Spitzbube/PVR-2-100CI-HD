/*!
********************************************************************************
********************************************************************************
**
** \file        ./fapi/drv/ata/src/drv_ata.h
**
** \brief       ATA Driver.
**
** This file contains the declaration of the ATA driver API.
**
** \attention   THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU MICROELECTRONICS
**              ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**              OMMISSIONS.
**
** (C) Copyright 2006-2009 by Fujitsu Microelectronics Europe GmbH
**
********************************************************************************
********************************************************************************
*/

#ifndef FAPI_ATA_H
    #define FAPI_ATA_H

    #include <fapi/sys_driver.h>

    //**************************************************************************
    //**************************************************************************
    //** Driver specific error codes
    //**************************************************************************
    //**************************************************************************

    /*!
    ********************************************************************************
    ** Standard error codes for the driver
    ********************************************************************************
    */

    /*! Bad parameter passed. */
    #define FAPI_ATA_ERR_BAD_PARAMETER         (FAPI_ATA_MODULE_BASE +\
                                                FAPI_ERR_BAD_PARAMETER)
    /*! Memory allocation failed. */
    #define FAPI_ATA_ERR_OUT_OF_MEMORY         (FAPI_ATA_MODULE_BASE +\
                                                FAPI_ERR_OUT_OF_MEMORY)
    /*! Device already initialised. */
    #define FAPI_ATA_ERR_ALREADY_INITIALIZED   (FAPI_ATA_MODULE_BASE +\
                                                FAPI_ERR_ALREADY_INITIALIZED)
    /*! Device not initialised. */
    #define FAPI_ATA_ERR_NOT_INITIALIZED       (FAPI_ATA_MODULE_BASE +\
                                                FAPI_ERR_NOT_INITIALIZED)
    /*! Feature or function is not available. */
    #define FAPI_ATA_ERR_FEATURE_NOT_SUPPORTED (FAPI_ATA_MODULE_BASE +\
                                                FAPI_ERR_FEATURE_NOT_SUPPORTED)
    /*! Timeout occured. */
    #define FAPI_ATA_ERR_TIMEOUT               (FAPI_ATA_MODULE_BASE +\
                                                FAPI_ERR_TIMEOUT)
    /*! The device is busy, try again later. */
    #define FAPI_ATA_ERR_DEVICE_BUSY           (FAPI_ATA_MODULE_BASE +\
                                                FAPI_ERR_DEVICE_BUSY)
    /*! Invalid handle was passed. */
    #define FAPI_ATA_ERR_INVALID_HANDLE        (FAPI_ATA_MODULE_BASE +\
                                                FAPI_ERR_INVALID_HANDLE)
    /*! Semaphore could not be created. */
    #define FAPI_ATA_ERR_SEMAPHORE_CREATE      (FAPI_ATA_MODULE_BASE +\
                                                FAPI_ERR_SEMAPHORE_CREATE)
    /*! The driver's used version is not supported. */
    #define FAPI_ATA_ERR_UNSUPPORTED_VERSION   (FAPI_ATA_MODULE_BASE +\
                                                FAPI_ERR_UNSUPPORTED_VERSION)

    /*!
    ****************************************************************************
    ** Additional driver error codes (reserved from -100 to -999)
    ** Each error code shall be decreased in steps of 1.
    ****************************************************************************
    */
    #define FAPI_ATA_ERR_BASE                   (FAPI_ATA_MODULE_BASE-100)
    #define FAPI_ATA_ERR_DEVICE_NOT_SUPPORTED   (FAPI_ATA_ERR_BASE- 1)     //!< device type is not supported
    #define FAPI_ATA_ERR_UPI_CONFIGURATION      (FAPI_ATA_ERR_BASE- 2)     //!< UPI configuration for ATA failed
    #define FAPI_ATA_ERR_SET_COMMAND            (FAPI_ATA_ERR_BASE- 3)     //!< issue ATA command failed
    #define FAPI_ATA_ERR_IDENTIFY_DEVICE        (FAPI_ATA_ERR_BASE- 4)     //!< identifying the device failed
    #define FAPI_ATA_ERR_NOT_ENABLED            (FAPI_ATA_ERR_BASE- 5)     //!< device mode/type is not enabled
    #define FAPI_ATA_ERR_SECTOR_COUNT           (FAPI_ATA_ERR_BASE- 6)     //!< sector count is out of range, >256
    #define FAPI_ATA_ERR_LBA_NUMBER             (FAPI_ATA_ERR_BASE- 7)     //!< sector count is out of range, >256
    #define FAPI_ATA_ERR_COMMAND_FAILED         (FAPI_ATA_ERR_BASE- 8)     //!< current ATA command failed
    #define FAPI_ATA_ERR_DMA_REQUEST_FAILED     (FAPI_ATA_ERR_BASE- 9)     //!< current DMA request failed
    #define FAPI_ATA_ERR_DMA_TRANSFER_FAILED    (FAPI_ATA_ERR_BASE-10)     //!< current DMA transfer failed
    #define FAPI_ATA_ERR_PIOMODE_NOT_SUPPORTED  (FAPI_ATA_ERR_BASE-11)     //!< pio mode not supported
    #define FAPI_ATA_ERR_DMAMODE_NOT_SUPPORTED  (FAPI_ATA_ERR_BASE-12)     //!< dma mode not supported
    #define FAPI_ATA_ERR_DEVICE_IN_STANDBY      (FAPI_ATA_ERR_BASE-13)     //!< Device is in stanbdy mode please call FAPI_ATA_PowerUp or FAPI_ATA_SpinUpWait

    //**************************************************************************
    //**************************************************************************
    //** Defines and Macros
    //**************************************************************************
    //**************************************************************************

    #define FAPI_ATA_TYPE_PARALLEL  0   //!< a PATA connected device
    #define FAPI_ATA_TYPE_SERIAL    1   //!< a SATA connected device, NOT supported in MB86H60
    #define FAPI_ATA_CHANNEL_MASTER 0   //!< the master device at parallel ata
    #define FAPI_ATA_CHANNEL_SLAVE  1   //!< the slave device at parallel ata
    #define FAPI_ATA_SECTOR_BYTES   512 //!< ATA uses 512 byte blocks
    #define FAPI_ATA_SECTOR_MIN     1   //!< min. allowed sector count per multiple dma read/write
    #define FAPI_ATA_SECTOR_MAX     255 //!< max. allowed sector count per multiple dma read/write

    //**************************************************************************
    //**************************************************************************
    //** Device information string length
    //**************************************************************************
    //**************************************************************************

    #define FAPI_ATA_SERIAL_NUMBER_LENGTH     20 //!< length of the serial number string
    #define FAPI_ATA_FIRMWARE_REVISION_LENGTH 8  //!< length of the firmware revision string
    #define FAPI_ATA_MODEL_NUMBER_LENGTH      40 //!< length of the model number string

    //**************************************************************************
    //**************************************************************************
    //** Supported Transfer modes
    //**************************************************************************
    //**************************************************************************
    #define FAPI_ATA_DMA_TRANSFER_MODE  0   //!< DMA transfer mode is the default value
    #define FAPI_ATA_PIO_TRANSFER_MODE  1   //!< PIO transfer mode

    //**************************************************************************
    //**************************************************************************
    //** Data types
    //**************************************************************************
    //**************************************************************************

    /*!
    ****************************************************************************
    ** \brief Version of the ATA driver.
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
    **       FAPI_ATA_OpenParamsT structure.
    **
    ****************************************************************************
    */
    typedef enum
    {
        FAPI_ATA_VERSION = (int32_t)0x00020002 //!< The current driver version
    } FAPI_ATA_VersionEnumT;

    /*!
    ****************************************************************************
    ** \brief Open parameters for the ATA driver.
    **
    ** This data structure covers all parameters which need to be specified
    ** when an instance of the driver is opened.
    **
    ****************************************************************************
    */
    typedef struct
    {
        /*!
        ** The version of the driver.
        ** \ref FAPI_ATA_VersionEnumT "FAPI_ATA_VERSION".
        */
        FAPI_ATA_VersionEnumT version; //0
        /*!
        ** The device mode, either #FAPI_ATA_TYPE_PARALLEL or
        ** #FAPI_ATA_TYPE_SERIAL.
        ** #FAPI_ATA_TYPE_SERIAL is NOT allowed for the MB86H60 chip.
        */
        uint32_t type; //4
        /*!
        ** The device to open, either #FAPI_ATA_CHANNEL_MASTER or
        ** #FAPI_ATA_CHANNEL_SLAVE.
        */
        uint32_t channel; //8
        /*!
        ** The number of bytes per sector, should be #FAPI_ATA_SECTOR_BYTES
        ** for harddisk devices.
        */
        uint32_t sectorBytes; //12
        /*!
        ** The addressing mode, 28 bit if 'false', 48 bit if 'true'
        */
        uint32_t use48BitLba; //16
        /*!
        ** The timeout value in milliseconds to wait until returning
        ** with a timeout error while waiting for spinup at identify device.
        */
        uint32_t waitTimeout; //20
        //24
    } FAPI_ATA_OpenParamsT;

    /*!
    ****************************************************************************
    ** \brief Device info (geometry) structure.
    **
    ** This data structure contains all geometry information about an open
    ** device.
    **
    ****************************************************************************
    */
    typedef struct
    {
        char_t   serialNumber[FAPI_ATA_SERIAL_NUMBER_LENGTH+1]; //0        //!< device serial number
        char_t   firmwareRevision[FAPI_ATA_FIRMWARE_REVISION_LENGTH+1]; //21 //!< the firmware revision
        char_t   modelNumber[FAPI_ATA_MODEL_NUMBER_LENGTH+1]; //30          //!< the model number
        uint32_t cylinders; //72         //!< number of logical cylinders
        uint32_t heads; //76             //!< number of logical heads
        uint32_t sectors; //80           //!< number of sectors per logical track
        uint32_t lbaCapacity; //84       //!< total number of addressable sectors
        uint32_t maxMultipleSectors; //88 //!< number of sectors which can be accessed at once
        uint32_t pioModeNumber; //92      //!< current pio mode
        uint32_t pioMode3; //96          //!< 'true' if device supports pio mode 3
        uint32_t pioMode4; //100          //!< 'true' if device supports pio mode 4
        uint32_t majorVersion; //104      //!< device version, major part
        uint32_t minorVersion; //108      //!< device version, minor part
        uint32_t removableMedia; //112    //!< 'true' if device supports removable media
        //116
    } FAPI_ATA_DeviceInfoT;

    /*!
    ****************************************************************************
    ** \brief Generic structure.
    **
    ** This generic structure contains all information for implementation
    ** a low level PIO mode transfer.
    **
    ****************************************************************************
    */
    typedef struct
    {
        uint32_t  feature;           //!< Feature register for write acces or error register for read acces
        uint32_t  sectors;           //!< Sectors register
        uint32_t  lba;               //!< lba register low/mid/high
        uint32_t  command;           //!< Command register for write acces or state register for read acces
    }FAPI_ATA_GenericPioModeT;

    //**************************************************************************
    //**************************************************************************
    //** Global Data
    //**************************************************************************
    //**************************************************************************

    extern FAPI_SYS_DriverT FAPI_ATA_Driver;

    //**************************************************************************
    //**************************************************************************
    //** API Functions
    //**************************************************************************
    //**************************************************************************

    #ifdef __cplusplus
    extern "C" {
    #endif

    void             FAPI_ATA_SetCommandTimeout( uint32_t deviceType, uint32_t msecsTimeout );
    void             FAPI_ATA_SetTypeEnable( uint32_t deviceType, uint32_t enableFlag );
    void             FAPI_ATA_SetChannelEnable( uint32_t deviceChannel, uint32_t enableFlag );
    void             FAPI_ATA_SetMultipleSectors( const uint32_t multipleMode );
    int32_t          FAPI_ATA_Init( void);
    void             FAPI_ATA_Exit( void);
    FAPI_SYS_HandleT FAPI_ATA_Open( const FAPI_ATA_OpenParamsT* paramsPtr, int32_t* errorCodePtr );
    int32_t          FAPI_ATA_Close( FAPI_SYS_HandleT handle );
    int32_t          FAPI_ATA_GetDeviceInfo( FAPI_SYS_HandleT handle, FAPI_ATA_DeviceInfoT* infoBufferPtr );
    int32_t          FAPI_ATA_ReadSector( FAPI_SYS_HandleT handle, uint32_t lba, void* bufferPtr );
    int32_t          FAPI_ATA_ReadSectors( FAPI_SYS_HandleT handle, uint32_t lba, uint32_t sectorCount, void* bufferPtr );
    int32_t          FAPI_ATA_WriteSector( FAPI_SYS_HandleT handle, uint32_t lba, void* bufferPtr );
    int32_t          FAPI_ATA_WriteSectors( FAPI_SYS_HandleT handle, uint32_t lba, uint32_t sectorCount, void* bufferPtr );
    int32_t          FAPI_ATA_SpinUpWait( FAPI_SYS_HandleT handle, uint32_t msecsTimeOut );
    int32_t          FAPI_ATA_PowerUp( FAPI_SYS_HandleT handle );
    int32_t          FAPI_ATA_PowerDown( FAPI_SYS_HandleT handle );
    int32_t          FAPI_ATA_GenericPioMode( const FAPI_SYS_HandleT handle,
                                              const FAPI_ATA_GenericPioModeT* genericSendDataPtr,
                                              const uint16_t* optDataPtrSend,
                                              const uint32_t  sector2write,
                                                    FAPI_ATA_GenericPioModeT* genericReadDataPtr,
                                                    uint16_t* optDataPtrRead,
                                              const uint32_t  sector2read,
                                              const uint32_t  msecTimeOut );
    int32_t         FAPI_ATA_SetTransferMode( FAPI_SYS_HandleT handle, const uint32_t transferMode );
    int32_t         FAPI_ATA_SetTransferModeGlobal( const uint32_t transferMode );
    int32_t         FAPI_ATA_GetPowerMode( FAPI_SYS_HandleT handle );

    #ifdef __cplusplus
    }
    #endif

#endif
