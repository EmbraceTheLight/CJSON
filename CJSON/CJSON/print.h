#pragma once
#include "define.h"

// make_value_string ��valueΪ�ַ����ļ�ֵ��ת��Ϊjson�ַ���
char* make_value_string(char* key, char* value);

// make_value_number ��valueΪ���ֵļ�ֵ��ת��Ϊjson�ַ���
char* make_value_number(char* key, double value);

// make_value_bool ��valueΪ����ֵ�ļ�ֵ��ת��Ϊjson�ַ���
char* make_value_bool(char* key, bool value);

// make_value_null ��valueΪnullֵ�ļ�ֵ��ת��Ϊjson�ַ���
char* make_value_null(char* key);

// object2string ��objectת��Ϊjson�ַ���
char* object2string(Obj* obj);

//��ӡJSON Object
void print_obj(Obj* obj);
