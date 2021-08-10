/*!
*****************************************************************************
**
** \file        ./subsystems/paint/freetype2/src/fontnotify.c
** 
** \brief       FONT Driver FreeType notification mechanism
**
** This file contains the implementation of the FONT driver FreeType
** notification mechanism API for the case that FreeType IS present.
**
** \attention   THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU MICROELECTRONICS
**              ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**              OMMISSIONS.
**
** (C) Copyright 2007 - 2008 by Fujitsu Microelectronics Europe GmbH
**
*****************************************************************************
*/

#include <font/font_ft_notify.h>

//***************************************************************************
//***************************************************************************
//** API Functions
//***************************************************************************
//***************************************************************************

/*!
*****************************************************************************
** \brief   Notify the FONT driver that the FreeType library is present.
*****************************************************************************
*/
void FONT_FreeTypeNotify(void)
{
    fontFreeTypeAvailable = 1;
}
