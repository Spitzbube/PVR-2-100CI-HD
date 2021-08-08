/*!
*****************************************************************************
**
** \file        ./fapi/drv/usb/src/drv_usb.h
**
** \brief       USB Driver.
**
** This file contains the declaration of the USB driver API. The driver
** controls the Mentor USB
** (<a href='subsys_bus_musb.html'>MUSBStack-S</a>)
** subsystem and handles its initialization, class driver registration as
** well as starting and stopping of the USB bus.
**
** \attention   THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU MICROELECTRONICS
**              ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**              OMMISSIONS.
**
** (C) Copyright 2006-2009 by Fujitsu Microelectronics Europe GmbH
**
*****************************************************************************
*/

#ifndef FAPI_USB_H
    #define FAPI_USB_H

    #include <fapi/sys_driver.h>
    #include <musb/mu_arch.h>
    #include <musb/mu_cdi.h>

    //***********************************************************************
    //***********************************************************************
    //** Defines and Macros
    //***********************************************************************
    //***********************************************************************

    //! The highest valid controller ID.
    #define FAPI_USB_MAX_CONTROLLER_ID 0

    //! The maximal length of the target peripheral list.
    #define FAPI_USB_PERIPHERAL_LIST_MAX 50

    /*!
    ********************************************************************************
    ** Standard error codes for the driver
    ********************************************************************************
    */
    /*! Bad parameter passed. */
    #define FAPI_USB_ERR_BAD_PARAMETER         (FAPI_USB_MODULE_BASE + FAPI_ERR_BAD_PARAMETER)

    /*! Memory allocation failed. */
    #define FAPI_USB_ERR_OUT_OF_MEMORY         (FAPI_USB_MODULE_BASE + FAPI_ERR_OUT_OF_MEMORY)

    /*! Device already initialised. */
    #define FAPI_USB_ERR_ALREADY_INITIALIZED   (FAPI_USB_MODULE_BASE + FAPI_ERR_ALREADY_INITIALIZED)

    /*! Device not initialised. */
    #define FAPI_USB_ERR_NOT_INITIALIZED       (FAPI_USB_MODULE_BASE + FAPI_ERR_NOT_INITIALIZED)

    /*! Feature or function is not available. */
    #define FAPI_USB_ERR_FEATURE_NOT_SUPPORTED (FAPI_USB_MODULE_BASE + FAPI_ERR_FEATURE_NOT_SUPPORTED)

    /*! Timeout occured. */
    #define FAPI_USB_ERR_TIMEOUT               (FAPI_USB_MODULE_BASE + FAPI_ERR_TIMEOUT)

    /*! The device is busy, try again later. */
    #define FAPI_USB_ERR_DEVICE_BUSY           (FAPI_USB_MODULE_BASE + FAPI_ERR_DEVICE_BUSY)

    /*! Invalid handle was passed. */
    #define FAPI_USB_ERR_INVALID_HANDLE        (FAPI_USB_MODULE_BASE + FAPI_ERR_INVALID_HANDLE)

    /*! Semaphore could not be created. */
    #define FAPI_USB_ERR_SEMAPHORE_CREATE      (FAPI_USB_MODULE_BASE + FAPI_ERR_SEMAPHORE_CREATE)

    /*! The driver's used version is not supported. */
    #define FAPI_USB_ERR_UNSUPPORTED_VERSION   (FAPI_USB_MODULE_BASE + FAPI_ERR_UNSUPPORTED_VERSION)


    /*!
    ********************************************************************************
    ** Additional driver error codes (reserved from -100 to -999)
    **
    ** Each error code shall be decreased in steps of 1.
    ********************************************************************************
    */
    /*! USB driver specfic error code base. */
    #define FAPI_USB_ERR_BASE             (FAPI_USB_MODULE_BASE - 100)

    /*! The given controller or volume ID is wrong. */
    #define FAPI_USB_ERR_WRONG_ID         (FAPI_USB_ERR_BASE - 0)

    /*! The controller was already opened. */
    #define FAPI_USB_ERR_ALREADY_OPEN     (FAPI_USB_ERR_BASE - 1)

    /*! The maximum number of host drivers has been reached. */
    #define FAPI_USB_ERR_TOO_MANY_DRIVERS (FAPI_USB_ERR_BASE - 2)

    /*! The USB controller is tried to start multiple times. */
    #define FAPI_USB_ERR_ALREADY_RUNNING  (FAPI_USB_ERR_BASE - 3)

    /*! The USB controller is not running yet. */
    #define FAPI_USB_ERR_NOT_RUNNING      (FAPI_USB_ERR_BASE - 4)

    /*! The MUSBStack-S doesn't start. */
    #define FAPI_USB_ERR_MUSB_START       (FAPI_USB_ERR_BASE - 5)

    //***********************************************************************
    //***********************************************************************
    //** Data types
    //***********************************************************************
    //***********************************************************************

    /*!
    *************************************************************************
    ** \brief Version of the USB driver.
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
    **       FAPI_USB_OpenParamsT structure.
    **
    *************************************************************************
    */

    typedef enum {
        FAPI_USB_VERSION = (int32_t)0x00020000   //!< The current driver version
    } FAPI_USB_VersionEnumT;

    /*!
    *************************************************************************
    ** Callback for OTG state changes.
    **
    ** This type of function can be provided when an USB controller is opened.
    ** It will be called by MUSBStack-S whenever an OTG state transition takes
    ** place.
    **
    ** \param ctrlId The ID of the USB controller.
    ** \param newState The new OTG state. See also:
    ** <a href='../../general/musb/ucdi/MUSB_OtgState..html'>MUSB_OtgState</a>
    **
    ** \sa FAPI_USB_OpenParamsT
    *************************************************************************
    */
    typedef void (*FAPI_USB_OtgStateFuncT)(uint32_t ctrlId,
                                           MUSB_OtgState newState
                                          );

    /*!
    *************************************************************************
    ** Callback for OTG errors.
    **
    ** This type of function can be provided when an USB controller is opened.
    ** It will be called by MUSBStack-S whenever an OTG error occurs.
    **
    ** \param ctrlId The ID of the USB controller.
    ** \param otgError The OTG error code:
    **                 - MUSB_STATUS_OTG_NO_RESPONSE
    **                 - MUSB_STATUS_UNSUPPORTED_DEVICE
    **                 - MUSB_STATUS_UNSUPPORTED_HUB
    **                 - MUSB_STATUS_OTG_SRP_FAILED
    **                 - MUSB_STATUS_OTG_VBUS_INVALID
    **
    ** \sa FAPI_USB_OpenParamsT
    *************************************************************************
    */
    typedef void (*FAPI_USB_OtgErrorFuncT)(uint32_t ctrlId,
                                           uint32_t otgError
                                          );

    /*!
    *************************************************************************
    ** \brief Open parameters for the USB driver.
    **
    ** This data structure covers all parameters which need to be specified
    ** when an instance of the driver is opened.
    **
    *************************************************************************
    */
    typedef struct {
        /*!
           The version of the driver.
           \ref FAPI_USB_VersionEnumT "FAPI_USB_VERSION".
        */
        FAPI_USB_VersionEnumT version;

        /*!
           The ID of the USB controller to be opened,
           A value between 0 and #FAPI_USB_MAX_CONTROLLER_ID.
        */
        uint32_t ctrlId;

        /*!
           Flag to indicate the desire of a B-device to get the host role
           at the next possible opportunity. For dual role controller only.
        */
        uint8_t desireHostRoleFlag;

        /*!
           Callback to notify application of OTG state changes.
           If NULL no callback is required.
        */
        FAPI_USB_OtgStateFuncT otgStateNotifyFct;

        /*!
           Callback to notify application of OTG errors.
           If NULL no callback is required.
        */
        FAPI_USB_OtgErrorFuncT otgErrorNotifyFct;
    } FAPI_USB_OpenParamsT;

    //***********************************************************************
    //***********************************************************************
    //** Global Data
    //***********************************************************************
    //***********************************************************************
    extern FAPI_SYS_DriverT FAPI_USB_Driver0;

    //***********************************************************************
    //***********************************************************************
    //** API Functions
    //***********************************************************************
    //***********************************************************************

    #ifdef __cplusplus
    extern "C" {
    #endif
    int32_t          FAPI_USB_Init(void);
    void             FAPI_USB_Exit(void);
    uint32_t         FAPI_USB_Isr0(void);
    void             FAPI_USB_Bsr0( uint32_t privData );
    void             FAPI_USB_SetBaseAddress( uint32_t ctrlId,
                                              uint32_t address );
    void             FAPI_USB_SetKeepVbus( uint32_t keepFlag );
    FAPI_SYS_HandleT FAPI_USB_Open  ( const FAPI_USB_OpenParamsT* paramsPtr,
                                      int32_t* errorCodePtr
                                    );
    int32_t          FAPI_USB_Close ( FAPI_SYS_HandleT handle );
    int32_t          FAPI_USB_RegHostDriver(
                                        FAPI_SYS_HandleT handle,
                                        const MUSB_DeviceDriver* driverPtr,
                                        const uint8_t* peripheralListPtr,
                                        uint32_t listLength);
    int32_t          FAPI_USB_RegFuncDriver(
                                        FAPI_SYS_HandleT handle,
                                        const MUSB_FunctionClient* driverPtr );
    int32_t          FAPI_USB_Start( FAPI_SYS_HandleT handle );
    void             FAPI_USB_Stop( FAPI_SYS_HandleT handle );
    int32_t          FAPI_USB_Connect( FAPI_SYS_HandleT handle );
    int32_t          FAPI_USB_Disconnect( FAPI_SYS_HandleT handle );
    int32_t          FAPI_USB_RelinquishHost( FAPI_SYS_HandleT handle );
    int32_t          FAPI_USB_RequestBus( FAPI_SYS_HandleT handle );
    int32_t          FAPI_USB_ResumeBus( FAPI_SYS_HandleT handle );
    int32_t          FAPI_USB_SuspendBus( FAPI_SYS_HandleT handle );
    uint32_t         FAPI_USB_IsHost( FAPI_SYS_HandleT handle );
    uint32_t         FAPI_USB_IsHighSpeed( FAPI_SYS_HandleT handle );
    uint32_t         FAPI_USB_IsFullSpeed( FAPI_SYS_HandleT handle );
    uint32_t         FAPI_USB_IsLowSpeed( FAPI_SYS_HandleT handle );

    #ifdef __cplusplus
    }
    #endif
#endif
