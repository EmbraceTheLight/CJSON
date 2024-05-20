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

// 对于object对象：返回下一个有效字符的位置
char* arr_next_token(char* mov);