/**
  ******************************************************************************
  * @file    app.c
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   This file provides all the Application firmware functions.
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

/* Includes ------------------------------------------------------------------ */
#include "usbd_msc_core.h"
#include "usbd_msc_hid_core.h"
#include  "usbd_hid_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */


/** @defgroup APP_MSC
  * @brief Mass storage application module
  * @{
  */
extern USBD_Class_cb_TypeDef USBD_MSC_HID_cb;
/** @defgroup APP_MSC_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup APP_MSC_Private_Defines
  * @{
  */

/**
  * @}
  */


/** @defgroup APP_MSC_Private_Macros
  * @{
  */
/**
  * @}
  */


/** @defgroup APP_MSC_Private_Variables
  * @{
  */

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined ( __ICCARM__ )      /* !< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif                          /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

__ALIGN_BEGIN USB_OTG_CORE_HANDLE USB_OTG_dev __ALIGN_END;

/**
  * @}
  */


/** @defgroup APP_MSC_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */


/** @defgroup APP_MSC_Private_Functions
  * @{
  */

void delay_ms(uint32_t cnt)
{
  uint32_t i;
  for (i=0; i<cnt; i++)
  {
    ;
  }
}

/**
* @brief  USBD_HID_GetPos
* @param  None
* @retval Pointer to report
*/
static void USBD_HID_GetPos(void)
{
  uint8_t Mouse_Buffer[4] = {0, 0, 0, 0};
  uint8_t X = 0, Y = 0; 
  static uint8_t Sens = 0;
  static uint8_t Step = 0;
  delay_ms(0xFFFFF);
  if (Step == 5)
  {
    Step = 0;
    Sens++;
  }
  
  if(Sens == 0)
  {
    X = Step++;
    Y = 0;
  }
  
  if(Sens == 1)
  {
    Y = Step++;
    X = 0;
  }      
  if (Sens==2)
  {
    X = 256 - Step++;
    Y = 0;
  } 
  
  if (Sens == 3)
  {
    Y = 256 - Step++;
    X = 0;
  }
  
  if (Sens == 4)
  { 
    Sens = 0;
    X = 0;
    Y = 0;
  }
  
  Mouse_Buffer[0] = 0;
  Mouse_Buffer[1] = X;
  Mouse_Buffer[2] = Y;
  Mouse_Buffer[3] = 0;

  USBD_HID_SendReport(&USB_OTG_dev, Mouse_Buffer, 4);
}

/**
  * @brief  Program entry point
  * @param  None
  * @retval None
  */
int main(void)
{
    /* !< At this stage the microcontroller clock setting is already configured,
     * this is done through SystemInit() function which is called from startup
     * file (startup_stm32fxxx_xx.s) before to branch to application main. To
     * reconfigure the default setting of SystemInit() function, refer to
     * system_stm32fxxx.c file */

    USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS
              USB_OTG_HS_CORE_ID,
#else
              USB_OTG_FS_CORE_ID,
#endif
              &USR_desc, &USBD_MSC_HID_cb, &USR_cb);

    while (1)
    {
        USBD_HID_GetPos();
    }
}

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t * file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line
     * number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     * line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
