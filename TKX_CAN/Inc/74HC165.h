#ifndef __74HC165_H
#define __74HC165_H
#include "main.h"

// 定义74HC165引脚
#define HC165_PL_PORT       GPIOE
#define HC165_PL_PIN        GPIO_PIN_5
#define HC165_CP_PORT       GPIOE
#define HC165_CP_PIN        GPIO_PIN_6
#define HC165_Q7_PORT       GPIOC
#define HC165_Q7_PIN        GPIO_PIN_0

// 定义引脚操作宏
#define HC165_PL_HIGH()     HAL_GPIO_WritePin(HC165_PL_PORT, HC165_PL_PIN, GPIO_PIN_SET)
#define HC165_PL_LOW()      HAL_GPIO_WritePin(HC165_PL_PORT, HC165_PL_PIN, GPIO_PIN_RESET)
#define HC165_CP_HIGH()     HAL_GPIO_WritePin(HC165_CP_PORT, HC165_CP_PIN, GPIO_PIN_SET)
#define HC165_CP_LOW()      HAL_GPIO_WritePin(HC165_CP_PORT, HC165_CP_PIN, GPIO_PIN_RESET)
#define HC165_Q7_READ()     HAL_GPIO_ReadPin(HC165_Q7_PORT, HC165_Q7_PIN)

// 函数声明
void HC165_Init(void);
uint8_t HC165_ReadByte(void);
void HC165_ReadMultiBytes(uint8_t* buffer, uint8_t len);
uint8_t HC165_ReadIO(uint8_t ioNum);
uint32_t HC165_ReadAllIOs(void);

#endif // !__74HC165_H
