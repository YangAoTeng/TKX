#include "74HC165.h"

// 定义全局缓存(3个字节，共24位IO)
static uint8_t HC165_Buffer[3] = {0};

/**
 * @brief  初始化74HC165相关引脚
 * @param  无
 * @retval 无
 */
void HC165_Init(void)
{
    // 初始状态
    HC165_PL_HIGH();  // 并行加载为高电平（不加载）
    HC165_CP_LOW();   // 时钟低电平
}

/**
 * @brief  读取一个字节数据
 * @param  无
 * @retval 读取到的数据
 */
uint8_t HC165_ReadByte(void)
{
    uint8_t i;
    uint8_t data = 0;
    
    // 先锁存当前输入状态
    HC165_PL_LOW();
    HAL_Delay(1);  // 延时确保加载稳定
    HC165_PL_HIGH();
    
    // 读取8位数据
    for (i = 0; i < 8; i++)
    {
        // 读取Q7输出
        if (HC165_Q7_READ() == GPIO_PIN_SET)
        {
            data |= (0x80 >> i);  // 设置对应位
        }
        
        // 时钟上升沿，移位下一位数据
        HC165_CP_HIGH();
        HAL_Delay(1);  // 延时确保时钟稳定
        HC165_CP_LOW();
        HAL_Delay(1);
    }
    
    return data;
}

/**
 * @brief  读取多个字节的数据
 * @param  buffer: 存储读取的数据缓存区
 * @param  len: 要读取的字节数
 * @retval 无
 */
void HC165_ReadMultiBytes(uint8_t* buffer, uint8_t len)
{
    uint8_t i;
    
    // 先锁存当前输入状态
    HC165_PL_LOW();
    HAL_Delay(1);  // 延时确保加载稳定
    HC165_PL_HIGH();
    
    // 读取多个字节数据
    for (i = 0; i < len; i++)
    {
        buffer[i] = 0;
        uint8_t j;
        
        for (j = 0; j < 8; j++)
        {
            // 读取Q7输出
            if (HC165_Q7_READ() == GPIO_PIN_SET)
            {
                buffer[i] |= (0x80 >> j);  // 设置对应位
            }
            
            // 时钟上升沿，移位下一位数据
            HC165_CP_HIGH();
            HAL_Delay(1);  // 延时确保时钟稳定
            HC165_CP_LOW();
            HAL_Delay(1);
        }
    }
    
    // 缓存读取的数据
    for (i = 0; i < len && i < sizeof(HC165_Buffer); i++)
    {
        HC165_Buffer[i] = buffer[i];
    }
}

/**
 * @brief  读取指定IO的状态
 * @param  ioNum: IO编号(0-23)
 * @retval 1:高电平，0:低电平
 */
uint8_t HC165_ReadIO(uint8_t ioNum)
{
    uint8_t buffer[3];
    
    // 读取所有IO状态
    HC165_ReadMultiBytes(buffer, 3);
    
    if (ioNum < 24)
    {
        uint8_t chipNum = ioNum / 8;     // 确定在第几片芯片上
        uint8_t bitNum = ioNum % 8;      // 确定在该芯片的第几位
        
        return (buffer[chipNum] & (1 << bitNum)) ? 1 : 0;
    }
    
    return 0;
}

/**
 * @brief  读取所有24个IO的状态
 * @param  无
 * @retval 所有IO状态（低24位有效）
 */
uint32_t HC165_ReadAllIOs(void)
{
    uint8_t buffer[3];
    uint32_t allIOs = 0;
    
    // 读取所有IO状态
    HC165_ReadMultiBytes(buffer, 3);
    
    // 合并3个字节到一个32位值
    allIOs = ((uint32_t)buffer[0]) | ((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[2] << 16);
    
    return allIOs;
}

