#ifndef USB_CH9_H_
#define USB_CH9_H_


/*-------------------------------------------------------------------------*/

/* CONTROL REQUEST SUPPORT */

/*
 * USB directions
 *
 * This bit flag is used in endpoint descriptors' bEndpointAddress field.
 * It's also one of three fields in control requests bRequestType.
 */
#define USB_DIR_OUT         0       /* to device */
#define USB_DIR_IN          0x80        /* to host */ 

/*
 * USB types, the second of three bRequestType fields
 */
#define USB_TYPE_MASK           (0x03 << 5)
#define USB_TYPE_STANDARD       (0x00 << 5)
#define USB_TYPE_CLASS          (0x01 << 5)
#define USB_TYPE_VENDOR         (0x02 << 5)
#define USB_TYPE_RESERVED       (0x03 << 5) 

/*
 * USB recipients, the third of three bRequestType fields
 */
#define USB_RECIP_MASK          0x1f
#define USB_RECIP_DEVICE        0x00
#define USB_RECIP_INTERFACE     0x01
#define USB_RECIP_ENDPOINT      0x02
#define USB_RECIP_OTHER         0x03 


/*
 * Standard requests, for the bRequest field of a SETUP packet.
 *
 * These are qualified by the bRequestType field, so that for example
 * TYPE_CLASS or TYPE_VENDOR specific feature flags could be retrieved
 * by a GET_STATUS request.
 */
#define USB_REQ_GET_STATUS      0x00
#define USB_REQ_CLEAR_FEATURE       0x01
#define USB_REQ_SET_FEATURE     0x03
#define USB_REQ_SET_ADDRESS     0x05
#define USB_REQ_GET_DESCRIPTOR      0x06
#define USB_REQ_SET_DESCRIPTOR      0x07
#define USB_REQ_GET_CONFIGURATION   0x08
#define USB_REQ_SET_CONFIGURATION   0x09
#define USB_REQ_GET_INTERFACE       0x0A
#define USB_REQ_SET_INTERFACE       0x0B
#define USB_REQ_SYNCH_FRAME     0x0C
 
/*
 * USB feature flags are written using USB_REQ_{CLEAR,SET}_FEATURE, and
 * are read as a bit array returned by USB_REQ_GET_STATUS.  (So there
 * are at most sixteen features of each type.)
 */
#define USB_DEVICE_SELF_POWERED     0   /* (read only) */
#define USB_DEVICE_REMOTE_WAKEUP    1   /* dev may initiate wakeup */
#define USB_DEVICE_TEST_MODE        2   /* (high speed only) */
#define USB_DEVICE_B_HNP_ENABLE     3   /* dev may initiate HNP */
#define USB_DEVICE_A_HNP_SUPPORT    4   /* RH port supports HNP */
#define USB_DEVICE_A_ALT_HNP_SUPPORT    5   /* other RH port does */ 



/*
 * STANDARD DESCRIPTORS ... as returned by GET_DESCRIPTOR, or
 * (rarely) accepted by SET_DESCRIPTOR.
 *
 * Note that all multi-byte values here are encoded in little endian
 * byte order "on the wire".  But when exposed through Linux-USB APIs,
 * they've been converted to cpu byte order.
 */

/*
 * Descriptor types ... USB 2.0 spec table 9.5
 */
#define USB_DT_DEVICE           0x01
#define USB_DT_CONFIG           0x02
#define USB_DT_STRING           0x03
#define USB_DT_INTERFACE        0x04
#define USB_DT_ENDPOINT         0x05
#define USB_DT_DEVICE_QUALIFIER     0x06
#define USB_DT_OTHER_SPEED_CONFIG   0x07
#define USB_DT_INTERFACE_POWER      0x08
/* these are from a minor usb 2.0 revision (ECN) */
#define USB_DT_OTG          0x09
#define USB_DT_DEBUG            0x0a
#define USB_DT_INTERFACE_ASSOCIATION    0x0b

/* conventional codes for class-specific descriptors */
#define USB_DT_CS_DEVICE        0x21
#define USB_DT_CS_CONFIG        0x22
#define USB_DT_CS_STRING        0x23
#define USB_DT_CS_INTERFACE     0x24
#define USB_DT_CS_ENDPOINT      0x25


/*-------------------------------------------------------------------------*/

#if 0
/* USB_DT_DEVICE: Device descriptor */
struct usb_device_descriptor 
{
    char  bLength; //0
    char  bDescriptorType; //1

    unsigned short bcdUSB; //2
    char  bDeviceClass; //4
    char  bDeviceSubClass; //5
    char  bDeviceProtocol; //6
    char  bMaxPacketSize0; //7
    unsigned short idVendor; //8
    unsigned short idProduct; //10
    unsigned short bcdDevice; //12
    char  iManufacturer; //14
    char  iProduct; //15
    char  iSerialNumber; //16
    char  bNumConfigurations; //17
};
#endif

#define USB_DT_DEVICE_SIZE      18


/*
 * Device and/or Interface Class codes
 * as found in bDeviceClass or bInterfaceClass
 * and defined by www.usb.org documents
 */
