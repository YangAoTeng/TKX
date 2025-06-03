#include "lcd_st7735s_driver.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;  // 假设SPI1句柄在main.c中已定义

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160

uint16_t lcd_buffer[SCREEN_WIDTH * SCREEN_HEIGHT]; // 用于存储屏幕数据

/**
 * @brief 通过SPI发送单字节数据
 * @param data 要发送的数据
 */
void LCD_SPI_Send(uint8_t data)
{
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
}

/**
 * @brief 向LCD发送命令
 * @param cmd 命令字节
 */
void LCD_Write_Command(uint8_t cmd)
{
    LCD_CS_Clr();         // 片选拉低，选中设备
    LCD_DC_Clr();         // DC拉低，表示发送命令
    LCD_SPI_Send(cmd);    // 发送命令字节
    LCD_CS_Set();         // 片选拉高，释放设备
}

/**
 * @brief 向LCD发送数据
 * @param data 数据字节
 */
void LCD_Write_Data(uint8_t data)
{
    LCD_CS_Clr();         // 片选拉低，选中设备
    LCD_DC_Set();         // DC拉高，表示发送数据
    LCD_SPI_Send(data);   // 发送数据字节
    LCD_CS_Set();         // 片选拉高，释放设备
}

/**
 * @brief 向LCD发送16位RGB数据
 * @param rgb 16位RGB数据
 */
void LCD_Write_RGB(uint16_t rgb)
{
    LCD_CS_Clr();           // 片选拉低，选中设备
    LCD_DC_Set();           // DC拉高，表示发送数据
    LCD_SPI_Send(rgb >> 8); // 发送高8位
    LCD_SPI_Send(rgb);      // 发送低8位
    LCD_CS_Set();           // 片选拉高，释放设备
}

/**
 * @brief 批量发送RGB数据
 * @param rgb_data RGB数据数组指针
 * @param count 数据数量
 */
void LCD_Write_RGB_Bulk(uint16_t *rgb_data, uint32_t count)
{
    uint32_t i;
    
    LCD_CS_Clr();         // 片选拉低，选中设备
    LCD_DC_Set();         // DC拉高，表示发送数据
    
    for (i = 0; i < count; i++)
    {
        LCD_SPI_Send(rgb_data[i] >> 8);  // 发送高8位
        LCD_SPI_Send(rgb_data[i]);       // 发送低8位
    }
    
    LCD_CS_Set();         // 片选拉高，释放设备
}

// 优化的批量发送RGB数据方法，直接批量发送数据
void LCD_Write_RGB_Bulk_Fast(uint16_t *rgb_data, uint32_t count)
{
    if (count == 0) return;
    
    LCD_CS_Clr();         // 片选拉低，选中设备
    LCD_DC_Set();         // DC拉高，表示发送数据
    
    // 直接批量发送，不进行数据格式转换
    HAL_SPI_Transmit(&hspi1, (uint8_t*)rgb_data, count * 2, HAL_MAX_DELAY);
    
    LCD_CS_Set();         // 片选拉高，释放设备
}

/**
 * @brief 延时函数
 * @param ms 延时毫秒数
 */
void LCD_Delay(uint32_t ms)
{
    osDelay(ms); // 使用FreeRTOS的延时函数
}

/**
 * @brief ST7735S LCD初始化
 */
