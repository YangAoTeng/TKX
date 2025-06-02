#ifndef __ESP01_H__
#define __ESP01_H__
#include "main.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// ESP01 模块初始化函数
void ESP01_Init(void);
uint8_t ESP01_test(void);
uint8_t ESP01_ConnectWiFi(const char *ssid, const char *password);
uint8_t ESP01_CreateTCPServer(uint16_t port);
uint8_t ESP01_SendTCPData(uint8_t link_id, const char *data, uint16_t len);
#ifdef __cplusplus
}
#endif

#endif //  __ESP01_H__