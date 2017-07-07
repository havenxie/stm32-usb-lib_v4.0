#ifndef __IAP_H__
#define __IAP_H__
#include "stm32f10x.h"
#include "iap_config.h"
/* Exported types ------------------------------------------------------------*/
typedef  void (*pFunction)(void);

extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

#if (IAP_MODE == DFU_MODE)
extern uint8_t DeviceState;
extern uint8_t DeviceStatus[6];
extern void IAP_IntoDFU(void);

#elif (IAP_MODE == UART_MODE)
extern void IAP_USART_Init(void);
extern void IAP_Main_Menu(void);
extern int8_t IAP_Update(void);
extern int8_t IAP_Upload(void);
extern int8_t IAP_Erase(void);
#endif

extern void IAP_FLASH_WriteFlag(uint16_t flag);
extern uint16_t IAP_FLASH_ReadFlag(void);
extern int8_t IAP_RunApp(void);

#endif
