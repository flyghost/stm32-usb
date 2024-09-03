/**
  ******************************************************************************
  * @file    usbh_ch340.c
  * @author  binhao
  * @version V1.0.0
  * @date    20240317
  * @brief   CH340主机驱动代码
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "lcd_log.h"
#include "usbh_usr.h"
#include "usbh_fs.h"
#include "usbh_cdc_core.h"

/** @defgroup USBH_CORE_Exported_TypesDefinitions
* @{
*/
extern USBH_Status USBH_HandleControl (USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost);

extern __ALIGN_BEGIN CDC_Machine_TypeDef   CDC_Machine __ALIGN_END ;

extern CDC_Requests                        CDC_ReqState;
extern CDC_Xfer_TypeDef                    CDC_RxParam;
extern __ALIGN_BEGIN uint8_t               TxBuf [1024] __ALIGN_END ;
extern CDC_Xfer_TypeDef                    CDC_TxParam;

extern __ALIGN_BEGIN uint8_t               RxBuf [1024] __ALIGN_END ;



/**
* @}
*/

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
  * @brief  This request allows the host to find out the currently 
  *         configured line coding.
  * @param  pdev: Selected device
  * @retval USBH_Status : USB ctl xfer status
  */
USBH_Status CH340_BaudRate9600(USB_OTG_CORE_HANDLE *pdev , USBH_HOST *phost)
{
  phost->Control.setup.b.bmRequestType = 0x40;
  
  phost->Control.setup.b.bRequest = 0x9A;
  phost->Control.setup.b.wValue.w = 0x1312;
  phost->Control.setup.b.wIndex.w = 0xB282;
  phost->Control.setup.b.wLength.w = 0x0000;           
  
      
  return USBH_CtlReq(pdev, phost, 0, 0);
}

USBH_Status CH340_BaudRate115200(USB_OTG_CORE_HANDLE *pdev , USBH_HOST *phost)
{
  phost->Control.setup.b.bmRequestType = 0x40;
  
  phost->Control.setup.b.bRequest = 0x9A;
  phost->Control.setup.b.wValue.w = 0x1312;
  phost->Control.setup.b.wIndex.w = 0xCC83;
  phost->Control.setup.b.wLength.w = 0x0000;           
  
      
  return USBH_CtlReq(pdev, phost, 0, 0);
}

static USBH_Status CH340CDC_ClassRequest(USB_OTG_CORE_HANDLE *pdev,
                                    void *phost)
{
    USBH_HOST *pphost = phost;

    USBH_Status status         = USBH_BUSY;
    USBH_Status ClassReqStatus = USBH_BUSY;

    switch (CDC_ReqState)
    {

        case CDC_GET_LINE_CODING_RQUEST:
            /*Issue the get line coding request*/
            ClassReqStatus = CH340_BaudRate115200(pdev, phost);
            //ClassReqStatus = USBH_OK;

            if ( ClassReqStatus == USBH_OK )
            {
                /*Change the state */
                CDC_ReqState = CDC_GET_LINE_CODING_RQUEST;
                status = USBH_OK;
            }

            break;

        default:

            break;
    }

    return status;
}

/**
  * @brief  Initialize the transmit and receive buffer and its parameter
  * @param  None
  * @retval None
  */
static void CH340CDC_InitTxRxParam(void)
{
    /*Initialize the Transmit buffer and its parameter*/
    CDC_TxParam.CDCState = CDC_IDLE;
    CDC_TxParam.DataLength = 0;
    CDC_TxParam.pRxTxBuff = TxBuf;

    /*Initialize the Receive buffer and its parameter*/
    CDC_RxParam.CDCState = CDC_IDLE;
    CDC_RxParam.DataLength = 0;
    CDC_RxParam.pFillBuff = RxBuf;
    CDC_RxParam.pEmptyBuff = RxBuf;
    CDC_RxParam.BufferLen = sizeof(RxBuf);
}


/**
  * @brief  CDC_InterfaceInit
  *         The function init the CDC class.
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval  USBH_Status :Response for USB CDC driver intialization
  */
