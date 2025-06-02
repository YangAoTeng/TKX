#ifndef IIC_OLED_H
#define IIC_OLED_H

#include "main.h"
#include "i2c.h"
#include <stdarg.h>  // 添加对可变参数的支持

// SSD1306 配置参数
#define OLED_ADDR                0x78  // OLED的I2C设备地址 (0x3C << 1)
#define OLED_CMD                 0x00  // 写命令
#define OLED_DATA                0x40  // 写数据
#define OLED_WIDTH               128   // OLED宽度
#define OLED_HEIGHT              64    // OLED高度
#define OLED_PAGE_NUM            8     // 页数 (64/8)
#define OLED_I2C_TIMEOUT         1000  // I2C通信超时时间

// 初始化与控制命令
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);

// 显示控制函数
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t mode);
void OLED_ShowChar(uint8_t x, uint8_t y, char chr, uint8_t size, uint8_t mode);
void OLED_ShowString(uint8_t x, uint8_t y, const char *str, uint8_t size, uint8_t mode);
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t index, uint8_t size, uint8_t mode);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode);
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t *BMP);

// 格式化输出函数
void OLED_Printf(uint8_t x, uint8_t y, uint8_t size, uint8_t mode, const char *fmt, ...);

// 底层通信函数
HAL_StatusTypeDef OLED_Write_Cmd(uint8_t cmd);
HAL_StatusTypeDef OLED_Write_Data(uint8_t data);

#endif // !IIC_OLED_H