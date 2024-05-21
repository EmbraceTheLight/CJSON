#pragma once
#include "define.h"
enum ValueType type;



//查找token在字符串中的位置
char* find_token(char* str, char token);

// 判断json值类型null或者boolean是否为合法的
Type is_str_legal(char* str);

// 判断是否为number类型,如果是，将解析的double值返回
Type is_number(char* str, char** end, double* ret);

// 对于object对象：返回下一个有效字符的位置
char*next_token(char* mov);

// check_arr_or_obj 检查传入的字符串是否为json数组或者对象
Type check_arr_or_obj(char* str);

// is_escape_legal 判断是否为合法的转义字符,若是，返回该字符，否则返回0
//char is_escape_legal(char c);

// num_of_escape_char 计算字符串中转义字符的个数
size_t num_of_escape_char(char* str, char* end);

// my_strcpy 字符串复制，并能处理转义字符
char* my_strcpy(char* dst,const char* start,const char* end);