#ifndef __LCD_ST7735S_DRIVER_H
#define __LCD_ST7735S_DRIVER_H
#include "main.h"


// LCD RES  
#define LCD_RES_Clr()  HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET)   // 复位 RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET)     // 置位 RES

// LCD DC
#define LCD_DC_Clr()   HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)    // 复位 DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)      // 置位 DC

// LCD CS
#define LCD_CS_Clr()   HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET)    // 复位 CS
#define LCD_CS_Set()   HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET)      // 置位 CS

// LCD BL
#define LCD_BL_Clr()   HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET)    // 复位 BL
#define LCD_BL_Set()   HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET)      // 置位 BL

// SPI通信函数声明
void LCD_SPI_Send(uint8_t data);                         // 通过SPI发送单字节数据
void LCD_Write_Command(uint8_t cmd);                     // 写命令
void LCD_Write_Data(uint8_t data);                       // 写数据
void LCD_Write_RGB(uint16_t rgb);                        // 写RGB数据(16位)
void LCD_Write_RGB_Bulk_Fast(uint16_t *rgb_data, uint32_t count);

// LCD初始化函数
void LCD_ST7735S_Init(void);
void LCD_Delay(uint32_t ms);

// 添加新函数声明
void LCD_ST7735S_SpecifyScope(uint8_t xs, uint8_t xe, uint8_t ys, uint8_t ye);
void LCD_ST7735S_RefreshAll(uint16_t rgb);

#endif // __LCD_ST7735S_DRIVER_H