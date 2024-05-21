#pragma once
#include "define.h"

//����token���ַ����е�λ��
char* find_token(char* str, char token);

// �ж�jsonֵ����null����boolean�Ƿ�Ϊ�Ϸ���
Type is_str_legal(char* str);

// next_token ������һ����Ч�ַ���λ��
char*next_token(char* mov);

// check_arr_or_obj ��鴫����ַ����Ƿ�Ϊjson������߶���
Type check_arr_or_obj(char* str);

// make_value_string ��valueΪ�ַ����ļ�ֵ��ת��Ϊjson�ַ���
char* make_value_string(char* key, char* value, bool is_obj_arr);

// make_value_number ��valueΪ���ֵļ�ֵ��ת��Ϊjson�ַ���
char* make_value_number(char* key, double value, bool is_obj_arr);

// make_value_bool ��valueΪ����ֵ�ļ�ֵ��ת��Ϊjson�ַ���
char* make_value_bool(char* key, bool value, bool is_obj_arr);

// make_value_null ��valueΪnullֵ�ļ�ֵ��ת��Ϊjson�ַ���
char* make_value_null(char* key, bool is_obj_arr);

// num_of_escape_char �����ַ�����ת���ַ��ĸ���
size_t num_of_escape_char(const char* str, const char* end);

// my_strcpy �ַ������ƣ����ܴ���ת���ַ�
char* my_strcpy(char* dst,const char* start,const char* end);

// cleanup �ͷ��ڴ�
void cleanup(void*json,Type type);
