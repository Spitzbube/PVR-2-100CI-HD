/*
********************************************************************************
**
** \file      ./fapi/reg/src/MB86H60/reg_dma.c
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
#define FREG_DMA_C

#include <stdint.h>
#include "reg_dma.h"

/*
********************************************************************************
**
** Mirror variables for write-only registers
**
********************************************************************************
*/
volatile uint32_t FREG_DMA_IntMaskMirror = 0;
volatile uint32_t FREG_DMA_IntClearMirror = 0;
volatile uint32_t FREG_DMA_UsbIntMaskMirror = 0;
volatile uint32_t FREG_DMA_UsbIntClearMirror = 0;
volatile uint32_t FREG_DMA_BlockFillMirror = 0;
volatile uint32_t FREG_DMA_UsbModeMirror = 0;
volatile uint32_t FREG_DMA_CcirHconfigMirror = 0;
volatile uint32_t FREG_DMA_CcirVconfigMirror = 0;
volatile uint32_t FREG_DMA_CsConfigMirror = 0;
volatile uint32_t FREG_DMA_CsYMirror = 0;
volatile uint32_t FREG_DMA_CsCrMirror = 0;
volatile uint32_t FREG_DMA_CsCbMirror = 0;
volatile uint32_t FREG_DMA_CsOffsetMirror = 0;
volatile uint32_t FREG_DMA_SrConfigMirror = 0;
volatile uint32_t FREG_DMA_SrMaskMirror = 0;
volatile uint32_t FREG_DMA_SrCompareMirror = 0;
volatile uint32_t FREG_DMA_SrReplaceMirror = 0;
volatile uint32_t FREG_DMA_SrReplaceMaskMirror = 0;
volatile uint32_t FREG_DMA_AudioConfigMirror = 0;
volatile uint32_t FREG_DMA_CryptoConfigMirror = 0;
volatile uint32_t FREG_DMA_CryptoK1MsbMirror = 0;
volatile uint32_t FREG_DMA_CryptoK1LsbMirror = 0;
volatile uint32_t FREG_DMA_CryptoK2MsbMirror = 0;
volatile uint32_t FREG_DMA_CryptoK2LsbMirror = 0;
volatile uint32_t FREG_DMA_CryptoK3MsbMirror = 0;
volatile uint32_t FREG_DMA_CryptoK3LsbMirror = 0;
volatile uint32_t FREG_DMA_IvMsbMirror = 0;
volatile uint32_t FREG_DMA_IvLsbMirror = 0;
volatile uint32_t FREG_DMA_ChLengthMirror[6] = {
    0,0,0,0,0,0
};
volatile uint32_t FREG_DMA_ChLladdrMirror[6] = {
    0,0,0,0,0,0
};
volatile uint32_t FREG_DMA_ChRdaddrMirror[6] = {
    0,0,0,0,0,0
};
volatile uint32_t FREG_DMA_ChRdlineMirror[6] = {
    0,0,0,0,0,0
};
volatile uint32_t FREG_DMA_ChRdincMirror[6] = {
    0,0,0,0,0,0
};
volatile uint32_t FREG_DMA_ChRdlpaddrMirror[6] = {
    0,0,0,0,0,0
};
volatile uint32_t FREG_DMA_ChWraddrMirror[6] = {
    0,0,0,0,0,0
};
volatile uint32_t FREG_DMA_ChWrlineMirror[6] = {
    0,0,0,0,0,0
};
volatile uint32_t FREG_DMA_ChWrincMirror[6] = {
    0,0,0,0,0,0
};
volatile uint32_t FREG_DMA_ChWrlpaddrMirror[6] = {
    0,0,0,0,0,0
};
volatile uint32_t FREG_DMA_CcirWbConfigMirror = 0;

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_1"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_1" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_1")))
#endif

void FREG_DMA_SetIntMask( uint32_t value )
{
    FREG_DMA_IntMaskMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_INTMASK, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_INTMASK)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_2"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_2" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_2")))
#endif

void FREG_DMA_SetIntMask_Mask( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_3"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_3" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_3")))
#endif

uint32_t FREG_DMA_GetmIntMask( void )
{
    register uint32_t value = FREG_DMA_IntMaskMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_4"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_4" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_4")))
#endif

uint32_t FREG_DMA_GetmIntMask_Mask( void )
{
    register uint32_t value = FREG_DMA_IntMaskMirror;
    value = ( value & 0x000003FFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_5"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_5" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_5")))
#endif

uint32_t FREG_DMA_GetIntStatus( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_INTSTATUS));
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_6"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_6" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_6")))
#endif

uint32_t FREG_DMA_GetIntStatus_CcirSyncChange( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_INTSTATUS));
    value = ( value & 0x00000200UL ) >> 9;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_7"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_7" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_7")))
#endif

uint32_t FREG_DMA_GetIntStatus_SearchHit( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_INTSTATUS));
    value = ( value & 0x00000100UL ) >> 8;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_8"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_8" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_8")))
#endif

uint32_t FREG_DMA_GetIntStatus_AudioOverflow( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_INTSTATUS));
    value = ( value & 0x00000080UL ) >> 7;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_9"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_9" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_9")))
#endif

uint32_t FREG_DMA_GetIntStatus_CcirOverflow( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_INTSTATUS));
    value = ( value & 0x00000040UL ) >> 6;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_10"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_10" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_10")))
#endif

