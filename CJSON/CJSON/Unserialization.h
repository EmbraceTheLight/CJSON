#pragma once
#include"utilities.h"
#include"define.h"

#define BUFFER_SIZE 4096 //�����ַ�����С
#define INIT_SIZE 1024//��ʼ���ַ�����С
// handle_input ���������JSON�ַ���������ָ����ַ�����ָ��
char* handle_input(void);

// parse_object ����json�е�object���󣬷��ؽ���������Obj����
Obj* parse_object(char** json_ptr);

// parse_array ����json�е�array���󣬷��ؽ���������array����
Array* parse_array(char** json_ptr);

// parse_key_value ����json�е�key:value
JsonValue* parse_key_value(char** json_ptr, JSON* json);