#define USB_CLASS_PER_INTERFACE     0   /* for DeviceClass */
#define USB_CLASS_AUDIO         1
#define USB_CLASS_COMM          2
#define USB_CLASS_HID           3
#define USB_CLASS_PHYSICAL      5
#define USB_CLASS_STILL_IMAGE       6
#define USB_CLASS_PRINTER       7
#define USB_CLASS_MASS_STORAGE      8
#define USB_CLASS_HUB           9
#define USB_CLASS_CDC_DATA      0x0a
#define USB_CLASS_CSCID         0x0b    /* chip+ smart card */
#define USB_CLASS_CONTENT_SEC       0x0d    /* content security */
#define USB_CLASS_VIDEO         0x0e
#define USB_CLASS_APP_SPEC      0xfe
#define USB_CLASS_VENDOR_SPEC       0xff 

/*-------------------------------------------------------------------------*/

/* USB_DT_CONFIG: Configuration descriptor information.
 *
 * USB_DT_OTHER_SPEED_CONFIG is the same descriptor, except that the
 * descriptor type is different.  Highspeed-capable devices can look
 * different depending on what speed they're currently running.  Only
 * devices with a USB_DT_DEVICE_QUALIFIER have any OTHER_SPEED_CONFIG
 * descriptors.
 */
struct usb_config_descriptor 
{
    char  bLength; //0
    char  bDescriptorType; //1

    unsigned short wTotalLength; //2
    char  bNumInterfaces; //4
    char  bConfigurationValue; //5
    char  iConfiguration; //6
    char  bmAttributes; //7
    char  bMaxPower; //8
};

#define USB_DT_CONFIG_SIZE      9 

/* from config descriptor bmAttributes */
#define USB_CONFIG_ATT_ONE      (1 << 7)    /* must be set */
#define USB_CONFIG_ATT_SELFPOWER    (1 << 6)    /* self powered */
#define USB_CONFIG_ATT_WAKEUP       (1 << 5)    /* can wakeup */
 
/*-------------------------------------------------------------------------*/

/* USB_DT_STRING: String descriptor */
struct usb_string_descriptor {
    char  bLength;
    char  bDescriptorType;

    unsigned short wData[1];        /* UTF-16LE encoded */
};

/* note that "string" zero is special, it holds language codes that
 * the device supports, not Unicode characters.
 */
 
/*-------------------------------------------------------------------------*/

/* USB_DT_INTERFACE: Interface descriptor */
struct usb_interface_descriptor 
{
    char  bLength; //0
    char  bDescriptorType; //1

    char  bInterfaceNumber; //2
    char  bAlternateSetting; //3
    char  bNumEndpoints; //4
    char  bInterfaceClass; //5
    char  bInterfaceSubClass; //6
    char  bInterfaceProtocol; //7
    char  iInterface; //8
};

#define USB_DT_INTERFACE_SIZE       9
 
/*-------------------------------------------------------------------------*/

/* USB_DT_ENDPOINT: Endpoint descriptor */
struct usb_endpoint_descriptor 
{
    char  bLength; //0
    char  bDescriptorType; //1

    char  bEndpointAddress; //2
    char  bmAttributes; //3
    unsigned short wMaxPacketSize; //4
    char  bInterval; //6

    // NOTE:  these two are _only_ in audio endpoints.
    // use USB_DT_ENDPOINT*_SIZE in bLength, not sizeof.
    char  bRefresh;
    char  bSynchAddress;
}; 

/*
 * Endpoints
 */
#define USB_ENDPOINT_NUMBER_MASK    0x0f    /* in bEndpointAddress */
#define USB_ENDPOINT_DIR_MASK       0x80

#define USB_ENDPOINT_XFERTYPE_MASK  0x03    /* in bmAttributes */
#define USB_ENDPOINT_XFER_CONTROL   0
#define USB_ENDPOINT_XFER_ISOC      1
#define USB_ENDPOINT_XFER_BULK      2
#define USB_ENDPOINT_XFER_INT       3 

/*-------------------------------------------------------------------------*/
 
/* USB_DT_DEVICE_QUALIFIER: Device Qualifier descriptor */
struct usb_qualifier_descriptor 
{
    char  bLength; //0
    char  bDescriptorType; //1

    unsigned short bcdUSB; //2
    char  bDeviceClass; //4
    char  bDeviceSubClass; //5
    char  bDeviceProtocol; //6
    char  bMaxPacketSize0; //7
    char  bNumConfigurations; //8
    char  bRESERVED; //9
};


/*-------------------------------------------------------------------------*/

/* USB_DT_OTG (from OTG 1.0a supplement) */
struct usb_otg_descriptor {
    char  bLength;
    char  bDescriptorType;

    char  bmAttributes; /* support for HNP, SRP, etc */
};

/* from usb_otg_descriptor.bmAttributes */
#define USB_OTG_SRP     (1 << 0)
#define USB_OTG_HNP     (1 << 1)    /* swap host/device roles */ 




#endif /*USB_CH9_H_*/
