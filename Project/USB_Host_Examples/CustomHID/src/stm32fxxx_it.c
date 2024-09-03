/**
  ******************************************************************************
  * @file    stm32fxxx_it.c
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   This file includes the interrupt handlers for the application
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
#include "usb_bsp.h"
#include "usb_hcd_int.h"
#include "usbh_core.h"
#include "usbh_ioreq.h"
#include "usbh_hid_core.h"
#include "stm32fxxx_it.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
extern USB_OTG_CORE_HANDLE USB_OTG_Core_dev;
extern USBH_HOST USB_Host;
extern HID_Machine_TypeDef HID_Machine;
/* Private function prototypes ----------------------------------------------- */
extern void USB_OTG_BSP_TimerIRQ(void);
extern uint32_t suspend_flag;
uint8_t arr[2] = {0x31, 0x32};
/* Private functions --------------------------------------------------------- */

/******************************************************************************/
/* Cortex-M Processor Exceptions Handlers */
/******************************************************************************/
/**
  * @brief  NMI_Handler
  *         This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  HardFault_Handler
  *         This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  MemManage_Handler
  *         This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  BusFault_Handler
  *         This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  UsageFault_Handler
  *         This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  SVC_Handler
  *         This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  DebugMon_Handler
  *         This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


/**
  * @brief  PendSV_Handler
  *         This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  TIM2_IRQHandler
  *         This function handles Timer2 Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
    USB_OTG_BSP_TimerIRQ();
}


/**
  * @brief  OTG_FS_IRQHandler
  *          This function handles USB-On-The-Go FS global interrupt request.
  *          requests.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_FS
    void OTG_FS_IRQHandler(void)
#else
    void OTG_HS_IRQHandler(void)
#endif
{
    USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);
}

#if defined (USB_OTG_FS_LOW_PWR_MGMT_SUPPORT) || defined (USB_OTG_HS_LOW_PWR_MGMT_SUPPORT)
/*
 * @brief  This function handles EXTI15_10_IRQ Handler.
 * @param  None
 * @retval None
 */
#ifdef USE_STM3210C_EVAL
    void EXTI9_5_IRQHandler(void)
#else
    void EXTI15_10_IRQHandler(void)
#endif
{
    if (suspend_flag == 1)
    {
        suspend_flag = 0;
        /* Reset SLEEPDEEP bit */
        SCB->SCR &= (uint32_t) ~ ((uint32_t) (SCB_SCR_SLEEPDEEP_Msk));

        /* After wake-up from sleep mode, reconfigure the system clock */
        /* After wake-up from STOP reconfigure the system clock */

        /* Enable HSE */
        RCC_HSEConfig(RCC_HSE_ON);

        /* Wait till HSE is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
        {
        }

        /* Enable PLL */
        RCC_PLLCmd(ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08)
        {
        }

        USB_OTG_BSP_Resume(&USB_OTG_Core_dev);
    }

    EXTI_ClearITPendingBit(EXTI_Line11);
    /* Clear the EXTI line pending bit */
    EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
}
#endif                          /* LOW_PWR_MGMT_SUPPORT */

#if defined (USB_OTG_FS_LOW_PWR_MGMT_SUPPORT)
/**
  * @brief  This function handles OTG_FS_WKUP_IRQ Handler.
  * @param  None
  * @retval None
  */
void OTG_FS_WKUP_IRQHandler(void)
{
    suspend_flag = 0;
    /* Reset SLEEPDEEP bit */
    SCB->SCR &= (uint32_t) ~ ((uint32_t) (SCB_SCR_SLEEPDEEP_Msk));

    /* After wake-up from sleep mode, reconfigure the system clock */
    /* After wake-up from STOP reconfigure the system clock */

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {
    }

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }

    USB_OTG_BSP_Resume(&USB_OTG_Core_dev);
    EXTI_ClearITPendingBit(EXTI_Line18);
}
#endif                          /* LOW_PWR_MGMT_SUPPORT */

#if defined (USB_OTG_HS_LOW_PWR_MGMT_SUPPORT)
/**
  * @brief  This function handles OTG_FS_WKUP_IRQ Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_WKUP_IRQHandler(void)
{
    suspend_flag = 0;
    /* Reset SLEEPDEEP bit */
    SCB->SCR &= (uint32_t) ~ ((uint32_t) (SCB_SCR_SLEEPDEEP_Msk));

    /* After wake-up from sleep mode, reconfigure the system clock */
    /* After wake-up from STOP reconfigure the system clock */

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {
    }

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }

    USB_OTG_BSP_Resume(&USB_OTG_Core_dev);
    EXTI_ClearITPendingBit(EXTI_Line20);
}
#endif                          /* LOW_PWR_MGMT_SUPPORT */

/**
  * @brief  KEY_BUTTON_EXTI_IRQHandler
  * @param  None
  * @retval None
  */
void KEY_BUTTON_EXTI_IRQHandler(void)
{
    if (EXTI_GetITStatus(KEY_BUTTON_EXTI_LINE) != RESET)
    {
        EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
        USBH_InterruptSendData(&USB_OTG_Core_dev,
                               arr,
                               2,
                               HID_Machine.hc_num_out);
    }
}
/******************************************************************************/
/* STM32Fxxx Peripherals Interrupt Handlers */
/* Add here the Interrupt Handler for the used peripheral(s) (PPP), for the */
/* available peripheral interrupt handler's name please refer to the startup */
/* file (startup_stm32fxxx.s).  */
/******************************************************************************/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
