/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.c
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   Generic media access Layer.
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

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #pragma     data_alignment = 4
#endif                          /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* Includes ------------------------------------------------------------------ */
#include "usbd_cdc_vcp.h"
#include "usbd_cdc_core_loopback.h"
#include "usbd_usr.h"
#include "usbd_desc.h"

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined ( __ICCARM__ )      /* !< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */

extern uint8_t Rxbuffer[RX_BUFFER_SIZE];
extern uint16_t USB_Rx_Cnt;

static __IO uint32_t data_sent;
static __IO uint32_t receive_flag = 0;
static __IO uint32_t receive_count;

/* Private function prototypes ----------------------------------------------- */
static uint16_t VCP_DataTx(void);
static uint16_t VCP_DataRx(uint32_t Len);

CDC_IF_Prop_TypeDef VCP_fops =
{
    .pIf_DataTx = VCP_DataTx,
    .pIf_DataRx = VCP_DataRx
};

/* Private functions --------------------------------------------------------- */

/**
  * @brief  VCP_DataTx
  *         CDC received data to be send over USB IN endpoint are managed in
  *         this function.
  * @retval Result of the operation: USBD_OK if all operations are OK else VCP_FAIL
  */
static uint16_t VCP_DataTx(void)
{
    data_sent = 1;

    return USBD_OK;
}

/**
  * @brief  VCP_DataRx
  *         Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         until exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else VCP_FAIL
  */
static uint16_t VCP_DataRx(uint32_t Len)
{
    receive_count = Len;

    return USBD_OK;
}

void USB_VCP_Init(USB_OTG_CORE_HANDLE * pdev)
{
#ifdef USE_USB_OTG_HS
    USBD_Init(pdev, USB_OTG_HS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);
#else
    USBD_Init(pdev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);
#endif
}

/* send data function */
void VCP_SendData(USB_OTG_CORE_HANDLE * pdev, uint8_t * pbuf, uint32_t buf_len)
{
    data_sent = 0;
    DCD_EP_Tx(pdev, CDC_IN_EP, pbuf, buf_len);
}

/* prepare data to be received */
void VCP_ReceiveData(USB_OTG_CORE_HANDLE * pdev, uint8_t * pbuf,
                     uint32_t buf_len)
{
    DCD_EP_PrepareRx(pdev, CDC_OUT_EP, pbuf, buf_len);
    receive_flag = 1;
}

bool VCP_DataReady(void)
{
    return receive_flag;
}

void VCP_ClearReceiveFlag(void)
{
    receive_flag = 0;
}

uint32_t VCP_RecvLenGet(void)
{
    return receive_count;
}

uint8_t * VCP_RecvBufGet(void)
{
    return Rxbuffer;
}

uint32_t VCP_RecvDataLenGet(void)
{
    return USB_Rx_Cnt;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
