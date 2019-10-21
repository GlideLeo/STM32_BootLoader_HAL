#ifndef __IAP_H__
#define __IAP_H__

#include <stdint.h>
#include "ymodem.h"
#include "main.h"
#include "iap_config.h"
#include "common.h"

/* Exported types ------------------------------------------------------------*/
typedef  void (*pFunction)(void);

extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;



extern uint32_t IAP_ReadFlag(void);
extern void IAP_WriteFlag(uint32_t flag);
extern int8_t IAP_RunApp(void);
extern void IAP_Main_Menu(void);
extern int8_t IAP_Update(void);
extern int8_t IAP_Upload(void);
extern int8_t IAP_Erase(void);



#endif
