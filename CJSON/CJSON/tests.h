#pragma once
#include"Serialization.h"
#include"Unserialization.h"
#include"helper.h"
void test_handle_input(void);
void test_parse_string(void);

//解析JSON 的测试用例
void test_json_string(void);

// 针对 JSON 反序列化得到的内存对象的CRUD测试
void test_json_find(void);
void test_json_addKV(void);
void test_json_deleteKV(void);
void test_json_updateKV(void);