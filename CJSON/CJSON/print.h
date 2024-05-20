#pragma once
#include "define.h"

// make_value_string ��valueΪ�ַ����ļ�ֵ��ת��Ϊjson�ַ���
char* make_value_string(char* key, char* value,bool is_obj_arr);

// make_value_number ��valueΪ���ֵļ�ֵ��ת��Ϊjson�ַ���
char* make_value_number(char* key, double value, bool is_obj_arr);

// make_value_bool ��valueΪ����ֵ�ļ�ֵ��ת��Ϊjson�ַ���
char* make_value_bool(char* key, bool value, bool is_obj_arr);

// make_value_null ��valueΪnullֵ�ļ�ֵ��ת��Ϊjson�ַ���
char* make_value_null(char* key, bool is_obj_arr);

// object2string ��objectת��Ϊjson�ַ���
char* object2string(Obj* obj);

//��ӡJSON Object
void print_obj(Obj* obj);

// array2string ��arrayת��Ϊjson�ַ���
char* array2string(Array* arr);
