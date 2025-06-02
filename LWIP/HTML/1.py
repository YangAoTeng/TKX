#!/usr/bin/env python
# -*- coding:utf-8 -*-

# 将一个文件转换成C语言数组
import sys
html_file = open(sys.argv[1], 'rb') # 打开文件
array_name = sys.argv[1] # 设置默认数组变量名
if len(sys.argv) > 2:
    array_name = sys.argv[2]
print("static const char " + array_name + "[] = {")
while True:
    data = html_file.read(16) # 每一次读取16个字符
    if not data:
        break
    str_line = "    " #空出4个空格
    for i in data:
        str_line += "0x{0:02x}".format(i) + ", "
    print(str_line)
print("};")
html_file.close()

# 使用方法: python 1.py hello_world.html hello_world