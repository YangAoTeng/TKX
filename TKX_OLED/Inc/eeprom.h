#ifndef EEPROM_H
#define EEPROM_H
#include "main.h"

// 24C02 设备地址 (1 0 1 0 A2 A1 A0)
#define EEPROM_ADDR 0xA0  // 基础地址，最后一位为R/W位

// 24C02容量为2K bits = 256 bytes
#define EEPROM_MAX_SIZE 256

// 超时定义
#define EEPROM_TIMEOUT 1000

// 函数声明
HAL_StatusTypeDef EEPROM_WriteByte(uint16_t address, uint8_t data);
HAL_StatusTypeDef EEPROM_ReadByte(uint16_t address, uint8_t *data);
HAL_StatusTypeDef EEPROM_WriteBuffer(uint16_t address, uint8_t *buffer, uint16_t size);
HAL_StatusTypeDef EEPROM_ReadBuffer(uint16_t address, uint8_t *buffer, uint16_t size);
void EEPROM_WaitForWriteEnd(void);

// 测试函数
uint8_t EEPROM_Test(void);

#endif // !EEPROM_H
