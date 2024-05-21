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