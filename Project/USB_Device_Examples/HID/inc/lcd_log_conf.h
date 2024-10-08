/**
  ******************************************************************************
  * @file    lcd_log_conf.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   lcd_log configuration template file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      <http://www.st.com/SLA0044>
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef  __LCD_LOG_CONF_H__
#define  __LCD_LOG_CONF_H__
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm324xg_eval_lcd.h"


/** @addtogroup LCD_LOG
  * @{
  */
  
/** @defgroup LCD_LOG
  * @brief This file is the 
  * @{
  */ 


/** @defgroup LCD_LOG_CONF_Exported_Defines
  * @{
  */ 

/* Comment the line below to disable the scroll back and forward features */
/* #define     LCD_SCROLL_ENABLED */
            
/* Define the LCD default text color */
#define     LCD_LOG_DEFAULT_COLOR    White

/* Define the display window settings */
#define     YWINDOW_MIN         3
#define     YWINDOW_SIZE        13
#define     XWINDOW_MAX         100

/* Define the cache depth */
#define     CACHE_SIZE          100

/** @defgroup LCD_LOG_CONF_Exported_TypesDefinitions
  * @{
  */
/**
  * @}
  */ 


/** @defgroup LCD_LOG_Exported_Macros
  * @{
  */ 


/**
  * @}
  */ 

/** @defgroup LCD_LOG_CONF_Exported_Variables
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup LCD_LOG_CONF_Exported_FunctionsPrototype
  * @{
  */ 

/**
  * @}
  */ 


#endif /* __LCD_LOG_CONF_H__ */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