static USBH_Status CH340_CDC_InterfaceInit ( USB_OTG_CORE_HANDLE *pdev,
                                       void *phost)
{
    USBH_HOST *pphost = phost;
    USBH_Status status = USBH_OK ;

    /* Communication Interface */
    if ((pphost->device_prop.Itf_Desc[0].bInterfaceClass  == 0xFF) && \
            (pphost->device_prop.Itf_Desc[0].bInterfaceSubClass  == 0x01) && \
            (pphost->device_prop.Itf_Desc[0].bInterfaceProtocol == 0x02))
    {
        for (int i=0; i<USBH_MAX_NUM_ENDPOINTS; i++)
        {
            /* INTERRUPT */
            if (pphost->device_prop.Ep_Desc[0][i].bmAttributes == EP_TYPE_INTR)
            {
                /*Collect the notification endpoint address and length*/
                CDC_Machine.CDC_CommItf.ep_addr = pphost->device_prop.Ep_Desc[0][i].bEndpointAddress;
                CDC_Machine.CDC_CommItf.length  = pphost->device_prop.Ep_Desc[0][i].wMaxPacketSize;

                if (pphost->device_prop.Ep_Desc[0][i].bEndpointAddress & 0x80)
                {
                    CDC_Machine.CDC_CommItf.notificationEp = \
                            (pphost->device_prop.Ep_Desc[0][i].bEndpointAddress);
                }

                /*Allocate the length for host channel number in*/
                CDC_Machine.CDC_CommItf.hc_num_in = USBH_Alloc_Channel(pdev,
                                                    CDC_Machine.CDC_CommItf.notificationEp );

                /* Open channel for IN endpoint */
                USBH_Open_Channel  (pdev,
                                    CDC_Machine.CDC_CommItf.hc_num_in,
                                    pphost->device_prop.address,
                                    pphost->device_prop.speed,
                                    EP_TYPE_INTR,
                                    CDC_Machine.CDC_CommItf.length);                
            }
            
            /* BULK */
            else if (pphost->device_prop.Ep_Desc[0][i].bmAttributes == EP_TYPE_BULK)
            {
                if (pphost->device_prop.Ep_Desc[0][i].bEndpointAddress & 0x80)  //IN
                {
                    /*Collect the class specific endpoint address and length*/
                    CDC_Machine.CDC_DataItf.ep_addr = pphost->device_prop.Ep_Desc[0][i].bEndpointAddress;
                    CDC_Machine.CDC_DataItf.length  = pphost->device_prop.Ep_Desc[0][i].wMaxPacketSize;

                    CDC_Machine.CDC_DataItf.cdcInEp = (pphost->device_prop.Ep_Desc[0][i].bEndpointAddress);

                    /*Allocate the length for host channel number in*/
                    CDC_Machine.CDC_DataItf.hc_num_in = USBH_Alloc_Channel(pdev,
                                                        CDC_Machine.CDC_DataItf.cdcInEp);
                    /* Open channel for IN endpoint */
                    USBH_Open_Channel  (pdev,
                                        CDC_Machine.CDC_DataItf.hc_num_in,
                                        pphost->device_prop.address,
                                        pphost->device_prop.speed,
                                        EP_TYPE_BULK,
                                        CDC_Machine.CDC_DataItf.length);                  
                }
                else  //OUT
                {
                    /*Collect the class specific endpoint address and length*/
                    CDC_Machine.CDC_DataItf.ep_addr = pphost->device_prop.Ep_Desc[0][i].bEndpointAddress;
                    CDC_Machine.CDC_DataItf.length  = pphost->device_prop.Ep_Desc[0][i].wMaxPacketSize;

                    CDC_Machine.CDC_DataItf.cdcOutEp = (pphost->device_prop.Ep_Desc[0][i].bEndpointAddress);

                    /*Allocate the length for host channel number out*/
                    CDC_Machine.CDC_DataItf.hc_num_out = USBH_Alloc_Channel(pdev,
                                                         CDC_Machine.CDC_DataItf.cdcOutEp);

                    /* Open channel for OUT endpoint */
                    USBH_Open_Channel  (pdev,
                                        CDC_Machine.CDC_DataItf.hc_num_out,
                                        pphost->device_prop.address,
                                        pphost->device_prop.speed,
                                        EP_TYPE_BULK,
                                        CDC_Machine.CDC_DataItf.length);                   
                }
            }            
        }
    }
    else
    {
        pphost->usr_cb->DeviceNotSupported();
    }

    /*Initilise the Tx/Rx Params*/
    CH340CDC_InitTxRxParam();

    /*Initialize the class specific request with "GET_LINE_CODING"*/
    CDC_ReqState = CDC_GET_LINE_CODING_RQUEST ; 

    return status;
}


