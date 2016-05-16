#include "bsp_fm25cl64.h"
#include "hal_spi2.h"

/* FM25CL64 -> 8K */

/*
需要移植的代码：
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//接口函数-毫秒级延时函数
#define BSP_FM25CL64_DelayMs_Port(ms)		CoTickDelay(ms)

//接口函数-FM25CL64 SPI读写一个字节
#define BSP_FM25CL64_ReadWrite_Port(data)		HAL_SPI2_ReadWriteByte(data)

//接口函数-FM25CL64 片选使能
#define BSP_FM25CL64_NSS_EN		GPIO_ResetBits(GPIOC, GPIO_Pin_0)

//接口函数-FM25CL64 片选失能
#define BSP_FM25CL64_NSS_DN		GPIO_SetBits(GPIOC, GPIO_Pin_0)

void BSP_FM25CL64_Init_Port(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	BSP_FM25CL64_NSS_DN;
	
	HAL_SPI2_Init();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BSP_FM25CL64_Init(void)
{
	BSP_FM25CL64_Init_Port();
}
/*
 ************************************************************
 *  名称：	BSP_FM25CL64_ReadState()
 *  功能：	读铁电状态寄存器
 *	输入：  无
 *	输出：  无
 ************************************************************
*/
u8 BSP_FM25CL64_ReadState(void)
{
  BSP_FM25CL64_NSS_EN;
  BSP_FM25CL64_ReadWrite_Port(FM25CL64_RDSR);
  u8 output = BSP_FM25CL64_ReadWrite_Port(0);
  BSP_FM25CL64_NSS_DN;
  
  return output;
}

/*
 ************************************************************
 *  名称：	BSP_FM25CL64_ReadState()
 *  功能：	铁电写使能
 *	输入：  无
 *	输出：  无
 ************************************************************
*/
void BSP_FM25CL64_WriteEnable(void)
{
  BSP_FM25CL64_NSS_EN;
  BSP_FM25CL64_ReadWrite_Port(FM25CL64_WREN);
  BSP_FM25CL64_NSS_DN;
}

/*
 ************************************************************
 *  名称：	BSP_FM25CL64_WriteState()
 *  功能：	写铁电状态寄存器
 *	输入：  无
 *	输出：  无
 ************************************************************
*/
void BSP_FM25CL64_WriteState(u8 data)
{
  BSP_FM25CL64_WriteEnable();
  
  BSP_FM25CL64_NSS_EN;
  BSP_FM25CL64_ReadWrite_Port(FM25CL64_WRSR);
  BSP_FM25CL64_ReadWrite_Port(data);
  BSP_FM25CL64_NSS_DN;
}

/*
 ************************************************************
 *  名称：	BSP_FM25CL64_ProAll()
 *  功能：	铁电保护全部
 *	输入：  无
 *	输出：  无
 ************************************************************
*/
void BSP_FM25CL64_ProAll(void)
{
  BSP_FM25CL64_WriteState(FM25CL64_PROTECT);
}

/*
 ************************************************************
 *  名称：	BSP_FM25CL64_UProAll()
 *  功能：	铁电去掉保护
 *	输入：  无
 *	输出：  无
 ************************************************************
*/
void BSP_FM25CL64_UProAll(void)
{
  BSP_FM25CL64_WriteState(FM25CL64_UNPROTECT);
}

/*
 ************************************************************
 *  名称：	BSP_FM25CL64_SingleRead()
 *  功能：	铁电读一个字节
 *	输入：  address-地址
 *	输出：  无
 ************************************************************
*/
u8 BSP_FM25CL64_SingleRead(u16 address)
{
  u8 output;
  
  BSP_FM25CL64_NSS_EN;
  BSP_FM25CL64_ReadWrite_Port(FM25CL64_READ);
  BSP_FM25CL64_ReadWrite_Port(((address & 0xff00) >> 8));
  BSP_FM25CL64_ReadWrite_Port((address & 0x00ff));
  output = BSP_FM25CL64_ReadWrite_Port(0);
  BSP_FM25CL64_NSS_DN;
  
  return output;
}

/*
 ************************************************************
 *  名称：	BSP_FM25CL64_MultipleRead()
 *  功能：	铁电读多个字节
 *	输入：  address-地址  num-个数  pointer-读出数据存放的地址指针
 *	输出：  无
 ************************************************************
*/
void BSP_FM25CL64_MultipleRead(u16 address, u16 num, u8 *pointer)
{	
  BSP_FM25CL64_NSS_EN;
  
	BSP_FM25CL64_ReadWrite_Port(FM25CL64_READ);
  BSP_FM25CL64_ReadWrite_Port((address & 0xff00) >> 8);
  BSP_FM25CL64_ReadWrite_Port(address & 0x00ff);
  
	for (u16 i=0; i < num; i++)
  {
    *(pointer + i) = BSP_FM25CL64_ReadWrite_Port(0);
  }
	
  BSP_FM25CL64_NSS_DN;
}

/*
 ************************************************************
 *  名称：	BSP_FM25CL64_SingleWrite()
 *  功能：	铁电写单个字节(写入之前先取消写保护！)
 *	输入：  address-地址  valve-写入数据数值
 *	输出：  无
 ************************************************************
*/
u8 BSP_FM25CL64_SingleWrite(u16 address, u8 valve)
{
  BSP_FM25CL64_WriteEnable();
	
  BSP_FM25CL64_NSS_EN;
  BSP_FM25CL64_ReadWrite_Port(FM25CL64_WRITE);
  BSP_FM25CL64_ReadWrite_Port((address & 0xff00) >> 8);
  BSP_FM25CL64_ReadWrite_Port(address & 0x00ff);
  u8 output = BSP_FM25CL64_ReadWrite_Port(valve);
  BSP_FM25CL64_NSS_DN;
  
  return output;
}

/*
 ************************************************************
 *  名称：	BSP_FM25CL64_MultipleWrite()
 *  功能：	铁电写多个字节(写入之前先取消写保护！)
 *	输入：  address-地址  num-个数  pointer-写入数据存放的地址指针
 *	输出：  无
 ************************************************************
*/
void BSP_FM25CL64_MultipleWrite(u16 address, u16 num, u8 *pointer)
{ 
  BSP_FM25CL64_WriteEnable();
	
  BSP_FM25CL64_NSS_EN;
  BSP_FM25CL64_ReadWrite_Port(FM25CL64_WRITE);
  BSP_FM25CL64_ReadWrite_Port((address & 0xff00) >> 8);
  BSP_FM25CL64_ReadWrite_Port(address & 0x00ff);
  for (u16 i = 0; i < num; i++)
  {
    BSP_FM25CL64_ReadWrite_Port(pointer[i]);
  }
  BSP_FM25CL64_NSS_DN;
}














