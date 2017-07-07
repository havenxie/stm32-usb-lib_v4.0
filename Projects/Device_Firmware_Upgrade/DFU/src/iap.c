
#include "iap.h"
#include "stmflash.h"

pFunction Jump_To_Application;
uint32_t JumpAddress;

#if (IAP_MODE == DFU_MODE)

#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "dfu_mal.h"

uint8_t DeviceState;
uint8_t DeviceStatus[6];


//static u8  fac_us=0;//us��ʱ������
//static u16 fac_ms=0;//ms��ʱ������
////��ʼ���ӳٺ���
////SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
////SYSCLK:ϵͳʱ��
// 
//void delay_init(u8 SYSCLK)
//{
////	SysTick->CTRL&=0xfffffffb;//bit2���,ѡ���ⲿʱ��  HCLK/8
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//bit2���,ѡ���ⲿʱ��  HCLK/8
//	fac_us=SYSCLK/8;		    
//	fac_ms=(u16)fac_us*1000;
//}								    
////��ʱnms
////ע��nms�ķ�Χ
////SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
////nms<=0xffffff*8*1000/SYSCLK
////SYSCLK��λΪHz,nms��λΪms
////��72M������,nms<=1864 
//void delay_ms(u16 nms)
//{	 		  	  
//	u32 temp;		   
//	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
//	SysTick->VAL =0x00;           //��ռ�����
//	SysTick->CTRL=0x01 ;          //��ʼ����  
//	do
//	{
//		temp=SysTick->CTRL;
//	}
//	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
//	SysTick->CTRL=0x00;       //�رռ�����
//	SysTick->VAL =0X00;       //��ռ�����	  	    
//}  
  /* Enter DFU mode */
void IAP_IntoDFU(void)
{
  uint16_t i = 0;
  DeviceState = STATE_dfuERROR;
  DeviceStatus[0] = STATUS_ERRFIRMWARE;
  DeviceStatus[4] = DeviceState;

  Set_System();
  Set_USBClock();
  USB_Init();  
  
  /* Main loop */
  while (1)
  {
	i++;
	if(i>=65535)
	{
	  i=0;
//	  if (DFU_Button_Read() != 0x00)//pushed
//	  {
//		IAP_FLASH_WriteFlag(APPRUN_FLAG_DATA);
//		//NVIC_SystemReset();
//		IAP_RunApp();
//	  }
	}
  }
}


#elif (IAP_MODE == UART_MODE)

#include "ymodem.h"

uint32_t BlockNbr = 0, UserMemoryMask = 0;
__IO uint32_t FlashProtection = 0;
uint8_t tab_1024[1024] = {0};

/************************************************************************/
static void FLASH_DisableWriteProtectionPages(void)
{
	uint32_t useroptionbyte = 0, WRPR = 0;
	uint16_t var1 = OB_IWDG_SW, var2 = OB_STOP_NoRST, var3 = OB_STDBY_NoRST;
	FLASH_Status status = FLASH_BUSY;

	WRPR = FLASH_GetWriteProtectionOptionByte();
	
	if ((WRPR & UserMemoryMask) != UserMemoryMask)
	{
		useroptionbyte = FLASH_GetUserOptionByte();

		UserMemoryMask |= WRPR;

		status = FLASH_EraseOptionBytes();

		if (UserMemoryMask != 0xFFFFFFFF)
		{
			status = FLASH_EnableWriteProtection((uint32_t)~UserMemoryMask);
		}
		if ((useroptionbyte & 0x07) != 0x07)
		{ 
			if ((useroptionbyte & 0x01) == 0x0)
			{
				var1 = OB_IWDG_HW;
			}
			if ((useroptionbyte & 0x02) == 0x0)
			{
				var2 = OB_STOP_RST;
			}
			if ((useroptionbyte & 0x04) == 0x0)
			{
				var3 = OB_STDBY_RST;
			}
			FLASH_UserOptionByteConfig(var1, var2, var3);
		}

		if (status == FLASH_COMPLETE)
		{
			SerialPutString("Write Protection Close.\r\n");
			NVIC_SystemReset();
		}
		else
		{
			SerialPutString("Error: unprotection failed.\r\n");
		}
	}
	else
	{
		SerialPutString("Flash not write protected.\r\n");
	}
}





/************************************************************************/
void IAP_USART_Init(void)
{
  USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(&USART_InitStructure);  
}





