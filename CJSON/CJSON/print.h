#pragma once
#include "define.h"

void main_menu();

char* handle_json_string(char* json_str);

//��ӡJSON Object
void print_obj(Obj* obj);

//��ӡJSON array
void print_arr(Array* arr);

//��ӡJSON ��ֵ��
void print_KeyValue(KeyValue* kv);