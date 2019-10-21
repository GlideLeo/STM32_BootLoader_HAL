#ifndef __IAP_CONFIG_H__
#define __IAP_CONFIG_H__
/* Define if use bkp save flag  -------------------------------*/
#define USE_BKP_SAVE_FLAG     1

/* Define the APP start address -------------------------------*/


/* End of the Flash address */
#define USER_FLASH_END_ADDRESS        0x080FFFFF
/* Define the user application size */
#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)

/* Define the address from where user application will be loaded.
   Note: the 1st sector 0x08000000-0x08003FFF is reserved for the IAP code */
#define APPLICATION_ADDRESS   (uint32_t)0x08010000 

/* Output printer switch --------------------------------------*/
#define ENABLE_PUTSTR         1

/* Bootloader command -----------------------------------------*/
#define CMD_UPDATE_STR        "update"
#define CMD_UPLOAD_STR        "upload"
#define CMD_ERASE_STR		  		"erase"
#define CMD_MENU_STR          "menu"
#define CMD_RUNAPP_STR        "runapp"
#define CMD_ERROR_STR         "error"
#define CMD_DISWP_STR         "diswp"//禁止写保护

/* IAP command------------------------------------------------ */

#define IAP_FLAG_ADDR    BKP_DR1


//#define IAP_FLASH_FLAG_ADDR   0x8002800

#if (USE_BKP_SAVE_FLAG == 1)
  #define INIT_FLAG_DATA      0x0000  //默认标志的数据(空片子的情况)
#else
  #define INIT_FLAG_DATA      0xFFFF   //默认标志的数据(空片子的情况)
#endif
#define UPDATE_FLAG_DATA      0xEEEE   //下载标志的数据
#define UPLOAD_FLAG_DATA      0xDDDD   //上传标志的数据
#define ERASE_FLAG_DATA       0xCCCC   //擦除标志的数据
#define APPRUN_FLAG_DATA      0x5A5A   //APP不需要做任何处理，直接运行状态

/* Define the Flsah area size ---------------------------------*/  

// #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
// #define FLASH_SIZE                        (0x100000) /* 1 MByte */


/* Compute the FLASH upload image size --------------------------*/  
#define FLASH_IMAGE_SIZE                   (uint32_t) (FLASH_SIZE - (ApplicationAddress - 0x08000000))

/* The maximum length of the command string -------------------*/
#define CMD_STRING_SIZE       128

#endif
