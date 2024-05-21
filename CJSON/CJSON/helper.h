#pragma once
#include "define.h"
enum ValueType type;



//����token���ַ����е�λ��
char* find_token(char* str, char token);

// �ж�jsonֵ����null����boolean�Ƿ�Ϊ�Ϸ���
Type is_str_legal(char* str);

// �ж��Ƿ�Ϊnumber����,����ǣ���������doubleֵ����
Type is_number(char* str, char** end, double* ret);

// ����object���󣺷�����һ����Ч�ַ���λ��
char*next_token(char* mov);

// check_arr_or_obj ��鴫����ַ����Ƿ�Ϊjson������߶���
Type check_arr_or_obj(char* str);

// is_escape_legal �ж��Ƿ�Ϊ�Ϸ���ת���ַ�,���ǣ����ظ��ַ������򷵻�0
//char is_escape_legal(char c);

// num_of_escape_char �����ַ�����ת���ַ��ĸ���
size_t num_of_escape_char(char* str, char* end);

// my_strcpy �ַ������ƣ����ܴ���ת���ַ�
char* my_strcpy(char* dst,const char* start,const char* end);