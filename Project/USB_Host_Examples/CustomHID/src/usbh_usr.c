/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   This file includes the user application layer
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
#include "lcd_log.h"
#include "usbh_usr.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_HID_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR
* @brief This file is the Header file for usbh_usr.c
* @{
*/


/** @defgroup USBH_CORE_Exported_TypesDefinitions
* @{
*/
#if defined(USE_STM324x9I_EVAL)
    #define KYBRD_FIRST_COLUMN               (uint16_t)479
    #define KYBRD_LAST_COLUMN                (uint16_t)7
    #define KYBRD_FIRST_LINE                 (uint8_t)120
    #define KYBRD_PAGE_LINE                  (uint8_t)50
    #define KYBRD_LAST_LINE                  (uint8_t)200

#else
    #define KYBRD_FIRST_COLUMN               (uint16_t)319
    #define KYBRD_LAST_COLUMN                (uint16_t)7
    #define KYBRD_FIRST_LINE                 (uint8_t)120
    #define KYBRD_PAGE_LINE                  (uint8_t)50
    #define KYBRD_LAST_LINE                  (uint8_t)200

#endif

/**
* @}
*/
uint8_t KeybrdCharXpos = 0;
uint16_t KeybrdCharYpos = 0;
extern int16_t x_loc, y_loc;
extern __IO int16_t prev_x, prev_y;

/** @addtogroup USER
* @{
*/

/** @defgroup USBH_USR
* @brief    This file includes the user application layer
* @{
*/

/** @defgroup USBH_CORE_Exported_Types
* @{
*/



/** @defgroup USBH_USR_Private_Defines
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_Private_Macros
* @{
*/
/**
* @}
*/

/** @defgroup USBH_USR_Private_Variables
* @{
*/

/* Points to the DEVICE_PROP structure of current device */
/* The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_Callbacks =
{
    USBH_USR_Init,
    USBH_USR_DeInit,
    USBH_USR_DeviceAttached,
    USBH_USR_ResetDevice,
    USBH_USR_DeviceDisconnected,
    USBH_USR_OverCurrentDetected,
    USBH_USR_DeviceSpeedDetected,
    USBH_USR_Device_DescAvailable,
    USBH_USR_DeviceAddressAssigned,
    USBH_USR_Configuration_DescAvailable,
    USBH_USR_Manufacturer_String,
    USBH_USR_Product_String,
    USBH_USR_SerialNum_String,
    USBH_USR_EnumerationDone,
    USBH_USR_UserInput,
    0,
    USBH_USR_DeviceNotSupported,
    USBH_USR_UnrecoveredError
};

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/

/**
* @}
*/



/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_Private_Functions
* @{
*/





/**
* @brief  USBH_USR_Init
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{

}

/**
* @brief  USBH_USR_DeviceAttached
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{

}

/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError(void)
{

}

/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval None
*/
void USBH_USR_DeviceDisconnected(void)
{

}

/**
* @brief  USBH_USR_ResetUSBDevice
*         Reset USB Device
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
    /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBH_USR_DeviceSpeedDetected
*         Displays the message on LCD for device speed
* @param  Devicespeed : Device Speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{

}

/**
* @brief  USBH_USR_Device_DescAvailable
*         Displays the message on LCD for device descriptor
* @param  DeviceDesc : device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{

}

/**
* @brief  USBH_USR_DeviceAddressAssigned
*         USB device is successfully assigned the Address
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{

}


/**
* @brief  USBH_USR_Conf_Desc
*         Displays the message on LCD for configuration descriptor
* @param  ConfDesc : Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
        USBH_InterfaceDesc_TypeDef * itfDesc,
        USBH_EpDesc_TypeDef * epDesc)
{

}

/**
* @brief  USBH_USR_Manufacturer_String
*         Displays the message on LCD for Manufacturer String
* @param  ManufacturerString : Manufacturer String of Device
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{

}

/**
* @brief  USBH_USR_Product_String
*         Displays the message on LCD for Product String
* @param  ProductString : Product String of Device
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{

}

/**
* @brief  USBH_USR_SerialNum_String
*         Displays the message on LCD for SerialNum_String
* @param  SerialNumString : SerialNum_String of device
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{

}

/**
* @brief  EnumerationDone
*         User response request is displayed to ask for
*         application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{

}

/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{

}


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{
    return USBH_USR_RESP_OK;
}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Device Overcurrent detection event
* @param  None
* @retval None
*/
void USBH_USR_OverCurrentDetected(void)
{

}

/**
* @brief  USR_MOUSE_Init
*         Init Mouse window
* @param  None
* @retval None
*/
void USR_MOUSE_Init(void)
{

}

/**
* @brief  USR_MOUSE_ProcessData
*         Process Mouse data
* @param  data : Mouse data to be displayed
* @retval None
*/
void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef * data)
{

}

/**
* @brief  USR_KEYBRD_Init
*         Init Keyboard window
* @param  None
* @retval None
*/
void USR_KEYBRD_Init(void)
{

}


/**
* @brief  USR_KEYBRD_ProcessData
*         Process Keyboard data
* @param  data : Keyboard data to be displayed
* @retval None
*/
void USR_KEYBRD_ProcessData(uint8_t data)
{

}

/**
* @brief  USBH_USR_DeInit
*         Deinit User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
}

/**
* @}
*/

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
