/*!
*****************************************************************************
**
** \file        ./fapi/sys/src/sys_init_customer.h
**
** \brief       Fujitsu DEVKIT specific board configuration for MB86H60.
**
** This file contains the settings for the Fujitsu DEVKIT board of MB86H60.
** It defines all available drivers and their required parameter settings.
** The file is included by sys_init.h if the following is set within the
** main configuration file Config.mak:
** - DECODER = MB86H60
** - BOARD = BOARD_CUSTOMER
**
** \note
** if the 'FAPI_GPIO_SetPinFunction()' calls are executed from a 'boards-driver'
** put the macro #FAPI_SYS_INIT_USE_BOARDS to 1 in the file where 'sys_init.h'
** is included (actually before the include-line)
**
** \attention   THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU MICROELECTRONICS
**              ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**              OMMISSIONS.
**
** (C) Copyright 2006-2009 by Fujitsu Microelectronics Europe GmbH
**
*****************************************************************************
*/

#ifndef FAPI_SYS_INIT_MB86H60_DEVKIT_H

    #define FAPI_SYS_INIT_MB86H60_DEVKIT_H

    #include <stdlib.h>
    #include <boot/cpu_mode.h>

    //***********************************************************************
    //***********************************************************************
    //** Define all needed hardware frequencies
    //***********************************************************************
    //***********************************************************************

    #define FAPI_SYS_CLOCK_FREQUENCY              162000000
    #define FAPI_SYS_TIMER_FREQUENCY               81000000
    #define FAPI_SYS_UART_FREQUENCY                81000000

    //***********************************************************************
    //***********************************************************************
    //** Define the memory mapping for all hardware blocks
    //***********************************************************************
    //***********************************************************************

    #define FAPI_SYS_MMAP_DDR_START              0x20000000UL //
    #define FAPI_SYS_MMAP_DDR_BYTES              0x04000000UL // 64 MB
    #define FAPI_SYS_MMAP_VIDEO_RESERVED_START   0x20000000UL //
    #define FAPI_SYS_MMAP_VIDEO_RESERVED_BYTES   0x014C0000UL // 21 MB
    #define FAPI_SYS_MMAP_BM01_GLOBAL_OFFSET     0x20000000UL //
    #define FAPI_SYS_MMAP_BM23_GLOBAL_OFFSET     0x20000000UL //
    #define FAPI_SYS_MMAP_BM_I2S0_START          0x218C0000UL //
    #define FAPI_SYS_MMAP_BM_I2S0_BYTES          0x00010000UL // 64 KB
    #define FAPI_SYS_MMAP_BM_I2S1_START          0x218D0000UL //
    #define FAPI_SYS_MMAP_BM_I2S1_BYTES          0x00010000UL // 64 KB
    #define FAPI_SYS_MMAP_BM_I2S2_START          0x218E0000UL //
    #define FAPI_SYS_MMAP_BM_I2S2_BYTES          0x00010000UL // 64 KB
    #define FAPI_SYS_MMAP_BM_I2S3_START          0x218F0000UL //
    #define FAPI_SYS_MMAP_BM_I2S3_BYTES          0x00010000UL // 64 KB
    #define FAPI_SYS_MMAP_BM_SPDIF_START         0x21900000UL //
    #define FAPI_SYS_MMAP_BM_SPDIF_BYTES         0x00010000UL // 64 KB
    #define FAPI_SYS_MMAP_BM_TELETEXT_START      0x21910000UL //
    #define FAPI_SYS_MMAP_BM_TELETEXT_BYTES      0x00010000UL // 64 KB
    #define FAPI_SYS_MMAP_BM_AUDIO_STREAM_START  0x21920000UL //
    #define FAPI_SYS_MMAP_BM_AUDIO_STREAM_BYTES  0x00010000UL // 64 KB
    #define FAPI_SYS_MMAP_BM_SECTION_START       0x21530000UL //
    #define FAPI_SYS_MMAP_BM_SECTION_BYTES       0x00100000UL // 1 MB
    #define FAPI_SYS_MMAP_BM_SUBTITLE_START      0x21630000UL //
    #define FAPI_SYS_MMAP_BM_SUBTITLE_BYTES      0x00010000UL // 64 KB
    #define FAPI_SYS_MMAP_SPARE_RESERVED_START   0x21640000UL //
    #define FAPI_SYS_MMAP_SPARE_RESERVED_BYTES   0x000C0000UL // 768 KB
    #define FAPI_SYS_MMAP_ARM_APPLICATION_START  0x21700000UL // 1MB border (MMU)!
    #define FAPI_SYS_MMAP_ARM_APPLICATION_BYTES  0x02500000UL // 37 MB
    #define FAPI_SYS_MMAP_ARC_APPLICATION_START  0x23C00000UL //
    #define FAPI_SYS_MMAP_ARC_APPLICATION_BYTES  0x00400000UL // 4 MB

    //***********************************************************************
    //***********************************************************************
    //** Define all needed hardware register blocks
    //***********************************************************************
    //***********************************************************************

    #ifndef FAPI_SYS_INIT_REG_AO
    #define FAPI_SYS_INIT_REG_AO        0
    #endif

    #ifndef FAPI_SYS_INIT_REG_BM
    #define FAPI_SYS_INIT_REG_BM        1
    #endif

    #ifndef FAPI_SYS_INIT_REG_CLKPWR
    #define FAPI_SYS_INIT_REG_CLKPWR    1
    #endif

    #ifndef FAPI_SYS_INIT_REG_DMA
    #define FAPI_SYS_INIT_REG_DMA       1
    #endif

    #ifndef FAPI_SYS_INIT_REG_ETH
    #define FAPI_SYS_INIT_REG_ETH       0
    #endif

    #ifndef FAPI_SYS_INIT_REG_FPC
    #define FAPI_SYS_INIT_REG_FPC       0
    #endif

    #ifndef FAPI_SYS_INIT_REG_GPIO
    #define FAPI_SYS_INIT_REG_GPIO      1
    #endif

    #ifndef FAPI_SYS_INIT_REG_I2C
    #define FAPI_SYS_INIT_REG_I2C       1
    #endif

    #ifndef FAPI_SYS_INIT_REG_ICC
    #define FAPI_SYS_INIT_REG_ICC       0
    #endif

    #ifndef FAPI_SYS_INIT_REG_IR
    #define FAPI_SYS_INIT_REG_IR        0
    #endif

    #ifndef FAPI_SYS_INIT_REG_IRQ_CTRL
    #define FAPI_SYS_INIT_REG_IRQ_CTRL  1
    #endif

    #ifndef FAPI_SYS_INIT_REG_SFLASH
    #define FAPI_SYS_INIT_REG_SFLASH    1
    #endif

    #ifndef FAPI_SYS_INIT_REG_SSP
    #define FAPI_SYS_INIT_REG_SSP       0
    #endif

    #ifndef FAPI_SYS_INIT_REG_TSD
    #define FAPI_SYS_INIT_REG_TSD       1
    #endif

    #ifndef FAPI_SYS_INIT_REG_UART
    #define FAPI_SYS_INIT_REG_UART      1
    #endif

    #ifndef FAPI_SYS_INIT_REG_UPI
    #define FAPI_SYS_INIT_REG_UPI       1
    #endif

    #ifndef FAPI_SYS_INIT_REG_VE
    #define FAPI_SYS_INIT_REG_VE        1
    #endif

    #ifndef FAPI_SYS_INIT_REG_VE_HD
    #define FAPI_SYS_INIT_REG_VE_HD     1
    #endif

    #ifndef FAPI_SYS_INIT_REG_VIC
    #define FAPI_SYS_INIT_REG_VIC       1
    #endif

    #ifndef FAPI_SYS_INIT_REG_VO
    #define FAPI_SYS_INIT_REG_VO        1
    #endif

    //***********************************************************************
    //***********************************************************************
    //** Define all needed drivers
    //** - some might already be excluded from application
    //***********************************************************************
    //***********************************************************************

    #ifndef FAPI_SYS_INIT_USE_BOARDS
    #define FAPI_SYS_INIT_USE_BOARDS    0
    #endif

    #if 1
    #define FAPI_SYS_INIT_MMU           1
    #endif

    #if 1
    #define FAPI_SYS_INIT_GPIO          1
    #endif

    #ifndef FAPI_SYS_INIT_AUDEC
    #define FAPI_SYS_INIT_AUDEC         0
    #endif

    #ifndef FAPI_SYS_INIT_AUOUT
    #define FAPI_SYS_INIT_AUOUT         0
    #endif

    #ifndef FAPI_SYS_INIT_ATA
    #define FAPI_SYS_INIT_ATA                   0
    #endif

    #ifndef FAPI_SYS_INIT_BM
    #define FAPI_SYS_INIT_BM            1
    #endif

    #ifndef FAPI_SYS_INIT_BOOT
    #define FAPI_SYS_INIT_BOOT          1
    #endif

    #ifndef FAPI_SYS_INIT_CLKPWR
    #define FAPI_SYS_INIT_CLKPWR        1
    #endif

    #ifndef FAPI_SYS_INIT_DESC
    #define FAPI_SYS_INIT_DESC                  0
    #endif

    #ifndef FAPI_SYS_INIT_DMA
    #define FAPI_SYS_INIT_DMA           1
    #endif

    #ifndef FAPI_SYS_INIT_ETH
    #define FAPI_SYS_INIT_ETH                   0
    #endif

    #ifndef FAPI_SYS_INIT_FPC
    #define FAPI_SYS_INIT_FPC                   0
    #endif

    #ifndef FAPI_SYS_INIT_FLASH
    #define FAPI_SYS_INIT_FLASH         1
    #endif

    #ifndef FAPI_SYS_INIT_GPREG
    #define FAPI_SYS_INIT_GPREG         1
    #endif

    #ifndef FAPI_SYS_INIT_HDMI
    #define FAPI_SYS_INIT_HDMI          1
    #endif

    #ifndef FAPI_SYS_INIT_I2C
    #define FAPI_SYS_INIT_I2C           1
    #endif

    #ifndef FAPI_SYS_INIT_ICC
    #define FAPI_SYS_INIT_ICC           0
    #endif

    #ifndef FAPI_SYS_INIT_INTR
    #define FAPI_SYS_INIT_INTR          1
    #endif

    #ifndef FAPI_SYS_INIT_IR
    #define FAPI_SYS_INIT_IR            0
    #endif

    #ifndef FAPI_SYS_INIT_SSP
    #define FAPI_SYS_INIT_SSP           0
    #endif

    #ifndef FAPI_SYS_INIT_SYNC
    #define FAPI_SYS_INIT_SYNC          1
    #endif

    #ifndef FAPI_SYS_INIT_TIMER
    #define FAPI_SYS_INIT_TIMER         1
    #endif

    #ifndef FAPI_SYS_INIT_TSD
    #define FAPI_SYS_INIT_TSD           1
    #endif

    #ifndef FAPI_SYS_INIT_TTX
    #define FAPI_SYS_INIT_TTX           0
    #endif

    #ifndef FAPI_SYS_INIT_UART
    #define FAPI_SYS_INIT_UART          1
    #endif

    #ifndef FAPI_SYS_INIT_UPI
    #define FAPI_SYS_INIT_UPI           1
    #endif

    #ifndef FAPI_SYS_INIT_USB
    #define FAPI_SYS_INIT_USB           1
    #endif

    #ifndef FAPI_SYS_INIT_VIDEC
    #define FAPI_SYS_INIT_VIDEC         1
    #endif

    #ifndef FAPI_SYS_INIT_VIENC
    #define FAPI_SYS_INIT_VIENC         1
    #endif

    #ifndef FAPI_SYS_INIT_VISCALE
    #define FAPI_SYS_INIT_VISCALE       1
    #endif

    // to be continued ...

    //***********************************************************************
    //***********************************************************************
    //** Set board specific parameters
    //** There must be a macro for each driver with board specific settings.
    //** This macro has to be called by FAPI_SYS_SET_BOARD_PARAMS below.
    //***********************************************************************
    //***********************************************************************

    #if FAPI_SYS_INIT_UART == 1
        #define FAPI_SYS_UART_SET_BOARD_PARAMS                       \
            FAPI_UART_SetClockFrequency( FAPI_SYS_UART_FREQUENCY );  \
            FAPI_UART_SetBaudrate( 0, FAPI_UART_BAUDRATE_115200 );   \
            FAPI_UART_SetDataBits( 0, FAPI_UART_DATABITS_8 );        \
            FAPI_UART_SetStopBits( 0, FAPI_UART_STOPBITS_1 );        \
            FAPI_UART_SetParity( 0, FAPI_UART_PARITY_NONE );         \
            FAPI_UART_SetFlowControl( 0, FAPI_UART_FLOWCTRL_NONE );  \
            FAPI_UART_SetStdioBlockIndex( 0 );
    #else
        #define FAPI_SYS_UART_SET_BOARD_PARAMS
    #endif

    #if FAPI_SYS_INIT_TIMER == 1
        #define FAPI_SYS_TIMER_SET_BOARD_PARAMS                           \
            FAPI_TIMER_SetClockFrequency( FAPI_SYS_TIMER_FREQUENCY );
    #else
        #define FAPI_SYS_TIMER_SET_BOARD_PARAMS
    #endif

    #if ( FAPI_SYS_INIT_GPIO == 1 ) && ( FAPI_SYS_INIT_USE_BOARDS == 0 )
        #define FAPI_SYS_GPIO_SET_BOARD_PARAMS                            \
        FAPI_GPIO_SetPinFunction(  2, FAPI_GPIO_IN_TS_B_DATA_7         ); \
        FAPI_GPIO_SetPinFunction(  3, FAPI_GPIO_IN_TS_B_DATA_6         ); \
        FAPI_GPIO_SetPinFunction(  4, FAPI_GPIO_IN_TS_B_DATA_3         ); \
        FAPI_GPIO_SetPinFunction(  5, FAPI_GPIO_IN_TS_A_DATA_7         ); \
        FAPI_GPIO_SetPinFunction(  6, FAPI_GPIO_IN_TS_A_DATA_6         ); \
        FAPI_GPIO_SetPinFunction(  7, FAPI_GPIO_IN_TS_B_DATA_5         ); \
        FAPI_GPIO_SetPinFunction(  8, FAPI_GPIO_IN_TS_B_PSTART         ); \
        FAPI_GPIO_SetPinFunction(  9, FAPI_GPIO_IN_TS_A_DATA_5         ); \
        FAPI_GPIO_SetPinFunction( 10, FAPI_GPIO_IN_TS_A_DATA_4         ); \
        FAPI_GPIO_SetPinFunction( 11, FAPI_GPIO_IN_TS_B_DATA_2         ); \
        FAPI_GPIO_SetPinFunction( 12, FAPI_GPIO_IN_TS_B_DATA_1         ); \
        FAPI_GPIO_SetPinFunction( 13, FAPI_GPIO_IN_TS_B_DATA_4         ); \
        FAPI_GPIO_SetPinFunction( 14, FAPI_GPIO_IN_TS_B_DATA_0         ); \
        FAPI_GPIO_SetPinFunction( 15, FAPI_GPIO_IN_TS_A_DATA_3         ); \
        FAPI_GPIO_SetPinFunction( 16, FAPI_GPIO_IN_TS_A_DATA_2         ); \
        FAPI_GPIO_SetPinFunction( 17, FAPI_GPIO_IN_TS_A_DATA_1         ); \
        FAPI_GPIO_SetPinFunction( 18, FAPI_GPIO_IN_TS_B_EN             ); \
        FAPI_GPIO_SetPinFunction( 19, FAPI_GPIO_IN_TS_A_DATA_0         ); \
        FAPI_GPIO_SetPinFunction( 20, FAPI_GPIO_IN_TS_A_EN             ); \
        FAPI_GPIO_SetPinFunction( 21, FAPI_GPIO_IN_TS_A_PSTART         ); \
        FAPI_GPIO_SetPinFunction( 22, FAPI_GPIO_IN_TS_B_CLK            ); \
        FAPI_GPIO_SetPinFunction( 23, FAPI_GPIO_IN_TS_A_CLK            ); \
        FAPI_GPIO_SetPinFunction( 24, FAPI_GPIO_IN_GENERIC_SW_FUNCTION ); \
        FAPI_GPIO_SetPinFunction( 25, FAPI_GPIO_OUT_0           ); \
        FAPI_GPIO_SetPinFunction( 26, FAPI_GPIO_OUT_0          ); \
        FAPI_GPIO_SetPinFunction( 27, FAPI_GPIO_OUT_0            ); \
        FAPI_GPIO_SetPinFunction( 28, FAPI_GPIO_OUT_0           ); \
        FAPI_GPIO_SetPinFunction( 29, FAPI_GPIO_OUT_0           ); \
        FAPI_GPIO_SetPinFunction( 30, FAPI_GPIO_OUT_0           ); \
        FAPI_GPIO_SetPinFunction( 31, FAPI_GPIO_OUT_AD3_DATA           ); \
        FAPI_GPIO_SetPinFunction( 32, FAPI_GPIO_IN_UART0_DATA          ); \
        FAPI_GPIO_SetPinFunction( 33, FAPI_GPIO_OUT_UART0_DATA         ); \
        FAPI_GPIO_SetPinFunction( 34, FAPI_GPIO_OUT_UART1_DATA         ); \
        FAPI_GPIO_SetPinFunction( 35, FAPI_GPIO_IN_UART1_DATA          ); \
        FAPI_GPIO_SetPinFunction( 36, FAPI_GPIO_OUT_SPDIF_OUT          ); \
        FAPI_GPIO_SetPinFunction( 37, FAPI_GPIO_BIDI_I2C0_DATA         ); \
        FAPI_GPIO_SetPinFunction( 38, FAPI_GPIO_BIDI_I2C0_CLK          ); \
        FAPI_GPIO_SetPinFunction( 39, FAPI_GPIO_RESERVED_39            ); \
        FAPI_GPIO_SetPinFunction( 41, FAPI_GPIO_RESERVED_41            ); \
        FAPI_GPIO_SetPinFunction( 42, FAPI_GPIO_OUT_0             ); \
        FAPI_GPIO_SetPinFunction( 43, FAPI_GPIO_IN_SFLASH_DATA         ); \
        FAPI_GPIO_SetPinFunction( 44, FAPI_GPIO_OUT_SFLASH_HOLD        ); \
        FAPI_GPIO_SetPinFunction( 45, FAPI_GPIO_OUT_SFLASH_CS          ); \
        FAPI_GPIO_SetPinFunction( 46, FAPI_GPIO_OUT_SFLASH_CLK         ); \
        FAPI_GPIO_SetPinFunction( 47, FAPI_GPIO_OUT_SFLASH_DATA        ); \
        FAPI_GPIO_SetPinFunction( 64, FAPI_GPIO_OUT_UPI_ADDRX0         ); \
        FAPI_GPIO_SetPinFunction( 65, FAPI_GPIO_OUT_UPI_ADDRX1         ); \
        FAPI_GPIO_SetPinFunction( 66, FAPI_GPIO_OUT_UPI_ADDRX2         ); \
        FAPI_GPIO_SetPinFunction( 67, FAPI_GPIO_OUT_UPI_ADDRX3         ); \
        FAPI_GPIO_SetPinFunction( 73, FAPI_GPIO_IN_SSP0_DATA           ); \
        FAPI_GPIO_SetPinFunction( 70, FAPI_GPIO_OUT_SSP0_CLK           ); \
        FAPI_GPIO_SetPinFunction( 71, FAPI_GPIO_OUT_SSP0_FFS           ); \
        FAPI_GPIO_SetPinFunction( 84, FAPI_GPIO_OUT_SSP0_DATA          ); \
        FAPI_GPIO_SetPinFunction( 82, FAPI_GPIO_OUT_ICC0_RST           ); \
        FAPI_GPIO_SetPinFunction( 83, FAPI_GPIO_OUT_ICC0_VCC           ); \
        FAPI_GPIO_SetPinFunction( 69, FAPI_GPIO_OUT_ICC0_VPEN          ); \
        FAPI_GPIO_SetPinFunction( 80, FAPI_GPIO_IN_IR_DATA/*0x10d*/          ); \
        FAPI_GPIO_SetPinFunction( 85, FAPI_GPIO_IN_ICC0_IN             ); \
        FAPI_GPIO_SetPinFunction( 86, FAPI_GPIO_OUT_ICC0_CLK           ); \
        FAPI_GPIO_SetPinFunction( 87, FAPI_GPIO_IN_ICC0_DATA           ); \
        FAPI_GPIO_SetPinFunction( 88, FAPI_GPIO_OUT_ICC0_DATA          ); \
        FAPI_GPIO_SetPinFunction( 89, FAPI_GPIO_OUT_ICC1_RST           ); \
        FAPI_GPIO_SetPinFunction( 90, FAPI_GPIO_OUT_ICC1_VCC           ); \
        FAPI_GPIO_SetPinFunction( 91, FAPI_GPIO_OUT_ICC1_VPEN          ); \
        FAPI_GPIO_SetPinFunction( 92, FAPI_GPIO_IN_ICC1_IN             ); \
        FAPI_GPIO_SetPinFunction( 93, FAPI_GPIO_OUT_ICC1_CLK           ); \
        FAPI_GPIO_SetPinFunction( 94, FAPI_GPIO_IN_ICC1_DATA           ); \
        FAPI_GPIO_SetPinFunction( 95, FAPI_GPIO_OUT_ICC1_DATA          );
    #else
        #define FAPI_SYS_GPIO_SET_BOARD_PARAMS
    #endif

    #if FAPI_SYS_INIT_INTR == 1
        #define FAPI_SYS_INTR_SET_BOARD_PARAMS                                     \
            FAPI_INTR_SetReceiver( FAPI_INTR_TIMER0   , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_TIMER1   , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_TIMER2   , FAPI_INTR_RECEIVER_NONE ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_UART0    , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_UART1    , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_SSP0     , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_SSP1     , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_ICC0     , FAPI_INTR_RECEIVER_NONE ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_ICC1     , FAPI_INTR_RECEIVER_NONE ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_FPC_IR   , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_I2C0     , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_I2C1     , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_I2C2     , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_AMCPU_AXP, FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_GPIO     , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_DMA      , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_USB      , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_ETH      , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_AUDIO    , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_BM01     , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_BM23     , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_TSD01    , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_TSD23    , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_DISP0    , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_DISP1    , FAPI_INTR_RECEIVER_ARM  ); \
            FAPI_INTR_SetReceiver( FAPI_INTR_DISP2    , FAPI_INTR_RECEIVER_NONE ); \
            FAPI_INTR_SetPriority( FAPI_INTR_TIMER0   , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_TIMER1   , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_TIMER2   , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_UART0    , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_UART1    , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_SSP0     , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_SSP1     , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_ICC0     , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_ICC1     , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_FPC_IR   , FAPI_INTR_PRIORITY_LOW  ); \
            FAPI_INTR_SetPriority( FAPI_INTR_I2C0     , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_I2C1     , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_I2C2     , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_AMCPU_AXP, FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_GPIO     , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_DMA      , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_USB      , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_ETH      , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_AUDIO    , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_BM01     , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_BM23     , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_TSD01    , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_TSD23    , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_DISP0    , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_DISP1    , FAPI_INTR_PRIORITY_HIGH ); \
            FAPI_INTR_SetPriority( FAPI_INTR_DISP2    , FAPI_INTR_PRIORITY_HIGH );
    #else
        #define FAPI_SYS_INTR_SET_BOARD_PARAMS
    #endif

    #if FAPI_SYS_INIT_BM == 1
        #define FAPI_SYS_BM_SET_BOARD_PARAMS                                                                                                                                                \
            FAPI_BM_SetGlobalOffset(FAPI_BM0, FAPI_SYS_MMAP_BM01_GLOBAL_OFFSET);                                                                                                            \
            FAPI_BM_SetGlobalOffset(FAPI_BM2, FAPI_SYS_MMAP_BM23_GLOBAL_OFFSET);                                                                                                            \
                                                                                                                                                                                            \
            FAPI_BM_SetBufferPurpose(FAPI_BM_I2S0_BUFFER,         FAPI_BM0, (FAPI_SYS_MMAP_BM_I2S0_START         - FAPI_SYS_MMAP_BM01_GLOBAL_OFFSET), FAPI_SYS_MMAP_BM_I2S0_BYTES);         \
            FAPI_BM_SetBufferPurpose(FAPI_BM_I2S1_BUFFER,         FAPI_BM0, (FAPI_SYS_MMAP_BM_I2S1_START         - FAPI_SYS_MMAP_BM01_GLOBAL_OFFSET), FAPI_SYS_MMAP_BM_I2S1_BYTES);         \
            FAPI_BM_SetBufferPurpose(FAPI_BM_I2S2_BUFFER,         FAPI_BM0, (FAPI_SYS_MMAP_BM_I2S2_START         - FAPI_SYS_MMAP_BM01_GLOBAL_OFFSET), FAPI_SYS_MMAP_BM_I2S2_BYTES);         \
            FAPI_BM_SetBufferPurpose(FAPI_BM_I2S3_BUFFER,         FAPI_BM0, (FAPI_SYS_MMAP_BM_I2S3_START         - FAPI_SYS_MMAP_BM01_GLOBAL_OFFSET), FAPI_SYS_MMAP_BM_I2S3_BYTES);         \
            FAPI_BM_SetBufferPurpose(FAPI_BM_SPDIF_BUFFER,        FAPI_BM0, (FAPI_SYS_MMAP_BM_SPDIF_START        - FAPI_SYS_MMAP_BM01_GLOBAL_OFFSET), FAPI_SYS_MMAP_BM_SPDIF_BYTES);        \
            FAPI_BM_SetBufferPurpose(FAPI_BM_TELETEXT_BUFFER,     FAPI_BM0, (FAPI_SYS_MMAP_BM_TELETEXT_START     - FAPI_SYS_MMAP_BM01_GLOBAL_OFFSET), FAPI_SYS_MMAP_BM_TELETEXT_BYTES);     \
            FAPI_BM_SetBufferPurpose(FAPI_BM_AUDIO_STREAM_BUFFER, FAPI_BM0, (FAPI_SYS_MMAP_BM_AUDIO_STREAM_START - FAPI_SYS_MMAP_BM01_GLOBAL_OFFSET), FAPI_SYS_MMAP_BM_AUDIO_STREAM_BYTES);
    #else
        #define FAPI_SYS_BM_SET_BOARD_PARAMS
    #endif

    #if FAPI_SYS_INIT_FLASH == 1
        #define FAPI_SYS_FLASH_SET_BOARD_PARAMS \
            FAPI_FLASH_SetEnable(FAPI_FLASH_DEVICE_PFLASH, 1);   \
            FAPI_FLASH_SetEnable(FAPI_FLASH_DEVICE_SFLASH, 1);   \
            FAPI_FLASH_SetUpiIndex(FAPI_FLASH_DEVICE_PFLASH, 0);
    #else
        #define FAPI_SYS_FLASH_SET_BOARD_PARAMS
    #endif

    #if FAPI_SYS_INIT_AUOUT == 1
        #define FAPI_SYS_AUOUT_SET_BOARD_PARAMS               \
            FAPI_AUOUT_SetSpeakers(FAPI_AUOUT_I2S_0,          \
                                   FAPI_AUOUT_SPEAKER_LEFT,   \
                                   FAPI_AUOUT_SPEAKER_RIGHT);

    #else
        #define FAPI_SYS_AUOUT_SET_BOARD_PARAMS
    #endif


    #if FAPI_SYS_INIT_VIDEC == 1
        #define FAPI_SYS_VIDEC_SET_BOARD_PARAMS                        \
            func_21c42e8c(0x20000000, 0x4000000, 0x40000000, 0x4000000);\
            FAPI_VIDEC_PreInit(FAPI_VIDEC_DECODER_NUMBER_0, FAPI_SYS_MMAP_VIDEO_RESERVED_START);
    #else
        #define FAPI_SYS_VIDEC_SET_BOARD_PARAMS
    #endif



    #define FAPI_SYS_SET_BOARD_PARAMS    \
        atexit( CPU_Polling );           \
        FAPI_SYS_UART_SET_BOARD_PARAMS   \
        FAPI_SYS_TIMER_SET_BOARD_PARAMS  \
        FAPI_SYS_GPIO_SET_BOARD_PARAMS   \
        FAPI_SYS_INTR_SET_BOARD_PARAMS   \
        FAPI_SYS_BM_SET_BOARD_PARAMS     \
        FAPI_SYS_FLASH_SET_BOARD_PARAMS  \
        FAPI_SYS_AUOUT_SET_BOARD_PARAMS  \
        FAPI_SYS_VIDEC_SET_BOARD_PARAMS

#define APP_SET_BOARD_PARAMS \
\
   func_21c0ef00(0, 0, 2);\
\
   FAPI_MMU_SetSegmentSizeMBytes(0, 12); \
   FAPI_MMU_SetSegmentSizeMBytes(1, 14); \
\
   func_21c0eee8(0);


#endif
