#pragma once
#include"utilities.h"
#include"define.h"


Type t;

// handle_input 处理输入的JSON字符串，返回指向该字符串的指针
char* handle_input(void);

// init_obj 初始化Obj对象
Obj* init_obj();

// init_array 初始化Array对象
Array* init_array();

// init_keyValue 初始化KeyValue对象
KeyValue* init_keyValue(char *str);

// init_jsonValue 初始化JsonValue对象
JsonValue* init_jsonValue();

// parse_object 解析json中的object对象，返回解析出来的Obj对象,并将json_ptr指向右大括号后一个字符
Obj* parse_object(char** json_ptr);

// parse_object 解析json中的string对象，返回解析出来的string字符串,并将json_ptr指向双引号后一个字符
char* parse_string(char** json_ptr);

// parse_array 解析json中的array对象，返回解析出来的array对象,并将json_ptr指向右中括号后一个字符
Array* parse_array(char** json_ptr);

