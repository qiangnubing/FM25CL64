#ifndef __BSP_FM25CL64_H__
#define __BSP_FM25CL64_H__

#include "user_config.h"

//----------------FM25CL64 Instruction-----
#define FM25CL64_WREN				0x06
#define FM25CL64_WRDI				0x04
#define FM25CL64_RDSR				0x05
#define FM25CL64_WRSR				0x01
#define FM25CL64_READ				0x03
#define FM25CL64_WRITE			0x02
#define FM25CL64_PROTECT 		0x0c
#define FM25CL64_UNPROTECT	0x00


void BSP_FM25CL64_Init(void);

void BSP_FM25CL64_ProAll(void);
void BSP_FM25CL64_UProAll(void);

u8 BSP_FM25CL64_SingleRead(u16 address);
void BSP_FM25CL64_MultipleRead(u16 address, u16 num, u8 *pointer);
u8 BSP_FM25CL64_SingleWrite(u16 address, u8 valve);
void BSP_FM25CL64_MultipleWrite(u16 address, u16 num, u8 *pointer);
















#endif
