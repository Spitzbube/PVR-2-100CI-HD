/*!
********************************************************************************
**
** \file      ./fapi/reg/src/MB86H60/reg_dma.h
**
** \brief     DMA access functions
**
** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU MICROELECTRONICS
**            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**            OMMISSIONS.
**
** \note      Do not modify this file as it is generated automatically.
**
** (C) Copyright 2006-2009 by Fujitsu Microelectronics Europe GmbH
**
********************************************************************************
*/
#if !defined(FREG_DMA_H)
#define FREG_DMA_H

#include <stdint.h>
#ifdef __LINUX__
#include "reg4linux.h"
#else
#define FIO_ADDRESS(block,address) (address)
#define FIO_MOFFSET(block,moffset) (moffset)
#endif

#define FREG_DMA_ENABLE_DEBUG_PRINT   0
#define FREG_DMA_DEBUG_PRINT_FUNCTION printf

#if FREG_DMA_ENABLE_DEBUG_PRINT == 1
#include <stdio.h>
#endif

/*
********************************************************************************
**
** Register definitions
**
********************************************************************************
*/
#define FREG_DMA_INTMASK                         FIO_ADDRESS(DMA,0xC7000004UL)
#define FREG_DMA_INTSTATUS                       FIO_ADDRESS(DMA,0xC7000008UL)
#define FREG_DMA_INTCLEAR                        FIO_ADDRESS(DMA,0xC700000CUL)
#define FREG_DMA_USBINTMASK                      FIO_ADDRESS(DMA,0xC70000F0UL)
#define FREG_DMA_USBINTSTATUS                    FIO_ADDRESS(DMA,0xC70000F4UL)
#define FREG_DMA_USBINTCLEAR                     FIO_ADDRESS(DMA,0xC70000F8UL)
#define FREG_DMA_BLOCKFILL                       FIO_ADDRESS(DMA,0xC7000010UL)
#define FREG_DMA_USBMODE                         FIO_ADDRESS(DMA,0xC7000014UL)
#define FREG_DMA_CCIRCONFIG                      FIO_ADDRESS(DMA,0xC7000020UL)
#define FREG_DMA_CCIRHCONFIG                     FIO_ADDRESS(DMA,0xC7000024UL)
#define FREG_DMA_CCIRVCONFIG                     FIO_ADDRESS(DMA,0xC7000028UL)
#define FREG_DMA_CSCONFIG                        FIO_ADDRESS(DMA,0xC7000060UL)
#define FREG_DMA_CSY                             FIO_ADDRESS(DMA,0xC7000064UL)
#define FREG_DMA_CSCR                            FIO_ADDRESS(DMA,0xC7000068UL)
#define FREG_DMA_CSCB                            FIO_ADDRESS(DMA,0xC700006CUL)
#define FREG_DMA_CSOFFSET                        FIO_ADDRESS(DMA,0xC7000070UL)
#define FREG_DMA_SRCONFIG                        FIO_ADDRESS(DMA,0xC7000080UL)
#define FREG_DMA_SRADDRESS                       FIO_ADDRESS(DMA,0xC7000084UL)
#define FREG_DMA_SRMASK                          FIO_ADDRESS(DMA,0xC7000088UL)
#define FREG_DMA_SRCOMPARE                       FIO_ADDRESS(DMA,0xC700008CUL)
#define FREG_DMA_SRREPLACE                       FIO_ADDRESS(DMA,0xC7000090UL)
#define FREG_DMA_SRREPLACEMASK                   FIO_ADDRESS(DMA,0xC7000094UL)
#define FREG_DMA_AUDIOCONFIG                     FIO_ADDRESS(DMA,0xC70000A0UL)
#define FREG_DMA_CRYPTOCONFIG                    FIO_ADDRESS(DMA,0xC70000C0UL)
#define FREG_DMA_CRYPTOK1MSB                     FIO_ADDRESS(DMA,0xC70000C4UL)
#define FREG_DMA_CRYPTOK1LSB                     FIO_ADDRESS(DMA,0xC70000C8UL)
#define FREG_DMA_CRYPTOK2MSB                     FIO_ADDRESS(DMA,0xC70000CCUL)
#define FREG_DMA_CRYPTOK2LSB                     FIO_ADDRESS(DMA,0xC70000D0UL)
#define FREG_DMA_CRYPTOK3MSB                     FIO_ADDRESS(DMA,0xC70000D4UL)
#define FREG_DMA_CRYPTOK3LSB                     FIO_ADDRESS(DMA,0xC70000D8UL)
#define FREG_DMA_IVMSB                           FIO_ADDRESS(DMA,0xC70000DCUL)
#define FREG_DMA_IVLSB                           FIO_ADDRESS(DMA,0xC70000E0UL)
#define FREG_DMA_CHCONFIG                        FIO_ADDRESS(DMA,0xC7000100UL)
#define FREG_DMA_CHLENGTH                        FIO_ADDRESS(DMA,0xC7000104UL)
#define FREG_DMA_CHLLADDR                        FIO_ADDRESS(DMA,0xC7000108UL)
#define FREG_DMA_CHRDADDR                        FIO_ADDRESS(DMA,0xC700010CUL)
#define FREG_DMA_CHRDLINE                        FIO_ADDRESS(DMA,0xC7000110UL)
#define FREG_DMA_CHRDINC                         FIO_ADDRESS(DMA,0xC7000114UL)
#define FREG_DMA_CHRDLPADDR                      FIO_ADDRESS(DMA,0xC7000118UL)
#define FREG_DMA_CHWRADDR                        FIO_ADDRESS(DMA,0xC700011CUL)
#define FREG_DMA_CHWRLINE                        FIO_ADDRESS(DMA,0xC7000120UL)
#define FREG_DMA_CHWRINC                         FIO_ADDRESS(DMA,0xC7000124UL)
#define FREG_DMA_CHWRLPADDR                      FIO_ADDRESS(DMA,0xC7000128UL)
#define FREG_DMA_CCIRWBCONFIG                    FIO_ADDRESS(DMA,0xC70000E4UL)
#define FREG_DMA_CCIRWBSTATUS                    FIO_ADDRESS(DMA,0xC70000E8UL)

/*
********************************************************************************
**
** Mirror variables for write-only registers
**
********************************************************************************
*/
extern volatile uint32_t FREG_DMA_IntMaskMirror;
extern volatile uint32_t FREG_DMA_IntClearMirror;
extern volatile uint32_t FREG_DMA_UsbIntMaskMirror;
extern volatile uint32_t FREG_DMA_UsbIntClearMirror;
extern volatile uint32_t FREG_DMA_BlockFillMirror;
extern volatile uint32_t FREG_DMA_UsbModeMirror;
extern volatile uint32_t FREG_DMA_CcirHconfigMirror;
extern volatile uint32_t FREG_DMA_CcirVconfigMirror;
extern volatile uint32_t FREG_DMA_CsConfigMirror;
extern volatile uint32_t FREG_DMA_CsYMirror;
extern volatile uint32_t FREG_DMA_CsCrMirror;
extern volatile uint32_t FREG_DMA_CsCbMirror;
extern volatile uint32_t FREG_DMA_CsOffsetMirror;
extern volatile uint32_t FREG_DMA_SrConfigMirror;
extern volatile uint32_t FREG_DMA_SrMaskMirror;
extern volatile uint32_t FREG_DMA_SrCompareMirror;
extern volatile uint32_t FREG_DMA_SrReplaceMirror;
extern volatile uint32_t FREG_DMA_SrReplaceMaskMirror;
extern volatile uint32_t FREG_DMA_AudioConfigMirror;
extern volatile uint32_t FREG_DMA_CryptoConfigMirror;
extern volatile uint32_t FREG_DMA_CryptoK1MsbMirror;
extern volatile uint32_t FREG_DMA_CryptoK1LsbMirror;
extern volatile uint32_t FREG_DMA_CryptoK2MsbMirror;
extern volatile uint32_t FREG_DMA_CryptoK2LsbMirror;
extern volatile uint32_t FREG_DMA_CryptoK3MsbMirror;
extern volatile uint32_t FREG_DMA_CryptoK3LsbMirror;
extern volatile uint32_t FREG_DMA_IvMsbMirror;
extern volatile uint32_t FREG_DMA_IvLsbMirror;
extern volatile uint32_t FREG_DMA_ChLengthMirror[6];
extern volatile uint32_t FREG_DMA_ChLladdrMirror[6];
extern volatile uint32_t FREG_DMA_ChRdaddrMirror[6];
extern volatile uint32_t FREG_DMA_ChRdlineMirror[6];
extern volatile uint32_t FREG_DMA_ChRdincMirror[6];
extern volatile uint32_t FREG_DMA_ChRdlpaddrMirror[6];
extern volatile uint32_t FREG_DMA_ChWraddrMirror[6];
extern volatile uint32_t FREG_DMA_ChWrlineMirror[6];
extern volatile uint32_t FREG_DMA_ChWrincMirror[6];
extern volatile uint32_t FREG_DMA_ChWrlpaddrMirror[6];
extern volatile uint32_t FREG_DMA_CcirWbConfigMirror;

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "IntMask".
**
** This function writes the given value to the register "IntMask".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetIntMask( uint32_t value );
#else
INLINE void FREG_DMA_SetIntMask( uint32_t value )
{
    FREG_DMA_IntMaskMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_INTMASK, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_INTMASK)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Mask" of register "IntMask".
**
** Mask bits for each IRQ source in DMA_INT_STATUS
** - '1': Mask IRQ
** - '0': Enable IRQ
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Mask" consists of the bits 9..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000003FFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetIntMask_Mask( uint32_t value );
#else
INLINE void FREG_DMA_SetIntMask_Mask( uint32_t value )
{
    register uint32_t mask = 0x000003FFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_IntMaskMirror & ~mask;
    FREG_DMA_IntMaskMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_INTMASK, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_INTMASK)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "IntMask".
**
** This function reads the value from the register "IntMask".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmIntMask( void );
#else
INLINE uint32_t FREG_DMA_GetmIntMask( void )
{
    register uint32_t value = FREG_DMA_IntMaskMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Mask" of register "IntMask".
**
** Mask bits for each IRQ source in DMA_INT_STATUS
** - '1': Mask IRQ
** - '0': Enable IRQ
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Mask" consists of the bits 9..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000003FFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmIntMask_Mask( void );
#else
INLINE uint32_t FREG_DMA_GetmIntMask_Mask( void )
{
    register uint32_t value = FREG_DMA_IntMaskMirror;
    value = ( value & 0x000003FFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "IntStatus".
**
** This function reads the value from the register "IntStatus".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetIntStatus( void );
#else
INLINE uint32_t FREG_DMA_GetIntStatus( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_INTSTATUS));
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "CcirSyncChange" of register "IntStatus".
**
** CCIR Sync status changed
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "CcirSyncChange" consists of the bits 9..9,
**       the given bitgroup value will be shifted left by 9 bits and
**       masked using the bitmask 0x00000200UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetIntStatus_CcirSyncChange( void );
#else
INLINE uint32_t FREG_DMA_GetIntStatus_CcirSyncChange( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_INTSTATUS));
    value = ( value & 0x00000200UL ) >> 9;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "SearchHit" of register "IntStatus".
**
** Search and replace unit found search pattern.
**  Hit address can be read from DMA_SR_ADDRESS
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "SearchHit" consists of the bits 8..8,
**       the given bitgroup value will be shifted left by 8 bits and
**       masked using the bitmask 0x00000100UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetIntStatus_SearchHit( void );
#else
INLINE uint32_t FREG_DMA_GetIntStatus_SearchHit( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_INTSTATUS));
    value = ( value & 0x00000100UL ) >> 8;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "AudioOverflow" of register "IntStatus".
**
** Audio input FIFO overflowed.
**  Data was lost.
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "AudioOverflow" consists of the bits 7..7,
**       the given bitgroup value will be shifted left by 7 bits and
**       masked using the bitmask 0x00000080UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetIntStatus_AudioOverflow( void );
#else
INLINE uint32_t FREG_DMA_GetIntStatus_AudioOverflow( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_INTSTATUS));
    value = ( value & 0x00000080UL ) >> 7;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "CcirOverflow" of register "IntStatus".
**
** CCIR input FIFO overflowed.
**  Data was lost.
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "CcirOverflow" consists of the bits 6..6,
**       the given bitgroup value will be shifted left by 6 bits and
**       masked using the bitmask 0x00000040UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetIntStatus_CcirOverflow( void );
#else
INLINE uint32_t FREG_DMA_GetIntStatus_CcirOverflow( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_INTSTATUS));
    value = ( value & 0x00000040UL ) >> 6;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ChannelComplete" of register "IntStatus".
