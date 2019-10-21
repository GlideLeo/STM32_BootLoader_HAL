
#include "iap.h"


pFunction Jump_To_Application;
uint32_t JumpAddress;
uint32_t BlockNbr = 0, UserMemoryMask = 0;
__IO uint32_t FlashProtection = 0;
uint8_t tab_1024[1024] = {0};
extern signed int OLED_printf(uint16_t color,const char *pFormat, ...);
extern RTC_HandleTypeDef hrtc;
#include "LCD.h"

/************************************************************************/
void IAP_WriteFlag(uint32_t flag)
{
	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0, flag);
}

/************************************************************************/
uint32_t IAP_ReadFlag(void)
{
	return HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0);
}





/************************************************************************/
int8_t IAP_RunApp(void)
{
	if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
	{   
		SerialPutString("\r\n Run to app.\r\n");
		JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
		Jump_To_Application();
		return 0;
	}
	else
	{
		SerialPutString("\r\n Run to app error.\r\n");
		return -1;
	}
}


/************************************************************************/
void IAP_Main_Menu(void)
{
	uint8_t cmdStr[CMD_STRING_SIZE] = {0};
//	BlockNbr = (APPLICATION_ADDRESS - 0x08000000) >> 12; //kb
//	

//	if (BlockNbr < 62)
//	{
//		UserMemoryMask = ((uint32_t)~((1 << BlockNbr) - 1));
//	}
//	else
//	{
//		UserMemoryMask = ((uint32_t)0x80000000);
//	}
	while (1)
	{
		SerialPutString("\r\n IAP Main Menu (V 0.1)\r\n");
		SerialPutString(" Update Image--------- update\n");
		SerialPutString(" Upload Image--------- upload\n");
		SerialPutString(" Erase Image----------- erase\n");
//		SerialPutString(" menu\r\n");
		SerialPutString(" Execute APP------------- run\n");
		OLED_printf(GREEN,"\n IAP Main Menu (V 0.1)\n");
		OLED_printf(GREEN," Update Image--------- update\n");
		OLED_printf(GREEN," Upload Image--------- upload\n");
		OLED_printf(GREEN," Erase Image----------- erase\n");
//		OLED_printf(GREEN," menu\n");
		OLED_printf(GREEN," Execute APP------------- run\n");
//		if(FlashProtection != 0)//There is write protected
//		{
//			SerialPutString(" diswp\r\n");
//		}
		
		GetInputString(cmdStr);
		SerialPutString(cmdStr);

		if(strcmp((char *)cmdStr, CMD_UPDATE_STR) == 0)
		{
			IAP_WriteFlag(UPDATE_FLAG_DATA);
			return;
		}
		else if(strcmp((char *)cmdStr, CMD_UPLOAD_STR) == 0)
		{
			IAP_WriteFlag(UPLOAD_FLAG_DATA);
			return;
		}
		else if(strcmp((char *)cmdStr, CMD_ERASE_STR) == 0)
		{
			IAP_WriteFlag(ERASE_FLAG_DATA);
			return;
		}
//		else if(strcmp((char *)cmdStr, CMD_MENU_STR) == 0)
//		{
//			IAP_WriteFlag(INIT_FLAG_DATA);
//		}
		else if(strcmp((char *)cmdStr, CMD_RUNAPP_STR) == 0)
		{
			IAP_WriteFlag(APPRUN_FLAG_DATA);
			return;
		}
//		else if(strcmp((char *)cmdStr, CMD_DISWP_STR) == 0)
//		{
////			FLASH_DisableWriteProtectionPages();
//		}
		else
		{
			SerialPutString(" Invalid CMD !\r\n");
			OLED_printf(GREEN," Invalid CMD !\n");
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
		OLED_printf(WHITE," \n Update Over!\n");
		SerialPutString(" Name: ");
		OLED_printf(WHITE," Name: %s",file_name);
		SerialPutString(file_name);
		Int2Str(Number, Size);
		SerialPutString("\r\n Size: ");
		OLED_printf(WHITE,"\n Size: %s",Number);
		SerialPutString(Number);
		SerialPutString(" Bytes.\r\n");
		OLED_printf(WHITE," Bytes.\n");
		return 0;
	}
	else if (Size == -1)
	{
		SerialPutString("\r\n Image Too Big!\r\n");
		OLED_printf(WHITE,"\n Image Too Big!\n");
		return -1;
	}
	else if (Size == -2)
	{
		SerialPutString("\r\n Update failed!\r\n");
		OLED_printf(WHITE,"\n Update failed!\n");
		return -2;
	}
	else if (Size == -3)
	{
		SerialPutString("\r\n Aborted by user.\r\n");
		OLED_printf(WHITE,"\n Aborted by user.\n");
		return -3;
	}
	else
	{
		SerialPutString(" Receive Filed.\r\n");
		OLED_printf(WHITE,"\n Receive Filed.\n");
		return -4;
	}
}


/************************************************************************/
int8_t IAP_Upload(void)
{
//	uint32_t status = 0; 
//	SerialPutString("\n\n\rSelect Receive File ... (press any key to abort)\n\r");
//	if (GetKey() == CRC16)
//	{
//		status = Ymodem_Transmit((uint8_t*)ApplicationAddress, (const uint8_t*)"UploadedFlashImage.bin", FLASH_IMAGE_SIZE);
//		if (status != 0) 
//		{
//			SerialPutString("\n\rError Occured while Transmitting File\n\r");
//			return -1;
//		}
//		else
//		{
//			SerialPutString("\n\rFile Trasmitted Successfully \n\r");
//			return -2;
//		}
//	}
//	else
//	{
//		SerialPutString("\r\n\nAborted by user.\n\r");  
//		return 0;
//	}

}


/************************************************************************/
int8_t IAP_Erase(void)
{
//	uint8_t erase_cont[3] = {0};
//	Int2Str(erase_cont, FLASH_IMAGE_SIZE / PAGE_SIZE);
//	SerialPutString(" @");//ÒªÓÐ¿Õ¸ñ£¬±ÜÃâbug
//	SerialPutString(erase_cont);
//	SerialPutString("@");
//	if(EraseSomePages(FLASH_IMAGE_SIZE, 1))
//		return 0;
//	else
//		return -1;

}
	

