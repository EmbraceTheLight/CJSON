#pragma once
#define  _CRT_SECURE_NO_WARNINGS 1
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
	unsigned int size; //Array�п��ٵ�Ԫ������
	unsigned int nums; //Array�е�ǰ�Ѵ洢Ԫ������
	struct JsonValue* jvs;//Array�е�Ԫ��
}Array;

typedef struct KeyValue {
	char* key;
	struct JsonValue* value;
}KeyValue;

typedef struct Object {
	unsigned int size; //Object�п��ٵ�Ԫ�ؿ��ÿռ��С
	unsigned int nums; //Object��Ԫ������
	struct KeyValue* kvs;//Object�е�Ԫ��
}Obj;

//typedef struct JSON {
//	KeyValue* pre;
//	KeyValue* cur;
//	KeyValue* next;
//}JSON;
