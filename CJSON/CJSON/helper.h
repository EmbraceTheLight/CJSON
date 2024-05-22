#pragma once
#include "define.h"

// init_obj 初始化Obj对象
Obj* init_obj();

// init_array 初始化Array对象
Array* init_array();

//查找token在字符串中的位置,返回字符串的起始位置。若token不存在，返回NULL
char* find_token(char* str, char token);

// 判断json值类型null或者boolean是否为合法的
Type is_str_legal(char* str);

// next_token 返回下一个有效字符的位置
char*next_token(char* mov);

// check_arr_or_obj 检查传入的字符串是否为json数组或者对象
Type check_arr_or_obj(char* str);

// make_value_string 将value为字符串的键值对转换为json字符串
char* make_value_string(char* key, char* value, bool is_obj_arr);

// make_value_number 将value为数字的键值对转换为json字符串
char* make_value_number(char* key, double value, bool is_obj_arr);

// make_value_bool 将value为布尔值的键值对转换为json字符串
char* make_value_bool(char* key, bool value, bool is_obj_arr);

// make_value_null 将value为null值的键值对转换为json字符串
char* make_value_null(char* key, bool is_obj_arr);

// num_of_escape_char 计算字符串中转义字符的个数
size_t num_of_escape_char(const char* str, const char* end);

// my_strcpy 字符串复制，并能处理转义字符
char* my_strcpy(char* dst,const char* start,const char* end);

// get_substr 获取子从start至(end-1)的子字符串
char* get_substr(char* start, char* end);

// cleanup 释放内存
void cleanup(void*json,Type type);
