#pragma once
#include"utilities.h"
#include"define.h"


Type t;

// handle_input ���������JSON�ַ���������ָ����ַ�����ָ��
char* handle_input(void);

// init_obj ��ʼ��Obj����
Obj* init_obj();

// init_array ��ʼ��Array����
Array* init_array();

// init_keyValue ��ʼ��KeyValue����
KeyValue* init_keyValue(char *str);

// init_jsonValue ��ʼ��JsonValue����
JsonValue* init_jsonValue();

// parse_object ����json�е�object���󣬷��ؽ���������Obj����,����json_ptrָ���Ҵ����ź�һ���ַ�
Obj* parse_object(char** json_ptr);

// parse_object ����json�е�string���󣬷��ؽ���������string�ַ���,����json_ptrָ��˫���ź�һ���ַ�
char* parse_string(char** json_ptr);

// parse_array ����json�е�array���󣬷��ؽ���������array����,����json_ptrָ���������ź�һ���ַ�
Array* parse_array(char** json_ptr);

