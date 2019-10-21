#include "stmflash.h"
#include "iap_config.h"
/**
  * @brief  Read half words (16-bit data) of the specified address
  * @note   This function can be used for all STM32F10x devices.
  * @param  faddr: The address to be read (the multiple of the address, which is 2)
  * @retval Value of specified address
  */
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}


/**
  * @brief  There is no check writing.
  * @note   This function can be used for all STM32F10x devices.
  * @param  WriteAddr: The starting address to be written.
  * @param  pBuffer: The pointer to the data.
  * @param  NumToWrite:  The number of half words written
  * @retval None
  */
static void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//add addr 2.
	}  
} 

u16 STMFLASH_BUF[PAGE_SIZE / 2];//Up to 2K bytes

/**
  * @brief  Write data from the specified address to the specified length.
  * @note   This function can be used for all STM32F10x devices.
  * @param  WriteAddr: The starting address to be written.(The address must be a multiple of two)
  * @param  pBuffer: The pointer to the data.
  * @param  NumToWrite:  The number of half words written
  * @retval None
  */
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if((WriteAddr < FLASH_BASE) || (WriteAddr >= FLASH_BASE + 1024 * FLASH_SIZE))return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr = WriteAddr - FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos = offaddr / PAGE_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff = (offaddr % PAGE_SIZE) / 2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain = PAGE_SIZE / 2 - secoff;		//����ʣ��ռ��С   
	if(NumToWrite <= secremain)
		secremain = NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos * PAGE_SIZE + FLASH_BASE, STMFLASH_BUF, PAGE_SIZE / 2);//������������������
		for(i = 0; i < secremain; i++)//У������
		{
			if(STMFLASH_BUF[secoff + i] != 0XFFFF)break;//��Ҫ����  	  
		}
		if(i < secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos * PAGE_SIZE + FLASH_BASE);//�����������
			for(i=0; i < secremain; i++)
			{
				STMFLASH_BUF[i + secoff] = pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos * PAGE_SIZE + FLASH_BASE, STMFLASH_BUF, PAGE_SIZE / 2);//д����������  
		}else 
			STMFLASH_Write_NoCheck(WriteAddr, pBuffer, secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite == secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff = 0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer += secremain;  	//ָ��ƫ��
			WriteAddr += secremain;	//д��ַƫ��	   
		   	NumToWrite -= secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite > (PAGE_SIZE / 2)) secremain = PAGE_SIZE / 2;//��һ����������д����
			else secremain = NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}

/**
  * @brief  Start reading the specified data from the specified address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  ReadAddr: Start addr
  * @param  pBuffer: The pointer to the data.
  * @param  NumToWrite:  The number of half words written(16bit)
  * @retval None
  */
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}















