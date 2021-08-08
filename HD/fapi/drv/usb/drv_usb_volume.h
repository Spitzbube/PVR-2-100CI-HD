/*!
*******************************************************************************
**
** \file        ./fapi/drv/usb/src/drv_usb_volume.h
** 
** \brief       USB Volume Management.
**
** This header file contains the declaration of public function(s)
** to manage USB mass storage volumes.
** This module has been designed to facilitate filesystem connection.
**
** \attention   THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU MICROELECTRONICS
**              ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**              OMMISSIONS.
**
** (C) Copyright 2006-2009 by Fujitsu Microelectronics Europe GmbH
**
*******************************************************************************
*/
#ifndef __FAPI_USB_VOLUME_H__
    #define __FAPI_USB_VOLUME_H__


    #include <fapi/sys_driver.h>

    //***********************************************************************
    //***********************************************************************
    //** Defines and Macros
    //***********************************************************************
    //***********************************************************************

    //! The highest valid volume ID.
    #define FAPI_USB_MAX_VOLUME_ID          7

    #define FAPI_USB_MAX_VOLUME_SERIAL      31
    #define FAPI_USB_MAX_DISK_VENDOR        15
    #define FAPI_USB_MAX_DISK_PRODUCT      31
    #define FAPI_USB_MAX_DISK_REVISION       7

    /*
    *************************************************************************
    *************************************************************************
    ** Data types
    *************************************************************************
    *************************************************************************
    */

    /*!
    *************************************************************************
    ** \brief Version of the USB volume management.
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
    **       FAPI_USB_VolumeOpenParamsT structure.
    **
    *************************************************************************
    */

    typedef enum {
        FAPI_USB_VOLUME_VERSION = (int32_t)0x00020000   //!< The current version
    } FAPI_USB_VolumeVersionEnumT;


    /*!
    *************************************************************************
    ** \brief Open parameters for the mass storage volume.
    **
    ** This data structure covers all parameters which need to be specified
    ** when an USB mass storage volume is opened.
    **
    *************************************************************************
    */
    typedef struct {
        /*!
           The version of the driver.
           \ref FAPI_USB_VolumeVersionEnumT "FAPI_USB_VOLUME_VERSION".
        */
        FAPI_USB_VolumeVersionEnumT version;

        /*!
           The ID of the USB volume to be opened,
           A value between 0 and #FAPI_USB_MAX_VOLUME_ID.
        */
        uint32_t volumeId;

        /*!
           Flag to request DMA for read/write transfer operation.
        */
        uint32_t useDMA;

        /*!
           Flag to indicate whether application runs with RTOS.
        */
        uint32_t useRTOS;

    } FAPI_USB_VolumeOpenParamsT;


    /*!
    *************************************************************************
    ** \brief Identity information about the volume.
    **
    ** This data structure provides data about the identity of the actual volume.
    **
    *************************************************************************
    */
    typedef struct {
        /*!
           The ID of the USB volume.
           A value between 0 and #FAPI_USB_MAX_VOLUME_ID.
        */
        uint32_t volumeId;

        /*!
           The connection flag.
           (1) if the volume is connected, otherwise (0).
        */
        uint32_t isConnected;

        /*!
           The removable flag.
           Only valid if volume is connected. (1) if the storage medium
           is removable, otherwise (0).
        */
        uint32_t isRemovable;

        /*!
           The USB bus address.
           Only valid if volume is connected.
        */
        uint32_t busAddress;

        /*!
           The sector size of the volume.
           Only valid if volume is connected. Zero (0) if no medium
           is inserted.
        */
        uint32_t sectorSize;

        /*!
           The number of sectors of the volume.
           Only valid if volume is connected.
           Zero (0) if no medium is inserted.
        */
        uint32_t sectorCount;

        /*!
           The device serial number string.
           Only valid if volume is connected.
        */
        uint8_t serialNumber[FAPI_USB_MAX_VOLUME_SERIAL+1];

        /*!
           The vendor name string of the underlying medium.
           Only valid if volume is connected.
        */
        uint8_t diskVendor[FAPI_USB_MAX_DISK_VENDOR+1];

        /*!
           The product name string of the underlying medium.
           Only valid if volume is connected.
        */
        uint8_t diskProduct[FAPI_USB_MAX_DISK_PRODUCT+1];

        /*!
           The revision name string of the underlying medium.
           Only valid if volume is connected.
        */
        uint8_t diskRevision[FAPI_USB_MAX_DISK_REVISION+1];

    } FAPI_USB_VolumeInfoT;

    /*!
    ****************************************************************************
    **
    ** \brief Media change notifier function type.
    **
    ** This type of function is accepted as notifier for the application in
    ** response to USB storage medium changes.
    **
    ** \param infoPtr    The pointer to the volume information.
    ** \param insertFlag (1) if the medium has been inserted and
    **                   (0) if the medium was removed.
    **
    ** \sa FAPI_USB_SetVolumeNotifyFct
    **
    ****************************************************************************
    */
    typedef void (*FAPI_USB_VolumeNotifyFuncT)(FAPI_USB_VolumeInfoT* infoPtr,
                                                uint32_t insertFlag);
    
    
    
    struct Struct_21c402a8
    {
       int Data_0; //0
       int fill_4; //4
       int Data_8; //8
       unsigned Data_12; //12
       unsigned Data_16; //16
       int fill_20[17]; //20
       int dwBlockLength; //88
       unsigned short arData_92[32]; //92
       unsigned short idVendor; //156
       unsigned short idProduct; //158
       unsigned short bcdDevice; //160
       char bDeviceAddress; //162
       unsigned short fill_164; //164
       unsigned short vendor[16]; //166
       unsigned short product[32]; //198
       unsigned short revision[8]; //262
       //280
    } /*Struct_b75bb8_Inner1*/;

    int func_21c402a8(void**, struct Struct_21c402a8*, void*);
    

    /*
    *********************************************************************
    *********************************************************************
    ** Volume functions
    *********************************************************************
    *********************************************************************
    */

    #ifdef __cplusplus
    extern "C" {
    #endif
    void                   FAPI_USB_SetVolumeNotifyFct(
                                        FAPI_USB_VolumeNotifyFuncT notifyFct );
    FAPI_SYS_HandleT       FAPI_USB_OpenVolume(
                                 const FAPI_USB_VolumeOpenParamsT* paramsPtr,
                                 int32_t* errorCodePtr
                           );
    int32_t                FAPI_USB_CloseVolume ( FAPI_SYS_HandleT handle );
    FAPI_USB_VolumeInfoT*  FAPI_USB_GetVolumeInfo( FAPI_SYS_HandleT handle );
    uint32_t               FAPI_USB_ReadVolume( FAPI_SYS_HandleT handle,
                                                void*            dataPtr,
                                                uint32_t         lba,
                                                uint32_t         lbaCnt);
    uint32_t               FAPI_USB_WriteVolume( FAPI_SYS_HandleT handle,
                                                 void*            dataPtr,
                                                 uint32_t         lba,
                                                 uint32_t         lbaCnt);
    void                   FAPI_USB_CheckMedium(void);
    #ifdef __cplusplus
    }
    #endif

#endif
