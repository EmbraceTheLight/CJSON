#pragma once
#include "define.h"

// make_value_string 将value为字符串的键值对转换为json字符串
char* make_value_string(char* key, char* value);

// make_value_number 将value为数字的键值对转换为json字符串
char* make_value_number(char* key, double value);

// make_value_bool 将value为布尔值的键值对转换为json字符串
char* make_value_bool(char* key, bool value);

// make_value_null 将value为null值的键值对转换为json字符串
char* make_value_null(char* key);

// object2string 将object转换为json字符串
char* object2string(Obj* obj);

//打印JSON Object
void print_obj(Obj* obj);
