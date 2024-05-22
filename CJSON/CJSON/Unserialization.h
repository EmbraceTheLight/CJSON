#pragma once
#include"utilities.h"
#include"define.h"
#include"helper.h"

// handle_input 处理输入的JSON字符串，返回指向该字符串的指针
char* handle_input(void);

// parse_object 解析json中的object对象，返回解析出来的Obj对象,并将json_ptr指向右大括号后一个字符
Obj* string2object(char** json_ptr);

// parse_object 解析json中的string对象，返回解析出来的string字符串,并将json_ptr指向双引号后一个字符
char* parse_string(char** json_ptr);

// parse_array 解析json中的array对象，返回解析出来的array对象,并将json_ptr指向右中括号后一个字符
Array* string2array(char** json_ptr);

// find_by_key 在Obj对象中查找key对应的KeyValue对象
Obj* find_by_key(Obj* obj, char* key, int* index);

// del_by_key 删除Obj对象中key对应的KeyValue对象
bool del_by_key(Obj* obj, char* key);

// update_value 更新Obj对象中key对应的KeyValue对象的value值
bool update_value(Obj* obj,char*key);

// add_key_value 添加KeyValue对象到Obj对象中
bool create_key_value(Obj* obj);