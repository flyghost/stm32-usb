/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   Header for usbd_cdc_vcp.c file.
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
#ifndef __USBD_CDC_VCP_H
#define __USBD_CDC_VCP_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_ioreq.h"
#include "string.h"
#include <stdbool.h>
/* Exported typef ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* send data function */
void USB_VCP_Init(USB_OTG_CORE_HANDLE * pdev);
void VCP_SendData( USB_OTG_CORE_HANDLE *pdev, uint8_t* pbuf, uint32_t  buf_len);
void VCP_ReceiveData(USB_OTG_CORE_HANDLE *pdev, uint8_t  *pbuf, uint32_t   buf_len);
uint32_t VCP_RecvLenGet(void);
uint8_t * VCP_RecvBufGet(void);
bool VCP_DataReady(void);
void VCP_ClearReceiveFlag(void);
uint32_t VCP_RecvDataLenGet(void);

#endif /* __USBD_CDC_VCP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
