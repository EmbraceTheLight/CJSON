#include "print.h"
#include "helper.h"
#include "Serialization.h"
void print_obj(Obj* obj) {
	char* json_str = object2string(obj);
	if (json_str != NULL) {
		printf("%s\n", json_str);
	}
	else {
		printf("parse object to json string failed.");
	}
	free(json_str);
}

void print_arr(Array* arr) {
	char* json_str = array2string(arr);
	if (json_str != NULL) {
		printf("%s\n", json_str);
	}
	else {
		printf("parse array to json string failed.");
	}
	free(json_str);
}

void print_KeyValue(KeyValue* kv) {
	printf("key is %s:\n ", kv->key);
	switch (kv->value.type) {
	case STRING:
		printf("value type is string\n");
		printf("value is %s\n", kv->value.string);
		break;
	case NUMBER:
		printf("value type is number\n");
		printf("value is %f\n", kv->value.number);
		break;
	case TRUE:
		printf("value type is boolean-->true\n");
		break;
	case FALSE:
		printf("value type is boolean-->false\n");
		break;
	case NULLTYPE:
		printf("value type is null\n");
		break;
	case ARRAY:
		printf("value type is array\n");
		print_arr(kv->value.array);
		break;
	case OBJECT:
		printf("value type is object\n");
		print_obj(kv->value.object);
	}
}