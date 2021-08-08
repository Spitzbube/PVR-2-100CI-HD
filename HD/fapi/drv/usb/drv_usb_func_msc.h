/*!
*******************************************************************************
**
** \file        ./fapi/drv/usb/src/drv_usb_func_msc.h
** 
** \brief       USB client for peripheral mass storage device class.
** 
** This header file contains the declaration of functions to register and
** operate the USB controller as peripheral mass storage device.
**
** \attention   THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU MICROELECTRONICS
**              ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**              OMMISSIONS.
**
** (C) Copyright 2006-2009 by Fujitsu Microelectronics Europe GmbH
** 
*******************************************************************************
*/
#ifndef FAPI_USB_FUNC_MSC_H
    #define FAPI_USB_FUNC_MSC_H
    
    #include <fapi/sys_services.h>
    #include <musb/mu_cdi.h>
    #include "drv_usb_func_vendor.h"
    
    //***********************************************************************
    //***********************************************************************
    //** Defines and Macros
    //***********************************************************************
    //***********************************************************************

    #define FAPI_USB_FUNC_MSC_REQ_GET_MAX_LUN 0xfe
    #define FAPI_USB_FUNC_MSC_REQ_RESET       0xff
    
    /*! 
        The USB Product ID to be used for this Mass Storage Device 
        driver in conjunction with #FAPI_USB_FUNC_VENDOR_ID. 
        DON'T CHANGE THIS!
    */    
    #define FAPI_USB_FUNC_MSC_PRODUCT_ID  0x2015
    
    /*! 
        Product name to be used for this Mass Storage Device 
        driver in conjunction with #FAPI_USB_FUNC_VENDOR_ID on Decate.
        DON'T CHANGE THIS!
    */
    #define FAPI_USB_FUNC_MSC_PRODUCT_NAME "MB86H60 Mass Storage"
    
    /*! 
        The Serial Number string for this Mass Storage Device 
        driver.
    */ 
    #define FAPI_USB_FUNC_MSC_SERIAL_NUMBER "0123456789"

    /*!
    *********************************************************************
    *********************************************************************
    ** This structure is for the MUSB-Stack only!!!!
    *********************************************************************
    *********************************************************************
    */
    typedef void (*FAPI_USB_FUNC_MSC_PataSetDmaCommandFuncT) (FAPI_SYS_HandleT handle, 
                                                              uint32_t lba, 
                                                              uint32_t sectors, 
                                                              uint32_t readWrite);
    
    typedef struct {
        uint32_t         lba;
        uint32_t         blockLength;
        FAPI_SYS_HandleT ataHandle;
        FAPI_USB_FUNC_MSC_PataSetDmaCommandFuncT pataSetCmdFct;
    } FAPI_USB_FUNC_MSC_HddDmaInfoT;

    /*!
    *********************************************************************
    *********************************************************************
    ** This enumartion defines the valid LUN types for the mass storage
    ** class driver.
    *********************************************************************
    *********************************************************************
    */
    typedef enum 
    {
        FAPI_USB_FUNC_MSC_LUN_UNDEF = (uint32_t)0,
        FAPI_USB_FUNC_MSC_LUN_RAM, //1
        FAPI_USB_FUNC_MSC_LUN_HDD //2
    } FAPI_USB_FUNC_MSC_LunEnumT;
     
    /*!
    *********************************************************************
    *********************************************************************
    ** This structure contains the description of a logical unit (LUN)
    ** to be managed by the mass storage class driver.  
    *********************************************************************
    *********************************************************************
    */    
    typedef struct
    {
        /*! Type of LUN (RAM or HDD) */
        FAPI_USB_FUNC_MSC_LunEnumT type; //0
        
        /*! 
            Pointer to the LUN's name or NULL if unnamed. 
        */
        char_t*  namePtr; //4
        
        /*! 
            Start address (if type #FAPI_USB_FUNC_MSC_LUN_RAM) or first logical 
            block of the device (if type #FAPI_USB_FUNC_MSC_LUN_HDD).
            
            \attention In case of RAM disk the start address must point to an
                       area of at least lbaCapacity * blockLength bytes.
        */
        uint32_t startAddrOrLba; //8
        
        /*!
            Total number of logical blocks for the device.
        */
        uint32_t lbaCapacity; //12
        
        /*!
            Length of logical block in bytes. (usually 512)
        */
        uint32_t blockLength; //16
        
         /*!
            TRUE (1) if the driver should prevent write access to this device.
        */
        uint32_t  readOnly; //20
        
        /*!
            The handle to the ATA device if type #FAPI_USB_FUNC_MSC_LUN_HDD.
        */
        FAPI_SYS_HandleT ataHandle; //24
        //28
    } FAPI_USB_FUNC_MSC_LunT;
     
    /*!
    *********************************************************************
    *********************************************************************
    ** This structure represents common initialzation parameter for the 
    ** USB mass storage device client.
    *********************************************************************
    *********************************************************************
    */        
    typedef struct
    {
        /*!
            The Vendor ID to transmit with the USB device descriptor. 
            If NULL #FAPI_USB_FUNC_VENDOR_ID will be assigned.
        */
        uint16_t  vendorID; //0
        
        /*!
            The product ID to transmit with the USB device descriptor. 
            If NULL #FAPI_USB_FUNC_MSC_PRODUCT_ID will be assigned.
        */
        uint16_t  productID; //2
        
        /*!
            The optional name of the vendor to transmit with the USB device 
            descriptor. NULL if not needed.
        */
        char_t*   vendorNamePtr; //4
        
        /*!
            The optional name of the product to transmit with the USB device 
            descriptor. NULL if not needed.
        */
        char_t*   productNamePtr; //8
        
        /*!
            The optional serial number string to transmit with the USB device 
            descriptor. NULL if not needed.
        */
        char_t*   serialNumberPtr; //12
        
        /*!
            Number of LUNs to be managed by the driver. 
        */
        uint32_t  lunCount; //16
        
        /*!
            List of LUNs to be managed by the driver. 
            List must contain lunCount elements.
        */
        FAPI_USB_FUNC_MSC_LunT* lunList; //20
        //24
    } FAPI_USB_FUNC_MSC_InitParamsT; 
 
 
    /*!
    ****************************************************************************
    **
    ** \brief Host access notifier function type.
    **
    ** This type of function is accepted as notifier for the application in
    ** response to an USB-HOST access.
    **
    ** \param connectFlag (1) if the USB host has been connected and
    **                    (0) if the USB host has been disconnected.
    **
    ** \sa FAPI_USB_SetVolumeNotifyFct
    **
    ****************************************************************************
    */
    typedef void (*FAPI_USB_FUNC_MSC_HostNotifyFuncT)(uint32_t connectFlag);
 
    #ifdef __cplusplus
    extern "C" {
    #endif
       
    /*
    *********************************************************************
    *********************************************************************
    **
    ** USB System functions for peripheral mass storage device mode.
    **
    *********************************************************************
    *********************************************************************
    */ 
    
    int32_t              FAPI_USB_FUNC_MSC_Init( FAPI_USB_FUNC_MSC_InitParamsT* paramsPtr ); 
    MUSB_FunctionClient* FAPI_USB_FUNC_MSC_GetFunctionClient(void);
    uint32_t             FAPI_USB_FUNC_MSC_Lock(void);
    void                 FAPI_USB_FUNC_MSC_Unlock(void);
    void                 FAPI_USB_FUNC_MSC_SetHostNotifyFct(
                                  FAPI_USB_FUNC_MSC_HostNotifyFuncT notifyFct );
    
    #ifdef __cplusplus
    }
    #endif
#endif
