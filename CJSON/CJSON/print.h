#pragma once
#include "define.h"

void main_menu();

char* handle_json_string(char* json_str);

//打印JSON Object
void print_obj(Obj* obj);

//打印JSON array
void print_arr(Array* arr);

//打印JSON 键值对
void print_KeyValue(KeyValue* kv);