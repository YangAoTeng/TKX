#ifndef __OLED_UI_DRIVER_H
#define __OLED_UI_DRIVER_H
/*【如果您需要移植此项目，则需要更改以下函数的实现方式。】 */
#include "main.h"

//获取确认，取消，上，下按键状态的函数(【Q：为什么使用宏定义而不是函数？A：因为这样可以提高效率，减少代码量】)
#define Key_GetEnterStatus()    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
#define Key_GetBackStatus()     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)
#define Key_GetUpStatus()       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define Key_GetDownStatus()     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)




//按键初始化函数
void Key_Init(void);

//编码器初始化函数
void Encoder_Init(void);

// 编码器使能函数
void Encoder_Enable(void);

//编码器失能函数
void Encoder_Disable(void);

//读取编码器的增量值
int16_t Encoder_Get(void);


#endif
