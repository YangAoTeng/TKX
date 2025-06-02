// filepath: c:\Users\q8979\Desktop\TKX\TKX_EEPROM\Src\eeprom.c

#include "eeprom.h"
#include "i2c.h"

// 外部I2C句柄声明，假设I2C1被用于EEPROM通信
extern I2C_HandleTypeDef hi2c1;

/**
 * @brief  向EEPROM写入一个字节
 * @param  address: EEPROM内部地址
 * @param  data: 要写入的数据
 * @retval HAL状态
 */
HAL_StatusTypeDef EEPROM_WriteByte(uint16_t address, uint8_t data)
{
    HAL_StatusTypeDef status;
    
    // 检查地址范围
    if (address >= EEPROM_MAX_SIZE)
        return HAL_ERROR;
    
    // 发送数据到EEPROM
    status = HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, address, I2C_MEMADD_SIZE_8BIT, &data, 1, EEPROM_TIMEOUT);
    
    // 等待写入完成
    EEPROM_WaitForWriteEnd();
    
    return status;
}

/**
 * @brief  从EEPROM读取一个字节
 * @param  address: EEPROM内部地址
 * @param  data: 读取数据的存储地址
 * @retval HAL状态
 */
HAL_StatusTypeDef EEPROM_ReadByte(uint16_t address, uint8_t *data)
{
    // 检查地址范围
    if (address >= EEPROM_MAX_SIZE)
        return HAL_ERROR;
    
    // 从EEPROM读取数据
    return HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, address, I2C_MEMADD_SIZE_8BIT, data, 1, EEPROM_TIMEOUT);
}

/**
 * @brief  向EEPROM写入多个字节
 * @param  address: EEPROM内部起始地址
 * @param  buffer: 要写入的数据缓冲区
 * @param  size: 要写入的数据大小（字节）
 * @retval HAL状态
 */
HAL_StatusTypeDef EEPROM_WriteBuffer(uint16_t address, uint8_t *buffer, uint16_t size)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint16_t remainingBytes;
    uint16_t pageSize = 8; // 24C02的页大小为8字节
    uint16_t currentAddress;
    uint16_t currentSize;
    uint8_t *currentBuffer;
    
    // 检查参数
    if (buffer == NULL || (address + size) > EEPROM_MAX_SIZE)
        return HAL_ERROR;
    
    // 处理第一页（可能不是完整页）
    currentAddress = address;
    currentBuffer = buffer;
    remainingBytes = size;
    
    // 计算第一页剩余空间
    currentSize = pageSize - (address % pageSize);
    if (currentSize > remainingBytes)
        currentSize = remainingBytes;
    
    // 写入第一页
    if (currentSize > 0)
    {
        status = HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, currentAddress, I2C_MEMADD_SIZE_8BIT, currentBuffer, currentSize, EEPROM_TIMEOUT);
        if (status != HAL_OK)
            return status;
        
        EEPROM_WaitForWriteEnd();
        
        remainingBytes -= currentSize;
        currentAddress += currentSize;
        currentBuffer += currentSize;
    }
    
    // 写入完整页
    while (remainingBytes > 0)
    {
        currentSize = (remainingBytes >= pageSize) ? pageSize : remainingBytes;
        
        status = HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, currentAddress, I2C_MEMADD_SIZE_8BIT, currentBuffer, currentSize, EEPROM_TIMEOUT);
        if (status != HAL_OK)
            return status;
        
        EEPROM_WaitForWriteEnd();
        
        remainingBytes -= currentSize;
        currentAddress += currentSize;
        currentBuffer += currentSize;
    }
    
    return HAL_OK;
}

/**
 * @brief  从EEPROM读取多个字节
 * @param  address: EEPROM内部起始地址
 * @param  buffer: 读取数据的存储缓冲区
 * @param  size: 要读取的数据大小（字节）
 * @retval HAL状态
 */
HAL_StatusTypeDef EEPROM_ReadBuffer(uint16_t address, uint8_t *buffer, uint16_t size)
{
    // 检查参数
    if (buffer == NULL || (address + size) > EEPROM_MAX_SIZE)
        return HAL_ERROR;
    
    // 从EEPROM读取数据
    return HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, address, I2C_MEMADD_SIZE_8BIT, buffer, size, EEPROM_TIMEOUT);
}

/**
 * @brief  等待EEPROM写入完成
 */
void EEPROM_WaitForWriteEnd(void)
{
    uint8_t status = 0;
    uint32_t timeout = HAL_GetTick();
    
    // 重复发送START + DEVICE_WRITE_ADDRESS直到收到ACK或者超时
    do
    {
        status = HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR, 1, EEPROM_TIMEOUT);
        
        if (HAL_GetTick() - timeout > EEPROM_TIMEOUT)
            break;
    } while (status != HAL_OK);
}

/**
 * @brief  EEPROM读写测试函数
 * @retval 0: 测试失败, 1: 测试成功
 */
uint8_t EEPROM_Test(void)
{
    uint8_t writeData[10] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0xAB, 0xCD};
    uint8_t readData[10] = {0};
    uint16_t testAddress = 0;
    HAL_StatusTypeDef status;
    uint8_t singleByte = 0xAA;
    uint8_t readByte = 0;
    
    // 测试单字节写入和读取
    status = EEPROM_WriteByte(testAddress, singleByte);
    if (status != HAL_OK)
    {
        printf("EEPROM WriteByte Error\n");
        return 0; // 单字节写入失败
    }
    
    status = EEPROM_ReadByte(testAddress, &readByte);
    if (status != HAL_OK || readByte != singleByte)
    {
        printf("EEPROM ReadByte Error\n");
        return 0; // 单字节读取失败或数据不匹配
    }
    
    // 测试多字节写入和读取
    testAddress = 16; // 使用不同的地址进行测试
    
    status = EEPROM_WriteBuffer(testAddress, writeData, sizeof(writeData));
    if (status != HAL_OK)
    {
        printf("EEPROM WriteBuffer Error\n");
        return 0; // 多字节写入失败
    }
    
    status = EEPROM_ReadBuffer(testAddress, readData, sizeof(readData));
    if (status != HAL_OK)
    {
        printf("EEPROM ReadBuffer Error\n");
        return 0; // 多字节读取失败
    }
    
    // 验证读取的数据是否与写入的数据一致
    for (uint8_t i = 0; i < sizeof(writeData); i++)
    {
        if (writeData[i] != readData[i])
        {
            printf("Data mismatch at index %d: %02X != %02X\n", i, writeData[i], readData[i]);
            return 0; // 数据不匹配
        }
    }
    
    // 测试跨页写入
    testAddress = 4; // 从页边界前开始写入
    
    status = EEPROM_WriteBuffer(testAddress, writeData, sizeof(writeData));
    if (status != HAL_OK)
    {
        printf("EEPROM WriteBuffer across page Error\n");
        return 0; // 跨页写入失败
    }
    
    // 清空读取缓冲区
    for (uint8_t i = 0; i < sizeof(readData); i++)
    {
        readData[i] = 0;
    }
    
    status = EEPROM_ReadBuffer(testAddress, readData, sizeof(readData));
    if (status != HAL_OK)
    {
        printf("EEPROM ReadBuffer across page Error\n");
        return 0; // 读取失败
    }
    
    // 验证跨页读取的数据
    for (uint8_t i = 0; i < sizeof(writeData); i++)
    {
        if (writeData[i] != readData[i])
        {
            printf("Cross-page data mismatch at index %d: %02X != %02X\n", i, writeData[i], readData[i]);
            return 0; // 数据不匹配
        }
    }
    
    return 1; // 所有测试都通过
}