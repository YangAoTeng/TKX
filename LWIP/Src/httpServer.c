#include "httpServer.h"
#include "fatfs.h"

extern FATFS fs; 


#define HTTP_PORT 80
#define MAX_HEADER_SIZE 512

// 路由表
static http_route_t routes[MAX_ROUTES];
static int route_count = 0;

static http_callback_t user_callback = NULL;

// MIME类型表
static const mime_type_t mime_types[] = {
    {".html", "text/html"},
    {".css",  "text/css"},
    {".js",   "application/javascript"},
    {".jpg",  "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".png",  "image/png"},
    {".ico",  "image/x-icon"},
    {NULL,    "text/plain"}  // 默认类型
};

// 获取文件的MIME类型
static const char* get_mime_type(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (dot) {
        for (const mime_type_t *mime = mime_types; mime->ext != NULL; mime++) {
            if (strcmp(dot, mime->ext) == 0) {
                return mime->type;
            }
        }
    }
    return "text/plain";
}

// 解析 HTTP 请求
static void parse_http_request(char *buffer, http_request_t *request) {
    char *method_str = strtok(buffer, " ");
    char *path = strtok(NULL, " ");
    char *version = strtok(NULL, "\r\n");

    // 解析请求方法
    if (strcmp(method_str, "GET") == 0) {
        request->method = HTTP_GET;
    } else if (strcmp(method_str, "POST") == 0) {
        request->method = HTTP_POST;
    } else {
        request->method = HTTP_UNKNOWN;
    }

    // 复制路径和版本
    strncpy(request->path, path, sizeof(request->path) - 1);
    strncpy(request->version, version, sizeof(request->version) - 1);
}

// 发送 HTTP 响应
void http_send_response(int socket, const char *status, const char *content_type, const char *body) {
    char header[256];
    snprintf(header, sizeof(header),
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n",
             status, content_type, strlen(body));
    
    send(socket, header, strlen(header), 0);
    send(socket, body, strlen(body), 0);
}

// HTTP 服务器任务
static void http_server_task(void *argument) {
    int server_fd;
    struct sockaddr_in server_addr;
    char rx_buffer[MAX_HEADER_SIZE];

    // 创建socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        printf("HTTP server socket creation failed\r\n");
        vTaskDelete(NULL);
        return;
    }

    // 配置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(HTTP_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 绑定socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("HTTP server bind failed\r\n");
        closesocket(server_fd);
        vTaskDelete(NULL);
        return;
    }

    // 监听连接
    if (listen(server_fd, 5) < 0) {
        printf("HTTP server listen failed\r\n");
        closesocket(server_fd);
        vTaskDelete(NULL);
        return;
    }

    printf("HTTP server started on port %d\r\n", HTTP_PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        // 接受新连接
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0) {
            continue;
        }

        // 接收请求
        memset(rx_buffer, 0, sizeof(rx_buffer));
        int len = recv(client_fd, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len > 0) {
            http_request_t request;
            parse_http_request(rx_buffer, &request);

            // 查找并执行路由处理函数
            route_handler_t handler = find_route_handler(request.path);
            if(handler) {
                handler(client_fd, &request);
            } else if(user_callback) {
                // 如果没有找到路由但有默认回调，则使用默认回调
                user_callback(client_fd, &request);
            } else {
                // 默认响应
                const char *response = "<html><body><h1>404 Not Found</h1></body></html>";
                http_send_response(client_fd, "404 Not Found", "text/html", response);
            }
        }

        // 关闭连接
        closesocket(client_fd);
    }
}

// 处理静态文件的回调函数
void static_file_handler(int socket, http_request_t *request) {
    char filepath[MAX_PATH_LEN];
    char *file_buffer = NULL;
    FIL file;
    UINT br;
    FILINFO fno;
    
    // 构建完整的文件路径
    // 如果是根路径，则返回index.html
    if (strcmp(request->path, "/") == 0) {
        strcpy(filepath, "0:/www/index.html");
    } else {
        snprintf(filepath, sizeof(filepath), "0:/www%s", request->path);
    }
    
    // 将URL中的%编码转换回实际字符
    // TODO: 实现URL解码
    printf("Request path: %s, File path: %s\r\n", request->path, filepath);
    // 检查文件是否存在
    if (f_stat(filepath + 2, &fno) != FR_OK) {  // +2 跳过"0:"
        const char *response = "<html><body><h1>404 Not Found</h1></body></html>";
        http_send_response(socket, "404 Not Found", "text/html", response);
        return;
    }
    
    // 分配文件缓冲区
    file_buffer = pvPortMalloc(MAX_FILE_BUF);
    if (!file_buffer) {
        const char *response = "<html><body><h1>500 Internal Server Error</h1></body></html>";
        http_send_response(socket, "500 Internal Server Error", "text/html", response);
        return;
    }
    
    // 打开文件
    if (f_open(&file, filepath, FA_READ) != FR_OK) {
        vPortFree(file_buffer);
        const char *response = "<html><body><h1>500 Internal Server Error</h1></body></html>";
        http_send_response(socket, "500 Internal Server Error", "text/html", response);
        return;
    }
    
    // 获取文件类型
    const char *content_type = get_mime_type(filepath);
    // 发送HTTP头
    char header[256];
    snprintf(header, sizeof(header),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: %s\r\n"
                "Content-Length: %lu\r\n"
                "Connection: close\r\n"
                "\r\n",
                content_type, fno.fsize);
    send(socket, header, strlen(header), 0);
    
    // 分块读取并发送文件内容
    while (f_read(&file, file_buffer, MAX_FILE_BUF, &br) == FR_OK && br > 0) {
        send(socket, file_buffer, br, 0);
    }
    
    // 清理
    f_close(&file);
    vPortFree(file_buffer);
}

// 注册路由处理函数
int8_t http_register_route(const char* path, route_handler_t handler) {
    if(route_count >= MAX_ROUTES) {
        return -1;  // 路由表已满
    }
    
    // 检查路径是否已存在
    for(int i = 0; i < route_count; i++) {
        if(strcmp(routes[i].path, path) == 0) {
            // 更新现有路由的处理函数
            routes[i].handler = handler;
            return 0;
        }
    }
    
    // 添加新路由
    strncpy(routes[route_count].path, path, sizeof(routes[route_count].path) - 1);
    routes[route_count].handler = handler;
    route_count++;
    
    return 0;
}

// 查找路由处理函数
route_handler_t find_route_handler(const char* path) {
    for(int i = 0; i < route_count; i++) {
        if(strncmp(routes[i].path, path, strlen(routes[i].path)) == 0) {
            return routes[i].handler;
        }
    }
    return NULL;
}


// 初始化 HTTP 服务器
void http_server_init(uint16_t port) {
    osThreadAttr_t attr = {
        .name = "httpTask",
        .stack_size = 1024*3,
        .priority = (osPriority_t)osPriorityBelowNormal7
    };
    osThreadNew(http_server_task, NULL, &attr);
    http_server_set_callback(static_file_handler);
}

// 设置回调函数
void http_server_set_callback(http_callback_t callback) {
    user_callback = callback;
}