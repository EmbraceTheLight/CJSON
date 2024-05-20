#pragma once
#define  _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>
#include<string.h>
#include<math.h>
#include"utilities.h"

#define BUFFER_SIZE 4096 //缓冲字符串大小
#define INIT_STR_SIZE 1024//初始化字符串大小
#define INIT_OBJ_NUMS 10 //初始化Obj对象个数
#define INIT_ARRAY_NUMS 15 //初始化Array对象个数

// 定义JSON值的枚举类型
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
	size_t size; //Array中开辟的元素数量
	unsigned int nums; //Array中当前已存储元素数量
	struct JsonValue* jvs;//Array中的元素
}Array;

typedef struct KeyValue {
	char* key;
	struct JsonValue value;
}KeyValue;

typedef struct Object {
	size_t size; //Object中开辟的元素可用空间大小
	unsigned int nums; //Object中元素数量
	struct KeyValue* kvs;//Object中的元素
}Obj;

