#pragma once
#include"utilities.h"
#include"define.h"
#include"helper.h"
#include"print.h"
#include"Unserialization.h"
// create_obj ���� object �ṹ��
Obj* create_obj();

// object2string ��objectת��Ϊjson�ַ���
char* object2string(Obj* obj);

// array2string ��arrayת��Ϊjson�ַ���
char* array2string(Array* arr);