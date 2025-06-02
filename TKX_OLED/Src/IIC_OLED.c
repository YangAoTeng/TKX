// filepath: c:\Users\q8979\Desktop\TKX\TKX_EEPROM\Src\IIC_OLED.c

#include "IIC_OLED.h"
#include "i2c.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 外部I2C句柄声明，假设I2C1被用于OLED通信
extern I2C_HandleTypeDef hi2c1;

// 添加在头部定义部分
bool OLED_ColorMode = true; // 0: 正常模式, 1: 反色模式

uint8_t OLED_DisplayBuf[8][128]; // 显存缓冲区，8页，每页128列

/**
 * @brief  向OLED写入命令
 * @param  cmd: 要写入的命令
 * @retval HAL状态
 */
HAL_StatusTypeDef OLED_Write_Cmd(uint8_t cmd)
{
    uint8_t buffer[2];
    buffer[0] = OLED_CMD;    // 命令控制字节
    buffer[1] = cmd;         // 命令字节
    
    return HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDR, buffer, 2, OLED_I2C_TIMEOUT);
}

/**
 * @brief  向OLED写入数据
 * @param  data: 要写入的数据
 * @retval HAL状态
 */
HAL_StatusTypeDef OLED_Write_Data(uint8_t data)
{
    uint8_t buffer[2];
    buffer[0] = OLED_DATA;   // 数据控制字节
    buffer[1] = data;        // 数据字节
    
    return HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDR, buffer, 2, OLED_I2C_TIMEOUT);
}


/**
 * @brief  连续写入多个数据
 * @param  Data: 数据数组指针
 * @param  Count: 数据数量
 */
void OLED_WriteData(uint8_t* Data, uint16_t Count)
{
    uint8_t i;
    uint8_t buffer[129]; // 最大一次写入128个数据，加上控制字节
    
    buffer[0] = OLED_DATA; // 控制字节，给0x40，表示即将写数据
    
    /*循环Count次，进行连续的数据写入*/
    for (i = 0; i < Count; i++)
    {
        if(OLED_ColorMode){
            buffer[i + 1] = Data[i]; // 依次填充Data的每一个数据
        }else{
            buffer[i + 1] = ~Data[i]; // 依次填充Data的每一个数据的反码
        }
    }
    
    // 使用HAL的I2C发送函数替代直接操作I2C引脚
    HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDR, buffer, Count + 1, OLED_I2C_TIMEOUT);
}

/**
 * @brief 设置显示模式
 * @param colormode true: 黑色模式，false: 白色模式
 * @return 无
 */
void OLED_SetColorMode(bool colormode){
	OLED_ColorMode = colormode;
}


/**
 * @brief  初始化OLED
 */
void OLED_Init(void)
{
    HAL_Delay(100);  // 等待OLED稳定
    
    // 初始化SSD1306
    OLED_Write_Cmd(0xAE);    // 关闭显示
    OLED_Write_Cmd(0xD5);    // 设置显示时钟分频比/振荡器频率
    OLED_Write_Cmd(0x80);    // 设置分频比，设置时钟为100帧/秒
    OLED_Write_Cmd(0xA8);    // 设置多路复用率
    OLED_Write_Cmd(0x3F);    // 1/64 duty
    OLED_Write_Cmd(0xD3);    // 设置显示偏移
    OLED_Write_Cmd(0x00);    // 不偏移
    OLED_Write_Cmd(0x40);    // 设置显示开始行 [5:0]
    OLED_Write_Cmd(0x8D);    // 电荷泵设置
    OLED_Write_Cmd(0x14);    // 启用电荷泵
    OLED_Write_Cmd(0x20);    // 设置内存地址模式
    OLED_Write_Cmd(0x02);    // 页地址模式
    OLED_Write_Cmd(0xA1);    // 段重定义设置，0xA1正常，0xA0左右反置
    OLED_Write_Cmd(0xC8);    // 设置COM扫描方向，0xC8正常，0xC0上下反置
    OLED_Write_Cmd(0xDA);    // 设置COM硬件引脚配置
    OLED_Write_Cmd(0x12);    // [5:4] COM引脚配置
    OLED_Write_Cmd(0x81);    // 对比度设置
    OLED_Write_Cmd(0xCF);    // 设置对比度为0xCF
    OLED_Write_Cmd(0xD9);    // 设置预充电周期
    OLED_Write_Cmd(0xF1);    // [3:0],PHASE 1;[7:4],PHASE 2;
    OLED_Write_Cmd(0xDB);    // 设置VCOMH电压倍率
    OLED_Write_Cmd(0x30);    // [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_Write_Cmd(0xA4);    // 全局显示开启;0xA4,开启;0xA5,关闭;
    OLED_Write_Cmd(0xA6);    // 设置显示方式;0xA6,正常显示;0xA7,反相显示;
    OLED_Write_Cmd(0xAF);    // 开启显示
    
    // OLED_Clear();            // 清屏
}