void LCD_ST7735S_Init(void)
{
    // 复位LCD
    LCD_RES_Clr();
    LCD_Delay(1);
    LCD_RES_Set();
    LCD_Delay(120); // 等待LCD复位完成
    
    // 退出睡眠模式
    LCD_Write_Command(0x11); // Sleep out 
    LCD_Delay(120);         // Delay 120ms 
    
    //------------------------------------ST7735S Frame Rate-----------------------------------------// 
    LCD_Write_Command(0xB1); 
    LCD_Write_Data(0x05); 
    LCD_Write_Data(0x3C); 
    LCD_Write_Data(0x3C); 
    
    LCD_Write_Command(0xB2); 
    LCD_Write_Data(0x05); 
    LCD_Write_Data(0x3C); 
    LCD_Write_Data(0x3C); 
    
    LCD_Write_Command(0xB3); 
    LCD_Write_Data(0x05); 
    LCD_Write_Data(0x3C); 
    LCD_Write_Data(0x3C); 
    LCD_Write_Data(0x05); 
    LCD_Write_Data(0x3C); 
    LCD_Write_Data(0x3C); 
    
    //------------------------------------End ST7735S Frame Rate---------------------------------// 
    LCD_Write_Command(0xB4); // Dot inversion 
    LCD_Write_Data(0x03); 
    
    //------------------------------------ST7735S Power Sequence---------------------------------// 
    LCD_Write_Command(0xC0); 
    LCD_Write_Data(0x28); 
    LCD_Write_Data(0x08); 
    LCD_Write_Data(0x04); 
    
    LCD_Write_Command(0xC1); 
    LCD_Write_Data(0xC0); 
    
    LCD_Write_Command(0xC2); 
    LCD_Write_Data(0x0D); 
    LCD_Write_Data(0x00); 
    
    LCD_Write_Command(0xC3); 
    LCD_Write_Data(0x8D); 
    LCD_Write_Data(0x2A); 
    
    LCD_Write_Command(0xC4); 
    LCD_Write_Data(0x8D); 
    LCD_Write_Data(0xEE); 
    
    //---------------------------------End ST7735S Power Sequence-------------------------------------// 
    LCD_Write_Command(0xC5); // VCOM 
    LCD_Write_Data(0x1A); 
    
    LCD_Write_Command(0x36); // MX, MY, RGB mode 
    LCD_Write_Data(0xC0); 
    
    //------------------------------------ST7735S Gamma Sequence---------------------------------// 
    LCD_Write_Command(0xE0); 
    LCD_Write_Data(0x04); 
    LCD_Write_Data(0x22); 
    LCD_Write_Data(0x07); 
    LCD_Write_Data(0x0A); 
    LCD_Write_Data(0x2E); 
    LCD_Write_Data(0x30); 
    LCD_Write_Data(0x25); 
    LCD_Write_Data(0x2A); 
    LCD_Write_Data(0x28); 
    LCD_Write_Data(0x26); 
    LCD_Write_Data(0x2E); 
    LCD_Write_Data(0x3A); 
    LCD_Write_Data(0x00); 
    LCD_Write_Data(0x01); 
    LCD_Write_Data(0x03); 
    LCD_Write_Data(0x13); 
    
    LCD_Write_Command(0xE1); 
    LCD_Write_Data(0x04); 
    LCD_Write_Data(0x16); 
    LCD_Write_Data(0x06); 
    LCD_Write_Data(0x0D); 
    LCD_Write_Data(0x2D); 
    LCD_Write_Data(0x26); 
    LCD_Write_Data(0x23); 
    LCD_Write_Data(0x27); 
    LCD_Write_Data(0x27); 
    LCD_Write_Data(0x25); 
    LCD_Write_Data(0x2D); 
    LCD_Write_Data(0x3B); 
    LCD_Write_Data(0x00); 
    LCD_Write_Data(0x01); 
    LCD_Write_Data(0x04); 
    LCD_Write_Data(0x13); 
    
    //------------------------------------End ST7735S Gamma Sequence-----------------------------// 
    LCD_Write_Command(0x3A); // 65k mode 
    LCD_Write_Data(0x05); 
    
    LCD_Write_Command(0x29); // Display on
    
    // 打开背光
    LCD_BL_Set();
    // LCD_Write_Command(0x01); // Memory write command to prepare for drawing
}

/**
 * @brief 指定LCD显示范围
 * @param xs 起始列地址
 * @param xe 结束列地址
 * @param ys 起始行地址
 * @param ye 结束行地址
 */
void LCD_ST7735S_SpecifyScope(uint8_t xs, uint8_t xe, uint8_t ys, uint8_t ye)
{
    LCD_Write_Command(0x2A);    // 列地址设置命令(Column Address Set)
    LCD_Write_Data(0x00);       // 起始列地址高8位
    LCD_Write_Data(xs);         // 起始列地址低8位
    LCD_Write_Data(0x00);       // 结束列地址高8位
    LCD_Write_Data(xe);         // 结束列地址低8位
    
    LCD_Write_Command(0x2B);    // 行地址设置命令(Row Address Set)
    LCD_Write_Data(0x00);       // 起始行地址高8位
    LCD_Write_Data(ys);         // 起始行地址低8位
    LCD_Write_Data(0x00);       // 结束行地址高8位
    LCD_Write_Data(ye);         // 结束行地址低8位
    
    LCD_Write_Command(0x2C);    // 开始内存写入命令(Memory Write)
}



/**
 * @brief 用指定颜色刷新整个屏幕
 * @param rgb 16位RGB颜色值
 */
void LCD_ST7735S_RefreshAll(uint16_t rgb)
{
    uint16_t swapped_rgb = ((rgb >> 8) | (rgb << 8));
    // 用指定颜色填充lcd_buffer数组
    for (uint32_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        lcd_buffer[i] = swapped_rgb;
    }
    
    // 设置显示范围为整个屏幕 (128x160)
    LCD_ST7735S_SpecifyScope(0, SCREEN_WIDTH-1, 0, SCREEN_HEIGHT-1);
    
    // 使用批量发送函数一次性刷新整个屏幕
    LCD_Write_RGB_Bulk_Fast(lcd_buffer, SCREEN_WIDTH * SCREEN_HEIGHT);
}


