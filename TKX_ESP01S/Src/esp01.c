

#include "esp01.h"
#include "at.h"
#include "main.h"
#include "usart.h"
#include "cmsis_os.h"



static at_obj_t at;          //定义AT控制器对象

static char urc_buf[256];    //URC主动上报缓冲区

void wifi_connected_handler(at_urc_ctx_t *ctx)
{
    // 处理WiFi连接成功的URC
    printf("WiFi connected: %s\r\n", ctx->buf);
}

// 添加到urc_tbl数组中
void tcp_client_connected_handler(at_urc_ctx_t *ctx)
{
    printf("TCP client connected: %s\r\n", ctx->buf);
}

void tcp_data_received_handler(at_urc_ctx_t *ctx)
{
    int link_id, len;
    char *data_start;
    
    // 解析数据接收信息，格式如：+IPD,0,10:HelloWorld
    if (sscanf(ctx->buf, "+IPD,%d,%d:", &link_id, &len) == 2) {
        // 找到数据开始的位置
        data_start = strstr(ctx->buf, ":");
        if (data_start != NULL) {
            
            data_start++; // 跳过冒号
            printf("%c", *data_start); // 打印冒号后第一个字符
            
            // 打印接收到的数据
            // printf("Received data from client %d, length %d: ", link_id, len);
            // for (int i = 0; i < len; i++) {
            //     printf("%c", data_start[i]);
            // }
        }
    }
}

urc_item_t urc_tbl[] = {     //定义URC表
	{",CONNECT", "", tcp_client_connected_handler},
    {"+IPD,", ":", tcp_data_received_handler},
};


/**
 * @brief  串口数据发送函数
 * @param  buf: 数据缓冲区
 * @param  len: 数据长度
 * @retval 已发送的数据长度
 */
unsigned int uart_write(const void *buf, unsigned int len)
{
    // 使用HAL库的串口发送函数
    if (HAL_UART_Transmit(&huart5, (uint8_t *)buf, len, 1000) == HAL_OK)
    {
        return len;
    }
    return 0;
}

/**
 * @brief  串口数据接收函数 - 从流缓冲区读取数据
 * @param  buf: 数据缓冲区
 * @param  len: 要接收的数据长度
 * @retval 实际接收到的数据长度
 */
unsigned int uart_read(void *buf, unsigned int len)
{
    size_t bytesRead = 0;
    
    // 检查流缓冲区是否已创建
    if (xUart5StreamBuffer == NULL) {
        return 0;
    }

    bytesRead = xStreamBufferReceive(
        xUart5StreamBuffer,
        buf,
        len,
        pdMS_TO_TICKS(50)  // 100ms超时
    );
    return bytesRead;  // 返回实际接收到的数据长度
}



const at_adapter_t adap = {  //AT适配器接口
	.urc_buf     = urc_buf,
	.urc_bufsize = 256,
	.utc_tbl     = urc_tbl,
	.urc_tbl_count = sizeof(urc_tbl) / sizeof(urc_item_t),	
	.debug       = NULL, 
	//适配GPRS模块的串口读写接口
	.write       = uart_write,
	.read        = uart_read
};

/**
 * @brief AT命令处理任务
 * @param argument: 任务参数
 * @retval 无
 */
static void AT_Task(void *argument)
{
    // 创建AT对象
    at_obj_init(&at, &adap);
    
    // AT命令处理循环
    while (1) {        
        at_process(&at);
    }
}

/**
 * @brief 初始化ESP01模块
 * @retval 无
 */
void ESP01_Init(void)
{
    // 定义任务句柄
    osThreadId_t atTaskHandle;
    
    // 任务属性
    const osThreadAttr_t atTask_attributes = {
        .name = "ATTask",
        .stack_size = 512 * 4,
        .priority = (osPriority_t) osPriorityNormal1,
    };
    
    // 创建AT命令处理任务
    atTaskHandle = osThreadNew(AT_Task, NULL, &atTask_attributes);
    
    if (atTaskHandle == NULL) {
        // 任务创建失败，输出调试信息
    }
}

uint8_t ESP01_test(void)
{
	//接收缓冲区
	char recvbuf[32];
    //AT应答
	at_respond_t r = {"OK", recvbuf, sizeof(recvbuf), 3000};
    if (at_do_cmd(&at, &r, "AT") != AT_RET_OK)
    {
        printf("ESP01 Test Failed\r\n");
        return 0;
    }

    // 发送ATE0
    if(at_do_cmd(&at, &r, "ATE0") != AT_RET_OK)
    {
        printf("ESP01 Test Failed\r\n");
        return 0;
    }

    printf("ESP01 Test OK\r\n");
    return 1;
}


