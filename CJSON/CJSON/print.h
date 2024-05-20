#pragma once
#include "define.h"

// make_value_string 将value为字符串的键值对转换为json字符串
char* make_value_string(char* key, char* value,bool is_obj_arr);

// make_value_number 将value为数字的键值对转换为json字符串
char* make_value_number(char* key, double value, bool is_obj_arr);

// make_value_bool 将value为布尔值的键值对转换为json字符串
char* make_value_bool(char* key, bool value, bool is_obj_arr);

// make_value_null 将value为null值的键值对转换为json字符串
char* make_value_null(char* key, bool is_obj_arr);

// object2string 将object转换为json字符串
char* object2string(Obj* obj);

//打印JSON Object
void print_obj(Obj* obj);

// array2string 将array转换为json字符串
char* array2string(Array* arr);
