#pragma once
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
	null,
	ARRAY,
	OBJECT
};

struct JsonValue;
typedef struct Array {
	unsigned int nums; //Array中元素数量
	struct JsonValue* jv;//Array中的元素
}Array;

typedef struct Object {
	struct JsonValue* jv;//Array中的元素
}Obj;

typedef struct JsonValue {
	enum Valuetype type;
	union  {
		char* string;
		double number;
		bool boolean;
		void* null;
		Array* array_m;
		Obj* object;
	};
}JsonValue;

typedef struct KeyValue {
	char* key;
	JsonValue* value;
}KeyValue;

typedef struct JSON {
	KeyValue* pre;
	KeyValue* cur;
	KeyValue* next;
}JSON;
