#pragma once
#include"utilities.h"
#include"define.h"

#define BUFFER_SIZE 4096 //缓冲字符串大小
#define INIT_STR_SIZE 1024//初始化字符串大小
#define INIT_OBJ_NUMS 10 //初始化Obj对象个数
#define INIT_ARRAY_NUMS 10 //初始化Array对象个数
// handle_input 处理输入的JSON字符串，返回指向该字符串的指针
char* handle_input(void);

// init_obj 初始化Obj对象
Obj* init_obj();

// init_array 初始化Array对象
Array* init_array();

// parse_object 解析json中的object对象，返回解析出来的Obj对象,并将json_ptr指向右大括号后一个字符
Obj* parse_object(char** json_ptr);
// parse_object 解析json中的string对象，返回解析出来的string字符串,并将json_ptr指向双引号后一个字符
char* parse_string(char** json_ptr);

// parse_array 解析json中的array对象，返回解析出来的array对象,并将json_ptr指向右中括号后一个字符
Array* parse_array(char** json_ptr);