uint32_t FREG_DMA_GetIntStatus_ChannelComplete( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_INTSTATUS));
    value = ( value & 0x0000003FUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_11"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_11" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_11")))
#endif

void FREG_DMA_SetIntClear( uint32_t value )
{
    FREG_DMA_IntClearMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_INTCLEAR, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_INTCLEAR)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_12"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_12" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_12")))
#endif

void FREG_DMA_SetIntClear_ClearIrq( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_13"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_13" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_13")))
#endif

uint32_t FREG_DMA_GetmIntClear( void )
{
    register uint32_t value = FREG_DMA_IntClearMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_14"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_14" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_14")))
#endif

uint32_t FREG_DMA_GetmIntClear_ClearIrq( void )
{
    register uint32_t value = FREG_DMA_IntClearMirror;
    value = ( value & 0x000003FFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_15"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_15" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_15")))
#endif

void FREG_DMA_SetUsbIntMask( uint32_t value )
{
    FREG_DMA_UsbIntMaskMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBINTMASK, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBINTMASK)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_16"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_16" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_16")))
#endif

void FREG_DMA_SetUsbIntMask_Mask( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_17"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_17" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_17")))
#endif

uint32_t FREG_DMA_GetmUsbIntMask( void )
{
    register uint32_t value = FREG_DMA_UsbIntMaskMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_18"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_18" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_18")))
#endif

uint32_t FREG_DMA_GetmUsbIntMask_Mask( void )
{
    register uint32_t value = FREG_DMA_UsbIntMaskMirror;
    value = ( value & 0x0000007FUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_19"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_19" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_19")))
#endif

uint32_t FREG_DMA_GetUsbIntStatus( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_USBINTSTATUS));
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_20"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_20" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_20")))
#endif

uint32_t FREG_DMA_GetUsbIntStatus_UsbEpDmaReadIrq( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_USBINTSTATUS));
    value = ( value & 0x00000070UL ) >> 4;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_21"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_21" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_21")))
#endif

uint32_t FREG_DMA_GetUsbIntStatus_UsbEpDmaWriteIrq( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_USBINTSTATUS));
    value = ( value & 0x0000000EUL ) >> 1;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_22"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_22" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_22")))
#endif

uint32_t FREG_DMA_GetUsbIntStatus_UsbIrq( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_USBINTSTATUS));
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_23"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_23" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_23")))
#endif

void FREG_DMA_SetUsbIntClear( uint32_t value )
{
    FREG_DMA_UsbIntClearMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBINTCLEAR, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBINTCLEAR)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_24"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_24" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_24")))
#endif

void FREG_DMA_SetUsbIntClear_ClearUsbIrq( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_25"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_25" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_25")))
#endif

uint32_t FREG_DMA_GetmUsbIntClear( void )
{
    register uint32_t value = FREG_DMA_UsbIntClearMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_26"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_26" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_26")))
#endif

uint32_t FREG_DMA_GetmUsbIntClear_ClearUsbIrq( void )
{
    register uint32_t value = FREG_DMA_UsbIntClearMirror;
    value = ( value & 0x0000007FUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_27"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_27" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_27")))
#endif

void FREG_DMA_SetBlockFill( uint32_t value )
{
    FREG_DMA_BlockFillMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_BLOCKFILL, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_BLOCKFILL)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_28"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_28" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_28")))
#endif

void FREG_DMA_SetBlockFill_BlockFillValue( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_29"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_29" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_29")))
#endif

uint32_t FREG_DMA_GetmBlockFill( void )
{
    register uint32_t value = FREG_DMA_BlockFillMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_30"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_30" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_30")))
#endif

uint32_t FREG_DMA_GetmBlockFill_BlockFillValue( void )
{
    register uint32_t value = FREG_DMA_BlockFillMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_31"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_31" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_31")))
#endif

void FREG_DMA_SetUsbMode( uint32_t value )
{
    FREG_DMA_UsbModeMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_USBMODE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_USBMODE)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_32"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_32" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_32")))
#endif

void FREG_DMA_SetUsbMode_ExternalCpEnable( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_33"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_33" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_33")))
#endif

void FREG_DMA_SetUsbMode_PacedEpReadChannel( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_34"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_34" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_34")))
#endif

void FREG_DMA_SetUsbMode_PacedEpWriteChannel( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_35"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_35" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_35")))
#endif

void FREG_DMA_SetUsbMode_LengthInput( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_36"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_36" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_36")))
#endif

void FREG_DMA_SetUsbMode_TestMode( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_37"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_37" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_37")))
#endif

uint32_t FREG_DMA_GetmUsbMode( void )
{
    register uint32_t value = FREG_DMA_UsbModeMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_38"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_38" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_38")))
#endif

uint32_t FREG_DMA_GetmUsbMode_ExternalCpEnable( void )
{
    register uint32_t value = FREG_DMA_UsbModeMirror;
    value = ( value & 0x00000080UL ) >> 7;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_39"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_39" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_39")))
#endif

uint32_t FREG_DMA_GetmUsbMode_PacedEpReadChannel( void )
{
    register uint32_t value = FREG_DMA_UsbModeMirror;
    value = ( value & 0x00000060UL ) >> 5;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_40"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_40" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_40")))
#endif

uint32_t FREG_DMA_GetmUsbMode_PacedEpWriteChannel( void )
{
    register uint32_t value = FREG_DMA_UsbModeMirror;
    value = ( value & 0x00000018UL ) >> 3;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_41"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_41" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_41")))
#endif

uint32_t FREG_DMA_GetmUsbMode_LengthInput( void )
{
    register uint32_t value = FREG_DMA_UsbModeMirror;
    value = ( value & 0x00000006UL ) >> 1;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_42"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_42" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_42")))
#endif

uint32_t FREG_DMA_GetmUsbMode_TestMode( void )
{
    register uint32_t value = FREG_DMA_UsbModeMirror;
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_43"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_43" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_43")))
#endif

void FREG_DMA_SetCcirConfig( uint32_t value )
{
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_44"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_44" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_44")))
#endif

void FREG_DMA_SetCcirConfig_RawEnableMode( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_45"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_45" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_45")))
#endif

void FREG_DMA_SetCcirConfig_ExternalInputSelect( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_46"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_46" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_46")))
#endif

void FREG_DMA_SetCcirConfig_SyncStatus( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_47"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_47" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_47")))
#endif

void FREG_DMA_SetCcirConfig_ClockInvert( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_48"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_48" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_48")))
#endif

void FREG_DMA_SetCcirConfig_Enable( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_49"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_49" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_49")))
#endif

void FREG_DMA_SetCcirConfig_RawMode( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_50"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_50" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_50")))
#endif

void FREG_DMA_SetCcirConfig_CaptureField1( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_51"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_51" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_51")))
#endif

void FREG_DMA_SetCcirConfig_CaptureField0( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_52"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_52" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_52")))
#endif

uint32_t FREG_DMA_GetCcirConfig( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_53"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_53" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_53")))
#endif

uint32_t FREG_DMA_GetCcirConfig_RawEnableMode( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000080UL ) >> 7;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_54"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_54" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_54")))
#endif

uint32_t FREG_DMA_GetCcirConfig_ExternalInputSelect( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000040UL ) >> 6;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_55"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_55" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_55")))
#endif

uint32_t FREG_DMA_GetCcirConfig_SyncStatus( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000020UL ) >> 5;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_56"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_56" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_56")))
#endif

uint32_t FREG_DMA_GetCcirConfig_ClockInvert( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000010UL ) >> 4;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_57"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_57" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_57")))
#endif

uint32_t FREG_DMA_GetCcirConfig_Enable( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000008UL ) >> 3;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_58"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_58" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_58")))
#endif

uint32_t FREG_DMA_GetCcirConfig_RawMode( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_59"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_59" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_59")))
#endif

uint32_t FREG_DMA_GetCcirConfig_CaptureField1( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000002UL ) >> 1;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_60"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_60" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_60")))
#endif

uint32_t FREG_DMA_GetCcirConfig_CaptureField0( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRCONFIG));
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_61"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_61" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_61")))
#endif

void FREG_DMA_SetCcirHconfig( uint32_t value )
{
    FREG_DMA_CcirHconfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRHCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRHCONFIG)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_62"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_62" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_62")))
#endif

void FREG_DMA_SetCcirHconfig_HStartPosition( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_63"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_63" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_63")))
#endif

void FREG_DMA_SetCcirHconfig_HStopPosition( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_64"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_64" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_64")))
#endif

uint32_t FREG_DMA_GetmCcirHconfig( void )
{
    register uint32_t value = FREG_DMA_CcirHconfigMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_65"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_65" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_65")))
#endif

uint32_t FREG_DMA_GetmCcirHconfig_HStartPosition( void )
{
    register uint32_t value = FREG_DMA_CcirHconfigMirror;
    value = ( value & 0x01FF0000UL ) >> 16;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_66"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_66" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_66")))
#endif

uint32_t FREG_DMA_GetmCcirHconfig_HStopPosition( void )
{
    register uint32_t value = FREG_DMA_CcirHconfigMirror;
    value = ( value & 0x000001FFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_67"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_67" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_67")))
#endif

void FREG_DMA_SetCcirVconfig( uint32_t value )
{
    FREG_DMA_CcirVconfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRVCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRVCONFIG)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_68"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_68" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_68")))
#endif

void FREG_DMA_SetCcirVconfig_VStartPosition( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_69"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_69" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_69")))
#endif

void FREG_DMA_SetCcirVconfig_VStopPosition( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_70"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_70" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_70")))
#endif

uint32_t FREG_DMA_GetmCcirVconfig( void )
{
    register uint32_t value = FREG_DMA_CcirVconfigMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_71"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_71" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_71")))
#endif

uint32_t FREG_DMA_GetmCcirVconfig_VStartPosition( void )
{
    register uint32_t value = FREG_DMA_CcirVconfigMirror;
    value = ( value & 0x01FF0000UL ) >> 16;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_72"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_72" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_72")))
#endif

uint32_t FREG_DMA_GetmCcirVconfig_VStopPosition( void )
{
    register uint32_t value = FREG_DMA_CcirVconfigMirror;
    value = ( value & 0x000001FFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_73"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_73" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_73")))
#endif

void FREG_DMA_SetCsConfig( uint32_t value )
{
    FREG_DMA_CsConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCONFIG)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_74"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_74" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_74")))
#endif

void FREG_DMA_SetCsConfig_ColorInputMode( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_75"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_75" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_75")))
#endif

void FREG_DMA_SetCsConfig_ManualAlphaValue( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_76"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_76" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_76")))
#endif

void FREG_DMA_SetCsConfig_ManualAlphaEnable( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_77"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_77" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_77")))
#endif

void FREG_DMA_SetCsConfig_ConversionMode( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_78"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_78" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_78")))
#endif

void FREG_DMA_SetCsConfig_Enable( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_79"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_79" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_79")))
#endif

uint32_t FREG_DMA_GetmCsConfig( void )
{
    register uint32_t value = FREG_DMA_CsConfigMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_80"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_80" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_80")))
#endif

uint32_t FREG_DMA_GetmCsConfig_ColorInputMode( void )
{
    register uint32_t value = FREG_DMA_CsConfigMirror;
    value = ( value & 0x00070000UL ) >> 16;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_81"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_81" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_81")))
#endif

uint32_t FREG_DMA_GetmCsConfig_ManualAlphaValue( void )
{
    register uint32_t value = FREG_DMA_CsConfigMirror;
    value = ( value & 0x0000FF00UL ) >> 8;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_82"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_82" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_82")))
#endif

uint32_t FREG_DMA_GetmCsConfig_ManualAlphaEnable( void )
{
    register uint32_t value = FREG_DMA_CsConfigMirror;
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_83"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_83" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_83")))
#endif

uint32_t FREG_DMA_GetmCsConfig_ConversionMode( void )
{
    register uint32_t value = FREG_DMA_CsConfigMirror;
    value = ( value & 0x00000002UL ) >> 1;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_84"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_84" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_84")))
#endif

uint32_t FREG_DMA_GetmCsConfig_Enable( void )
{
    register uint32_t value = FREG_DMA_CsConfigMirror;
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_85"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_85" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_85")))
#endif

void FREG_DMA_SetCsY( uint32_t value )
{
    FREG_DMA_CsYMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSY, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSY)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_86"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_86" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_86")))
#endif

void FREG_DMA_SetCsY_RedCoefficient( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_87"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_87" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_87")))
#endif

void FREG_DMA_SetCsY_GreenCoefficient( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_88"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_88" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_88")))
#endif

void FREG_DMA_SetCsY_BlueCoefficient( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_89"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_89" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_89")))
#endif

uint32_t FREG_DMA_GetmCsY( void )
{
    register uint32_t value = FREG_DMA_CsYMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_90"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_90" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_90")))
#endif

uint32_t FREG_DMA_GetmCsY_RedCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsYMirror;
    value = ( value & 0x07FC0000UL ) >> 18;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_91"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_91" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_91")))
#endif

uint32_t FREG_DMA_GetmCsY_GreenCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsYMirror;
    value = ( value & 0x0003FE00UL ) >> 9;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_92"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_92" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_92")))
#endif

uint32_t FREG_DMA_GetmCsY_BlueCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsYMirror;
    value = ( value & 0x000001FFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_93"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_93" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_93")))
#endif

void FREG_DMA_SetCsCr( uint32_t value )
{
    FREG_DMA_CsCrMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCR, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCR)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_94"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_94" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_94")))
#endif

void FREG_DMA_SetCsCr_RedCoefficient( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_95"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_95" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_95")))
#endif

void FREG_DMA_SetCsCr_GreenCoefficient( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_96"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_96" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_96")))
#endif

void FREG_DMA_SetCsCr_BlueCoefficient( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_97"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_97" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_97")))
#endif

uint32_t FREG_DMA_GetmCsCr( void )
{
    register uint32_t value = FREG_DMA_CsCrMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_98"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_98" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_98")))
#endif

uint32_t FREG_DMA_GetmCsCr_RedCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsCrMirror;
    value = ( value & 0x07FC0000UL ) >> 18;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_99"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_99" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_99")))
#endif

uint32_t FREG_DMA_GetmCsCr_GreenCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsCrMirror;
    value = ( value & 0x0003FE00UL ) >> 9;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_100"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_100" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_100")))
#endif

uint32_t FREG_DMA_GetmCsCr_BlueCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsCrMirror;
    value = ( value & 0x000001FFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_101"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_101" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_101")))
#endif

void FREG_DMA_SetCsCb( uint32_t value )
{
    FREG_DMA_CsCbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSCB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSCB)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_102"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_102" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_102")))
#endif

void FREG_DMA_SetCsCb_RedCoefficient( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_103"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_103" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_103")))
#endif

void FREG_DMA_SetCsCb_GreenCoefficient( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_104"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_104" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_104")))
#endif

void FREG_DMA_SetCsCb_BlueCoefficient( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_105"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_105" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_105")))
#endif

uint32_t FREG_DMA_GetmCsCb( void )
{
    register uint32_t value = FREG_DMA_CsCbMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_106"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_106" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_106")))
#endif

uint32_t FREG_DMA_GetmCsCb_RedCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsCbMirror;
    value = ( value & 0x07FC0000UL ) >> 18;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_107"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_107" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_107")))
#endif

uint32_t FREG_DMA_GetmCsCb_GreenCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsCbMirror;
    value = ( value & 0x0003FE00UL ) >> 9;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_108"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_108" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_108")))
#endif

uint32_t FREG_DMA_GetmCsCb_BlueCoefficient( void )
{
    register uint32_t value = FREG_DMA_CsCbMirror;
    value = ( value & 0x000001FFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_109"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_109" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_109")))
#endif

void FREG_DMA_SetCsOffset( uint32_t value )
{
    FREG_DMA_CsOffsetMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CSOFFSET, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CSOFFSET)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_110"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_110" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_110")))
#endif

void FREG_DMA_SetCsOffset_YOffset( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_111"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_111" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_111")))
#endif

void FREG_DMA_SetCsOffset_CrOffset( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_112"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_112" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_112")))
#endif

void FREG_DMA_SetCsOffset_CbOffset( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_113"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_113" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_113")))
#endif

uint32_t FREG_DMA_GetmCsOffset( void )
{
    register uint32_t value = FREG_DMA_CsOffsetMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_114"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_114" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_114")))
#endif

uint32_t FREG_DMA_GetmCsOffset_YOffset( void )
{
    register uint32_t value = FREG_DMA_CsOffsetMirror;
    value = ( value & 0x3FF00000UL ) >> 20;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_115"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_115" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_115")))
#endif

uint32_t FREG_DMA_GetmCsOffset_CrOffset( void )
{
    register uint32_t value = FREG_DMA_CsOffsetMirror;
    value = ( value & 0x000FFC00UL ) >> 10;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_116"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_116" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_116")))
#endif

uint32_t FREG_DMA_GetmCsOffset_CbOffset( void )
{
    register uint32_t value = FREG_DMA_CsOffsetMirror;
    value = ( value & 0x000003FFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_117"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_117" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_117")))
#endif

void FREG_DMA_SetSrConfig( uint32_t value )
{
    FREG_DMA_SrConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRCONFIG)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_118"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_118" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_118")))
#endif

void FREG_DMA_SetSrConfig_Enable( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_119"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_119" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_119")))
#endif

void FREG_DMA_SetSrConfig_ReplacementEnable( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_120"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_120" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_120")))
#endif

void FREG_DMA_SetSrConfig_Alignment( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_121"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_121" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_121")))
#endif

uint32_t FREG_DMA_GetmSrConfig( void )
{
    register uint32_t value = FREG_DMA_SrConfigMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_122"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_122" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_122")))
#endif

uint32_t FREG_DMA_GetmSrConfig_Enable( void )
{
    register uint32_t value = FREG_DMA_SrConfigMirror;
    value = ( value & 0x00000008UL ) >> 3;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_123"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_123" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_123")))
#endif

uint32_t FREG_DMA_GetmSrConfig_ReplacementEnable( void )
{
    register uint32_t value = FREG_DMA_SrConfigMirror;
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_124"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_124" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_124")))
#endif

uint32_t FREG_DMA_GetmSrConfig_Alignment( void )
{
    register uint32_t value = FREG_DMA_SrConfigMirror;
    value = ( value & 0x00000003UL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_125"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_125" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_125")))
#endif

uint32_t FREG_DMA_GetSrAddress( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_SRADDRESS));
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_126"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_126" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_126")))
#endif

uint32_t FREG_DMA_GetSrAddress_HitAddress( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_SRADDRESS));
    value = ( value & 0x07FFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_127"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_127" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_127")))
#endif

void FREG_DMA_SetSrMask( uint32_t value )
{
    FREG_DMA_SrMaskMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRMASK, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRMASK)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_128"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_128" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_128")))
#endif

void FREG_DMA_SetSrMask_SearchMask( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_129"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_129" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_129")))
#endif

uint32_t FREG_DMA_GetmSrMask( void )
{
    register uint32_t value = FREG_DMA_SrMaskMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_130"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_130" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_130")))
#endif

uint32_t FREG_DMA_GetmSrMask_SearchMask( void )
{
    register uint32_t value = FREG_DMA_SrMaskMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_131"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_131" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_131")))
#endif

void FREG_DMA_SetSrCompare( uint32_t value )
{
    FREG_DMA_SrCompareMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRCOMPARE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRCOMPARE)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_132"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_132" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_132")))
#endif

void FREG_DMA_SetSrCompare_SearchCompareValue( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_133"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_133" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_133")))
#endif

uint32_t FREG_DMA_GetmSrCompare( void )
{
    register uint32_t value = FREG_DMA_SrCompareMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_134"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_134" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_134")))
#endif

uint32_t FREG_DMA_GetmSrCompare_SearchCompareValue( void )
{
    register uint32_t value = FREG_DMA_SrCompareMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_135"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_135" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_135")))
#endif

void FREG_DMA_SetSrReplace( uint32_t value )
{
    FREG_DMA_SrReplaceMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRREPLACE, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRREPLACE)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_136"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_136" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_136")))
#endif

void FREG_DMA_SetSrReplace_ReplacementValue( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_137"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_137" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_137")))
#endif

uint32_t FREG_DMA_GetmSrReplace( void )
{
    register uint32_t value = FREG_DMA_SrReplaceMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_138"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_138" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_138")))
#endif

uint32_t FREG_DMA_GetmSrReplace_ReplacementValue( void )
{
    register uint32_t value = FREG_DMA_SrReplaceMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_139"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_139" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_139")))
#endif

void FREG_DMA_SetSrReplaceMask( uint32_t value )
{
    FREG_DMA_SrReplaceMaskMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_SRREPLACEMASK, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_SRREPLACEMASK)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_140"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_140" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_140")))
#endif

void FREG_DMA_SetSrReplaceMask_ReplacementMask( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_141"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_141" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_141")))
#endif

uint32_t FREG_DMA_GetmSrReplaceMask( void )
{
    register uint32_t value = FREG_DMA_SrReplaceMaskMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_142"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_142" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_142")))
#endif

uint32_t FREG_DMA_GetmSrReplaceMask_ReplacementMask( void )
{
    register uint32_t value = FREG_DMA_SrReplaceMaskMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_143"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_143" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_143")))
#endif

void FREG_DMA_SetAudioConfig( uint32_t value )
{
    FREG_DMA_AudioConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_AUDIOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_AUDIOCONFIG)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_144"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_144" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_144")))
#endif

void FREG_DMA_SetAudioConfig_ExternalInputSelect( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_145"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_145" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_145")))
#endif

void FREG_DMA_SetAudioConfig_EndPosition( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_146"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_146" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_146")))
#endif

void FREG_DMA_SetAudioConfig_BitClockInvert( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_147"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_147" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_147")))
#endif

void FREG_DMA_SetAudioConfig_WordClockInvert( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_148"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_148" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_148")))
#endif

void FREG_DMA_SetAudioConfig_DataInvert( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_149"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_149" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_149")))
#endif

void FREG_DMA_SetAudioConfig_Enable( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_150"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_150" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_150")))
#endif

void FREG_DMA_SetAudioConfig_LeftChannelEnable( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_151"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_151" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_151")))
#endif

void FREG_DMA_SetAudioConfig_RightChannelEnable( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_152"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_152" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_152")))
#endif

void FREG_DMA_SetAudioConfig_NumberOfBits( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_153"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_153" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_153")))
#endif

void FREG_DMA_SetAudioConfig_MsbSwitch( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_154"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_154" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_154")))
#endif

uint32_t FREG_DMA_GetmAudioConfig( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_155"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_155" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_155")))
#endif

uint32_t FREG_DMA_GetmAudioConfig_ExternalInputSelect( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00002000UL ) >> 13;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_156"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_156" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_156")))
#endif

uint32_t FREG_DMA_GetmAudioConfig_EndPosition( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00001F00UL ) >> 8;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_157"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_157" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_157")))
#endif

uint32_t FREG_DMA_GetmAudioConfig_BitClockInvert( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000080UL ) >> 7;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_158"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_158" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_158")))
#endif

uint32_t FREG_DMA_GetmAudioConfig_WordClockInvert( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000040UL ) >> 6;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_159"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_159" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_159")))
#endif

uint32_t FREG_DMA_GetmAudioConfig_DataInvert( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000020UL ) >> 5;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_160"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_160" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_160")))
#endif

uint32_t FREG_DMA_GetmAudioConfig_Enable( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000010UL ) >> 4;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_161"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_161" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_161")))
#endif

uint32_t FREG_DMA_GetmAudioConfig_LeftChannelEnable( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000008UL ) >> 3;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_162"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_162" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_162")))
#endif

uint32_t FREG_DMA_GetmAudioConfig_RightChannelEnable( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_163"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_163" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_163")))
#endif

uint32_t FREG_DMA_GetmAudioConfig_NumberOfBits( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000002UL ) >> 1;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_164"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_164" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_164")))
#endif

uint32_t FREG_DMA_GetmAudioConfig_MsbSwitch( void )
{
    register uint32_t value = FREG_DMA_AudioConfigMirror;
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_165"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_165" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_165")))
#endif

void FREG_DMA_SetCryptoConfig( uint32_t value )
{
    FREG_DMA_CryptoConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOCONFIG)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_166"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_166" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_166")))
#endif

void FREG_DMA_SetCryptoConfig_GenerateSessionKey( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_167"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_167" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_167")))
#endif

void FREG_DMA_SetCryptoConfig_KeySelection( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_168"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_168" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_168")))
#endif

void FREG_DMA_SetCryptoConfig_EndianSwap( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_169"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_169" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_169")))
#endif

void FREG_DMA_SetCryptoConfig_Mode( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_170"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_170" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_170")))
#endif

void FREG_DMA_SetCryptoConfig_Enable( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_171"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_171" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_171")))
#endif

void FREG_DMA_SetCryptoConfig_Decrypt( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_172"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_172" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_172")))
#endif

uint32_t FREG_DMA_GetmCryptoConfig( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_173"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_173" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_173")))
#endif

uint32_t FREG_DMA_GetmCryptoConfig_GenerateSessionKey( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    value = ( value & 0x00000020UL ) >> 5;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_174"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_174" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_174")))
#endif

uint32_t FREG_DMA_GetmCryptoConfig_KeySelection( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    value = ( value & 0x00000010UL ) >> 4;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_175"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_175" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_175")))
#endif

uint32_t FREG_DMA_GetmCryptoConfig_EndianSwap( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    value = ( value & 0x00000008UL ) >> 3;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_176"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_176" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_176")))
#endif

uint32_t FREG_DMA_GetmCryptoConfig_Mode( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_177"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_177" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_177")))
#endif

uint32_t FREG_DMA_GetmCryptoConfig_Enable( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    value = ( value & 0x00000002UL ) >> 1;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_178"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_178" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_178")))
#endif

uint32_t FREG_DMA_GetmCryptoConfig_Decrypt( void )
{
    register uint32_t value = FREG_DMA_CryptoConfigMirror;
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_179"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_179" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_179")))
#endif

void FREG_DMA_SetCryptoK1Msb( uint32_t value )
{
    FREG_DMA_CryptoK1MsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK1MSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK1MSB)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_180"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_180" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_180")))
#endif

void FREG_DMA_SetCryptoK1Msb_Key1Msb( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_181"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_181" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_181")))
#endif

uint32_t FREG_DMA_GetmCryptoK1Msb( void )
{
    register uint32_t value = FREG_DMA_CryptoK1MsbMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_182"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_182" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_182")))
#endif

uint32_t FREG_DMA_GetmCryptoK1Msb_Key1Msb( void )
{
    register uint32_t value = FREG_DMA_CryptoK1MsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_183"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_183" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_183")))
#endif

void FREG_DMA_SetCryptoK1Lsb( uint32_t value )
{
    FREG_DMA_CryptoK1LsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK1LSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK1LSB)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_184"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_184" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_184")))
#endif

void FREG_DMA_SetCryptoK1Lsb_Key1Lsb( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_185"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_185" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_185")))
#endif

uint32_t FREG_DMA_GetmCryptoK1Lsb( void )
{
    register uint32_t value = FREG_DMA_CryptoK1LsbMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_186"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_186" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_186")))
#endif

uint32_t FREG_DMA_GetmCryptoK1Lsb_Key1Lsb( void )
{
    register uint32_t value = FREG_DMA_CryptoK1LsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_187"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_187" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_187")))
#endif

void FREG_DMA_SetCryptoK2Msb( uint32_t value )
{
    FREG_DMA_CryptoK2MsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK2MSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK2MSB)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_188"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_188" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_188")))
#endif

void FREG_DMA_SetCryptoK2Msb_Key2Msb( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_189"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_189" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_189")))
#endif

uint32_t FREG_DMA_GetmCryptoK2Msb( void )
{
    register uint32_t value = FREG_DMA_CryptoK2MsbMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_190"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_190" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_190")))
#endif

uint32_t FREG_DMA_GetmCryptoK2Msb_Key2Msb( void )
{
    register uint32_t value = FREG_DMA_CryptoK2MsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_191"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_191" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_191")))
#endif

void FREG_DMA_SetCryptoK2Lsb( uint32_t value )
{
    FREG_DMA_CryptoK2LsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK2LSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK2LSB)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_192"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_192" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_192")))
#endif

void FREG_DMA_SetCryptoK2Lsb_Key2Lsb( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_193"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_193" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_193")))
#endif

uint32_t FREG_DMA_GetmCryptoK2Lsb( void )
{
    register uint32_t value = FREG_DMA_CryptoK2LsbMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_194"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_194" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_194")))
#endif

uint32_t FREG_DMA_GetmCryptoK2Lsb_Key2Lsb( void )
{
    register uint32_t value = FREG_DMA_CryptoK2LsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_195"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_195" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_195")))
#endif

void FREG_DMA_SetCryptoK3Msb( uint32_t value )
{
    FREG_DMA_CryptoK3MsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK3MSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK3MSB)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_196"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_196" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_196")))
#endif

void FREG_DMA_SetCryptoK3Msb_Key3Msb( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_197"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_197" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_197")))
#endif

uint32_t FREG_DMA_GetmCryptoK3Msb( void )
{
    register uint32_t value = FREG_DMA_CryptoK3MsbMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_198"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_198" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_198")))
#endif

uint32_t FREG_DMA_GetmCryptoK3Msb_Key3Msb( void )
{
    register uint32_t value = FREG_DMA_CryptoK3MsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_199"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_199" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_199")))
#endif

void FREG_DMA_SetCryptoK3Lsb( uint32_t value )
{
    FREG_DMA_CryptoK3LsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CRYPTOK3LSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CRYPTOK3LSB)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_200"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_200" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_200")))
#endif

void FREG_DMA_SetCryptoK3Lsb_Key3Lsb( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_201"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_201" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_201")))
#endif

uint32_t FREG_DMA_GetmCryptoK3Lsb( void )
{
    register uint32_t value = FREG_DMA_CryptoK3LsbMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_202"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_202" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_202")))
#endif

uint32_t FREG_DMA_GetmCryptoK3Lsb_Key3Lsb( void )
{
    register uint32_t value = FREG_DMA_CryptoK3LsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_203"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_203" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_203")))
#endif

void FREG_DMA_SetIvMsb( uint32_t value )
{
    FREG_DMA_IvMsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_IVMSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_IVMSB)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_204"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_204" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_204")))
#endif

void FREG_DMA_SetIvMsb_IvMsb( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_205"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_205" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_205")))
#endif

uint32_t FREG_DMA_GetmIvMsb( void )
{
    register uint32_t value = FREG_DMA_IvMsbMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_206"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_206" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_206")))
#endif

uint32_t FREG_DMA_GetmIvMsb_IvMsb( void )
{
    register uint32_t value = FREG_DMA_IvMsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_207"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_207" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_207")))
#endif

void FREG_DMA_SetIvLsb( uint32_t value )
{
    FREG_DMA_IvLsbMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_IVLSB, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_IVLSB)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_208"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_208" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_208")))
#endif

void FREG_DMA_SetIvLsb_IvLsb( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_209"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_209" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_209")))
#endif

uint32_t FREG_DMA_GetmIvLsb( void )
{
    register uint32_t value = FREG_DMA_IvLsbMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_210"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_210" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_210")))
#endif

uint32_t FREG_DMA_GetmIvLsb_IvLsb( void )
{
    register uint32_t value = FREG_DMA_IvLsbMirror;
    value = ( value & 0xFFFFFFFFUL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_211"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_211" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_211")))
#endif

void FREG_DMA_SetChConfig( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_212"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_212" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_212")))
#endif

void FREG_DMA_SetChConfig_EndianSwapWrite( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_213"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_213" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_213")))
#endif

void FREG_DMA_SetChConfig_PeripheralAddressWrite( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_214"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_214" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_214")))
#endif

void FREG_DMA_SetChConfig_EndianSwapRead( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_215"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_215" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_215")))
#endif

void FREG_DMA_SetChConfig_PeripheralAddressRead( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_216"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_216" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_216")))
#endif

void FREG_DMA_SetChConfig_Done( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_217"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_217" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_217")))
#endif

void FREG_DMA_SetChConfig_Active( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_218"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_218" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_218")))
#endif

void FREG_DMA_SetChConfig_LinkedListEnable( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_219"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_219" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_219")))
#endif

void FREG_DMA_SetChConfig_Enable( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_220"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_220" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_220")))
#endif

void FREG_DMA_SetChConfig_AutoRestartEnable( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_221"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_221" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_221")))
#endif

uint32_t FREG_DMA_GetChConfig( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = *((volatile uint32_t*)(FREG_DMA_CHCONFIG+(index*0x40)));
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_222"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_222" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_222")))
#endif

uint32_t FREG_DMA_GetChConfig_EndianSwapWrite( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_223"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_223" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_223")))
#endif

uint32_t FREG_DMA_GetChConfig_PeripheralAddressWrite( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_224"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_224" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_224")))
#endif

uint32_t FREG_DMA_GetChConfig_EndianSwapRead( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_225"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_225" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_225")))
#endif

uint32_t FREG_DMA_GetChConfig_PeripheralAddressRead( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_226"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_226" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_226")))
#endif

uint32_t FREG_DMA_GetChConfig_Done( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_227"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_227" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_227")))
#endif

uint32_t FREG_DMA_GetChConfig_Active( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_228"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_228" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_228")))
#endif

uint32_t FREG_DMA_GetChConfig_LinkedListEnable( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_229"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_229" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_229")))
#endif

uint32_t FREG_DMA_GetChConfig_Enable( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_230"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_230" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_230")))
#endif

uint32_t FREG_DMA_GetChConfig_AutoRestartEnable( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_231"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_231" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_231")))
#endif

void FREG_DMA_SetChLength( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_232"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_232" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_232")))
#endif

void FREG_DMA_SetChLength_Length( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_233"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_233" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_233")))
#endif

uint32_t FREG_DMA_GetmChLength( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChLengthMirror[index];
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_234"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_234" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_234")))
#endif

uint32_t FREG_DMA_GetmChLength_Length( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_235"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_235" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_235")))
#endif

void FREG_DMA_SetChLladdr( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_236"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_236" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_236")))
#endif

void FREG_DMA_SetChLladdr_LinkedListAddress( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_237"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_237" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_237")))
#endif

uint32_t FREG_DMA_GetmChLladdr( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChLladdrMirror[index];
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_238"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_238" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_238")))
#endif

uint32_t FREG_DMA_GetmChLladdr_LinkedListAddress( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_239"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_239" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_239")))
#endif

void FREG_DMA_SetChRdaddr( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_240"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_240" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_240")))
#endif

void FREG_DMA_SetChRdaddr_OffsetAddress( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_241"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_241" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_241")))
#endif

uint32_t FREG_DMA_GetmChRdaddr( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdaddrMirror[index];
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_242"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_242" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_242")))
#endif

uint32_t FREG_DMA_GetmChRdaddr_OffsetAddress( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_243"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_243" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_243")))
#endif

void FREG_DMA_SetChRdline( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_244"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_244" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_244")))
#endif

void FREG_DMA_SetChRdline_LoopIncrement( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_245"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_245" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_245")))
#endif

void FREG_DMA_SetChRdline_Lines( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_246"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_246" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_246")))
#endif

uint32_t FREG_DMA_GetmChRdline( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdlineMirror[index];
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_247"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_247" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_247")))
#endif

uint32_t FREG_DMA_GetmChRdline_LoopIncrement( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_248"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_248" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_248")))
#endif

uint32_t FREG_DMA_GetmChRdline_Lines( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_249"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_249" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_249")))
#endif

void FREG_DMA_SetChRdinc( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_250"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_250" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_250")))
#endif

void FREG_DMA_SetChRdinc_LineIncrement( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_251"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_251" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_251")))
#endif

void FREG_DMA_SetChRdinc_LineLength( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_252"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_252" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_252")))
#endif

uint32_t FREG_DMA_GetmChRdinc( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdincMirror[index];
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_253"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_253" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_253")))
#endif

uint32_t FREG_DMA_GetmChRdinc_LineIncrement( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_254"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_254" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_254")))
#endif

uint32_t FREG_DMA_GetmChRdinc_LineLength( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_255"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_255" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_255")))
#endif

void FREG_DMA_SetChRdlpaddr( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_256"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_256" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_256")))
#endif

void FREG_DMA_SetChRdlpaddr_LoopAddress( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_257"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_257" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_257")))
#endif

uint32_t FREG_DMA_GetmChRdlpaddr( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChRdlpaddrMirror[index];
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_258"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_258" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_258")))
#endif

uint32_t FREG_DMA_GetmChRdlpaddr_LoopAddress( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_259"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_259" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_259")))
#endif

void FREG_DMA_SetChWraddr( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_260"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_260" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_260")))
#endif

void FREG_DMA_SetChWraddr_OffsetAddress( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_261"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_261" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_261")))
#endif

uint32_t FREG_DMA_GetmChWraddr( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWraddrMirror[index];
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_262"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_262" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_262")))
#endif

uint32_t FREG_DMA_GetmChWraddr_OffsetAddress( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_263"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_263" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_263")))
#endif

void FREG_DMA_SetChWrline( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_264"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_264" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_264")))
#endif

void FREG_DMA_SetChWrline_LoopIncrement( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_265"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_265" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_265")))
#endif

void FREG_DMA_SetChWrline_Lines( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_266"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_266" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_266")))
#endif

uint32_t FREG_DMA_GetmChWrline( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWrlineMirror[index];
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_267"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_267" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_267")))
#endif

uint32_t FREG_DMA_GetmChWrline_LoopIncrement( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_268"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_268" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_268")))
#endif

uint32_t FREG_DMA_GetmChWrline_Lines( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_269"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_269" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_269")))
#endif

void FREG_DMA_SetChWrinc( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_270"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_270" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_270")))
#endif

void FREG_DMA_SetChWrinc_LineIncrement( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_271"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_271" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_271")))
#endif

void FREG_DMA_SetChWrinc_LineLength( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_272"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_272" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_272")))
#endif

uint32_t FREG_DMA_GetmChWrinc( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWrincMirror[index];
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_273"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_273" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_273")))
#endif

uint32_t FREG_DMA_GetmChWrinc_LineIncrement( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_274"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_274" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_274")))
#endif

uint32_t FREG_DMA_GetmChWrinc_LineLength( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_275"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_275" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_275")))
#endif

void FREG_DMA_SetChWrlpaddr( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_276"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_276" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_276")))
#endif

void FREG_DMA_SetChWrlpaddr_LoopAddress( uint32_t index, uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_277"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_277" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_277")))
#endif

uint32_t FREG_DMA_GetmChWrlpaddr( uint32_t index )
{
    register uint32_t value;
    #if defined(FREG_CHECK_INDEX)
        if( index >= 6 )
            return( 0xFFFFFFFFUL );
    #endif
    value = FREG_DMA_ChWrlpaddrMirror[index];
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_278"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_278" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_278")))
#endif

uint32_t FREG_DMA_GetmChWrlpaddr_LoopAddress( uint32_t index )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_279"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_279" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_279")))
#endif

void FREG_DMA_SetCcirWbConfig( uint32_t value )
{
    FREG_DMA_CcirWbConfigMirror = value;
    #if FREG_DMA_ENABLE_DEBUG_PRINT == 1
        FREG_DMA_DEBUG_PRINT_FUNCTION( "[%s] 0x%08x <- 0x%08x\n", "FREG_DMA", FREG_DMA_CCIRWBCONFIG, value );
    #endif
    *((volatile uint32_t*)(FREG_DMA_CCIRWBCONFIG)) = value;
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_280"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_280" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_280")))
#endif

void FREG_DMA_SetCcirWbConfig_CcirWbEnable( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_281"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_281" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_281")))
#endif

void FREG_DMA_SetCcirWbConfig_SourceSelect( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_282"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_282" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_282")))
#endif

void FREG_DMA_SetCcirWbConfig_BankSize( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_283"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_283" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_283")))
#endif

void FREG_DMA_SetCcirWbConfig_ColumnWidth( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_284"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_284" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_284")))
#endif

void FREG_DMA_SetCcirWbConfig_ClockInvert( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_285"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_285" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_285")))
#endif

void FREG_DMA_SetCcirWbConfig_PalNtsc( uint32_t value )
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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_286"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_286" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_286")))
#endif

uint32_t FREG_DMA_GetmCcirWbConfig( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_287"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_287" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_287")))
#endif

uint32_t FREG_DMA_GetmCcirWbConfig_CcirWbEnable( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    value = ( value & 0x80000000UL ) >> 31;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_288"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_288" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_288")))
#endif

uint32_t FREG_DMA_GetmCcirWbConfig_SourceSelect( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    value = ( value & 0x02000000UL ) >> 25;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_289"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_289" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_289")))
#endif

uint32_t FREG_DMA_GetmCcirWbConfig_BankSize( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    value = ( value & 0x00000300UL ) >> 8;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_290"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_290" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_290")))
#endif

uint32_t FREG_DMA_GetmCcirWbConfig_ColumnWidth( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    value = ( value & 0x00000030UL ) >> 4;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_291"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_291" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_291")))
#endif

uint32_t FREG_DMA_GetmCcirWbConfig_ClockInvert( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    value = ( value & 0x00000002UL ) >> 1;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_292"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_292" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_292")))
#endif

uint32_t FREG_DMA_GetmCcirWbConfig_PalNtsc( void )
{
    register uint32_t value = FREG_DMA_CcirWbConfigMirror;
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_293"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_293" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_293")))
#endif

uint32_t FREG_DMA_GetCcirWbStatus( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRWBSTATUS));
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_294"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_294" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_294")))
#endif

uint32_t FREG_DMA_GetCcirWbStatus_LineNumber( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRWBSTATUS));
    value = ( value & 0x0003FF00UL ) >> 8;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_295"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_295" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_295")))
#endif

uint32_t FREG_DMA_GetCcirWbStatus_Vsync( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRWBSTATUS));
    value = ( value & 0x00000008UL ) >> 3;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_296"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_296" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_296")))
#endif

uint32_t FREG_DMA_GetCcirWbStatus_Hsync( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRWBSTATUS));
    value = ( value & 0x00000004UL ) >> 2;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_297"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_297" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_297")))
#endif

uint32_t FREG_DMA_GetCcirWbStatus_Field( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRWBSTATUS));
    value = ( value & 0x00000002UL ) >> 1;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code = ".text$$FREG_DMA_298"
#endif

#if defined(_ARC)
#pragma Code( ".text$$FREG_DMA_298" )
#endif

#if defined(__GNUC__)
__attribute__((section(".text$$FREG_DMA_298")))
#endif

uint32_t FREG_DMA_GetCcirWbStatus_Frame( void )
{
    register uint32_t value;
    value = *((volatile uint32_t*)(FREG_DMA_CCIRWBSTATUS));
    value = ( value & 0x00000001UL ) >> 0;
    return( value );
}

#if defined(__arm__) && !defined(__GNUC__)
#pragma arm section code
#endif

#if defined(_ARC)
#pragma Code()
#endif

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
void FREG_DMA_Init( void )
{
    uint32_t index = 0;
    FREG_DMA_IntMaskMirror |= ( ( 0 << 0 ) & 0x000003FFUL );
    FREG_DMA_IntClearMirror |= ( ( 0 << 0 ) & 0x000003FFUL );
    FREG_DMA_UsbIntMaskMirror |= ( ( 0 << 0 ) & 0x0000007FUL );
    FREG_DMA_UsbIntClearMirror |= ( ( 0 << 0 ) & 0x0000007FUL );
    FREG_DMA_BlockFillMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_UsbModeMirror |= ( ( 0 << 7 ) & 0x00000080UL );
    FREG_DMA_UsbModeMirror |= ( ( 0 << 5 ) & 0x00000060UL );
    FREG_DMA_UsbModeMirror |= ( ( 0 << 3 ) & 0x00000018UL );
    FREG_DMA_UsbModeMirror |= ( ( 0 << 1 ) & 0x00000006UL );
    FREG_DMA_UsbModeMirror |= ( ( 0 << 0 ) & 0x00000001UL );
    FREG_DMA_CcirHconfigMirror |= ( ( 0 << 16 ) & 0x01FF0000UL );
    FREG_DMA_CcirHconfigMirror |= ( ( 0 << 0 ) & 0x000001FFUL );
    FREG_DMA_CcirVconfigMirror |= ( ( 0 << 16 ) & 0x01FF0000UL );
    FREG_DMA_CcirVconfigMirror |= ( ( 0 << 0 ) & 0x000001FFUL );
    FREG_DMA_CsConfigMirror |= ( ( 0 << 16 ) & 0x00070000UL );
    FREG_DMA_CsConfigMirror |= ( ( 0 << 8 ) & 0x0000FF00UL );
    FREG_DMA_CsConfigMirror |= ( ( 0 << 2 ) & 0x00000004UL );
    FREG_DMA_CsConfigMirror |= ( ( 0 << 1 ) & 0x00000002UL );
    FREG_DMA_CsConfigMirror |= ( ( 0 << 0 ) & 0x00000001UL );
    FREG_DMA_CsYMirror |= ( ( 0 << 18 ) & 0x07FC0000UL );
    FREG_DMA_CsYMirror |= ( ( 0 << 9 ) & 0x0003FE00UL );
    FREG_DMA_CsYMirror |= ( ( 0 << 0 ) & 0x000001FFUL );
    FREG_DMA_CsCrMirror |= ( ( 0 << 18 ) & 0x07FC0000UL );
    FREG_DMA_CsCrMirror |= ( ( 0 << 9 ) & 0x0003FE00UL );
    FREG_DMA_CsCrMirror |= ( ( 0 << 0 ) & 0x000001FFUL );
    FREG_DMA_CsCbMirror |= ( ( 0 << 18 ) & 0x07FC0000UL );
    FREG_DMA_CsCbMirror |= ( ( 0 << 9 ) & 0x0003FE00UL );
    FREG_DMA_CsCbMirror |= ( ( 0 << 0 ) & 0x000001FFUL );
    FREG_DMA_CsOffsetMirror |= ( ( 0 << 20 ) & 0x3FF00000UL );
    FREG_DMA_CsOffsetMirror |= ( ( 0 << 10 ) & 0x000FFC00UL );
    FREG_DMA_CsOffsetMirror |= ( ( 0 << 0 ) & 0x000003FFUL );
    FREG_DMA_SrConfigMirror |= ( ( 0 << 3 ) & 0x00000008UL );
    FREG_DMA_SrConfigMirror |= ( ( 0 << 2 ) & 0x00000004UL );
    FREG_DMA_SrConfigMirror |= ( ( 0 << 0 ) & 0x00000003UL );
    FREG_DMA_SrMaskMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_SrCompareMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_SrReplaceMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_SrReplaceMaskMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_AudioConfigMirror |= ( ( 0 << 13 ) & 0x00002000UL );
    FREG_DMA_AudioConfigMirror |= ( ( 0 << 8 ) & 0x00001F00UL );
    FREG_DMA_AudioConfigMirror |= ( ( 0 << 7 ) & 0x00000080UL );
    FREG_DMA_AudioConfigMirror |= ( ( 0 << 6 ) & 0x00000040UL );
    FREG_DMA_AudioConfigMirror |= ( ( 0 << 5 ) & 0x00000020UL );
    FREG_DMA_AudioConfigMirror |= ( ( 0 << 4 ) & 0x00000010UL );
    FREG_DMA_AudioConfigMirror |= ( ( 0 << 3 ) & 0x00000008UL );
    FREG_DMA_AudioConfigMirror |= ( ( 0 << 2 ) & 0x00000004UL );
    FREG_DMA_AudioConfigMirror |= ( ( 0 << 1 ) & 0x00000002UL );
    FREG_DMA_AudioConfigMirror |= ( ( 0 << 0 ) & 0x00000001UL );
    FREG_DMA_CryptoConfigMirror |= ( ( 0 << 5 ) & 0x00000020UL );
    FREG_DMA_CryptoConfigMirror |= ( ( 0 << 4 ) & 0x00000010UL );
    FREG_DMA_CryptoConfigMirror |= ( ( 0 << 3 ) & 0x00000008UL );
    FREG_DMA_CryptoConfigMirror |= ( ( 0 << 2 ) & 0x00000004UL );
    FREG_DMA_CryptoConfigMirror |= ( ( 0 << 1 ) & 0x00000002UL );
    FREG_DMA_CryptoConfigMirror |= ( ( 0 << 0 ) & 0x00000001UL );
    FREG_DMA_CryptoK1MsbMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_CryptoK1LsbMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_CryptoK2MsbMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_CryptoK2LsbMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_CryptoK3MsbMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_CryptoK3LsbMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_IvMsbMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_IvLsbMirror |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChLengthMirror[index] |= ( ( 0 << 0 ) & 0x001FFFFFUL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChLladdrMirror[index] |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChRdaddrMirror[index] |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChRdlineMirror[index] |= ( ( 0 << 16 ) & 0x1FFF0000UL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChRdlineMirror[index] |= ( ( 0 << 0 ) & 0x000003FFUL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChRdincMirror[index] |= ( ( 0 << 16 ) & 0x1FFF0000UL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChRdincMirror[index] |= ( ( 0 << 0 ) & 0x00000FFFUL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChRdlpaddrMirror[index] |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChWraddrMirror[index] |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChWrlineMirror[index] |= ( ( 0 << 16 ) & 0x1FFF0000UL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChWrlineMirror[index] |= ( ( 0 << 0 ) & 0x000003FFUL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChWrincMirror[index] |= ( ( 0 << 16 ) & 0x1FFF0000UL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChWrincMirror[index] |= ( ( 0 << 0 ) & 0x00000FFFUL );
    for( index=0; index < 6; index++ )
        FREG_DMA_ChWrlpaddrMirror[index] |= ( ( 0 << 0 ) & 0xFFFFFFFFUL );
    FREG_DMA_CcirWbConfigMirror |= ( ( 0 << 31 ) & 0x80000000UL );
    FREG_DMA_CcirWbConfigMirror |= ( ( 0 << 25 ) & 0x02000000UL );
    FREG_DMA_CcirWbConfigMirror |= ( ( 0 << 8 ) & 0x00000300UL );
    FREG_DMA_CcirWbConfigMirror |= ( ( 0 << 4 ) & 0x00000030UL );
    FREG_DMA_CcirWbConfigMirror |= ( ( 0 << 1 ) & 0x00000002UL );
    FREG_DMA_CcirWbConfigMirror |= ( ( 0 << 0 ) & 0x00000001UL );
}
