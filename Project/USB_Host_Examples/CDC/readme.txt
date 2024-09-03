/*
* @file name: readme
* @author: binhao
*/

V1.0.0/2024-1-17
1. 首次发布

******************************例程详细描述*****************************
功能描述：
1. 本例程提供了基于STM32F407芯片的OTG HS和OTG FS设备说明
2. 本例程实现OTG主机模式下CDC测试功能，用户需要接一个串口从机板才能实现数据的收发
3. 通过按键选择是否打开串口，K2是打开串口，K1是发送数据，数据为0x31 0x32
4. 本例程支持全速和高速模式

时钟配置：
1. 本例程采用HSE8M PLL 168MHz作为系统时钟

适用芯片：
STM32F407

涉及外设：
OTG FS
OTG HS
KEY

涉及管脚：
KEY1 - PA0
KEY2 - PC13

OTG_FS_SOF - PA8
OTG_FS_VBUS - PA9
OTG_FS_ID - PA10
OTG_FS_DM - PA11
OTG_FS_DP - PA12

OTG_HS_SOF - PA4
OTG_HS_VBUS - PB13（和OTG_HS_ULPI_D6共用）
OTG_HS_ID - PB12（和OTG_HS_ULPI_D5共用）
OTG_HS_DM - PB14
OTG_HS_DP - PB15

OTG_HS_ULPI_CK - PA5
OTG_HS_ULPI_DIR - PC2
OTG_HS_ULPI_NXT - PC3
OTG_HS_ULPI_STP - PC0
OTG_HS_ULPI_D0 - PA3
OTG_HS_ULPI_D1 - PB0
OTG_HS_ULPI_D2 - PB1
OTG_HS_ULPI_D3 - PB10
OTG_HS_ULPI_D4 - PB11
OTG_HS_ULPI_D5 - PB12
OTG_HS_ULPI_D6 - PB13
OTG_HS_ULPI_D7 - PB5

注意事项：
1. 推荐使用MDK5.00及其以上版本编译本工程（MDK - ARM）

如何使用：
1. 直接编译程序下载到评估板，重启评估板
2. 通过设备管理器查看设备是否枚举成功
***********************************************************************