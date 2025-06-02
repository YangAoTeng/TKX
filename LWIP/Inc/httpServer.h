#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H

#include "lwip/sockets.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"

#include <string.h>
#include <stdio.h>



// HTTP 方法枚举
typedef enum {
    HTTP_GET,
    HTTP_POST,
    HTTP_UNKNOWN
} http_method_t;

// HTTP 请求结构体
typedef struct {
    http_method_t method;
    char path[128];
    char version[16];
} http_request_t;

// 路由处理函数类型
typedef void (*route_handler_t)(int socket, http_request_t *request);

// 路由结构体
typedef struct {
    char path[64];              // 路由路径
    route_handler_t handler;    // 处理函数
} http_route_t;

#define MAX_ROUTES 16  // 最大路由数量

// 在httpServer.h中添加以下内容
#define MAX_PATH_LEN 128
#define MAX_FILE_BUF 4096

// 文件类型结构体
typedef struct {
    const char *ext;    // 文件扩展名
    const char *type;   // MIME类型
} mime_type_t;


// 回调函数类型定义
typedef void (*http_callback_t)(int socket, http_request_t *request);

// 函数声明
void http_server_init(uint16_t port);
void http_server_set_callback(http_callback_t callback);
void http_send_response(int socket, const char *status, const char *content_type, const char *body);

int8_t http_register_route(const char* path, route_handler_t handler);
route_handler_t find_route_handler(const char* path);
#endif