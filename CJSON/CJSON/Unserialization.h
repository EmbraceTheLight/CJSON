#pragma once
#include"utilities.h"
#include"define.h"
#include"helper.h"

// handle_input ���������JSON�ַ���������ָ����ַ�����ָ��
char* handle_input(void);

// parse_object ����json�е�object���󣬷��ؽ���������Obj����,����json_ptrָ���Ҵ����ź�һ���ַ�
Obj* string2object(char** json_ptr);

// parse_object ����json�е�string���󣬷��ؽ���������string�ַ���,����json_ptrָ��˫���ź�һ���ַ�
char* parse_string(char** json_ptr);

// parse_array ����json�е�array���󣬷��ؽ���������array����,����json_ptrָ���������ź�һ���ַ�
Array* string2array(char** json_ptr);

// find_by_key ��Obj�����в���key��Ӧ��KeyValue����
Obj* find_by_key(Obj* obj, char* key, int* index);

// del_by_key ɾ��Obj������key��Ӧ��KeyValue����
bool del_by_key(Obj* obj, char* key);

// update_value ����Obj������key��Ӧ��KeyValue�����valueֵ
bool update_value(Obj* obj,char*key);

// add_key_value ���KeyValue����Obj������
bool create_key_value(Obj* obj);