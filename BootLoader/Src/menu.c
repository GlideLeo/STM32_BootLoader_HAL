/**
  ******************************************************************************
  * @file    IAP/IAP_Main/Src/menu.c 
  * @author  MCD Application Team

  * @brief   This file provides the software which contains the main menu routine.
  *          The main menu gives the options of:
  *             - downloading a new binary file, 
  *             - uploading internal flash memory,
  *             - executing the binary file already loaded 
  *             - configuring the write protection of the Flash sectors where the 
  *               user loads his binary file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/** @addtogroup STM32F4xx_IAP_Main
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "flash_if.h"
#include "menu.h"
#include "main.h"
#include "ymodem.h"
extern UART_HandleTypeDef huart1;
#define UartHandle  huart1
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction JumpToApplication;
uint32_t JumpAddress;
uint32_t FlashProtection = 0;
uint8_t aFileName[FILE_NAME_LENGTH];

/* Private function prototypes -----------------------------------------------*/
void SerialDownload(void);
void SerialUpload(void);
void RunApp(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Run to app
  * @param  None
  * @retval None
  */
void RunApp(void)
{
			Serial_PutString((uint8_t *)"Start program execution......\r\n\n");
      /* execute the new program */
      JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
      /* Jump to user application */
      JumpToApplication = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
      JumpToApplication();
}
/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
void SerialDownload(void)
{
  uint8_t number[11] = {0};
  uint32_t size = 0;
  COM_StatusTypeDef result;

  Serial_PutString((uint8_t *)"Waiting for the file to be sent ... (press 'a' to abort)\n\r");
  result = Ymodem_Receive( &size );
  if (result == COM_OK)
  {
    Serial_PutString((uint8_t *)"\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
    Serial_PutString(aFileName);
    Int2Str(number, size);
    Serial_PutString((uint8_t *)"\n\r Size: ");
    Serial_PutString(number);
    Serial_PutString((uint8_t *)" Bytes\r\n");
    Serial_PutString((uint8_t *)"-------------------\n");
  }
  else if (result == COM_LIMIT)
  {
    Serial_PutString((uint8_t *)"\n\n\rThe image size is higher than the allowed space memory!\n\r");
  }
  else if (result == COM_DATA)
  {
    Serial_PutString((uint8_t *)"\n\n\rVerification failed!\n\r");
  }
  else if (result == COM_ABORT)
  {
    Serial_PutString((uint8_t *)"\r\n\nAborted by user.\n\r");
  }
  else
  {
    Serial_PutString((uint8_t *)"\n\rFailed to receive the file!\n\r");
  }
}

/**
  * @brief  Upload a file via serial port.
  * @param  None
  * @retval None
  */
void SerialUpload(void)
{
  uint8_t status = 0;

  Serial_PutString((uint8_t *)"\n\n\rSelect Receive File\n\r");

  HAL_UART_Receive(&UartHandle, &status, 1, RX_TIMEOUT);
  if ( status == CRC16)
  {
    /* Transmit the flash image through ymodem protocol */
    status = Ymodem_Transmit((uint8_t*)APPLICATION_ADDRESS, (const uint8_t*)"UploadedFlashImage.bin", USER_FLASH_SIZE);

    if (status != 0)
    {
      Serial_PutString((uint8_t *)"\n\rError Occurred while Transmitting File\n\r");
    }
    else
    {
      Serial_PutString((uint8_t *)"\n\rFile uploaded successfully \n\r");
    }
  }
}

/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
void Main_Menu(void)
{
  uint8_t key = 0;

  Serial_PutString((uint8_t *)"\r\n======================================================================");
  Serial_PutString((uint8_t *)"\r\n=              (C) COPYRIGHT(c) 2016 STMicroelectronics                 =");
  Serial_PutString((uint8_t *)"\r\n=                                                                    =");
  Serial_PutString((uint8_t *)"\r\n=          STM32F4xx In-Application Programming Application          =");
  Serial_PutString((uint8_t *)"\r\n=                                                                    =");
  Serial_PutString((uint8_t *)"\r\n=                       By MCD Application Team                      =");
  Serial_PutString((uint8_t *)"\r\n======================================================================");
  Serial_PutString((uint8_t *)"\r\n\r\n");

  while (1)
  {

    /* Test if any sector of Flash memory where user application will be loaded is write protected */
    FlashProtection = FLASH_If_GetWriteProtectionStatus();
    
    Serial_PutString((uint8_t *)"\r\n=================== Main Menu ============================\r\n\n");
    Serial_PutString((uint8_t *)"  Download image to the internal Flash ----------------- 1\r\n\n");
    Serial_PutString((uint8_t *)"  Upload image from the internal Flash ----------------- 2\r\n\n");
    Serial_PutString((uint8_t *)"  Execute the loaded application ----------------------- 3\r\n\n");

    if(FlashProtection != FLASHIF_PROTECTION_NONE)
    {
      Serial_PutString((uint8_t *)"  Disable the write protection ------------------------- 4\r\n\n");
    }
    else
    {
      Serial_PutString((uint8_t *)"  Enable the write protection -------------------------- 4\r\n\n");
    }
    Serial_PutString((uint8_t *)"==========================================================\r\n\n");

    /* Clean the input path */
    __HAL_UART_FLUSH_DRREGISTER(&UartHandle);
	
    /* Receive key */
    HAL_UART_Receive(&UartHandle, &key, 1, RX_TIMEOUT);

    switch (key)
    {
    case '1' :
      /* Download user application in the Flash */
      SerialDownload();
      break;
    case '2' :
      /* Upload user application from the Flash */
      SerialUpload();
      break;
    case '3' :
      Serial_PutString((uint8_t *)"Start program execution......\r\n\n");
      /* execute the new program */
      JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
      /* Jump to user application */
      JumpToApplication = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
      JumpToApplication();
      break;
    case '4' :
      if (FlashProtection != FLASHIF_PROTECTION_NONE)
      {
        /* Disable the write protection */
        if (FLASH_If_WriteProtectionConfig(OB_WRPSTATE_DISABLE) == HAL_OK)
        {
          Serial_PutString((uint8_t *)"Write Protection disabled...\r\n");
          Serial_PutString((uint8_t *)"System will now restart...\r\n");
          /* Launch the option byte loading */
          HAL_FLASH_OB_Launch();
          /* Ulock the flash */
          HAL_FLASH_Unlock();
        }
        else
        {
          Serial_PutString((uint8_t *)"Error: Flash write un-protection failed...\r\n");
        }
      }
      else
      {
        if (FLASH_If_WriteProtectionConfig(OB_WRPSTATE_ENABLE) == HAL_OK)
        {
          Serial_PutString((uint8_t *)"Write Protection enabled...\r\n");
          Serial_PutString((uint8_t *)"System will now restart...\r\n");
          /* Launch the option byte loading */
          HAL_FLASH_OB_Launch();
        }
        else
        {
          Serial_PutString((uint8_t *)"Error: Flash write protection failed...\r\n");
        }
      }
      break;
    default:
	Serial_PutString((uint8_t *)"Invalid Number ! ==> The number should be either 1, 2, 3 or 4\r");
	break;
    }
  }
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
