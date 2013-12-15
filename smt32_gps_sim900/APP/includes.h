/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*
* Filename      : includes.h
* Version       : V1.10
* Programmer(s) : BAN
*********************************************************************************************************
*/

#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  <math.h>

#include  <ucos_ii.h>

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_mem.h>
#include  <lib_str.h>

#if (OS_VIEW_MODULE == DEF_ENABLED)
#include    <os_viewc.h>
#include    <os_view.h>
#endif

#if (GPS_MODULE == DEF_ENABLED)
#include    <GPS.h>
#endif

#include  <stm32f10x_conf.h>
#include  <stm32f10x_lib.h>
#include "eeprom.h"
#include "IIC_API.h"
#include "ASCII_Option.h"
#include "GT20L16P1Y.h"

#if APP_USE_CNLib == DEF_ENABLED
#include "CNLib15x16.h"
#endif

#include  <app_cfg.h>
#include  <bsp.h>

#if (TFT_MODULE	 == DEF_ENABLED)
#include "TFTDriver.h"
#endif

#if (GSM_MODULE == DEF_ENABLED)
#include "GSM.H"
#endif

//////
typedef struct {
    u16 buf_ok;
    u16 com_cnt;    
    char app_buf[3][512];
    
}com_app_t;
//////
#endif