/************************************************************************/
void IAP_Main_Menu(void)
{
	uint8_t cmdStr[CMD_STRING_SIZE] = {0};
	BlockNbr = (ApplicationAddress - 0x08000000) >> 12;
	
#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
    UserMemoryMask = ((uint32_t)~((1 << BlockNbr) - 1));
#else /* USE_STM3210E_EVAL */
	if (BlockNbr < 62)
	{
		UserMemoryMask = ((uint32_t)~((1 << BlockNbr) - 1));
	}
	else
	{
		UserMemoryMask = ((uint32_t)0x80000000);
	}
#endif 
	
	if ((FLASH_GetWriteProtectionOptionByte() & UserMemoryMask) != UserMemoryMask)
	{
		FlashProtection = 1;
	}
	else
	{
		FlashProtection = 0;
	}
	while (1)
	{
		SerialPutString("\r\n IAP Main Menu (V 0.1.4)\r\n");
		SerialPutString(" update\r\n");
		SerialPutString(" upload\r\n");
		SerialPutString(" erase\r\n");
		SerialPutString(" menu\r\n");
		SerialPutString(" runapp\r\n");
		if(FlashProtection != 0)//There is write protected
		{
			SerialPutString(" diswp\r\n");
		}
		
		GetInputString(cmdStr);
		
		if(strcmp((char *)cmdStr, CMD_UPDATE_STR) == 0)
		{
			IAP_FLASH_WriteFlag(UPDATE_FLAG_DATA);
			return;
		}
		else if(strcmp((char *)cmdStr, CMD_UPLOAD_STR) == 0)
		{
			IAP_FLASH_WriteFlag(UPLOAD_FLAG_DATA);
			return;
		}
		else if(strcmp((char *)cmdStr, CMD_ERASE_STR) == 0)
		{
			IAP_FLASH_WriteFlag(ERASE_FLAG_DATA);
			return;
		}
		else if(strcmp((char *)cmdStr, CMD_MENU_STR) == 0)
		{
			IAP_FLASH_WriteFlag(INIT_FLAG_DATA);
		}
		else if(strcmp((char *)cmdStr, CMD_RUNAPP_STR) == 0)
		{
			IAP_FLASH_WriteFlag(APPRUN_FLAG_DATA);
			return;
		}
		else if(strcmp((char *)cmdStr, CMD_DISWP_STR) == 0)
		{
			FLASH_DisableWriteProtectionPages();
		}
		else
		{
			SerialPutString(" Invalid CMD !\r\n");
		}
	}
}


/************************************************************************/
int8_t IAP_Update(void)
{
	uint8_t Number[10] = "";
	int32_t Size = 0;
	Size = Ymodem_Receive(&tab_1024[0]);
	if (Size > 0)
	{
		SerialPutString("\r\n Update Over!\r\n");
		SerialPutString(" Name: ");
		SerialPutString(file_name);
		Int2Str(Number, Size);
		SerialPutString("\r\n Size: ");
		SerialPutString(Number);
		SerialPutString(" Bytes.\r\n");
		return 0;
	}
	else if (Size == -1)
	{
		SerialPutString("\r\n Image Too Big!\r\n");
		return -1;
	}
	else if (Size == -2)
	{
		SerialPutString("\r\n Update failed!\r\n");
		return -2;
	}
	else if (Size == -3)
	{
		SerialPutString("\r\n Aborted by user.\r\n");
		return -3;
	}
	else
	{
		SerialPutString(" Receive Filed.\r\n");
		return -4;
	}
}


/************************************************************************/
int8_t IAP_Upload(void)
{
	uint32_t status = 0; 
	SerialPutString("\n\n\rSelect Receive File ... (press any key to abort)\n\r");
	if (GetKey() == CRC16)
	{
		status = Ymodem_Transmit((uint8_t*)ApplicationAddress, (const uint8_t*)"UploadedFlashImage.bin", FLASH_IMAGE_SIZE);
		if (status != 0) 
		{
			SerialPutString("\n\rError Occured while Transmitting File\n\r");
			return -1;
		}
		else
		{
			SerialPutString("\n\rFile Trasmitted Successfully \n\r");
			return -2;
		}
	}
	else
	{
		SerialPutString("\r\n\nAborted by user.\n\r");  
		return 0;
	}
}


/************************************************************************/
int8_t IAP_Erase(void)
{
	uint8_t erase_cont[3] = {0};
	Int2Str(erase_cont, FLASH_IMAGE_SIZE / PAGE_SIZE);
	SerialPutString(" @");//Ҫ�пո񣬱���bug
	SerialPutString(erase_cont);
	SerialPutString("@");
	if(EraseSomePages(FLASH_IMAGE_SIZE, 1))
		return 0;
	else
		return -1;
}
	
#endif

/************************************************************************/
void IAP_FLASH_WriteFlag(uint16_t flag) 
{
	FLASH_Unlock();
	STMFLASH_Write(IAP_FLASH_FLAG_ADDR, &flag, 1);
	FLASH_Lock();
}


/************************************************************************/
uint16_t IAP_FLASH_ReadFlag(void)
{
	return STMFLASH_ReadHalfWord(IAP_FLASH_FLAG_ADDR);   
}

/************************************************************************/
int8_t IAP_RunApp(void)
{
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
	{   
		#if (IAP_MODE == UART_MODE)
		SerialPutString("\r\n Run to app.\r\n");
		#endif
		JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		__set_MSP(*(__IO uint32_t*) ApplicationAddress);
		Jump_To_Application();
		return 0;
	}
	else
	{
		#if (IAP_MODE == UART_MODE)
		SerialPutString("\r\n Run to app error.\r\n");
		#endif
		return -1;
	}
}

