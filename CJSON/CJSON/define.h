#pragma once
#define  _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>
#include<string.h>
#include<math.h>
#include"utilities.h"

#define BUFFER_SIZE 4096 //�����ַ�����С
#define INIT_STR_SIZE 1024//��ʼ���ַ�����С
#define INIT_OBJ_NUMS 10 //��ʼ��Obj�������
#define INIT_ARRAY_NUMS 15 //��ʼ��Array�������

// ����JSONֵ��ö������
typedef enum ValueType {
	UNDEFINED,
	STRING,
	NUMBER,
	TRUE,
	FALSE,
	NULLTYPE,
	ARRAY,
	OBJECT
}Type;

struct Array;
struct Object;

typedef struct JsonValue {
	Type type;
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
	size_t size; //Array�п��ٵ�Ԫ������
	unsigned int nums; //Array�е�ǰ�Ѵ洢Ԫ������
	struct JsonValue* jvs;//Array�е�Ԫ��
}Array;

typedef struct KeyValue {
	char* key;
	struct JsonValue value;
}KeyValue;

typedef struct Object {
	size_t size; //Object�п��ٵ�Ԫ�ؿ��ÿռ��С
	unsigned int nums; //Object��Ԫ������
	struct KeyValue* kvs;//Object�е�Ԫ��
}Obj;