/**
  * @brief  USBH_HandleCH340Enum 
  *         This function includes the complete enumeration process
  * @param  pdev: Selected device
  * @retval USBH_Status
  */
static USBH_Status USBH_HandleCH340Enum(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost)
{
  USBH_Status Status = USBH_BUSY;  
  uint8_t Local_Buffer[64];
  
  switch (phost->EnumState)
  {
  case ENUM_IDLE:  
    /* Get Device Desc for only 1st 8 bytes : To get EP0 MaxPacketSize */
    if ( USBH_Get_DevDesc(pdev , phost, 8) == USBH_OK)
    {
      phost->Control.ep0size = phost->device_prop.Dev_Desc.bMaxPacketSize;
      
      phost->EnumState = ENUM_GET_FULL_DEV_DESC;
      
      /* modify control channels configuration for MaxPacket size */
      USBH_Modify_Channel (pdev,
                           phost->Control.hc_num_out,
                           0,
                           0,
                           0,
                           phost->Control.ep0size);
      
      USBH_Modify_Channel (pdev,
                           phost->Control.hc_num_in,
                           0,
                           0,
                           0,
                           phost->Control.ep0size);      
    }
    break;
    
  case ENUM_GET_FULL_DEV_DESC:  
    /* Get FULL Device Desc  */
    if ( USBH_Get_DevDesc(pdev, phost, USB_DEVICE_DESC_SIZE)\
      == USBH_OK)
    {
      /* user callback for device descriptor available */
      phost->usr_cb->DeviceDescAvailable(&phost->device_prop.Dev_Desc);      
      phost->EnumState = ENUM_SET_ADDR;
    }
    break;
   
  case ENUM_SET_ADDR: 
    /* set address */
    if ( USBH_SetAddress(pdev, phost, USBH_DEVICE_ADDRESS) == USBH_OK)
    {
      USB_OTG_BSP_mDelay(2);
      phost->device_prop.address = USBH_DEVICE_ADDRESS;
      
      /* user callback for device address assigned */
      phost->usr_cb->DeviceAddressAssigned();
      phost->EnumState = ENUM_GET_CFG_DESC;
      
      /* modify control channels to update device address */
      USBH_Modify_Channel (pdev,
                           phost->Control.hc_num_in,
                           phost->device_prop.address,
                           0,
                           0,
                           0);
      
      USBH_Modify_Channel (pdev,
                           phost->Control.hc_num_out,
                           phost->device_prop.address,
                           0,
                           0,
                           0);         
    }
    break;
    
  case ENUM_GET_CFG_DESC:  
    /* get standard configuration descriptor */
    if ( USBH_Get_CfgDesc(pdev, 
                          phost,
                          USB_CONFIGURATION_DESC_SIZE) == USBH_OK)
    {
      /* before getting full config descriptor, check if it does not exceed 
      buffer size allocated to config descriptor USBH_MAX_DATA_BUFFER
      in the usbh_conf.h*/
      if (phost->device_prop.Cfg_Desc.wTotalLength <= USBH_MAX_DATA_BUFFER)
      {
        phost->EnumState = ENUM_GET_FULL_CFG_DESC;
      }
    }
    break;
    
  case ENUM_GET_FULL_CFG_DESC:  
    /* get FULL config descriptor (config, interface, endpoints) */
    if (USBH_Get_CfgDesc(pdev, 
                         phost,
                         phost->device_prop.Cfg_Desc.wTotalLength) == USBH_OK)
    {
      /* User callback for configuration descriptors available */
      phost->usr_cb->ConfigurationDescAvailable(&phost->device_prop.Cfg_Desc,
                                                      phost->device_prop.Itf_Desc,
                                                      phost->device_prop.Ep_Desc[0]);
      
      phost->EnumState = ENUM_GET_MFC_STRING_DESC;
    }
    break;
    
  case ENUM_GET_MFC_STRING_DESC:  
    if (phost->device_prop.Dev_Desc.iManufacturer != 0)
    { /* Check that Manufacturer String is available */
      
      if ( USBH_Get_StringDesc(pdev,
                               phost,
                               phost->device_prop.Dev_Desc.iManufacturer, 
                               Local_Buffer , 
                               0xff) == USBH_OK)
      {
        /* User callback for Manufacturing string */
        phost->usr_cb->ManufacturerString(Local_Buffer);
        phost->EnumState = ENUM_GET_PRODUCT_STRING_DESC;
      }
    }
    else
    {
      phost->usr_cb->ManufacturerString("N/A");      
      phost->EnumState = ENUM_GET_PRODUCT_STRING_DESC;
    }
    break;
    
  case ENUM_GET_PRODUCT_STRING_DESC:   
    if (phost->device_prop.Dev_Desc.iProduct != 0)
    { /* Check that Product string is available */
      if ( USBH_Get_StringDesc(pdev,
                               phost,
                               phost->device_prop.Dev_Desc.iProduct, 
                               Local_Buffer, 
                               0xff) == USBH_OK)
      {
        /* User callback for Product string */
        phost->usr_cb->ProductString(Local_Buffer);
        phost->EnumState = ENUM_GET_SERIALNUM_STRING_DESC;
      }
    }
    else
    {
      phost->usr_cb->ProductString("N/A");
      phost->EnumState = ENUM_GET_SERIALNUM_STRING_DESC;
    } 
    break;
    
  case ENUM_GET_SERIALNUM_STRING_DESC:   
    if (phost->device_prop.Dev_Desc.iSerialNumber != 0)
    { /* Check that Serial number string is available */    
      if ( USBH_Get_StringDesc(pdev, 
                               phost,
                               phost->device_prop.Dev_Desc.iSerialNumber, 
                               Local_Buffer, 
                               0xff) == USBH_OK)
      {
        /* User callback for Serial number string */
        phost->usr_cb->SerialNumString(Local_Buffer);
        phost->EnumState = ENUM_SET_CONFIGURATION;
      }
    }
    else
    {
      phost->usr_cb->SerialNumString("N/A");      
      phost->EnumState = ENUM_SET_CONFIGURATION;
    }  
    break;
      
  case ENUM_SET_CONFIGURATION:
    /* set configuration  (default config) */
    if (USBH_SetCfg(pdev, 
                    phost,
                    phost->device_prop.Cfg_Desc.bConfigurationValue) == USBH_OK)
    {
      phost->EnumState = ENUM_DEV_CONFIGURED;
    }
    break;

    
  case ENUM_DEV_CONFIGURED:
    /* user callback for enumeration done */
    Status = USBH_OK;
    break;
    
  default:
    break;
  }  
  return Status;
}