/**
 * @brief  连接WiFi网络
 * @param  ssid: WiFi名称
 * @param  password: WiFi密码
 * @retval 0: 连接失败, 1: 连接成功
 */
uint8_t ESP01_ConnectWiFi(const char *ssid, const char *password)
{
    char recvbuf[128];
    char cmd_buf[64]; // 用于存储拼接后的AT命令
    at_respond_t r;
    
    // 设置工作模式为Station模式(客户端)
    r.matcher = "OK";  
    r.recvbuf = recvbuf;  
    r.bufsize = sizeof(recvbuf);
    r.timeout = 1000;
    
    if (at_do_cmd(&at, &r, "AT+CWMODE=1") != AT_RET_OK) {
        return 0;
    }
    
    // 连接到指定的WiFi网络
    r.matcher = "WIFI GOT IP";  
    r.recvbuf = recvbuf;  
    r.bufsize = sizeof(recvbuf);
    r.timeout = 3000;  

    // printf("Connecting to WiFi: %s, Password: %s\r\n", ssid, password);

    snprintf(cmd_buf, sizeof(cmd_buf), "AT+CWJAP=\"%s\",\"%s\"", ssid, password);
    
    if (at_do_cmd(&at, &r, cmd_buf) != AT_RET_OK) {
        printf("Failed to connect to WiFi: %s\r\n", ssid);
        return 0;
    }
    printf("Connected to WiFi: %s\r\n", ssid);

    // 获取IP地址
    r.matcher = "OK";
    r.recvbuf = recvbuf;
    r.bufsize = sizeof(recvbuf);
    r.timeout = 3000;
    if (at_do_cmd(&at, &r, "AT+CIFSR") != AT_RET_OK) {
        printf("Failed to get IP address\r\n");
        return 0;
    }
    /*
        +CIFSR:STAIP,"192.168.1.31"
        +CIFSR:STAMAC,"c4:4f:33:d4:ee:a7"
    */
    // 提取IP地址和MAC地址
    char ip_addr[16] = {0};  // IPv4地址最长为15个字符加上结束符
    
    if (sscanf(recvbuf, "+CIFSR:STAIP,\"%[^\"]\"", ip_addr) == 1) {
        printf("IP Address: %s\r\n", ip_addr);
    } else {
        printf("Failed to parse IP address\r\n");
    }
    return 1;
}

/**
 * @brief  创建TCP服务器
 * @param  port: 服务器监听端口
 * @retval 0: 创建失败, 1: 创建成功
 */
uint8_t ESP01_CreateTCPServer(uint16_t port)
{
    char recvbuf[128];
    char cmd_buf[64]; // 用于存储拼接后的AT命令
    at_respond_t r;
    
    // 1. 设置为多连接模式
    r.matcher = "OK";
    r.recvbuf = recvbuf;
    r.bufsize = sizeof(recvbuf);
    r.timeout = 3000;
    
    if (at_do_cmd(&at, &r, "AT+CIPMUX=1") != AT_RET_OK) {
        printf("Failed to set multiple connections mode\r\n");
        return 0;
    }
    
    snprintf(cmd_buf, sizeof(cmd_buf), "AT+CIPSERVER=1,%d", port);

    if (at_do_cmd(&at, &r, cmd_buf) != AT_RET_OK) {
        printf("Failed to create TCP server on port %d\r\n", port);
        return 0;
    }
    
    if (at_do_cmd(&at, &r, "AT+CIPSTO=180") != AT_RET_OK) {
        printf("Warning: Failed to set server timeout\r\n");
        // 不返回错误，因为这只是一个警告
    }
    
    printf("TCP server created successfully on port %d\r\n", port);
    return 1;
}


/**
 * @brief  发送数据到指定的TCP客户端
 * @param  link_id: 连接ID (0-4)
 * @param  data: 要发送的数据
 * @param  len: 数据长度
 * @retval 0: 发送失败, 1: 发送成功
 */
uint8_t ESP01_SendTCPData(uint8_t link_id, const char *data, uint16_t len)
{
    char recvbuf[128];
    at_respond_t r;
    char cmd_buf[64]; // 用于存储拼接后的AT命令
    
    // 1. 设置发送长度
    r.matcher = ">";  // 等待">"提示符
    r.recvbuf = recvbuf;
    r.bufsize = sizeof(recvbuf);
    r.timeout = 5000;
    
    snprintf(cmd_buf, sizeof(cmd_buf), "AT+CIPSEND=%d,%d", link_id, len);
    
    if (at_do_cmd(&at, &r, cmd_buf) != AT_RET_OK) {
        printf("Failed to start data sending\r\n");
        return 0;
    }
    
    
    // 使用uart_write直接发送数据，避免AT命令框架的格式化
    uart_write(data, len);

    printf("Data sent successfully to client %d\r\n", link_id);
    return 1;
}




