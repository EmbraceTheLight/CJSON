#pragma once
#include"utilities.h"
#include"define.h"

// parse_object ����json�е�object����
Obj* parse_object(char** json_ptr);
// parse_array ����json�е�array����
Array* parse_array(char** json_ptr);
// parse_key_value ����json�е�key:value
JsonValue* parse_key_value(char** json_ptr, JSON* json);