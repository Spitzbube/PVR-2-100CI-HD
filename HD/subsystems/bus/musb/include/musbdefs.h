#ifndef MUSBDEFS_H_
#define MUSBDEFS_H_



/****************************** DEBUG CONSTANTS ********************************/

#define MGC_TEST_PACKET_SIZE 53
 
/****************************** CONSTANTS ********************************/


#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef MUSB_C_NUM_EPS
#define MUSB_C_NUM_EPS ((char)16)
#endif 




/*************************** REGISTER ACCESS ********************************/

#define MGC_SelectEnd(_pBase, _bEnd) \
    MGC_Write8(_pBase, MGC_O_HDRC_INDEX, _bEnd)
#define MGC_ReadCsr8(_pBase, _bOffset, _bEnd) \
    MGC_Read8(_pBase, (_bOffset + 0x10))
#define MGC_ReadCsr16(_pBase, _bOffset, _bEnd) \
    MGC_Read16(_pBase, (_bOffset + 0x10))
#define MGC_WriteCsr8(_pBase, _bOffset, _bEnd, _bData) \
    MGC_Write8(_pBase, (_bOffset + 0x10), _bData)
#define MGC_WriteCsr16(_pBase, _bOffset, _bEnd, _bData) \
    MGC_Write16(_pBase, (_bOffset + 0x10), _bData)



/************************** ULPI Registers ********************************/

/* Added in HDRC 1.9(?) & MHDRC 1.4 */
/* ULPI pass-through */
#define MGC_O_HDRC_ULPI_VBUSCTL 0x70
#define MGC_O_HDRC_ULPI_REGDATA 0x74
#define MGC_O_HDRC_ULPI_REGADDR 0x75
#define MGC_O_HDRC_ULPI_REGCTL  0x76 



/* Added in HDRC 1.9(?) & MHDRC 1.4 */
/* ULPI */
#define MGC_M_ULPI_VBUSCTL_USEEXTVBUSIND    0x02
#define MGC_M_ULPI_VBUSCTL_USEEXTVBUS       0x01
#define MGC_M_ULPI_REGCTL_INT_ENABLE        0x08
#define MGC_M_ULPI_REGCTL_READNOTWRITE      0x04
#define MGC_M_ULPI_REGCTL_COMPLETE      0x02
#define MGC_M_ULPI_REGCTL_REG           0x01 




/******************************** TYPES *************************************/


/**
 * The device request.
 */
typedef struct
{
    char bmRequestType; //0
    char bRequest; //1
    unsigned short wValue; //2
    unsigned short wIndex; //4
    unsigned short wLength; //6
} MUSB_DeviceRequest;


#endif /*MUSBDEFS_H_*/
