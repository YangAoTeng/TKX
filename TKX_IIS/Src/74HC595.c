#include "74HC595.h"

// 定义全局变量保存74HC595的状态(3个byte，共24位，对应0-23 IO)
static uint8_t HC595_Buffer[3] = {0};

/**
 * @brief  初始化74HC595相关引脚
 * @param  无
 * @retval 无
 */
void HC595_Init(void)
{
    // 初始状态：时钟低，锁存低
    HC595_SHCP_LOW();
    HC595_STCP_LOW();
    HC595_DS_LOW();
    
    // 清空所有输出
    HC595_ClearAllIOs();
}

/**
 * @brief  发送一个字节到74HC595
 * @param  byte: 要发送的字节
 * @retval 无
 */
void HC595_WriteByte(uint8_t byte)
{
    uint8_t i;
    
    // 按位发送数据，从高位到低位
    for (i = 0; i < 8; i++)
    {
        // 先将时钟拉低
        HC595_SHCP_LOW();
        
        // 设置数据位
        if (byte & 0x80)
            HC595_DS_HIGH();
        else
            HC595_DS_LOW();
        
        // 时钟上升沿，数据被移入寄存器
        HC595_SHCP_HIGH();
        
        // 左移一位，准备发送下一位
        byte <<= 1;
    }
}

/**
 * @brief  锁存数据，使移位寄存器中的数据输出到存储寄存器
 * @param  无
 * @retval 无
 */
void HC595_Latch(void)
{
    HC595_STCP_LOW();
    HC595_STCP_HIGH();
    HC595_STCP_LOW();
}

/**
 * @brief  发送多个字节到多个级联的74HC595
 * @param  bytes: 要发送的字节数组
 * @param  len: 字节数组长度
 * @retval 无
 */
void HC595_WriteMultiBytes(uint8_t* bytes, uint8_t len)
{
    uint8_t i;
    
    // 从最后一个芯片开始发送数据（最远端的芯片）
    for (i = len; i > 0; i--)
    {
        HC595_WriteByte(bytes[i-1]);
    }
    
    // 所有数据发送完成后，锁存输出
    HC595_Latch();
}

/**
 * @brief  设置指定IO为高电平
 * @param  ioNum: IO编号(0-23)
 * @retval 无
 */
void HC595_SetIOHigh(uint8_t ioNum)
{
    if(ioNum < 24)
    {
        uint8_t chipNum = ioNum / 8;     // 确定在第几片芯片上
        uint8_t bitNum = ioNum % 8;      // 确定在该芯片的第几位
        
        HC595_Buffer[chipNum] |= (1 << bitNum);  // 将对应位置1

        HC595_UpdateAllIOs(); // Update the state of all IOss
    }
}

/**
 * @brief  设置指定IO为低电平
 * @param  ioNum: IO编号(0-23)
 * @retval 无
 */
void HC595_SetIOLow(uint8_t ioNum)
{
    if(ioNum < 24)
    {
        uint8_t chipNum = ioNum / 8;     // 确定在第几片芯片上
        uint8_t bitNum = ioNum % 8;      // 确定在该芯片的第几位
        
        HC595_Buffer[chipNum] &= ~(1 << bitNum); // 将对应位置0

        HC595_UpdateAllIOs(); // Update the state of all IOs
    }
}

/**
 * @brief  翻转指定IO的电平
 * @param  ioNum: IO编号(0-23)
 * @retval 无
 */
void HC595_ToggleIO(uint8_t ioNum)
{
    if(ioNum < 24)
    {
        uint8_t chipNum = ioNum / 8;     // 确定在第几片芯片上
        uint8_t bitNum = ioNum % 8;      // 确定在该芯片的第几位
        
        HC595_Buffer[chipNum] ^= (1 << bitNum);  // 将对应位翻转

        HC595_UpdateAllIOs(); // Update the state of all IOs
    }
}

/**
 * @brief  更新所有IO状态
 * @param  无
 * @retval 无
 */
void HC595_UpdateAllIOs(void)
{
    HC595_WriteMultiBytes(HC595_Buffer, 3);
}

/**
 * @brief  清除所有IO（全部设为低电平）
 * @param  无
 * @retval 无
 */
void HC595_ClearAllIOs(void)
{
    // 所有IO设置为0
    HC595_Buffer[0] = 0;
    HC595_Buffer[1] = 0;
    HC595_Buffer[2] = 0;
    
    // 更新输出
    HC595_UpdateAllIOs();
}

/**
 * @brief  设置所有IO（全部设为高电平）
 * @param  无
 * @retval 无
 */
void HC595_SetAllIOs(void)
{
    // 所有IO设置为1
    HC595_Buffer[0] = 0xFF;
    HC595_Buffer[1] = 0xFF;
    HC595_Buffer[2] = 0xFF;
    
    // 更新输出
    HC595_UpdateAllIOs();
}

