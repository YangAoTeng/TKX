# 天空星扩展板程序
1. LWIP
    - 使用RMII和LAN8720A，已验证完成。
    - 下一步完成TCP / UDP 等例程
2. 8080 MCU LCD
    - 已完成验证
    - 下一步移植LWIP框架
3. TKX BEEP 
    - 无源蜂鸣器，已验证完成。
4. TKX_EEPROM_OLED
    - OLED/EEPROM，已验证完成。
5. TKX_ESP01S
    - F407与ESP8266通信，移植AT-Command框架，已完成验证。
    - 下一步完成ymodem串口升级。
    - 完成ESP01S升级天空星程序。
6. TKX_IIS
    - 已验证IIC通信，剩余正在编写验证。
7. TKX_LED/INPUT
    - 使用 74HC165D 作为输出接口，使用 74HC595D作为输入接口。已验证完成。
8. TKX_OLED
    - 验证IIC OLED ，已验证完成。并且移植 https://github.com/bdth-7777777/OLED_UI 框架
9. TKX_SD验证
    - 已验证完成，并且移植FATFS
10. TKX_UART
    - 验证 485 / 422 /232 芯片
    - 下一步移植Modbus主机和从机
11. TKX_CAN 
    - 已验证完成
    - 下一步进行CAN升级F407的程序
12. SPI LCD
    - 未验证
13. SPI OLED
    - 未验证
14. SPI Flash
    - 未验证
