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

// ����object���󣺷�����һ����Ч�ַ���λ��
char* arr_next_token(char* mov);