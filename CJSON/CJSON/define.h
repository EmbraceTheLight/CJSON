#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>
#include<string.h>
// ����JSONֵ��ö������
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
	unsigned int nums; //Array��Ԫ������
	struct JsonValue* jv;//Array�е�Ԫ��
}Array;

typedef struct Object {
	struct JsonValue* jv;//Array�е�Ԫ��
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
