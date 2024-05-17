#pragma once
#include"utilities.h"
#include"define.h"

// parse_object 解析json中的object对象
Obj* parse_object(char** json_ptr);
// parse_array 解析json中的array对象
Array* parse_array(char** json_ptr);
// parse_key_value 解析json中的key:value
JsonValue* parse_key_value(char** json_ptr, JSON* json);