// /**
//  * @brief  清屏
//  */
// void OLED_Clear(void)
// {
//     uint8_t i, n;
    
//     for(i = 0; i < OLED_PAGE_NUM; i++)
//     {
//         OLED_SetPos(0, i);
//         for(n = 0; n < OLED_WIDTH; n++)
//         {
//             OLED_Write_Data(0x00);
//         }
//     }
// }

/**
 * @brief  设置OLED光标位置
 * @param  page: 页地址，范围0-7
 * @param  col: 列地址，范围0-127
 */
void OLED_SetCursor(uint8_t page, uint8_t col)
{
    OLED_Write_Cmd(0xB0 + page);              // 设置页地址
    OLED_Write_Cmd(0x10 | ((col & 0xF0) >> 4)); // 设置列地址高4位
    OLED_Write_Cmd(0x00 | (col & 0x0F));        // 设置列地址低4位
}

/**
 * @brief  将OLED显存数组更新到OLED屏幕
 * @note   所有的显示函数，都只是对OLED显存数组进行读写
 *         随后调用OLED_Update函数或OLED_UpdateArea函数
 *         才会将显存数组的数据发送到OLED硬件，进行显示
 */
void OLED_Update(void)
{
    uint8_t j;
    /*遍历每一页*/
    for (j = 0; j < 8; j++)
    {
        /*设置光标位置为每一页的第一列*/
        OLED_SetCursor(j, 0);
        /*连续写入128个数据，将显存数组的数据写入到OLED硬件*/
        OLED_WriteData(OLED_DisplayBuf[j], 128);
    }
}

/**
 * @brief  将OLED显存数组部分更新到OLED屏幕
 * @param  X: 指定区域左上角的横坐标，范围：0~127
 * @param  Y: 指定区域左上角的纵坐标，范围：0~63
 * @param  Width: 指定区域的宽度，范围：0~128
 * @param  Height: 指定区域的高度，范围：0~64
 * @note   此函数会至少更新参数指定的区域
 *         如果更新区域Y轴只包含部分页，则同一页的剩余部分会跟随一起更新
 */
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
    uint8_t j;
    
    /*参数检查，保证指定区域不会超出屏幕范围*/
    if (X > 128-1) {return;}
    if (Y > 64-1) {return;}
    if (X + Width > 128) {Width = 128 - X;}
    if (Y + Height > 64) {Height = 64 - Y;}
    
    /*遍历指定区域涉及的相关页*/
    /*(Y + Height - 1) / 8 + 1的目的是(Y + Height) / 8并向上取整*/
    for (j = Y / 8; j < (Y + Height - 1) / 8 + 1; j++)
    {
        /*设置光标位置为相关页的指定列*/
        OLED_SetCursor(j, X);
        /*连续写入Width个数据，将显存数组的数据写入到OLED硬件*/
        OLED_WriteData(&OLED_DisplayBuf[j][X], Width);
    }
}

/**
 * @brief  OLED设置亮度
 * @param  Brightness: 亮度值，0-255，不同显示芯片效果可能不相同
 * @note   不要设置过大或者过小
 */
void OLED_Brightness(int16_t Brightness)
{
    if(Brightness > 255) {
        Brightness = 255;
    }
    if(Brightness < 0) {
        Brightness = 0;
    }
    OLED_Write_Cmd(0x81);
    OLED_Write_Cmd(Brightness);
}




