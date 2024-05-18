#pragma once
#include"utilities.h"
#include"define.h"

#define BUFFER_SIZE 4096 //缓冲字符串大小
#define INIT_SIZE 1024//初始化字符串大小
// handle_input 处理输入的JSON字符串，返回指向该字符串的指针
char* handle_input(void);

// parse_object 解析json中的object对象，返回解析出来的Obj对象
Obj* parse_object(char** json_ptr);

// parse_array 解析json中的array对象，返回解析出来的array对象
Array* parse_array(char** json_ptr);

// parse_key_value 解析json中的key:value
JsonValue* parse_key_value(char** json_ptr, JSON* json);
