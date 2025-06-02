#ifndef IIC_OLED_H
#define IIC_OLED_H

#include "main.h"
#include "i2c.h"

#include <math.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

// SSD1306 配置参数
#define OLED_ADDR                0x78  // OLED的I2C设备地址 (0x3C << 1)
#define OLED_CMD                 0x00  // 写命令
#define OLED_DATA                0x40  // 写数据
// #define OLED_WIDTH               128   // OLED宽度
// #define OLED_HEIGHT              64    // OLED高度
#define OLED_PAGE_NUM            8     // 页数 (64/8)
#define OLED_I2C_TIMEOUT         1000  // I2C通信超时时间

//	oled初始化函数
void OLED_Init(void);
// 设置颜色模式
void OLED_SetColorMode(bool colormode);
//	oled全局刷新函数
void OLED_Update(void);
//	oled局部刷新函数
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);


// OLED 设置亮度函数
void OLED_Brightness(int16_t Brightness);


// 底层通信函数
HAL_StatusTypeDef OLED_Write_Cmd(uint8_t cmd);
HAL_StatusTypeDef OLED_Write_Data(uint8_t data);

#endif // !IIC_OLED_H