/**
* @brief  USBH_CH340_Process
*         USB Host core main state machine process
* @param  None 
* @retval None
*/
void USBH_CH340_Process(USB_OTG_CORE_HANDLE *pdev , USBH_HOST *phost)
{
  volatile USBH_Status status = USBH_FAIL;

  /* check for Host port events */
  if (((HCD_IsDeviceConnected(pdev) == 0)|| (HCD_IsPortEnabled(pdev) == 0))&& (phost->gState != HOST_IDLE)) 
  {
    if(phost->gState != HOST_DEV_DISCONNECTED) 
    {
      phost->gState = HOST_DEV_DISCONNECTED;
    }
  }
    
  switch (phost->gState)
  {
  
  case HOST_IDLE :
    
    if (HCD_IsDeviceConnected(pdev))  
    {
      phost->gState = HOST_WAIT_PRT_ENABLED; 
      
      /*wait denounce delay */
      USB_OTG_BSP_mDelay(100);
      
      /* Apply a port RESET */
      HCD_ResetPort(pdev);
      
      /* User RESET callback*/
      phost->usr_cb->ResetDevice();
    }
    break;
    
  case HOST_WAIT_PRT_ENABLED:
    if (pdev->host.PortEnabled == 1)
    { 
      phost->gState = HOST_DEV_ATTACHED; 
      USB_OTG_BSP_mDelay(50);
    }
    break;
      
  case HOST_DEV_ATTACHED :
    
    phost->usr_cb->DeviceAttached();
    phost->Control.hc_num_out = USBH_Alloc_Channel(pdev, 0x00);
    phost->Control.hc_num_in = USBH_Alloc_Channel(pdev, 0x80);  
  
    /* Reset USB Device */
    if ( HCD_ResetPort(pdev) == 0)
    {
      phost->usr_cb->ResetDevice();
     
      /* Host is Now ready to start the Enumeration */
      phost->device_prop.speed = HCD_GetCurrentSpeed(pdev);
      
      phost->gState = HOST_ENUMERATION;
      phost->usr_cb->DeviceSpeedDetected(phost->device_prop.speed);
      
      /* Open Control pipes */
      USBH_Open_Channel (pdev,
                         phost->Control.hc_num_in,
                         phost->device_prop.address,
                         phost->device_prop.speed,
                         EP_TYPE_CTRL,
                         phost->Control.ep0size); 
      
      /* Open Control pipes */
      USBH_Open_Channel (pdev,
                         phost->Control.hc_num_out,
                         phost->device_prop.address,
                         phost->device_prop.speed,
                         EP_TYPE_CTRL,
                         phost->Control.ep0size);          
    }
    break;
    
  case HOST_ENUMERATION:     
    /* Check for enumeration status */  
    if ( USBH_HandleCH340Enum(pdev , phost) == USBH_OK)
    { 
      /* The function shall return USBH_OK when full enumeration is complete */
      
      /* user callback for end of device basic enumeration */
      phost->usr_cb->EnumerationDone();
#if defined (USB_OTG_FS_LOW_PWR_MGMT_SUPPORT) || defined (USB_OTG_HS_LOW_PWR_MGMT_SUPPORT)
      phost->gState  = HOST_SUSPENDED;
#else
      phost->gState  = HOST_USR_INPUT;
#endif /* LOW_PWR_MGMT_SUPPORT*/
    }
    break;
    
  case HOST_USR_INPUT:    
    /*The function should return user response true to move to class state */
    if ( phost->usr_cb->UserInput() == USBH_USR_RESP_OK)
    {
//      if((phost->class_cb->Init(pdev, phost))\
//        == USBH_OK)
      if((CH340_CDC_InterfaceInit(pdev, phost))\
        == USBH_OK)      
      {
        phost->gState  = HOST_CLASS_REQUEST;     
      }     
    }   
    break;
    
  case HOST_CLASS_REQUEST:  
    /* process class standard control requests state machine */ 
    //status = phost->class_cb->Requests(pdev, phost);
  
    status = CH340CDC_ClassRequest(pdev, phost);
    
     if(status == USBH_OK)
     {
       phost->gState  = HOST_CLASS;
     }  
     else
     {
       USBH_ErrorHandle(phost, status);
     }
 
    
    break;    
  case HOST_CLASS:   
    /* process class state machine */
    status = phost->class_cb->Machine(pdev, phost);
    USBH_ErrorHandle(phost, status);
    break;       
    
  case HOST_CTRL_XFER:
    /* process control transfer state machine */
    USBH_HandleControl(pdev, phost);    
    break;
#if defined (USB_OTG_FS_LOW_PWR_MGMT_SUPPORT) || defined (USB_OTG_HS_LOW_PWR_MGMT_SUPPORT)
  case HOST_SUSPENDED:
    if (USBH_SetDeviceFeature(pdev, phost, FEATURE_SELECTOR_DEVICE, 0)==USBH_OK)
    {
      suspend_flag = 1;
      USB_OTG_BSP_Suspend(pdev);
      phost->usr_cb->UserInput();
      PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
      /* After wakeup got to HOST_WAKEUP state */
      phost->gState  = HOST_WAKEUP;
    }
    break;
      
  case HOST_WAKEUP:
    /* issue  a ClearDeviceFeature request */
    if (USBH_ClearDeviceFeature(pdev, phost, FEATURE_SELECTOR_DEVICE, 0)==USBH_OK)
    {
      phost->gState  = HOST_USR_INPUT;
    }
    break;
#endif /* USE_HOST_MODE */
  case HOST_ERROR_STATE:
    /* Re-Initialize Host for new Enumeration */
    USBH_DeInit(pdev, phost);
    phost->usr_cb->DeInit();
    phost->class_cb->DeInit(pdev, &phost->device_prop);
    break;
    
  case HOST_DEV_DISCONNECTED :
    
    /* Manage User disconnect operations*/
    phost->usr_cb->DeviceDisconnected();
    
    /* Re-Initialize Host for new Enumeration */
    USBH_DeInit(pdev, phost);
    phost->usr_cb->DeInit();
    phost->class_cb->DeInit(pdev, &phost->device_prop); 
    USBH_DeAllocate_AllChannel(pdev);  
    phost->gState = HOST_IDLE;
   
    /* Re-Initialize Host for new Enumeration */
    HCD_Init(pdev, 
#ifdef USE_USB_OTG_FS
            USB_OTG_FS_CORE_ID
#else
            USB_OTG_HS_CORE_ID
#endif
);
    
    break;
    
  default :
    break;
  }

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
