#pragma once
#include"utilities.h"
#include"define.h"

// handle_input ���������JSON�ַ���������ָ����ַ�����ָ��
char* handle_input(void);

// init_obj ��ʼ��Obj����
Obj* init_obj();

// init_array ��ʼ��Array����
Array* init_array();

// parse_object ����json�е�object���󣬷��ؽ���������Obj����,����json_ptrָ���Ҵ����ź�һ���ַ�
Obj* parse_object(char** json_ptr);

// parse_object ����json�е�string���󣬷��ؽ���������string�ַ���,����json_ptrָ��˫���ź�һ���ַ�
char* parse_string(char** json_ptr);

// parse_array ����json�е�array���󣬷��ؽ���������array����,����json_ptrָ���������ź�һ���ַ�
Array* parse_array(char** json_ptr);

// find_by_key ��Obj�����в���key��Ӧ��KeyValue����
KeyValue* find_by_key(Obj* obj, char* key);

// del_by_key ɾ��Obj������key��Ӧ��KeyValue����
bool del_by_key(Obj* obj, char* key);

// update_value ����Obj������key��Ӧ��KeyValue�����valueֵ
bool update_value(Obj* obj,char*key);

// add_key_value ���KeyValue����Obj������
bool create_key_value(Obj* obj);