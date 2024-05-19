#pragma once
#define  _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>
#include<string.h>

// 定义JSON值的枚举类型
enum ValueType {
	STRING,
	NUMBER,
	BOOLEAN,
	NULLTYPE,
	ARRAY,
	OBJECT
};

struct Array;
struct Object;

typedef struct JsonValue {
	enum ValueType type;
	union  {
		char* string;
		double number;
		bool boolean;
		void* null;
		struct Array* array;
		struct Obj* object;
	};
}JsonValue;

typedef struct Array {
	unsigned int size; //Array中开辟的元素数量
	unsigned int nums; //Array中当前已存储元素数量
	struct JsonValue* jvs;//Array中的元素
}Array;

typedef struct KeyValue {
	char* key;
	struct JsonValue* value;
}KeyValue;

typedef struct Object {
	unsigned int size; //Object中开辟的元素可用空间大小
	unsigned int nums; //Object中元素数量
	struct KeyValue* kvs;//Object中的元素
}Obj;

//typedef struct JSON {
//	KeyValue* pre;
//	KeyValue* cur;
//	KeyValue* next;
//}JSON;
