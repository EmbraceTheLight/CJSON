#pragma once
#include"Serialization.h"
#include"Unserialization.h"
#include"helper.h"
void test_handle_input(void);
void test_parse_string(void);

//����JSON �Ĳ�������
void test_json_string(void);

// ��� JSON �����л��õ����ڴ�����CRUD����
void test_json_find(void);
void test_json_addKV(void);
void test_json_deleteKV(void);
void test_json_updateKV(void);