**
** Channel has completed request.
**  1 bit per channel.
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ChannelComplete" consists of the bits 5..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x0000003FUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetIntStatus_ChannelComplete( void );
#else
INLINE uint32_t FREG_DMA_GetIntStatus_ChannelComplete( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_INTSTATUS));
    value = ( value & 0x0000003FUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "IntClear".
**
** This function writes the given value to the register "IntClear".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetIntClear( uint32_t value );
#else
INLINE void FREG_DMA_SetIntClear( uint32_t value )
{
    FREG_DMA_IntClearMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_INTCLEAR, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_INTCLEAR)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ClearIrq" of register "IntClear".
**
** Clear DMA IRQ
** - '1' Clear IRQ
** - '0' Do not clear IRQ
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ClearIrq" consists of the bits 9..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000003FFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetIntClear_ClearIrq( uint32_t value );
#else
INLINE void FREG_DMA_SetIntClear_ClearIrq( uint32_t value )
{
    register uint32_t mask = 0x000003FFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_IntClearMirror & ~mask;
    FREG_DMA_IntClearMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_INTCLEAR, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_INTCLEAR)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "IntClear".
**
** This function reads the value from the register "IntClear".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmIntClear( void );
#else
INLINE uint32_t FREG_DMA_GetmIntClear( void )
{
    register uint32_t value = FREG_DMA_IntClearMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ClearIrq" of register "IntClear".
**
** Clear DMA IRQ
** - '1' Clear IRQ
** - '0' Do not clear IRQ
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ClearIrq" consists of the bits 9..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000003FFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmIntClear_ClearIrq( void );
#else
INLINE uint32_t FREG_DMA_GetmIntClear_ClearIrq( void )
{
    register uint32_t value = FREG_DMA_IntClearMirror;
    value = ( value & 0x000003FFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "UsbIntMask".
**
** This function writes the given value to the register "UsbIntMask".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetUsbIntMask( uint32_t value );
#else
INLINE void FREG_DMA_SetUsbIntMask( uint32_t value )
{
    FREG_DMA_UsbIntMaskMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBINTMASK, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBINTMASK)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Mask" of register "UsbIntMask".
**
** Mask bits for each IRQ source in DMA_USB_INT_STATUS
** - '1' Mask IRQ
** - '0' Enable IRQ
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Mask" consists of the bits 6..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x0000007FUL
**
********************************************************************************
*/
extern void FREG_DMA_SetUsbIntMask_Mask( uint32_t value );
#else
INLINE void FREG_DMA_SetUsbIntMask_Mask( uint32_t value )
{
    register uint32_t mask = 0x0000007FUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_UsbIntMaskMirror & ~mask;
    FREG_DMA_UsbIntMaskMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBINTMASK, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBINTMASK)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "UsbIntMask".
**
** This function reads the value from the register "UsbIntMask".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmUsbIntMask( void );
#else
INLINE uint32_t FREG_DMA_GetmUsbIntMask( void )
{
    register uint32_t value = FREG_DMA_UsbIntMaskMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Mask" of register "UsbIntMask".
**
** Mask bits for each IRQ source in DMA_USB_INT_STATUS
** - '1' Mask IRQ
** - '0' Enable IRQ
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Mask" consists of the bits 6..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x0000007FUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmUsbIntMask_Mask( void );
#else
INLINE uint32_t FREG_DMA_GetmUsbIntMask_Mask( void )
{
    register uint32_t value = FREG_DMA_UsbIntMaskMirror;
    value = ( value & 0x0000007FUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "UsbIntStatus".
**
** This function reads the value from the register "UsbIntStatus".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetUsbIntStatus( void );
#else
INLINE uint32_t FREG_DMA_GetUsbIntStatus( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_USBINTSTATUS));
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "UsbEpDmaReadIrq" of register "UsbIntStatus".
**
** USB DMA Endpoint Read Request IRQ (for endpoints 1, 2, 3)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "UsbEpDmaReadIrq" consists of the bits 6..4,
**       the given bitgroup value will be shifted left by 4 bits and
**       masked using the bitmask 0x00000070UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetUsbIntStatus_UsbEpDmaReadIrq( void );
#else
INLINE uint32_t FREG_DMA_GetUsbIntStatus_UsbEpDmaReadIrq( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_USBINTSTATUS));
    value = ( value & 0x00000070UL ) >> 4;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "UsbEpDmaWriteIrq" of register "UsbIntStatus".
**
** USB DMA Endpoint Write Request IRQ (for endpoints 1, 2, 3)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "UsbEpDmaWriteIrq" consists of the bits 3..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x0000000EUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetUsbIntStatus_UsbEpDmaWriteIrq( void );
#else
INLINE uint32_t FREG_DMA_GetUsbIntStatus_UsbEpDmaWriteIrq( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_USBINTSTATUS));
    value = ( value & 0x0000000EUL ) >> 1;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "UsbIrq" of register "UsbIntStatus".
**
** USB General IRQ
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "UsbIrq" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetUsbIntStatus_UsbIrq( void );
#else
INLINE uint32_t FREG_DMA_GetUsbIntStatus_UsbIrq( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_USBINTSTATUS));
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "UsbIntClear".
**
** This function writes the given value to the register "UsbIntClear".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetUsbIntClear( uint32_t value );
#else
INLINE void FREG_DMA_SetUsbIntClear( uint32_t value )
{
    FREG_DMA_UsbIntClearMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBINTCLEAR, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBINTCLEAR)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ClearUsbIrq" of register "UsbIntClear".
**
** Clear USB IRQ
** - '1' Clear IRQ
** - '0' Do not clear IRQ
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ClearUsbIrq" consists of the bits 6..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x0000007FUL
**
********************************************************************************
*/
extern void FREG_DMA_SetUsbIntClear_ClearUsbIrq( uint32_t value );
#else
INLINE void FREG_DMA_SetUsbIntClear_ClearUsbIrq( uint32_t value )
{
    register uint32_t mask = 0x0000007FUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_UsbIntClearMirror & ~mask;
    FREG_DMA_UsbIntClearMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBINTCLEAR, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBINTCLEAR)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "UsbIntClear".
**
** This function reads the value from the register "UsbIntClear".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmUsbIntClear( void );
#else
INLINE uint32_t FREG_DMA_GetmUsbIntClear( void )
{
    register uint32_t value = FREG_DMA_UsbIntClearMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ClearUsbIrq" of register "UsbIntClear".
**
** Clear USB IRQ
** - '1' Clear IRQ
** - '0' Do not clear IRQ
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ClearUsbIrq" consists of the bits 6..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x0000007FUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmUsbIntClear_ClearUsbIrq( void );
#else
INLINE uint32_t FREG_DMA_GetmUsbIntClear_ClearUsbIrq( void )
{
    register uint32_t value = FREG_DMA_UsbIntClearMirror;
    value = ( value & 0x0000007FUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "BlockFill".
**
** This function writes the given value to the register "BlockFill".
**
** Block fill value for block fill operations
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetBlockFill( uint32_t value );
#else
INLINE void FREG_DMA_SetBlockFill( uint32_t value )
{
    FREG_DMA_BlockFillMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_BLOCKFILL, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_BLOCKFILL)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "BlockFillValue" of register "BlockFill".
**
** Block fill value for block fill operations
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "BlockFillValue" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetBlockFill_BlockFillValue( uint32_t value );
#else
INLINE void FREG_DMA_SetBlockFill_BlockFillValue( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_BlockFillMirror & ~mask;
    FREG_DMA_BlockFillMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_BLOCKFILL, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_BLOCKFILL)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "BlockFill".
**
** This function reads the value from the register "BlockFill".
**
** Block fill value for block fill operations
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmBlockFill( void );
#else
INLINE uint32_t FREG_DMA_GetmBlockFill( void )
{
    register uint32_t value = FREG_DMA_BlockFillMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "BlockFillValue" of register "BlockFill".
**
** Block fill value for block fill operations
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "BlockFillValue" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmBlockFill_BlockFillValue( void );
#else
INLINE uint32_t FREG_DMA_GetmBlockFill_BlockFillValue( void )
{
    register uint32_t value = FREG_DMA_BlockFillMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "UsbMode".
**
** This function writes the given value to the register "UsbMode".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetUsbMode( uint32_t value );
#else
INLINE void FREG_DMA_SetUsbMode( uint32_t value )
{
    FREG_DMA_UsbModeMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBMODE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBMODE)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ExternalCpEnable" of register "UsbMode".
**
** External Charge Pump Control for ULPI Interface
** - '1' : Use external charge pump (OTG Control Bit 6 = 1)
** - '0' : Use internal charge pump (OTG Control Bit 5 = 1)
** *Note: USB core must be reset when changing this bit.
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ExternalCpEnable" consists of the bits 7..7,
**       the given bitgroup value will be shifted left by 7 bits and
**       masked using the bitmask 0x00000080UL
**
********************************************************************************
*/
extern void FREG_DMA_SetUsbMode_ExternalCpEnable( uint32_t value );
#else
INLINE void FREG_DMA_SetUsbMode_ExternalCpEnable( uint32_t value )
{
    register uint32_t mask = 0x00000080UL;
    value = ( value << 7 ) & mask;
    value |= FREG_DMA_UsbModeMirror & ~mask;
    FREG_DMA_UsbModeMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBMODE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBMODE)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "PacedEpReadChannel" of register "UsbMode".
**
** Endpoint Read DMA request to use for paced DMA transfers
** - '01' ? '11' EP 1, 2, 3
** - '00' ? Disable paced transfers
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "PacedEpReadChannel" consists of the bits 6..5,
**       the given bitgroup value will be shifted left by 5 bits and
**       masked using the bitmask 0x00000060UL
**
********************************************************************************
*/
extern void FREG_DMA_SetUsbMode_PacedEpReadChannel( uint32_t value );
#else
INLINE void FREG_DMA_SetUsbMode_PacedEpReadChannel( uint32_t value )
{
    register uint32_t mask = 0x00000060UL;
    value = ( value << 5 ) & mask;
    value |= FREG_DMA_UsbModeMirror & ~mask;
    FREG_DMA_UsbModeMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBMODE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBMODE)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "PacedEpWriteChannel" of register "UsbMode".
**
** Endpoint Write DMA request to use for paced DMA transfers
** - '01' ? '11' EP 1, 2, 3
** - '00' ? Disable paced transfers
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "PacedEpWriteChannel" consists of the bits 4..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000018UL
**
********************************************************************************
*/
extern void FREG_DMA_SetUsbMode_PacedEpWriteChannel( uint32_t value );
#else
INLINE void FREG_DMA_SetUsbMode_PacedEpWriteChannel( uint32_t value )
{
    register uint32_t mask = 0x00000018UL;
    value = ( value << 3 ) & mask;
    value |= FREG_DMA_UsbModeMirror & ~mask;
    FREG_DMA_UsbModeMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBMODE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBMODE)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "LengthInput" of register "UsbMode".
**
** Length of access to USB register space
** - '00' 1 byte
** - '01' 2 bytes
** - '10' 4 bytes
** - '11' Use LS_MASK signal to determine length
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "LengthInput" consists of the bits 2..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000006UL
**
********************************************************************************
*/
extern void FREG_DMA_SetUsbMode_LengthInput( uint32_t value );
#else
INLINE void FREG_DMA_SetUsbMode_LengthInput( uint32_t value )
{
    register uint32_t mask = 0x00000006UL;
    value = ( value << 1 ) & mask;
    value |= FREG_DMA_UsbModeMirror & ~mask;
    FREG_DMA_UsbModeMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBMODE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBMODE)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "TestMode" of register "UsbMode".
**
** Enable USB Test Mode : RESERVED ? SET TO '0'
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "TestMode" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern void FREG_DMA_SetUsbMode_TestMode( uint32_t value );
#else
INLINE void FREG_DMA_SetUsbMode_TestMode( uint32_t value )
{
    register uint32_t mask = 0x00000001UL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_UsbModeMirror & ~mask;
    FREG_DMA_UsbModeMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBMODE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBMODE)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "UsbMode".
**
** This function reads the value from the register "UsbMode".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmUsbMode( void );
#else
INLINE uint32_t FREG_DMA_GetmUsbMode( void )
{
    register uint32_t value = FREG_DMA_UsbModeMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ExternalCpEnable" of register "UsbMode".
**
** External Charge Pump Control for ULPI Interface
** - '1' : Use external charge pump (OTG Control Bit 6 = 1)
** - '0' : Use internal charge pump (OTG Control Bit 5 = 1)
** *Note: USB core must be reset when changing this bit.
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ExternalCpEnable" consists of the bits 7..7,
**       the given bitgroup value will be shifted left by 7 bits and
**       masked using the bitmask 0x00000080UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmUsbMode_ExternalCpEnable( void );
#else
INLINE uint32_t FREG_DMA_GetmUsbMode_ExternalCpEnable( void )
{
    register uint32_t value = FREG_DMA_UsbModeMirror;
    value = ( value & 0x00000080UL ) >> 7;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "PacedEpReadChannel" of register "UsbMode".
**
** Endpoint Read DMA request to use for paced DMA transfers
** - '01' ? '11' EP 1, 2, 3
** - '00' ? Disable paced transfers
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "PacedEpReadChannel" consists of the bits 6..5,
**       the given bitgroup value will be shifted left by 5 bits and
**       masked using the bitmask 0x00000060UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmUsbMode_PacedEpReadChannel( void );
#else
INLINE uint32_t FREG_DMA_GetmUsbMode_PacedEpReadChannel( void )
{
    register uint32_t value = FREG_DMA_UsbModeMirror;
    value = ( value & 0x00000060UL ) >> 5;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "PacedEpWriteChannel" of register "UsbMode".
**
** Endpoint Write DMA request to use for paced DMA transfers
** - '01' ? '11' EP 1, 2, 3
** - '00' ? Disable paced transfers
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "PacedEpWriteChannel" consists of the bits 4..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000018UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmUsbMode_PacedEpWriteChannel( void );
#else
INLINE uint32_t FREG_DMA_GetmUsbMode_PacedEpWriteChannel( void )
{
    register uint32_t value = FREG_DMA_UsbModeMirror;
    value = ( value & 0x00000018UL ) >> 3;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LengthInput" of register "UsbMode".
**
** Length of access to USB register space
** - '00' 1 byte
** - '01' 2 bytes
** - '10' 4 bytes
** - '11' Use LS_MASK signal to determine length
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "LengthInput" consists of the bits 2..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000006UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmUsbMode_LengthInput( void );
#else
INLINE uint32_t FREG_DMA_GetmUsbMode_LengthInput( void )
{
    register uint32_t value = FREG_DMA_UsbModeMirror;
    value = ( value & 0x00000006UL ) >> 1;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "TestMode" of register "UsbMode".
**
** Enable USB Test Mode : RESERVED ? SET TO '0'
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "TestMode" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmUsbMode_TestMode( void );
#else
INLINE uint32_t FREG_DMA_GetmUsbMode_TestMode( void )
{
    register uint32_t value = FREG_DMA_UsbModeMirror;
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CcirConfig".
**
** This function writes the given value to the register "CcirConfig".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirConfig( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirConfig( uint32_t value )
{
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "RawEnableMode" of register "CcirConfig".
**
** Select the RAW mode enable signal in external input mode
** - '1' external CCIR H-sync is enable signal
** - '0' Ignore enable (always '1')
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "RawEnableMode" consists of the bits 7..7,
**       the given bitgroup value will be shifted left by 7 bits and
**       masked using the bitmask 0x00000080UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirConfig_RawEnableMode( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirConfig_RawEnableMode( uint32_t value )
{
    register uint32_t mask = 0x00000080UL;
    value = ( value << 7 ) & mask;
    #define REGISTER_BITGROUP_COUNT 8
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ExternalInputSelect" of register "CcirConfig".
**
** Select external CCIR input
** - '1' External (external CCIR Input)
** - '0' Internal (CCIR Output of VE)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ExternalInputSelect" consists of the bits 6..6,
**       the given bitgroup value will be shifted left by 6 bits and
**       masked using the bitmask 0x00000040UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirConfig_ExternalInputSelect( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirConfig_ExternalInputSelect( uint32_t value )
{
    register uint32_t mask = 0x00000040UL;
    value = ( value << 6 ) & mask;
    #define REGISTER_BITGROUP_COUNT 8
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "SyncStatus" of register "CcirConfig".
**
** Synchronization status of CCIR slave (read only bit)
** - '1' In sync
** - '0' Not in sync
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "SyncStatus" consists of the bits 5..5,
**       the given bitgroup value will be shifted left by 5 bits and
**       masked using the bitmask 0x00000020UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirConfig_SyncStatus( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirConfig_SyncStatus( uint32_t value )
{
    register uint32_t mask = 0x00000020UL;
    value = ( value << 5 ) & mask;
    #define REGISTER_BITGROUP_COUNT 8
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ClockInvert" of register "CcirConfig".
**
** Invert input clock
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ClockInvert" consists of the bits 4..4,
**       the given bitgroup value will be shifted left by 4 bits and
**       masked using the bitmask 0x00000010UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirConfig_ClockInvert( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirConfig_ClockInvert( uint32_t value )
{
    register uint32_t mask = 0x00000010UL;
    value = ( value << 4 ) & mask;
    #define REGISTER_BITGROUP_COUNT 8
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Enable" of register "CcirConfig".
**
** Enable capture unit
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Enable" consists of the bits 3..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000008UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirConfig_Enable( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirConfig_Enable( uint32_t value )
{
    register uint32_t mask = 0x00000008UL;
    value = ( value << 3 ) & mask;
    #define REGISTER_BITGROUP_COUNT 8
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "RawMode" of register "CcirConfig".
**
** Enable raw mode (no sync)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "RawMode" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirConfig_RawMode( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirConfig_RawMode( uint32_t value )
{
    register uint32_t mask = 0x00000004UL;
    value = ( value << 2 ) & mask;
    #define REGISTER_BITGROUP_COUNT 8
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "CaptureField1" of register "CcirConfig".
**
** Enable capture of field 1
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "CaptureField1" consists of the bits 1..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000002UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirConfig_CaptureField1( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirConfig_CaptureField1( uint32_t value )
{
    register uint32_t mask = 0x00000002UL;
    value = ( value << 1 ) & mask;
    #define REGISTER_BITGROUP_COUNT 8
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "CaptureField0" of register "CcirConfig".
**
** Enable capture of field 0
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "CaptureField0" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirConfig_CaptureField0( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirConfig_CaptureField0( uint32_t value )
{
    register uint32_t mask = 0x00000001UL;
    value = ( value << 0 ) & mask;
    #define REGISTER_BITGROUP_COUNT 8
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CcirConfig".
**
** This function reads the value from the register "CcirConfig".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirConfig( void );
#else
INLINE uint32_t FREG_DMA_GetCcirConfig( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "RawEnableMode" of register "CcirConfig".
**
** Select the RAW mode enable signal in external input mode
** - '1' external CCIR H-sync is enable signal
** - '0' Ignore enable (always '1')
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "RawEnableMode" consists of the bits 7..7,
**       the given bitgroup value will be shifted left by 7 bits and
**       masked using the bitmask 0x00000080UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirConfig_RawEnableMode( void );
#else
INLINE uint32_t FREG_DMA_GetCcirConfig_RawEnableMode( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000080UL ) >> 7;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ExternalInputSelect" of register "CcirConfig".
**
** Select external CCIR input
** - '1' External (external CCIR Input)
** - '0' Internal (CCIR Output of VE)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ExternalInputSelect" consists of the bits 6..6,
**       the given bitgroup value will be shifted left by 6 bits and
**       masked using the bitmask 0x00000040UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirConfig_ExternalInputSelect( void );
#else
INLINE uint32_t FREG_DMA_GetCcirConfig_ExternalInputSelect( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000040UL ) >> 6;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "SyncStatus" of register "CcirConfig".
**
** Synchronization status of CCIR slave (read only bit)
** - '1' In sync
** - '0' Not in sync
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "SyncStatus" consists of the bits 5..5,
**       the given bitgroup value will be shifted left by 5 bits and
**       masked using the bitmask 0x00000020UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirConfig_SyncStatus( void );
#else
INLINE uint32_t FREG_DMA_GetCcirConfig_SyncStatus( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000020UL ) >> 5;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ClockInvert" of register "CcirConfig".
**
** Invert input clock
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ClockInvert" consists of the bits 4..4,
**       the given bitgroup value will be shifted left by 4 bits and
**       masked using the bitmask 0x00000010UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirConfig_ClockInvert( void );
#else
INLINE uint32_t FREG_DMA_GetCcirConfig_ClockInvert( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000010UL ) >> 4;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Enable" of register "CcirConfig".
**
** Enable capture unit
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Enable" consists of the bits 3..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000008UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirConfig_Enable( void );
#else
INLINE uint32_t FREG_DMA_GetCcirConfig_Enable( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000008UL ) >> 3;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "RawMode" of register "CcirConfig".
**
** Enable raw mode (no sync)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "RawMode" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirConfig_RawMode( void );
#else
INLINE uint32_t FREG_DMA_GetCcirConfig_RawMode( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "CaptureField1" of register "CcirConfig".
**
** Enable capture of field 1
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "CaptureField1" consists of the bits 1..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000002UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirConfig_CaptureField1( void );
#else
INLINE uint32_t FREG_DMA_GetCcirConfig_CaptureField1( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000002UL ) >> 1;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "CaptureField0" of register "CcirConfig".
**
** Enable capture of field 0
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "CaptureField0" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirConfig_CaptureField0( void );
#else
INLINE uint32_t FREG_DMA_GetCcirConfig_CaptureField0( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CcirHconfig".
**
** This function writes the given value to the register "CcirHconfig".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirHconfig( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirHconfig( uint32_t value )
{
    FREG_DMA_CcirHconfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRHCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRHCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "HStartPosition" of register "CcirHconfig".
**
** Horizontal start position for CCIR capture (in long words after EAV code)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "HStartPosition" consists of the bits 24..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x01FF0000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirHconfig_HStartPosition( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirHconfig_HStartPosition( uint32_t value )
{
    register uint32_t mask = 0x01FF0000UL;
    value = ( value << 16 ) & mask;
    value |= FREG_DMA_CcirHconfigMirror & ~mask;
    FREG_DMA_CcirHconfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRHCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRHCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "HStopPosition" of register "CcirHconfig".
**
** Horizontal stop position for CCIR capture (in long words after EAV code).
**  This pixel is included in the captured area.
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "HStopPosition" consists of the bits 8..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000001FFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirHconfig_HStopPosition( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirHconfig_HStopPosition( uint32_t value )
{
    register uint32_t mask = 0x000001FFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CcirHconfigMirror & ~mask;
    FREG_DMA_CcirHconfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRHCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRHCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CcirHconfig".
**
** This function reads the value from the register "CcirHconfig".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirHconfig( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirHconfig( void )
{
    register uint32_t value = FREG_DMA_CcirHconfigMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "HStartPosition" of register "CcirHconfig".
**
** Horizontal start position for CCIR capture (in long words after EAV code)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "HStartPosition" consists of the bits 24..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x01FF0000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirHconfig_HStartPosition( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirHconfig_HStartPosition( void )
{
    register uint32_t value = FREG_DMA_CcirHconfigMirror;
    value = ( value & 0x01FF0000UL ) >> 16;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "HStopPosition" of register "CcirHconfig".
**
** Horizontal stop position for CCIR capture (in long words after EAV code).
**  This pixel is included in the captured area.
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "HStopPosition" consists of the bits 8..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000001FFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirHconfig_HStopPosition( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirHconfig_HStopPosition( void )
{
    register uint32_t value = FREG_DMA_CcirHconfigMirror;
    value = ( value & 0x000001FFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CcirVconfig".
**
** This function writes the given value to the register "CcirVconfig".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirVconfig( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirVconfig( uint32_t value )
{
    FREG_DMA_CcirVconfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRVCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRVCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "VStartPosition" of register "CcirVconfig".
**
** Vertical start position for CCIR capture.
**  Line numbers refer to the particular field.
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "VStartPosition" consists of the bits 24..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x01FF0000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirVconfig_VStartPosition( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirVconfig_VStartPosition( uint32_t value )
{
    register uint32_t mask = 0x01FF0000UL;
    value = ( value << 16 ) & mask;
    value |= FREG_DMA_CcirVconfigMirror & ~mask;
    FREG_DMA_CcirVconfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRVCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRVCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "VStopPosition" of register "CcirVconfig".
**
** Vertical stop position for CCIR capture.
**  This pixel is included in the captured area. Line numbers refer to the particular field.
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "VStopPosition" consists of the bits 8..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000001FFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirVconfig_VStopPosition( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirVconfig_VStopPosition( uint32_t value )
{
    register uint32_t mask = 0x000001FFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CcirVconfigMirror & ~mask;
    FREG_DMA_CcirVconfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRVCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRVCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CcirVconfig".
**
** This function reads the value from the register "CcirVconfig".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirVconfig( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirVconfig( void )
{
    register uint32_t value = FREG_DMA_CcirVconfigMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "VStartPosition" of register "CcirVconfig".
**
** Vertical start position for CCIR capture.
**  Line numbers refer to the particular field.
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "VStartPosition" consists of the bits 24..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x01FF0000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirVconfig_VStartPosition( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirVconfig_VStartPosition( void )
{
    register uint32_t value = FREG_DMA_CcirVconfigMirror;
    value = ( value & 0x01FF0000UL ) >> 16;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "VStopPosition" of register "CcirVconfig".
**
** Vertical stop position for CCIR capture.
**  This pixel is included in the captured area. Line numbers refer to the particular field.
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "VStopPosition" consists of the bits 8..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000001FFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirVconfig_VStopPosition( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirVconfig_VStopPosition( void )
{
    register uint32_t value = FREG_DMA_CcirVconfigMirror;
    value = ( value & 0x000001FFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CsConfig".
**
** This function writes the given value to the register "CsConfig".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCsConfig( uint32_t value );
#else
INLINE void FREG_DMA_SetCsConfig( uint32_t value )
{
    FREG_DMA_CsConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ColorInputMode" of register "CsConfig".
**
** Determines color mapping mode for input data (8-bits per input value, MSB indicated first)
** - '000' ARGB
** - '001' BGRA
** - '010' RGBA
** - '011' ABGR
** - '100' RGB(R2) (24-bit)
** - '101' (R2)BGR (24-bit)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ColorInputMode" consists of the bits 18..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x00070000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsConfig_ColorInputMode( uint32_t value );
#else
INLINE void FREG_DMA_SetCsConfig_ColorInputMode( uint32_t value )
{
    register uint32_t mask = 0x00070000UL;
    value = ( value << 16 ) & mask;
    value |= FREG_DMA_CsConfigMirror & ~mask;
    FREG_DMA_CsConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ManualAlphaValue" of register "CsConfig".
**
** Manual value to replace RGB alpha value with
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ManualAlphaValue" consists of the bits 15..8,
**       the given bitgroup value will be shifted left by 8 bits and
**       masked using the bitmask 0x0000FF00UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsConfig_ManualAlphaValue( uint32_t value );
#else
INLINE void FREG_DMA_SetCsConfig_ManualAlphaValue( uint32_t value )
{
    register uint32_t mask = 0x0000FF00UL;
    value = ( value << 8 ) & mask;
    value |= FREG_DMA_CsConfigMirror & ~mask;
    FREG_DMA_CsConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ManualAlphaEnable" of register "CsConfig".
**
** Alpha value replacement
** - '1' Use manual alpha for output
** - '0' Use RGB alpha for output (bits 31:24 of RGB sample)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ManualAlphaEnable" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsConfig_ManualAlphaEnable( uint32_t value );
#else
INLINE void FREG_DMA_SetCsConfig_ManualAlphaEnable( uint32_t value )
{
    register uint32_t mask = 0x00000004UL;
    value = ( value << 2 ) & mask;
    value |= FREG_DMA_CsConfigMirror & ~mask;
    FREG_DMA_CsConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ConversionMode" of register "CsConfig".
**
** Determines conversion mode
** - '1' Output 4:4:4 data samples
** - '0' Output 4:2:2 data samples
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ConversionMode" consists of the bits 1..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000002UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsConfig_ConversionMode( uint32_t value );
#else
INLINE void FREG_DMA_SetCsConfig_ConversionMode( uint32_t value )
{
    register uint32_t mask = 0x00000002UL;
    value = ( value << 1 ) & mask;
    value |= FREG_DMA_CsConfigMirror & ~mask;
    FREG_DMA_CsConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Enable" of register "CsConfig".
**
** Enable conversion unit
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Enable" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsConfig_Enable( uint32_t value );
#else
INLINE void FREG_DMA_SetCsConfig_Enable( uint32_t value )
{
    register uint32_t mask = 0x00000001UL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CsConfigMirror & ~mask;
    FREG_DMA_CsConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CsConfig".
**
** This function reads the value from the register "CsConfig".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsConfig( void );
#else
INLINE uint32_t FREG_DMA_GetmCsConfig( void )
{
    register uint32_t value = FREG_DMA_CsConfigMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ColorInputMode" of register "CsConfig".
**
** Determines color mapping mode for input data (8-bits per input value, MSB indicated first)
** - '000' ARGB
** - '001' BGRA
** - '010' RGBA
** - '011' ABGR
** - '100' RGB(R2) (24-bit)
** - '101' (R2)BGR (24-bit)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ColorInputMode" consists of the bits 18..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x00070000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsConfig_ColorInputMode( void );
#else
INLINE uint32_t FREG_DMA_GetmCsConfig_ColorInputMode( void )
{
    register uint32_t value = FREG_DMA_CsConfigMirror;
    value = ( value & 0x00070000UL ) >> 16;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ManualAlphaValue" of register "CsConfig".
**
** Manual value to replace RGB alpha value with
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ManualAlphaValue" consists of the bits 15..8,
**       the given bitgroup value will be shifted left by 8 bits and
**       masked using the bitmask 0x0000FF00UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsConfig_ManualAlphaValue( void );
#else
INLINE uint32_t FREG_DMA_GetmCsConfig_ManualAlphaValue( void )
{
    register uint32_t value = FREG_DMA_CsConfigMirror;
    value = ( value & 0x0000FF00UL ) >> 8;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ManualAlphaEnable" of register "CsConfig".
**
** Alpha value replacement
** - '1' Use manual alpha for output
** - '0' Use RGB alpha for output (bits 31:24 of RGB sample)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ManualAlphaEnable" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsConfig_ManualAlphaEnable( void );
#else
INLINE uint32_t FREG_DMA_GetmCsConfig_ManualAlphaEnable( void )
{
    register uint32_t value = FREG_DMA_CsConfigMirror;
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ConversionMode" of register "CsConfig".
**
** Determines conversion mode
** - '1' Output 4:4:4 data samples
** - '0' Output 4:2:2 data samples
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ConversionMode" consists of the bits 1..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000002UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsConfig_ConversionMode( void );
#else
INLINE uint32_t FREG_DMA_GetmCsConfig_ConversionMode( void )
{
    register uint32_t value = FREG_DMA_CsConfigMirror;
    value = ( value & 0x00000002UL ) >> 1;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Enable" of register "CsConfig".
**
** Enable conversion unit
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Enable" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsConfig_Enable( void );
#else
INLINE uint32_t FREG_DMA_GetmCsConfig_Enable( void )
{
    register uint32_t value = FREG_DMA_CsConfigMirror;
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CsY".
**
** This function writes the given value to the register "CsY".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCsY( uint32_t value );
#else
INLINE void FREG_DMA_SetCsY( uint32_t value )
{
    FREG_DMA_CsYMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSY, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSY)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "RedCoefficient" of register "CsY".
**
** Signed Red coefficient for multiplication (typical value: 0x42)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "RedCoefficient" consists of the bits 26..18,
**       the given bitgroup value will be shifted left by 18 bits and
**       masked using the bitmask 0x07FC0000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsY_RedCoefficient( uint32_t value );
#else
INLINE void FREG_DMA_SetCsY_RedCoefficient( uint32_t value )
{
    register uint32_t mask = 0x07FC0000UL;
    value = ( value << 18 ) & mask;
    value |= FREG_DMA_CsYMirror & ~mask;
    FREG_DMA_CsYMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSY, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSY)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "GreenCoefficient" of register "CsY".
**
** Signed Green coefficient for multiplication (typical value: 0x81)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "GreenCoefficient" consists of the bits 17..9,
**       the given bitgroup value will be shifted left by 9 bits and
**       masked using the bitmask 0x0003FE00UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsY_GreenCoefficient( uint32_t value );
#else
INLINE void FREG_DMA_SetCsY_GreenCoefficient( uint32_t value )
{
    register uint32_t mask = 0x0003FE00UL;
    value = ( value << 9 ) & mask;
    value |= FREG_DMA_CsYMirror & ~mask;
    FREG_DMA_CsYMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSY, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSY)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "BlueCoefficient" of register "CsY".
**
** Signed Blue coefficient for multiplication (typical value: 0x19)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "BlueCoefficient" consists of the bits 8..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000001FFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsY_BlueCoefficient( uint32_t value );
#else
INLINE void FREG_DMA_SetCsY_BlueCoefficient( uint32_t value )
{
    register uint32_t mask = 0x000001FFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CsYMirror & ~mask;
    FREG_DMA_CsYMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSY, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSY)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CsY".
**
** This function reads the value from the register "CsY".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsY( void );
#else
INLINE uint32_t FREG_DMA_GetmCsY( void )
{
    register uint32_t value = FREG_DMA_CsYMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "RedCoefficient" of register "CsY".
**
** Signed Red coefficient for multiplication (typical value: 0x42)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "RedCoefficient" consists of the bits 26..18,
**       the given bitgroup value will be shifted left by 18 bits and
**       masked using the bitmask 0x07FC0000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsY_RedCoefficient( void );
#else
INLINE uint32_t FREG_DMA_GetmCsY_RedCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsYMirror;
    value = ( value & 0x07FC0000UL ) >> 18;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "GreenCoefficient" of register "CsY".
**
** Signed Green coefficient for multiplication (typical value: 0x81)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "GreenCoefficient" consists of the bits 17..9,
**       the given bitgroup value will be shifted left by 9 bits and
**       masked using the bitmask 0x0003FE00UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsY_GreenCoefficient( void );
#else
INLINE uint32_t FREG_DMA_GetmCsY_GreenCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsYMirror;
    value = ( value & 0x0003FE00UL ) >> 9;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "BlueCoefficient" of register "CsY".
**
** Signed Blue coefficient for multiplication (typical value: 0x19)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "BlueCoefficient" consists of the bits 8..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000001FFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsY_BlueCoefficient( void );
#else
INLINE uint32_t FREG_DMA_GetmCsY_BlueCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsYMirror;
    value = ( value & 0x000001FFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CsCr".
**
** This function writes the given value to the register "CsCr".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCsCr( uint32_t value );
#else
INLINE void FREG_DMA_SetCsCr( uint32_t value )
{
    FREG_DMA_CsCrMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCR, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCR)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "RedCoefficient" of register "CsCr".
**
** Signed Red coefficient for multiplication (typical value: 0x70)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "RedCoefficient" consists of the bits 26..18,
**       the given bitgroup value will be shifted left by 18 bits and
**       masked using the bitmask 0x07FC0000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsCr_RedCoefficient( uint32_t value );
#else
INLINE void FREG_DMA_SetCsCr_RedCoefficient( uint32_t value )
{
    register uint32_t mask = 0x07FC0000UL;
    value = ( value << 18 ) & mask;
    value |= FREG_DMA_CsCrMirror & ~mask;
    FREG_DMA_CsCrMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCR, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCR)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "GreenCoefficient" of register "CsCr".
**
** Signed Green coefficient for multiplication (typical value: 0x1A2)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "GreenCoefficient" consists of the bits 17..9,
**       the given bitgroup value will be shifted left by 9 bits and
**       masked using the bitmask 0x0003FE00UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsCr_GreenCoefficient( uint32_t value );
#else
INLINE void FREG_DMA_SetCsCr_GreenCoefficient( uint32_t value )
{
    register uint32_t mask = 0x0003FE00UL;
    value = ( value << 9 ) & mask;
    value |= FREG_DMA_CsCrMirror & ~mask;
    FREG_DMA_CsCrMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCR, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCR)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "BlueCoefficient" of register "CsCr".
**
** Signed Blue coefficient for multiplication (typical value: 0x1EE)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "BlueCoefficient" consists of the bits 8..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000001FFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsCr_BlueCoefficient( uint32_t value );
#else
INLINE void FREG_DMA_SetCsCr_BlueCoefficient( uint32_t value )
{
    register uint32_t mask = 0x000001FFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CsCrMirror & ~mask;
    FREG_DMA_CsCrMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCR, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCR)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CsCr".
**
** This function reads the value from the register "CsCr".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsCr( void );
#else
INLINE uint32_t FREG_DMA_GetmCsCr( void )
{
    register uint32_t value = FREG_DMA_CsCrMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "RedCoefficient" of register "CsCr".
**
** Signed Red coefficient for multiplication (typical value: 0x70)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "RedCoefficient" consists of the bits 26..18,
**       the given bitgroup value will be shifted left by 18 bits and
**       masked using the bitmask 0x07FC0000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsCr_RedCoefficient( void );
#else
INLINE uint32_t FREG_DMA_GetmCsCr_RedCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsCrMirror;
    value = ( value & 0x07FC0000UL ) >> 18;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "GreenCoefficient" of register "CsCr".
**
** Signed Green coefficient for multiplication (typical value: 0x1A2)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "GreenCoefficient" consists of the bits 17..9,
**       the given bitgroup value will be shifted left by 9 bits and
**       masked using the bitmask 0x0003FE00UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsCr_GreenCoefficient( void );
#else
INLINE uint32_t FREG_DMA_GetmCsCr_GreenCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsCrMirror;
    value = ( value & 0x0003FE00UL ) >> 9;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "BlueCoefficient" of register "CsCr".
**
** Signed Blue coefficient for multiplication (typical value: 0x1EE)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "BlueCoefficient" consists of the bits 8..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000001FFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsCr_BlueCoefficient( void );
#else
INLINE uint32_t FREG_DMA_GetmCsCr_BlueCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsCrMirror;
    value = ( value & 0x000001FFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CsCb".
**
** This function writes the given value to the register "CsCb".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCsCb( uint32_t value );
#else
INLINE void FREG_DMA_SetCsCb( uint32_t value )
{
    FREG_DMA_CsCbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "RedCoefficient" of register "CsCb".
**
** Signed Red coefficient for multiplication (typical value: 0x1DA)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "RedCoefficient" consists of the bits 26..18,
**       the given bitgroup value will be shifted left by 18 bits and
**       masked using the bitmask 0x07FC0000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsCb_RedCoefficient( uint32_t value );
#else
INLINE void FREG_DMA_SetCsCb_RedCoefficient( uint32_t value )
{
    register uint32_t mask = 0x07FC0000UL;
    value = ( value << 18 ) & mask;
    value |= FREG_DMA_CsCbMirror & ~mask;
    FREG_DMA_CsCbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "GreenCoefficient" of register "CsCb".
**
** Signed Green coefficient for multiplication (typical value: 0x1B6)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "GreenCoefficient" consists of the bits 17..9,
**       the given bitgroup value will be shifted left by 9 bits and
**       masked using the bitmask 0x0003FE00UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsCb_GreenCoefficient( uint32_t value );
#else
INLINE void FREG_DMA_SetCsCb_GreenCoefficient( uint32_t value )
{
    register uint32_t mask = 0x0003FE00UL;
    value = ( value << 9 ) & mask;
    value |= FREG_DMA_CsCbMirror & ~mask;
    FREG_DMA_CsCbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "BlueCoefficient" of register "CsCb".
**
** Signed Blue coefficient for multiplication (typical value: 0x70)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "BlueCoefficient" consists of the bits 8..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000001FFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsCb_BlueCoefficient( uint32_t value );
#else
INLINE void FREG_DMA_SetCsCb_BlueCoefficient( uint32_t value )
{
    register uint32_t mask = 0x000001FFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CsCbMirror & ~mask;
    FREG_DMA_CsCbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CsCb".
**
** This function reads the value from the register "CsCb".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsCb( void );
#else
INLINE uint32_t FREG_DMA_GetmCsCb( void )
{
    register uint32_t value = FREG_DMA_CsCbMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "RedCoefficient" of register "CsCb".
**
** Signed Red coefficient for multiplication (typical value: 0x1DA)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "RedCoefficient" consists of the bits 26..18,
**       the given bitgroup value will be shifted left by 18 bits and
**       masked using the bitmask 0x07FC0000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsCb_RedCoefficient( void );
#else
INLINE uint32_t FREG_DMA_GetmCsCb_RedCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsCbMirror;
    value = ( value & 0x07FC0000UL ) >> 18;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "GreenCoefficient" of register "CsCb".
**
** Signed Green coefficient for multiplication (typical value: 0x1B6)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "GreenCoefficient" consists of the bits 17..9,
**       the given bitgroup value will be shifted left by 9 bits and
**       masked using the bitmask 0x0003FE00UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsCb_GreenCoefficient( void );
#else
INLINE uint32_t FREG_DMA_GetmCsCb_GreenCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsCbMirror;
    value = ( value & 0x0003FE00UL ) >> 9;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "BlueCoefficient" of register "CsCb".
**
** Signed Blue coefficient for multiplication (typical value: 0x70)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "BlueCoefficient" consists of the bits 8..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000001FFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsCb_BlueCoefficient( void );
#else
INLINE uint32_t FREG_DMA_GetmCsCb_BlueCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsCbMirror;
    value = ( value & 0x000001FFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CsOffset".
**
** This function writes the given value to the register "CsOffset".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCsOffset( uint32_t value );
#else
INLINE void FREG_DMA_SetCsOffset( uint32_t value )
{
    FREG_DMA_CsOffsetMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSOFFSET, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSOFFSET)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "YOffset" of register "CsOffset".
**
** Unsigned Offset value for Y output X4 (typical value: 0x040)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "YOffset" consists of the bits 29..20,
**       the given bitgroup value will be shifted left by 20 bits and
**       masked using the bitmask 0x3FF00000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsOffset_YOffset( uint32_t value );
#else
INLINE void FREG_DMA_SetCsOffset_YOffset( uint32_t value )
{
    register uint32_t mask = 0x3FF00000UL;
    value = ( value << 20 ) & mask;
    value |= FREG_DMA_CsOffsetMirror & ~mask;
    FREG_DMA_CsOffsetMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSOFFSET, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSOFFSET)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "CrOffset" of register "CsOffset".
**
** Unsigned Offset value for Cr output X4 (typical value: 0x200)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "CrOffset" consists of the bits 19..10,
**       the given bitgroup value will be shifted left by 10 bits and
**       masked using the bitmask 0x000FFC00UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsOffset_CrOffset( uint32_t value );
#else
INLINE void FREG_DMA_SetCsOffset_CrOffset( uint32_t value )
{
    register uint32_t mask = 0x000FFC00UL;
    value = ( value << 10 ) & mask;
    value |= FREG_DMA_CsOffsetMirror & ~mask;
    FREG_DMA_CsOffsetMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSOFFSET, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSOFFSET)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "CbOffset" of register "CsOffset".
**
** Unsigned Offset value for Cb output X4 (typical value: 0x200)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "CbOffset" consists of the bits 9..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000003FFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetCsOffset_CbOffset( uint32_t value );
#else
INLINE void FREG_DMA_SetCsOffset_CbOffset( uint32_t value )
{
    register uint32_t mask = 0x000003FFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CsOffsetMirror & ~mask;
    FREG_DMA_CsOffsetMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSOFFSET, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSOFFSET)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CsOffset".
**
** This function reads the value from the register "CsOffset".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsOffset( void );
#else
INLINE uint32_t FREG_DMA_GetmCsOffset( void )
{
    register uint32_t value = FREG_DMA_CsOffsetMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "YOffset" of register "CsOffset".
**
** Unsigned Offset value for Y output X4 (typical value: 0x040)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "YOffset" consists of the bits 29..20,
**       the given bitgroup value will be shifted left by 20 bits and
**       masked using the bitmask 0x3FF00000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsOffset_YOffset( void );
#else
INLINE uint32_t FREG_DMA_GetmCsOffset_YOffset( void )
{
    register uint32_t value = FREG_DMA_CsOffsetMirror;
    value = ( value & 0x3FF00000UL ) >> 20;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "CrOffset" of register "CsOffset".
**
** Unsigned Offset value for Cr output X4 (typical value: 0x200)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "CrOffset" consists of the bits 19..10,
**       the given bitgroup value will be shifted left by 10 bits and
**       masked using the bitmask 0x000FFC00UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsOffset_CrOffset( void );
#else
INLINE uint32_t FREG_DMA_GetmCsOffset_CrOffset( void )
{
    register uint32_t value = FREG_DMA_CsOffsetMirror;
    value = ( value & 0x000FFC00UL ) >> 10;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "CbOffset" of register "CsOffset".
**
** Unsigned Offset value for Cb output X4 (typical value: 0x200)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "CbOffset" consists of the bits 9..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000003FFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCsOffset_CbOffset( void );
#else
INLINE uint32_t FREG_DMA_GetmCsOffset_CbOffset( void )
{
    register uint32_t value = FREG_DMA_CsOffsetMirror;
    value = ( value & 0x000003FFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "SrConfig".
**
** This function writes the given value to the register "SrConfig".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetSrConfig( uint32_t value );
#else
INLINE void FREG_DMA_SetSrConfig( uint32_t value )
{
    FREG_DMA_SrConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Enable" of register "SrConfig".
**
** Enable search and replace unit
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Enable" consists of the bits 3..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000008UL
**
********************************************************************************
*/
extern void FREG_DMA_SetSrConfig_Enable( uint32_t value );
#else
INLINE void FREG_DMA_SetSrConfig_Enable( uint32_t value )
{
    register uint32_t mask = 0x00000008UL;
    value = ( value << 3 ) & mask;
    value |= FREG_DMA_SrConfigMirror & ~mask;
    FREG_DMA_SrConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ReplacementEnable" of register "SrConfig".
**
** Enable replacement on hit
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ReplacementEnable" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern void FREG_DMA_SetSrConfig_ReplacementEnable( uint32_t value );
#else
INLINE void FREG_DMA_SetSrConfig_ReplacementEnable( uint32_t value )
{
    register uint32_t mask = 0x00000004UL;
    value = ( value << 2 ) & mask;
    value |= FREG_DMA_SrConfigMirror & ~mask;
    FREG_DMA_SrConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Alignment" of register "SrConfig".
**
** Determines search alignment
** - '00' 32-bit aligned
** - '01' 16-bit aligned
** - '10' 8-bit aligned
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Alignment" consists of the bits 1..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000003UL
**
********************************************************************************
*/
extern void FREG_DMA_SetSrConfig_Alignment( uint32_t value );
#else
INLINE void FREG_DMA_SetSrConfig_Alignment( uint32_t value )
{
    register uint32_t mask = 0x00000003UL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_SrConfigMirror & ~mask;
    FREG_DMA_SrConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "SrConfig".
**
** This function reads the value from the register "SrConfig".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmSrConfig( void );
#else
INLINE uint32_t FREG_DMA_GetmSrConfig( void )
{
    register uint32_t value = FREG_DMA_SrConfigMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Enable" of register "SrConfig".
**
** Enable search and replace unit
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Enable" consists of the bits 3..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000008UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmSrConfig_Enable( void );
#else
INLINE uint32_t FREG_DMA_GetmSrConfig_Enable( void )
{
    register uint32_t value = FREG_DMA_SrConfigMirror;
    value = ( value & 0x00000008UL ) >> 3;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ReplacementEnable" of register "SrConfig".
**
** Enable replacement on hit
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ReplacementEnable" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmSrConfig_ReplacementEnable( void );
#else
INLINE uint32_t FREG_DMA_GetmSrConfig_ReplacementEnable( void )
{
    register uint32_t value = FREG_DMA_SrConfigMirror;
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Alignment" of register "SrConfig".
**
** Determines search alignment
** - '00' 32-bit aligned
** - '01' 16-bit aligned
** - '10' 8-bit aligned
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Alignment" consists of the bits 1..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000003UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmSrConfig_Alignment( void );
#else
INLINE uint32_t FREG_DMA_GetmSrConfig_Alignment( void )
{
    register uint32_t value = FREG_DMA_SrConfigMirror;
    value = ( value & 0x00000003UL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "SrAddress".
**
** This function reads the value from the register "SrAddress".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetSrAddress( void );
#else
INLINE uint32_t FREG_DMA_GetSrAddress( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_SRADDRESS));
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "HitAddress" of register "SrAddress".
**
** Last hit address (4-byte boundary! not the exact byte address!)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "HitAddress" consists of the bits 26..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x07FFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetSrAddress_HitAddress( void );
#else
INLINE uint32_t FREG_DMA_GetSrAddress_HitAddress( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_SRADDRESS));
    value = ( value & 0x07FFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "SrMask".
**
** This function writes the given value to the register "SrMask".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetSrMask( uint32_t value );
#else
INLINE void FREG_DMA_SetSrMask( uint32_t value )
{
    FREG_DMA_SrMaskMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRMASK, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRMASK)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "SearchMask" of register "SrMask".
**
** Mask value to use for search
** - '1' Compare
** - '0' Do not compare
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "SearchMask" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetSrMask_SearchMask( uint32_t value );
#else
INLINE void FREG_DMA_SetSrMask_SearchMask( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_SrMaskMirror & ~mask;
    FREG_DMA_SrMaskMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRMASK, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRMASK)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "SrMask".
**
** This function reads the value from the register "SrMask".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmSrMask( void );
#else
INLINE uint32_t FREG_DMA_GetmSrMask( void )
{
    register uint32_t value = FREG_DMA_SrMaskMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "SearchMask" of register "SrMask".
**
** Mask value to use for search
** - '1' Compare
** - '0' Do not compare
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "SearchMask" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmSrMask_SearchMask( void );
#else
INLINE uint32_t FREG_DMA_GetmSrMask_SearchMask( void )
{
    register uint32_t value = FREG_DMA_SrMaskMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "SrCompare".
**
** This function writes the given value to the register "SrCompare".
**
** Compare value for search
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetSrCompare( uint32_t value );
#else
INLINE void FREG_DMA_SetSrCompare( uint32_t value )
{
    FREG_DMA_SrCompareMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRCOMPARE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRCOMPARE)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "SearchCompareValue" of register "SrCompare".
**
** Compare value for search
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "SearchCompareValue" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetSrCompare_SearchCompareValue( uint32_t value );
#else
INLINE void FREG_DMA_SetSrCompare_SearchCompareValue( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_SrCompareMirror & ~mask;
    FREG_DMA_SrCompareMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRCOMPARE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRCOMPARE)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "SrCompare".
**
** This function reads the value from the register "SrCompare".
**
** Compare value for search
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmSrCompare( void );
#else
INLINE uint32_t FREG_DMA_GetmSrCompare( void )
{
    register uint32_t value = FREG_DMA_SrCompareMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "SearchCompareValue" of register "SrCompare".
**
** Compare value for search
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "SearchCompareValue" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmSrCompare_SearchCompareValue( void );
#else
INLINE uint32_t FREG_DMA_GetmSrCompare_SearchCompareValue( void )
{
    register uint32_t value = FREG_DMA_SrCompareMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "SrReplace".
**
** This function writes the given value to the register "SrReplace".
**
** Replacement Value
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetSrReplace( uint32_t value );
#else
INLINE void FREG_DMA_SetSrReplace( uint32_t value )
{
    FREG_DMA_SrReplaceMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRREPLACE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRREPLACE)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ReplacementValue" of register "SrReplace".
**
** Replacement Value
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ReplacementValue" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetSrReplace_ReplacementValue( uint32_t value );
#else
INLINE void FREG_DMA_SetSrReplace_ReplacementValue( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_SrReplaceMirror & ~mask;
    FREG_DMA_SrReplaceMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRREPLACE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRREPLACE)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "SrReplace".
**
** This function reads the value from the register "SrReplace".
**
** Replacement Value
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmSrReplace( void );
#else
INLINE uint32_t FREG_DMA_GetmSrReplace( void )
{
    register uint32_t value = FREG_DMA_SrReplaceMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ReplacementValue" of register "SrReplace".
**
** Replacement Value
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ReplacementValue" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmSrReplace_ReplacementValue( void );
#else
INLINE uint32_t FREG_DMA_GetmSrReplace_ReplacementValue( void )
{
    register uint32_t value = FREG_DMA_SrReplaceMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "SrReplaceMask".
**
** This function writes the given value to the register "SrReplaceMask".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetSrReplaceMask( uint32_t value );
#else
INLINE void FREG_DMA_SetSrReplaceMask( uint32_t value )
{
    FREG_DMA_SrReplaceMaskMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRREPLACEMASK, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRREPLACEMASK)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ReplacementMask" of register "SrReplaceMask".
**
** Mask value to use for replacement
** - '1' Replace
** - '0' Do not replace
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ReplacementMask" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetSrReplaceMask_ReplacementMask( uint32_t value );
#else
INLINE void FREG_DMA_SetSrReplaceMask_ReplacementMask( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_SrReplaceMaskMirror & ~mask;
    FREG_DMA_SrReplaceMaskMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRREPLACEMASK, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRREPLACEMASK)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "SrReplaceMask".
**
** This function reads the value from the register "SrReplaceMask".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmSrReplaceMask( void );
#else
INLINE uint32_t FREG_DMA_GetmSrReplaceMask( void )
{
    register uint32_t value = FREG_DMA_SrReplaceMaskMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ReplacementMask" of register "SrReplaceMask".
**
** Mask value to use for replacement
** - '1' Replace
** - '0' Do not replace
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ReplacementMask" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmSrReplaceMask_ReplacementMask( void );
#else
INLINE uint32_t FREG_DMA_GetmSrReplaceMask_ReplacementMask( void )
{
    register uint32_t value = FREG_DMA_SrReplaceMaskMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "AudioConfig".
**
** This function writes the given value to the register "AudioConfig".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetAudioConfig( uint32_t value );
#else
INLINE void FREG_DMA_SetAudioConfig( uint32_t value )
{
    FREG_DMA_AudioConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_AUDIOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_AUDIOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ExternalInputSelect" of register "AudioConfig".
**
** Select external audio input
** - '1' External (please refer to GPIO chapter)
** - '0' do not use
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ExternalInputSelect" consists of the bits 13..13,
**       the given bitgroup value will be shifted left by 13 bits and
**       masked using the bitmask 0x00002000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetAudioConfig_ExternalInputSelect( uint32_t value );
#else
INLINE void FREG_DMA_SetAudioConfig_ExternalInputSelect( uint32_t value )
{
    register uint32_t mask = 0x00002000UL;
    value = ( value << 13 ) & mask;
    value |= FREG_DMA_AudioConfigMirror & ~mask;
    FREG_DMA_AudioConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_AUDIOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_AUDIOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "EndPosition" of register "AudioConfig".
**
** Position of LSB counted in clocks after change of Left/Right (word) clock
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "EndPosition" consists of the bits 12..8,
**       the given bitgroup value will be shifted left by 8 bits and
**       masked using the bitmask 0x00001F00UL
**
********************************************************************************
*/
extern void FREG_DMA_SetAudioConfig_EndPosition( uint32_t value );
#else
INLINE void FREG_DMA_SetAudioConfig_EndPosition( uint32_t value )
{
    register uint32_t mask = 0x00001F00UL;
    value = ( value << 8 ) & mask;
    value |= FREG_DMA_AudioConfigMirror & ~mask;
    FREG_DMA_AudioConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_AUDIOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_AUDIOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "BitClockInvert" of register "AudioConfig".
**
** Invert bit clock
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "BitClockInvert" consists of the bits 7..7,
**       the given bitgroup value will be shifted left by 7 bits and
**       masked using the bitmask 0x00000080UL
**
********************************************************************************
*/
extern void FREG_DMA_SetAudioConfig_BitClockInvert( uint32_t value );
#else
INLINE void FREG_DMA_SetAudioConfig_BitClockInvert( uint32_t value )
{
    register uint32_t mask = 0x00000080UL;
    value = ( value << 7 ) & mask;
    value |= FREG_DMA_AudioConfigMirror & ~mask;
    FREG_DMA_AudioConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_AUDIOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_AUDIOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "WordClockInvert" of register "AudioConfig".
**
** Invert word clock
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "WordClockInvert" consists of the bits 6..6,
**       the given bitgroup value will be shifted left by 6 bits and
**       masked using the bitmask 0x00000040UL
**
********************************************************************************
*/
extern void FREG_DMA_SetAudioConfig_WordClockInvert( uint32_t value );
#else
INLINE void FREG_DMA_SetAudioConfig_WordClockInvert( uint32_t value )
{
    register uint32_t mask = 0x00000040UL;
    value = ( value << 6 ) & mask;
    value |= FREG_DMA_AudioConfigMirror & ~mask;
    FREG_DMA_AudioConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_AUDIOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_AUDIOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "DataInvert" of register "AudioConfig".
**
** Invert data
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "DataInvert" consists of the bits 5..5,
**       the given bitgroup value will be shifted left by 5 bits and
**       masked using the bitmask 0x00000020UL
**
********************************************************************************
*/
extern void FREG_DMA_SetAudioConfig_DataInvert( uint32_t value );
#else
INLINE void FREG_DMA_SetAudioConfig_DataInvert( uint32_t value )
{
    register uint32_t mask = 0x00000020UL;
    value = ( value << 5 ) & mask;
    value |= FREG_DMA_AudioConfigMirror & ~mask;
    FREG_DMA_AudioConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_AUDIOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_AUDIOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Enable" of register "AudioConfig".
**
** Enable capture unit
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Enable" consists of the bits 4..4,
**       the given bitgroup value will be shifted left by 4 bits and
**       masked using the bitmask 0x00000010UL
**
********************************************************************************
*/
extern void FREG_DMA_SetAudioConfig_Enable( uint32_t value );
#else
INLINE void FREG_DMA_SetAudioConfig_Enable( uint32_t value )
{
    register uint32_t mask = 0x00000010UL;
    value = ( value << 4 ) & mask;
    value |= FREG_DMA_AudioConfigMirror & ~mask;
    FREG_DMA_AudioConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_AUDIOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_AUDIOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "LeftChannelEnable" of register "AudioConfig".
**
** Enable capture of left channel
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "LeftChannelEnable" consists of the bits 3..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000008UL
**
********************************************************************************
*/
extern void FREG_DMA_SetAudioConfig_LeftChannelEnable( uint32_t value );
#else
INLINE void FREG_DMA_SetAudioConfig_LeftChannelEnable( uint32_t value )
{
    register uint32_t mask = 0x00000008UL;
    value = ( value << 3 ) & mask;
    value |= FREG_DMA_AudioConfigMirror & ~mask;
    FREG_DMA_AudioConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_AUDIOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_AUDIOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "RightChannelEnable" of register "AudioConfig".
**
** Enable capture of right channel
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "RightChannelEnable" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern void FREG_DMA_SetAudioConfig_RightChannelEnable( uint32_t value );
#else
INLINE void FREG_DMA_SetAudioConfig_RightChannelEnable( uint32_t value )
{
    register uint32_t mask = 0x00000004UL;
    value = ( value << 2 ) & mask;
    value |= FREG_DMA_AudioConfigMirror & ~mask;
    FREG_DMA_AudioConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_AUDIOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_AUDIOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "NumberOfBits" of register "AudioConfig".
**
** Number of bits to capture
** - '1' do not use
** - '0' 16-bits
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "NumberOfBits" consists of the bits 1..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000002UL
**
********************************************************************************
*/
extern void FREG_DMA_SetAudioConfig_NumberOfBits( uint32_t value );
#else
INLINE void FREG_DMA_SetAudioConfig_NumberOfBits( uint32_t value )
{
    register uint32_t mask = 0x00000002UL;
    value = ( value << 1 ) & mask;
    value |= FREG_DMA_AudioConfigMirror & ~mask;
    FREG_DMA_AudioConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_AUDIOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_AUDIOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "MsbSwitch" of register "AudioConfig".
**
** Reverse MSB position
** - '0' MSB first
** - '1' LSB first
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "MsbSwitch" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern void FREG_DMA_SetAudioConfig_MsbSwitch( uint32_t value );
#else
INLINE void FREG_DMA_SetAudioConfig_MsbSwitch( uint32_t value )
{
    register uint32_t mask = 0x00000001UL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_AudioConfigMirror & ~mask;
    FREG_DMA_AudioConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_AUDIOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_AUDIOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "AudioConfig".
**
** This function reads the value from the register "AudioConfig".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmAudioConfig( void );
#else
INLINE uint32_t FREG_DMA_GetmAudioConfig( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ExternalInputSelect" of register "AudioConfig".
**
** Select external audio input
** - '1' External (please refer to GPIO chapter)
** - '0' do not use
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ExternalInputSelect" consists of the bits 13..13,
**       the given bitgroup value will be shifted left by 13 bits and
**       masked using the bitmask 0x00002000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmAudioConfig_ExternalInputSelect( void );
#else
INLINE uint32_t FREG_DMA_GetmAudioConfig_ExternalInputSelect( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00002000UL ) >> 13;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "EndPosition" of register "AudioConfig".
**
** Position of LSB counted in clocks after change of Left/Right (word) clock
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "EndPosition" consists of the bits 12..8,
**       the given bitgroup value will be shifted left by 8 bits and
**       masked using the bitmask 0x00001F00UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmAudioConfig_EndPosition( void );
#else
INLINE uint32_t FREG_DMA_GetmAudioConfig_EndPosition( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00001F00UL ) >> 8;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "BitClockInvert" of register "AudioConfig".
**
** Invert bit clock
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "BitClockInvert" consists of the bits 7..7,
**       the given bitgroup value will be shifted left by 7 bits and
**       masked using the bitmask 0x00000080UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmAudioConfig_BitClockInvert( void );
#else
INLINE uint32_t FREG_DMA_GetmAudioConfig_BitClockInvert( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000080UL ) >> 7;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "WordClockInvert" of register "AudioConfig".
**
** Invert word clock
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "WordClockInvert" consists of the bits 6..6,
**       the given bitgroup value will be shifted left by 6 bits and
**       masked using the bitmask 0x00000040UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmAudioConfig_WordClockInvert( void );
#else
INLINE uint32_t FREG_DMA_GetmAudioConfig_WordClockInvert( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000040UL ) >> 6;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "DataInvert" of register "AudioConfig".
**
** Invert data
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "DataInvert" consists of the bits 5..5,
**       the given bitgroup value will be shifted left by 5 bits and
**       masked using the bitmask 0x00000020UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmAudioConfig_DataInvert( void );
#else
INLINE uint32_t FREG_DMA_GetmAudioConfig_DataInvert( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000020UL ) >> 5;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Enable" of register "AudioConfig".
**
** Enable capture unit
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Enable" consists of the bits 4..4,
**       the given bitgroup value will be shifted left by 4 bits and
**       masked using the bitmask 0x00000010UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmAudioConfig_Enable( void );
#else
INLINE uint32_t FREG_DMA_GetmAudioConfig_Enable( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000010UL ) >> 4;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LeftChannelEnable" of register "AudioConfig".
**
** Enable capture of left channel
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "LeftChannelEnable" consists of the bits 3..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000008UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmAudioConfig_LeftChannelEnable( void );
#else
INLINE uint32_t FREG_DMA_GetmAudioConfig_LeftChannelEnable( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000008UL ) >> 3;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "RightChannelEnable" of register "AudioConfig".
**
** Enable capture of right channel
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "RightChannelEnable" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmAudioConfig_RightChannelEnable( void );
#else
INLINE uint32_t FREG_DMA_GetmAudioConfig_RightChannelEnable( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "NumberOfBits" of register "AudioConfig".
**
** Number of bits to capture
** - '1' do not use
** - '0' 16-bits
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "NumberOfBits" consists of the bits 1..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000002UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmAudioConfig_NumberOfBits( void );
#else
INLINE uint32_t FREG_DMA_GetmAudioConfig_NumberOfBits( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000002UL ) >> 1;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "MsbSwitch" of register "AudioConfig".
**
** Reverse MSB position
** - '0' MSB first
** - '1' LSB first
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "MsbSwitch" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmAudioConfig_MsbSwitch( void );
#else
INLINE uint32_t FREG_DMA_GetmAudioConfig_MsbSwitch( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CryptoConfig".
**
** This function writes the given value to the register "CryptoConfig".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoConfig( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoConfig( uint32_t value )
{
    FREG_DMA_CryptoConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "GenerateSessionKey" of register "CryptoConfig".
**
** Generate session key (output data to memory will be invalid),
** 'Key Selection' must be '10' to succeed
** - '0' Do not generate session key
** - '1' Generate session key
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "GenerateSessionKey" consists of the bits 5..5,
**       the given bitgroup value will be shifted left by 5 bits and
**       masked using the bitmask 0x00000020UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoConfig_GenerateSessionKey( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoConfig_GenerateSessionKey( uint32_t value )
{
    register uint32_t mask = 0x00000020UL;
    value = ( value << 5 ) & mask;
    value |= FREG_DMA_CryptoConfigMirror & ~mask;
    FREG_DMA_CryptoConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "KeySelection" of register "CryptoConfig".
**
** Select which key set to use
** - '0' Key from DMA_CRYPTO_K registers
** - '1' Session key
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "KeySelection" consists of the bits 4..4,
**       the given bitgroup value will be shifted left by 4 bits and
**       masked using the bitmask 0x00000010UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoConfig_KeySelection( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoConfig_KeySelection( uint32_t value )
{
    register uint32_t mask = 0x00000010UL;
    value = ( value << 4 ) & mask;
    value |= FREG_DMA_CryptoConfigMirror & ~mask;
    FREG_DMA_CryptoConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "EndianSwap" of register "CryptoConfig".
**
** Control endian type of crypto unit
** - '0' Big Endian
** - '1' Little Endian
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "EndianSwap" consists of the bits 3..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000008UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoConfig_EndianSwap( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoConfig_EndianSwap( uint32_t value )
{
    register uint32_t mask = 0x00000008UL;
    value = ( value << 3 ) & mask;
    value |= FREG_DMA_CryptoConfigMirror & ~mask;
    FREG_DMA_CryptoConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Mode" of register "CryptoConfig".
**
** TDES Mode
** - '0' ECB Mode (no IV)
** - '1' CBC Mode
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Mode" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoConfig_Mode( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoConfig_Mode( uint32_t value )
{
    register uint32_t mask = 0x00000004UL;
    value = ( value << 2 ) & mask;
    value |= FREG_DMA_CryptoConfigMirror & ~mask;
    FREG_DMA_CryptoConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Enable" of register "CryptoConfig".
**
** Enable encryption / decryption unit
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Enable" consists of the bits 1..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000002UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoConfig_Enable( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoConfig_Enable( uint32_t value )
{
    register uint32_t mask = 0x00000002UL;
    value = ( value << 1 ) & mask;
    value |= FREG_DMA_CryptoConfigMirror & ~mask;
    FREG_DMA_CryptoConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Decrypt" of register "CryptoConfig".
**
** Determines operation
** - '1' Decrypt
** - '0' Encrypt
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Decrypt" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoConfig_Decrypt( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoConfig_Decrypt( uint32_t value )
{
    register uint32_t mask = 0x00000001UL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CryptoConfigMirror & ~mask;
    FREG_DMA_CryptoConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CryptoConfig".
**
** This function reads the value from the register "CryptoConfig".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoConfig( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoConfig( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "GenerateSessionKey" of register "CryptoConfig".
**
** Generate session key (output data to memory will be invalid),
** 'Key Selection' must be '10' to succeed
** - '0' Do not generate session key
** - '1' Generate session key
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "GenerateSessionKey" consists of the bits 5..5,
**       the given bitgroup value will be shifted left by 5 bits and
**       masked using the bitmask 0x00000020UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoConfig_GenerateSessionKey( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoConfig_GenerateSessionKey( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    value = ( value & 0x00000020UL ) >> 5;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "KeySelection" of register "CryptoConfig".
**
** Select which key set to use
** - '0' Key from DMA_CRYPTO_K registers
** - '1' Session key
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "KeySelection" consists of the bits 4..4,
**       the given bitgroup value will be shifted left by 4 bits and
**       masked using the bitmask 0x00000010UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoConfig_KeySelection( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoConfig_KeySelection( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    value = ( value & 0x00000010UL ) >> 4;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "EndianSwap" of register "CryptoConfig".
**
** Control endian type of crypto unit
** - '0' Big Endian
** - '1' Little Endian
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "EndianSwap" consists of the bits 3..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000008UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoConfig_EndianSwap( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoConfig_EndianSwap( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    value = ( value & 0x00000008UL ) >> 3;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Mode" of register "CryptoConfig".
**
** TDES Mode
** - '0' ECB Mode (no IV)
** - '1' CBC Mode
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Mode" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoConfig_Mode( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoConfig_Mode( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Enable" of register "CryptoConfig".
**
** Enable encryption / decryption unit
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Enable" consists of the bits 1..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000002UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoConfig_Enable( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoConfig_Enable( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    value = ( value & 0x00000002UL ) >> 1;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Decrypt" of register "CryptoConfig".
**
** Determines operation
** - '1' Decrypt
** - '0' Encrypt
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Decrypt" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoConfig_Decrypt( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoConfig_Decrypt( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CryptoK1Msb".
**
** This function writes the given value to the register "CryptoK1Msb".
**
** Key 1 55:28 (31:25)(23:17)(15:9)(7:1)
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoK1Msb( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoK1Msb( uint32_t value )
{
    FREG_DMA_CryptoK1MsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK1MSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK1MSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Key1Msb" of register "CryptoK1Msb".
**
** Key 1 55:28 (31:25)(23:17)(15:9)(7:1)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Key1Msb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoK1Msb_Key1Msb( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoK1Msb_Key1Msb( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CryptoK1MsbMirror & ~mask;
    FREG_DMA_CryptoK1MsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK1MSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK1MSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CryptoK1Msb".
**
** This function reads the value from the register "CryptoK1Msb".
**
** Key 1 55:28 (31:25)(23:17)(15:9)(7:1)
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoK1Msb( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoK1Msb( void )
{
    register uint32_t value = FREG_DMA_CryptoK1MsbMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Key1Msb" of register "CryptoK1Msb".
**
** Key 1 55:28 (31:25)(23:17)(15:9)(7:1)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Key1Msb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoK1Msb_Key1Msb( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoK1Msb_Key1Msb( void )
{
    register uint32_t value = FREG_DMA_CryptoK1MsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CryptoK1Lsb".
**
** This function writes the given value to the register "CryptoK1Lsb".
**
** Key 1 27:0 (31:25)(23:17)(15:9)(7:1)
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoK1Lsb( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoK1Lsb( uint32_t value )
{
    FREG_DMA_CryptoK1LsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK1LSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK1LSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Key1Lsb" of register "CryptoK1Lsb".
**
** Key 1 27:0 (31:25)(23:17)(15:9)(7:1)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Key1Lsb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoK1Lsb_Key1Lsb( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoK1Lsb_Key1Lsb( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CryptoK1LsbMirror & ~mask;
    FREG_DMA_CryptoK1LsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK1LSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK1LSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CryptoK1Lsb".
**
** This function reads the value from the register "CryptoK1Lsb".
**
** Key 1 27:0 (31:25)(23:17)(15:9)(7:1)
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoK1Lsb( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoK1Lsb( void )
{
    register uint32_t value = FREG_DMA_CryptoK1LsbMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Key1Lsb" of register "CryptoK1Lsb".
**
** Key 1 27:0 (31:25)(23:17)(15:9)(7:1)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Key1Lsb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoK1Lsb_Key1Lsb( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoK1Lsb_Key1Lsb( void )
{
    register uint32_t value = FREG_DMA_CryptoK1LsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CryptoK2Msb".
**
** This function writes the given value to the register "CryptoK2Msb".
**
** Key 2 55:28 (31:25)(23:17)(15:9)(7:1)
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoK2Msb( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoK2Msb( uint32_t value )
{
    FREG_DMA_CryptoK2MsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK2MSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK2MSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Key2Msb" of register "CryptoK2Msb".
**
** Key 2 55:28 (31:25)(23:17)(15:9)(7:1)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Key2Msb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoK2Msb_Key2Msb( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoK2Msb_Key2Msb( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CryptoK2MsbMirror & ~mask;
    FREG_DMA_CryptoK2MsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK2MSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK2MSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CryptoK2Msb".
**
** This function reads the value from the register "CryptoK2Msb".
**
** Key 2 55:28 (31:25)(23:17)(15:9)(7:1)
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoK2Msb( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoK2Msb( void )
{
    register uint32_t value = FREG_DMA_CryptoK2MsbMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Key2Msb" of register "CryptoK2Msb".
**
** Key 2 55:28 (31:25)(23:17)(15:9)(7:1)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Key2Msb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoK2Msb_Key2Msb( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoK2Msb_Key2Msb( void )
{
    register uint32_t value = FREG_DMA_CryptoK2MsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CryptoK2Lsb".
**
** This function writes the given value to the register "CryptoK2Lsb".
**
** Key 2 27:0 (31:25)(23:17)(15:9)(7:1)
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoK2Lsb( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoK2Lsb( uint32_t value )
{
    FREG_DMA_CryptoK2LsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK2LSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK2LSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Key2Lsb" of register "CryptoK2Lsb".
**
** Key 2 27:0 (31:25)(23:17)(15:9)(7:1)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Key2Lsb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoK2Lsb_Key2Lsb( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoK2Lsb_Key2Lsb( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CryptoK2LsbMirror & ~mask;
    FREG_DMA_CryptoK2LsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK2LSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK2LSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CryptoK2Lsb".
**
** This function reads the value from the register "CryptoK2Lsb".
**
** Key 2 27:0 (31:25)(23:17)(15:9)(7:1)
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoK2Lsb( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoK2Lsb( void )
{
    register uint32_t value = FREG_DMA_CryptoK2LsbMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Key2Lsb" of register "CryptoK2Lsb".
**
** Key 2 27:0 (31:25)(23:17)(15:9)(7:1)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Key2Lsb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoK2Lsb_Key2Lsb( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoK2Lsb_Key2Lsb( void )
{
    register uint32_t value = FREG_DMA_CryptoK2LsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CryptoK3Msb".
**
** This function writes the given value to the register "CryptoK3Msb".
**
** Key 3 55:28 (31:25)(23:17)(15:9)(7:1)
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoK3Msb( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoK3Msb( uint32_t value )
{
    FREG_DMA_CryptoK3MsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK3MSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK3MSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Key3Msb" of register "CryptoK3Msb".
**
** Key 3 55:28 (31:25)(23:17)(15:9)(7:1)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Key3Msb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoK3Msb_Key3Msb( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoK3Msb_Key3Msb( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CryptoK3MsbMirror & ~mask;
    FREG_DMA_CryptoK3MsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK3MSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK3MSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CryptoK3Msb".
**
** This function reads the value from the register "CryptoK3Msb".
**
** Key 3 55:28 (31:25)(23:17)(15:9)(7:1)
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoK3Msb( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoK3Msb( void )
{
    register uint32_t value = FREG_DMA_CryptoK3MsbMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Key3Msb" of register "CryptoK3Msb".
**
** Key 3 55:28 (31:25)(23:17)(15:9)(7:1)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Key3Msb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoK3Msb_Key3Msb( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoK3Msb_Key3Msb( void )
{
    register uint32_t value = FREG_DMA_CryptoK3MsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CryptoK3Lsb".
**
** This function writes the given value to the register "CryptoK3Lsb".
**
** Key 3 27:0 (31:25)(23:17)(15:9)(7:1)
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoK3Lsb( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoK3Lsb( uint32_t value )
{
    FREG_DMA_CryptoK3LsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK3LSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK3LSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Key3Lsb" of register "CryptoK3Lsb".
**
** Key 3 27:0 (31:25)(23:17)(15:9)(7:1)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Key3Lsb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetCryptoK3Lsb_Key3Lsb( uint32_t value );
#else
INLINE void FREG_DMA_SetCryptoK3Lsb_Key3Lsb( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CryptoK3LsbMirror & ~mask;
    FREG_DMA_CryptoK3LsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK3LSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK3LSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CryptoK3Lsb".
**
** This function reads the value from the register "CryptoK3Lsb".
**
** Key 3 27:0 (31:25)(23:17)(15:9)(7:1)
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoK3Lsb( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoK3Lsb( void )
{
    register uint32_t value = FREG_DMA_CryptoK3LsbMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Key3Lsb" of register "CryptoK3Lsb".
**
** Key 3 27:0 (31:25)(23:17)(15:9)(7:1)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Key3Lsb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCryptoK3Lsb_Key3Lsb( void );
#else
INLINE uint32_t FREG_DMA_GetmCryptoK3Lsb_Key3Lsb( void )
{
    register uint32_t value = FREG_DMA_CryptoK3LsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "IvMsb".
**
** This function writes the given value to the register "IvMsb".
**
** Initialization vector 63:32
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetIvMsb( uint32_t value );
#else
INLINE void FREG_DMA_SetIvMsb( uint32_t value )
{
    FREG_DMA_IvMsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_IVMSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_IVMSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "IvMsb" of register "IvMsb".
**
** Initialization vector 63:32
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "IvMsb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetIvMsb_IvMsb( uint32_t value );
#else
INLINE void FREG_DMA_SetIvMsb_IvMsb( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_IvMsbMirror & ~mask;
    FREG_DMA_IvMsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_IVMSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_IVMSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "IvMsb".
**
** This function reads the value from the register "IvMsb".
**
** Initialization vector 63:32
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmIvMsb( void );
#else
INLINE uint32_t FREG_DMA_GetmIvMsb( void )
{
    register uint32_t value = FREG_DMA_IvMsbMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "IvMsb" of register "IvMsb".
**
** Initialization vector 63:32
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "IvMsb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmIvMsb_IvMsb( void );
#else
INLINE uint32_t FREG_DMA_GetmIvMsb_IvMsb( void )
{
    register uint32_t value = FREG_DMA_IvMsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "IvLsb".
**
** This function writes the given value to the register "IvLsb".
**
** Initialization vector 31:0
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetIvLsb( uint32_t value );
#else
INLINE void FREG_DMA_SetIvLsb( uint32_t value )
{
    FREG_DMA_IvLsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_IVLSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_IVLSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "IvLsb" of register "IvLsb".
**
** Initialization vector 31:0
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "IvLsb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetIvLsb_IvLsb( uint32_t value );
#else
INLINE void FREG_DMA_SetIvLsb_IvLsb( uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_IvLsbMirror & ~mask;
    FREG_DMA_IvLsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_IVLSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_IVLSB)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "IvLsb".
**
** This function reads the value from the register "IvLsb".
**
** Initialization vector 31:0
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmIvLsb( void );
#else
INLINE uint32_t FREG_DMA_GetmIvLsb( void )
{
    register uint32_t value = FREG_DMA_IvLsbMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "IvLsb" of register "IvLsb".
**
** Initialization vector 31:0
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "IvLsb" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmIvLsb_IvLsb( void );
#else
INLINE uint32_t FREG_DMA_GetmIvLsb_IvLsb( void )
{
    register uint32_t value = FREG_DMA_IvLsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "ChConfig".
**
** This function writes the given value to the index-th entry of the register
** array ChConfig.
**
** \param index Array index to access.
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetChConfig( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChConfig( uint32_t index, uint32_t value )
{
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHCONFIG+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "EndianSwapWrite" of register "ChConfig".
**
** Enable endian swap for write channel
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "EndianSwapWrite" consists of the bits 20..20,
**       the given bitgroup value will be shifted left by 20 bits and
**       masked using the bitmask 0x00100000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChConfig_EndianSwapWrite( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChConfig_EndianSwapWrite( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x00100000UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 20 ) & mask;
    #define REGISTER_BITGROUP_COUNT 9
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHCONFIG+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "PeripheralAddressWrite" of register "ChConfig".
**
** Determines which memory or peripheral is accessed
** - 0x0 DRAM / Register Space
** - 0x1 DRAM / Register Space
** - 0x2 CCIR-656 Capture Unit
** - 0x3 Color Conversion Unit
** - 0x4 Search and Replace Unit
** - 0x5 Audio Capture
** - 0x6 Crypto Unit
** - 0x7 USB DMA Channel
** - 0x8 ATA DMA Channel
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "PeripheralAddressWrite" consists of the bits 19..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x000F0000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChConfig_PeripheralAddressWrite( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChConfig_PeripheralAddressWrite( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x000F0000UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 16 ) & mask;
    #define REGISTER_BITGROUP_COUNT 9
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHCONFIG+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "EndianSwapRead" of register "ChConfig".
**
** Enable endian swap for read channel
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "EndianSwapRead" consists of the bits 12..12,
**       the given bitgroup value will be shifted left by 12 bits and
**       masked using the bitmask 0x00001000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChConfig_EndianSwapRead( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChConfig_EndianSwapRead( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x00001000UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 12 ) & mask;
    #define REGISTER_BITGROUP_COUNT 9
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHCONFIG+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "PeripheralAddressRead" of register "ChConfig".
**
** Determines which memory or peripheral is accessed
** - 0x0 DRAM / Register Space
** - 0x1 DRAM / Register Space
** - 0x2 CCIR-656 Capture Unit
** - 0x3 Color Conversion Unit
** - 0x4 Search and Replace Unit
** - 0x5 Audio Capture
** - 0x6 Crypto Unit
** - 0x7 USB DMA Channel
** - 0x8 ATA DMA Channel
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "PeripheralAddressRead" consists of the bits 11..8,
**       the given bitgroup value will be shifted left by 8 bits and
**       masked using the bitmask 0x00000F00UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChConfig_PeripheralAddressRead( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChConfig_PeripheralAddressRead( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x00000F00UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 8 ) & mask;
    #define REGISTER_BITGROUP_COUNT 9
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHCONFIG+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Done" of register "ChConfig".
**
** Channel has completed request (read only)
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Done" consists of the bits 5..5,
**       the given bitgroup value will be shifted left by 5 bits and
**       masked using the bitmask 0x00000020UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChConfig_Done( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChConfig_Done( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x00000020UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 5 ) & mask;
    #define REGISTER_BITGROUP_COUNT 9
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHCONFIG+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Active" of register "ChConfig".
**
** Channel is currently active.
**  Indicates that channel is currently being serviced by time-multiplexed controller, bit toggles during operation (read only).
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Active" consists of the bits 4..4,
**       the given bitgroup value will be shifted left by 4 bits and
**       masked using the bitmask 0x00000010UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChConfig_Active( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChConfig_Active( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x00000010UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 4 ) & mask;
    #define REGISTER_BITGROUP_COUNT 9
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHCONFIG+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "LinkedListEnable" of register "ChConfig".
**
** Enable linked list operation (Channel 0 only and write only)
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "LinkedListEnable" consists of the bits 3..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000008UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChConfig_LinkedListEnable( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChConfig_LinkedListEnable( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x00000008UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 3 ) & mask;
    #define REGISTER_BITGROUP_COUNT 9
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHCONFIG+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Enable" of register "ChConfig".
**
** Enable channel
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Enable" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChConfig_Enable( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChConfig_Enable( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x00000004UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 2 ) & mask;
    #define REGISTER_BITGROUP_COUNT 9
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHCONFIG+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "AutoRestartEnable" of register "ChConfig".
**
** Enable auto restart at completed (Channel 0 only)
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "AutoRestartEnable" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChConfig_AutoRestartEnable( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChConfig_AutoRestartEnable( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x00000001UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 0 ) & mask;
    #define REGISTER_BITGROUP_COUNT 9
    #if REGISTER_BITGROUP_COUNT > 1
        value |= ( *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) & ~mask );
    #endif
    #undef REGISTER_BITGROUP_COUNT
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHCONFIG+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "ChConfig".
**
** This function reads the value from the index-th entry of the register
** array ChConfig.
**
** 
**
** \param index Array index to access.
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetChConfig( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetChConfig( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40)));
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "EndianSwapWrite" of register "ChConfig".
**
** Enable endian swap for write channel
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "EndianSwapWrite" consists of the bits 20..20,
**       the given bitgroup value will be shifted left by 20 bits and
**       masked using the bitmask 0x00100000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetChConfig_EndianSwapWrite( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetChConfig_EndianSwapWrite( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40)));
    value = ( value & 0x00100000UL ) >> 20;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "PeripheralAddressWrite" of register "ChConfig".
**
** Determines which memory or peripheral is accessed
** - 0x0 DRAM / Register Space
** - 0x1 DRAM / Register Space
** - 0x2 CCIR-656 Capture Unit
** - 0x3 Color Conversion Unit
** - 0x4 Search and Replace Unit
** - 0x5 Audio Capture
** - 0x6 Crypto Unit
** - 0x7 USB DMA Channel
** - 0x8 ATA DMA Channel
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "PeripheralAddressWrite" consists of the bits 19..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x000F0000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetChConfig_PeripheralAddressWrite( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetChConfig_PeripheralAddressWrite( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40)));
    value = ( value & 0x000F0000UL ) >> 16;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "EndianSwapRead" of register "ChConfig".
**
** Enable endian swap for read channel
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "EndianSwapRead" consists of the bits 12..12,
**       the given bitgroup value will be shifted left by 12 bits and
**       masked using the bitmask 0x00001000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetChConfig_EndianSwapRead( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetChConfig_EndianSwapRead( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40)));
    value = ( value & 0x00001000UL ) >> 12;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "PeripheralAddressRead" of register "ChConfig".
**
** Determines which memory or peripheral is accessed
** - 0x0 DRAM / Register Space
** - 0x1 DRAM / Register Space
** - 0x2 CCIR-656 Capture Unit
** - 0x3 Color Conversion Unit
** - 0x4 Search and Replace Unit
** - 0x5 Audio Capture
** - 0x6 Crypto Unit
** - 0x7 USB DMA Channel
** - 0x8 ATA DMA Channel
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "PeripheralAddressRead" consists of the bits 11..8,
**       the given bitgroup value will be shifted left by 8 bits and
**       masked using the bitmask 0x00000F00UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetChConfig_PeripheralAddressRead( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetChConfig_PeripheralAddressRead( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40)));
    value = ( value & 0x00000F00UL ) >> 8;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Done" of register "ChConfig".
**
** Channel has completed request (read only)
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "Done" consists of the bits 5..5,
**       the given bitgroup value will be shifted left by 5 bits and
**       masked using the bitmask 0x00000020UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetChConfig_Done( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetChConfig_Done( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40)));
    value = ( value & 0x00000020UL ) >> 5;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Active" of register "ChConfig".
**
** Channel is currently active.
**  Indicates that channel is currently being serviced by time-multiplexed controller, bit toggles during operation (read only).
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "Active" consists of the bits 4..4,
**       the given bitgroup value will be shifted left by 4 bits and
**       masked using the bitmask 0x00000010UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetChConfig_Active( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetChConfig_Active( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40)));
    value = ( value & 0x00000010UL ) >> 4;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LinkedListEnable" of register "ChConfig".
**
** Enable linked list operation (Channel 0 only and write only)
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "LinkedListEnable" consists of the bits 3..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000008UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetChConfig_LinkedListEnable( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetChConfig_LinkedListEnable( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40)));
    value = ( value & 0x00000008UL ) >> 3;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Enable" of register "ChConfig".
**
** Enable channel
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "Enable" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetChConfig_Enable( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetChConfig_Enable( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40)));
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "AutoRestartEnable" of register "ChConfig".
**
** Enable auto restart at completed (Channel 0 only)
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "AutoRestartEnable" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetChConfig_AutoRestartEnable( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetChConfig_AutoRestartEnable( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40)));
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "ChLength".
**
** This function writes the given value to the index-th entry of the register
** array ChLength.
**
** \param index Array index to access.
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetChLength( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChLength( uint32_t index, uint32_t value )
{
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    FREG_DMA_ChLengthMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHLENGTH+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHLENGTH+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Length" of register "ChLength".
**
** Transfer length in bytes
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Length" consists of the bits 20..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x001FFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetChLength_Length( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChLength_Length( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x001FFFFFUL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_ChLengthMirror[index] & ~mask;
    FREG_DMA_ChLengthMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHLENGTH+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHLENGTH+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "ChLength".
**
** This function reads the value from the index-th entry of the register
** array ChLength.
**
** 
**
** \param index Array index to access.
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChLength( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChLength( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChLengthMirror[index];
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Length" of register "ChLength".
**
** Transfer length in bytes
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "Length" consists of the bits 20..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x001FFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChLength_Length( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChLength_Length( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChLengthMirror[index];
    value = ( value & 0x001FFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "ChLladdr".
**
** This function writes the given value to the index-th entry of the register
** array ChLladdr.
**
** \param index Array index to access.
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetChLladdr( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChLladdr( uint32_t index, uint32_t value )
{
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    FREG_DMA_ChLladdrMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHLLADDR+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHLLADDR+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "LinkedListAddress" of register "ChLladdr".
**
** Address to fetch next command in linked list mode (Channel 0 only)
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "LinkedListAddress" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetChLladdr_LinkedListAddress( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChLladdr_LinkedListAddress( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_ChLladdrMirror[index] & ~mask;
    FREG_DMA_ChLladdrMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHLLADDR+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHLLADDR+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "ChLladdr".
**
** This function reads the value from the index-th entry of the register
** array ChLladdr.
**
** Address to fetch next command in linked list mode (Channel 0 only)
**
** \param index Array index to access.
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChLladdr( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChLladdr( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChLladdrMirror[index];
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LinkedListAddress" of register "ChLladdr".
**
** Address to fetch next command in linked list mode (Channel 0 only)
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "LinkedListAddress" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChLladdr_LinkedListAddress( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChLladdr_LinkedListAddress( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChLladdrMirror[index];
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "ChRdaddr".
**
** This function writes the given value to the index-th entry of the register
** array ChRdaddr.
**
** \param index Array index to access.
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetChRdaddr( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChRdaddr( uint32_t index, uint32_t value )
{
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    FREG_DMA_ChRdaddrMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHRDADDR+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHRDADDR+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "OffsetAddress" of register "ChRdaddr".
**
** Starting address for transfer
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "OffsetAddress" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetChRdaddr_OffsetAddress( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChRdaddr_OffsetAddress( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_ChRdaddrMirror[index] & ~mask;
    FREG_DMA_ChRdaddrMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHRDADDR+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHRDADDR+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "ChRdaddr".
**
** This function reads the value from the index-th entry of the register
** array ChRdaddr.
**
** Starting address for transfer
**
** \param index Array index to access.
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChRdaddr( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChRdaddr( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdaddrMirror[index];
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "OffsetAddress" of register "ChRdaddr".
**
** Starting address for transfer
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "OffsetAddress" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChRdaddr_OffsetAddress( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChRdaddr_OffsetAddress( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdaddrMirror[index];
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "ChRdline".
**
** This function writes the given value to the index-th entry of the register
** array ChRdline.
**
** \param index Array index to access.
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetChRdline( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChRdline( uint32_t index, uint32_t value )
{
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    FREG_DMA_ChRdlineMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHRDLINE+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHRDLINE+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "LoopIncrement" of register "ChRdline".
**
** Increment at end of each loop
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "LoopIncrement" consists of the bits 28..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x1FFF0000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChRdline_LoopIncrement( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChRdline_LoopIncrement( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x1FFF0000UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 16 ) & mask;
    value |= FREG_DMA_ChRdlineMirror[index] & ~mask;
    FREG_DMA_ChRdlineMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHRDLINE+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHRDLINE+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Lines" of register "ChRdline".
**
** Number of lines in a single loop
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Lines" consists of the bits 9..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000003FFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetChRdline_Lines( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChRdline_Lines( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x000003FFUL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_ChRdlineMirror[index] & ~mask;
    FREG_DMA_ChRdlineMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHRDLINE+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHRDLINE+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "ChRdline".
**
** This function reads the value from the index-th entry of the register
** array ChRdline.
**
** 
**
** \param index Array index to access.
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChRdline( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChRdline( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdlineMirror[index];
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LoopIncrement" of register "ChRdline".
**
** Increment at end of each loop
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "LoopIncrement" consists of the bits 28..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x1FFF0000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChRdline_LoopIncrement( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChRdline_LoopIncrement( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdlineMirror[index];
    value = ( value & 0x1FFF0000UL ) >> 16;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Lines" of register "ChRdline".
**
** Number of lines in a single loop
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "Lines" consists of the bits 9..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000003FFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChRdline_Lines( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChRdline_Lines( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdlineMirror[index];
    value = ( value & 0x000003FFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "ChRdinc".
**
** This function writes the given value to the index-th entry of the register
** array ChRdinc.
**
** \param index Array index to access.
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetChRdinc( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChRdinc( uint32_t index, uint32_t value )
{
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    FREG_DMA_ChRdincMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHRDINC+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHRDINC+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "LineIncrement" of register "ChRdinc".
**
** Increment at end of line
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "LineIncrement" consists of the bits 28..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x1FFF0000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChRdinc_LineIncrement( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChRdinc_LineIncrement( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x1FFF0000UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 16 ) & mask;
    value |= FREG_DMA_ChRdincMirror[index] & ~mask;
    FREG_DMA_ChRdincMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHRDINC+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHRDINC+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "LineLength" of register "ChRdinc".
**
** Length of a line
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "LineLength" consists of the bits 11..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000FFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetChRdinc_LineLength( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChRdinc_LineLength( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x00000FFFUL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_ChRdincMirror[index] & ~mask;
    FREG_DMA_ChRdincMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHRDINC+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHRDINC+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "ChRdinc".
**
** This function reads the value from the index-th entry of the register
** array ChRdinc.
**
** 
**
** \param index Array index to access.
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChRdinc( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChRdinc( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdincMirror[index];
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LineIncrement" of register "ChRdinc".
**
** Increment at end of line
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "LineIncrement" consists of the bits 28..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x1FFF0000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChRdinc_LineIncrement( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChRdinc_LineIncrement( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdincMirror[index];
    value = ( value & 0x1FFF0000UL ) >> 16;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LineLength" of register "ChRdinc".
**
** Length of a line
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "LineLength" consists of the bits 11..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000FFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChRdinc_LineLength( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChRdinc_LineLength( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdincMirror[index];
    value = ( value & 0x00000FFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "ChRdlpaddr".
**
** This function writes the given value to the index-th entry of the register
** array ChRdlpaddr.
**
** \param index Array index to access.
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetChRdlpaddr( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChRdlpaddr( uint32_t index, uint32_t value )
{
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    FREG_DMA_ChRdlpaddrMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHRDLPADDR+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHRDLPADDR+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "LoopAddress" of register "ChRdlpaddr".
**
** Address to loop back to start address at
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "LoopAddress" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetChRdlpaddr_LoopAddress( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChRdlpaddr_LoopAddress( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_ChRdlpaddrMirror[index] & ~mask;
    FREG_DMA_ChRdlpaddrMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHRDLPADDR+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHRDLPADDR+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "ChRdlpaddr".
**
** This function reads the value from the index-th entry of the register
** array ChRdlpaddr.
**
** Address to loop back to start address at
**
** \param index Array index to access.
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChRdlpaddr( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChRdlpaddr( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdlpaddrMirror[index];
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LoopAddress" of register "ChRdlpaddr".
**
** Address to loop back to start address at
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "LoopAddress" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChRdlpaddr_LoopAddress( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChRdlpaddr_LoopAddress( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdlpaddrMirror[index];
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "ChWraddr".
**
** This function writes the given value to the index-th entry of the register
** array ChWraddr.
**
** \param index Array index to access.
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetChWraddr( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChWraddr( uint32_t index, uint32_t value )
{
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    FREG_DMA_ChWraddrMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHWRADDR+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHWRADDR+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "OffsetAddress" of register "ChWraddr".
**
** Starting address for transfer
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "OffsetAddress" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetChWraddr_OffsetAddress( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChWraddr_OffsetAddress( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_ChWraddrMirror[index] & ~mask;
    FREG_DMA_ChWraddrMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHWRADDR+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHWRADDR+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "ChWraddr".
**
** This function reads the value from the index-th entry of the register
** array ChWraddr.
**
** Starting address for transfer
**
** \param index Array index to access.
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChWraddr( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChWraddr( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWraddrMirror[index];
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "OffsetAddress" of register "ChWraddr".
**
** Starting address for transfer
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "OffsetAddress" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChWraddr_OffsetAddress( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChWraddr_OffsetAddress( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWraddrMirror[index];
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "ChWrline".
**
** This function writes the given value to the index-th entry of the register
** array ChWrline.
**
** \param index Array index to access.
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetChWrline( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChWrline( uint32_t index, uint32_t value )
{
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    FREG_DMA_ChWrlineMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHWRLINE+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHWRLINE+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "LoopIncrement" of register "ChWrline".
**
** Increment at end of each loop
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "LoopIncrement" consists of the bits 28..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x1FFF0000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChWrline_LoopIncrement( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChWrline_LoopIncrement( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x1FFF0000UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 16 ) & mask;
    value |= FREG_DMA_ChWrlineMirror[index] & ~mask;
    FREG_DMA_ChWrlineMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHWRLINE+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHWRLINE+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "Lines" of register "ChWrline".
**
** Number of lines in a single loop
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "Lines" consists of the bits 9..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000003FFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetChWrline_Lines( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChWrline_Lines( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x000003FFUL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_ChWrlineMirror[index] & ~mask;
    FREG_DMA_ChWrlineMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHWRLINE+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHWRLINE+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "ChWrline".
**
** This function reads the value from the index-th entry of the register
** array ChWrline.
**
** 
**
** \param index Array index to access.
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChWrline( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChWrline( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWrlineMirror[index];
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LoopIncrement" of register "ChWrline".
**
** Increment at end of each loop
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "LoopIncrement" consists of the bits 28..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x1FFF0000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChWrline_LoopIncrement( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChWrline_LoopIncrement( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWrlineMirror[index];
    value = ( value & 0x1FFF0000UL ) >> 16;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Lines" of register "ChWrline".
**
** Number of lines in a single loop
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "Lines" consists of the bits 9..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x000003FFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChWrline_Lines( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChWrline_Lines( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWrlineMirror[index];
    value = ( value & 0x000003FFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "ChWrinc".
**
** This function writes the given value to the index-th entry of the register
** array ChWrinc.
**
** \param index Array index to access.
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetChWrinc( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChWrinc( uint32_t index, uint32_t value )
{
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    FREG_DMA_ChWrincMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHWRINC+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHWRINC+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "LineIncrement" of register "ChWrinc".
**
** Increment at end of line
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "LineIncrement" consists of the bits 28..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x1FFF0000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetChWrinc_LineIncrement( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChWrinc_LineIncrement( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x1FFF0000UL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 16 ) & mask;
    value |= FREG_DMA_ChWrincMirror[index] & ~mask;
    FREG_DMA_ChWrincMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHWRINC+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHWRINC+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "LineLength" of register "ChWrinc".
**
** Length of a line
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "LineLength" consists of the bits 11..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000FFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetChWrinc_LineLength( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChWrinc_LineLength( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0x00000FFFUL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_ChWrincMirror[index] & ~mask;
    FREG_DMA_ChWrincMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHWRINC+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHWRINC+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "ChWrinc".
**
** This function reads the value from the index-th entry of the register
** array ChWrinc.
**
** 
**
** \param index Array index to access.
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChWrinc( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChWrinc( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWrincMirror[index];
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LineIncrement" of register "ChWrinc".
**
** Increment at end of line
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "LineIncrement" consists of the bits 28..16,
**       the given bitgroup value will be shifted left by 16 bits and
**       masked using the bitmask 0x1FFF0000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChWrinc_LineIncrement( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChWrinc_LineIncrement( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWrincMirror[index];
    value = ( value & 0x1FFF0000UL ) >> 16;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LineLength" of register "ChWrinc".
**
** Length of a line
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "LineLength" consists of the bits 11..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000FFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChWrinc_LineLength( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChWrinc_LineLength( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWrincMirror[index];
    value = ( value & 0x00000FFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "ChWrlpaddr".
**
** This function writes the given value to the index-th entry of the register
** array ChWrlpaddr.
**
** \param index Array index to access.
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetChWrlpaddr( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChWrlpaddr( uint32_t index, uint32_t value )
{
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    FREG_DMA_ChWrlpaddrMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHWRLPADDR+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHWRLPADDR+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "LoopAddress" of register "ChWrlpaddr".
**
** Address to loop back to start address at
**
** \param index Array index to access.
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "LoopAddress" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern void FREG_DMA_SetChWrlpaddr_LoopAddress( uint32_t index, uint32_t value );
#else
INLINE void FREG_DMA_SetChWrlpaddr_LoopAddress( uint32_t index, uint32_t value )
{
    register uint32_t mask = 0xFFFFFFFFUL;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return;
    #endif
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_ChWrlpaddrMirror[index] & ~mask;
    FREG_DMA_ChWrlpaddrMirror[index] = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CHWRLPADDR+(index*0x40), value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CHWRLPADDR+(index*0x40))) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "ChWrlpaddr".
**
** This function reads the value from the index-th entry of the register
** array ChWrlpaddr.
**
** Address to loop back to start address at
**
** \param index Array index to access.
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChWrlpaddr( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChWrlpaddr( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWrlpaddrMirror[index];
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LoopAddress" of register "ChWrlpaddr".
**
** Address to loop back to start address at
**
** \param index Array index to access.
** \return Read bitgroup value.
**
** \note internally the bitgroup "LoopAddress" consists of the bits 31..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0xFFFFFFFFUL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmChWrlpaddr_LoopAddress( uint32_t index );
#else
INLINE uint32_t FREG_DMA_GetmChWrlpaddr_LoopAddress( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
    if( index >= 6 )
        return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWrlpaddrMirror[index];
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the entire register "CcirWbConfig".
**
** This function writes the given value to the register "CcirWbConfig".
**
** 
**
** \param value Register value to write.
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirWbConfig( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirWbConfig( uint32_t value )
{
    FREG_DMA_CcirWbConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRWBCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRWBCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "CcirWbEnable" of register "CcirWbConfig".
**
** Enable CCIR WB
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "CcirWbEnable" consists of the bits 31..31,
**       the given bitgroup value will be shifted left by 31 bits and
**       masked using the bitmask 0x80000000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirWbConfig_CcirWbEnable( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirWbConfig_CcirWbEnable( uint32_t value )
{
    register uint32_t mask = 0x80000000UL;
    value = ( value << 31 ) & mask;
    value |= FREG_DMA_CcirWbConfigMirror & ~mask;
    FREG_DMA_CcirWbConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRWBCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRWBCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "SourceSelect" of register "CcirWbConfig".
**
** Select CCIR WB source
** - '0' VD
** - '1' External CCIR-656
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "SourceSelect" consists of the bits 25..25,
**       the given bitgroup value will be shifted left by 25 bits and
**       masked using the bitmask 0x02000000UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirWbConfig_SourceSelect( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirWbConfig_SourceSelect( uint32_t value )
{
    register uint32_t mask = 0x02000000UL;
    value = ( value << 25 ) & mask;
    value |= FREG_DMA_CcirWbConfigMirror & ~mask;
    FREG_DMA_CcirWbConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRWBCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRWBCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "BankSize" of register "CcirWbConfig".
**
** Bank Size of the SDRAM device:
** - '0': 2 MByte = 16 Mbit (for 64Mbit \@ 16bit & 32bit
** devices)
** - '1': 4 MByte = 32 Mbit (for 128Mbit \@ 16bit & 32bit
** devices)
** - '2': 8 MByte = 64 Mbit (for 256Mbit \@ 16bit & 32bit
** devices)
** - '3': 16 MByte = 128 Mbit (for 512Mbit \@ 16bit & 32bit
** devices)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "BankSize" consists of the bits 9..8,
**       the given bitgroup value will be shifted left by 8 bits and
**       masked using the bitmask 0x00000300UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirWbConfig_BankSize( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirWbConfig_BankSize( uint32_t value )
{
    register uint32_t mask = 0x00000300UL;
    value = ( value << 8 ) & mask;
    value |= FREG_DMA_CcirWbConfigMirror & ~mask;
    FREG_DMA_CcirWbConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRWBCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRWBCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ColumnWidth" of register "CcirWbConfig".
**
** Defines the size of the SDRAM column in 32 bit words (Long Words = LW)
** - '00': 7 bit (128 LW = 512 Bytes)
** - '01': 8 bit (256 LW = 1024 Bytes)
** - '10': 9 bit (512 LW = 2048 Bytes)
** - '11': 10 bit (1024 LW = 4096 Bytes)
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ColumnWidth" consists of the bits 5..4,
**       the given bitgroup value will be shifted left by 4 bits and
**       masked using the bitmask 0x00000030UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirWbConfig_ColumnWidth( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirWbConfig_ColumnWidth( uint32_t value )
{
    register uint32_t mask = 0x00000030UL;
    value = ( value << 4 ) & mask;
    value |= FREG_DMA_CcirWbConfigMirror & ~mask;
    FREG_DMA_CcirWbConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRWBCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRWBCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "ClockInvert" of register "CcirWbConfig".
**
** Clock invert
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "ClockInvert" consists of the bits 1..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000002UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirWbConfig_ClockInvert( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirWbConfig_ClockInvert( uint32_t value )
{
    register uint32_t mask = 0x00000002UL;
    value = ( value << 1 ) & mask;
    value |= FREG_DMA_CcirWbConfigMirror & ~mask;
    FREG_DMA_CcirWbConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRWBCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRWBCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Writes the bitgroup "PalNtsc" of register "CcirWbConfig".
**
** Select the video input format
** - '0' PAL
** - '1' NTSC
**
** \param value Bitgroup value to write.
**
** \note internally the bitgroup "PalNtsc" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern void FREG_DMA_SetCcirWbConfig_PalNtsc( uint32_t value );
#else
INLINE void FREG_DMA_SetCcirWbConfig_PalNtsc( uint32_t value )
{
    register uint32_t mask = 0x00000001UL;
    value = ( value << 0 ) & mask;
    value |= FREG_DMA_CcirWbConfigMirror & ~mask;
    FREG_DMA_CcirWbConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRWBCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRWBCONFIG)) = value;
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CcirWbConfig".
**
** This function reads the value from the register "CcirWbConfig".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirWbConfig( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirWbConfig( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "CcirWbEnable" of register "CcirWbConfig".
**
** Enable CCIR WB
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "CcirWbEnable" consists of the bits 31..31,
**       the given bitgroup value will be shifted left by 31 bits and
**       masked using the bitmask 0x80000000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirWbConfig_CcirWbEnable( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirWbConfig_CcirWbEnable( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    value = ( value & 0x80000000UL ) >> 31;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "SourceSelect" of register "CcirWbConfig".
**
** Select CCIR WB source
** - '0' VD
** - '1' External CCIR-656
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "SourceSelect" consists of the bits 25..25,
**       the given bitgroup value will be shifted left by 25 bits and
**       masked using the bitmask 0x02000000UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirWbConfig_SourceSelect( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirWbConfig_SourceSelect( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    value = ( value & 0x02000000UL ) >> 25;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "BankSize" of register "CcirWbConfig".
**
** Bank Size of the SDRAM device:
** - '0': 2 MByte = 16 Mbit (for 64Mbit \@ 16bit & 32bit
** devices)
** - '1': 4 MByte = 32 Mbit (for 128Mbit \@ 16bit & 32bit
** devices)
** - '2': 8 MByte = 64 Mbit (for 256Mbit \@ 16bit & 32bit
** devices)
** - '3': 16 MByte = 128 Mbit (for 512Mbit \@ 16bit & 32bit
** devices)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "BankSize" consists of the bits 9..8,
**       the given bitgroup value will be shifted left by 8 bits and
**       masked using the bitmask 0x00000300UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirWbConfig_BankSize( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirWbConfig_BankSize( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    value = ( value & 0x00000300UL ) >> 8;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ColumnWidth" of register "CcirWbConfig".
**
** Defines the size of the SDRAM column in 32 bit words (Long Words = LW)
** - '00': 7 bit (128 LW = 512 Bytes)
** - '01': 8 bit (256 LW = 1024 Bytes)
** - '10': 9 bit (512 LW = 2048 Bytes)
** - '11': 10 bit (1024 LW = 4096 Bytes)
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ColumnWidth" consists of the bits 5..4,
**       the given bitgroup value will be shifted left by 4 bits and
**       masked using the bitmask 0x00000030UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirWbConfig_ColumnWidth( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirWbConfig_ColumnWidth( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    value = ( value & 0x00000030UL ) >> 4;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "ClockInvert" of register "CcirWbConfig".
**
** Clock invert
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "ClockInvert" consists of the bits 1..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000002UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirWbConfig_ClockInvert( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirWbConfig_ClockInvert( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    value = ( value & 0x00000002UL ) >> 1;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "PalNtsc" of register "CcirWbConfig".
**
** Select the video input format
** - '0' PAL
** - '1' NTSC
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "PalNtsc" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetmCcirWbConfig_PalNtsc( void );
#else
INLINE uint32_t FREG_DMA_GetmCcirWbConfig_PalNtsc( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the entire register "CcirWbStatus".
**
** This function reads the value from the register "CcirWbStatus".
**
** 
**
** \return Read register value.
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirWbStatus( void );
#else
INLINE uint32_t FREG_DMA_GetCcirWbStatus( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRWBSTATUS));
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "LineNumber" of register "CcirWbStatus".
**
** Indicates the current video line
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "LineNumber" consists of the bits 17..8,
**       the given bitgroup value will be shifted left by 8 bits and
**       masked using the bitmask 0x0003FF00UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirWbStatus_LineNumber( void );
#else
INLINE uint32_t FREG_DMA_GetCcirWbStatus_LineNumber( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRWBSTATUS));
    value = ( value & 0x0003FF00UL ) >> 8;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Vsync" of register "CcirWbStatus".
**
** Indicates the current Vertical sync as decoded from the ccir stream
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Vsync" consists of the bits 3..3,
**       the given bitgroup value will be shifted left by 3 bits and
**       masked using the bitmask 0x00000008UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirWbStatus_Vsync( void );
#else
INLINE uint32_t FREG_DMA_GetCcirWbStatus_Vsync( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRWBSTATUS));
    value = ( value & 0x00000008UL ) >> 3;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Hsync" of register "CcirWbStatus".
**
** Indicates the current Horizontal sync as decoded from the ccir stream
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Hsync" consists of the bits 2..2,
**       the given bitgroup value will be shifted left by 2 bits and
**       masked using the bitmask 0x00000004UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirWbStatus_Hsync( void );
#else
INLINE uint32_t FREG_DMA_GetCcirWbStatus_Hsync( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRWBSTATUS));
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Field" of register "CcirWbStatus".
**
** Indicates the current Video Field as decoded from the ccir stream
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Field" consists of the bits 1..1,
**       the given bitgroup value will be shifted left by 1 bits and
**       masked using the bitmask 0x00000002UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirWbStatus_Field( void );
#else
INLINE uint32_t FREG_DMA_GetCcirWbStatus_Field( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRWBSTATUS));
    value = ( value & 0x00000002UL ) >> 1;
    return( value );
}
#endif

#if !defined(FREG_USE_INLINE)
/*!
********************************************************************************
**
** \brief Reads the bitgroup "Frame" of register "CcirWbStatus".
**
** Indicates the current video frame that is currently written into memory
**
** \return Read bitgroup value.
**
** \note internally the bitgroup "Frame" consists of the bits 0..0,
**       the given bitgroup value will be shifted left by 0 bits and
**       masked using the bitmask 0x00000001UL
**
********************************************************************************
*/
extern uint32_t FREG_DMA_GetCcirWbStatus_Frame( void );
#else
INLINE uint32_t FREG_DMA_GetCcirWbStatus_Frame( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRWBSTATUS));
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}
#endif

/*!
********************************************************************************
**
** \brief Initializes the "DMA" block.
**
** Initializes all required registers and mirror variables.
**
********************************************************************************
*/
extern void FREG_DMA_Init( void );

#endif
