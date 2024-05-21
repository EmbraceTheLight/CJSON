#include"tests.h"
#include"print.h"
void test_handle_input(void) {
	char* res = handle_input();
	if (res != NULL) {
		while (*res != '\0') {
			putchar(*(res++));
		}
	}
	else {
		printf("input failed.");
	}
}

void test_parse_string(void) {
	char* str = "\"52255\",\nevgvss";
	printf("original: %s\n", str);
	char* res = parse_string(&str);
	if (res != NULL) {
		printf("res: %s\n", res);
		printf("str: %s\n", str);
	}
	else {
		printf("parse string failed.");
	}
}

void test_json_string(void) {
	char* res = handle_input();
	Type t = check_arr_or_obj(res);
	if (t == OBJECT) {
		Obj* obj = parse_object(&res);
		if (obj != NULL) {
			print_obj(obj);
		}
		else {
			printf("parse object failed.");
		}
	}
	else if (t == ARRAY) {
		Array* arr = parse_array(&res);
		if (arr != NULL) {
			print_arr(arr);
		}
		else {
			printf("parse array failed.");
		}
	}
	else {
		printf("not a valid json string.");
	}
}

void test_json_find(void) {
	char*ret = handle_input();
	Obj*obj = parse_object(&ret);
	if (!obj) {
		printf("parse object failed.\n");
		return;
	}
	printf("Now the object is:\n");
	print_obj(obj);
	while (1) {
		char key[INIT_STR_SIZE];
		printf("input key: ");
		gets_s(key, INIT_STR_SIZE);
		KeyValue* kv = find_by_key(obj, key);
		if (!kv) {
			printf("key not found.\n");
			continue;
		}
		print_KeyValue(kv);
	}
}

void test_json_addKV(void) {
	char* ret = handle_input();
	Obj* obj = parse_object(&ret);
	if (!obj) {
		printf("parse object failed.\n");
		return;
	}
	printf("Now the object is:\n");
	print_obj(obj);
	while (1) {
		bool ok = create_key_value(obj);
		if (!ok) {
			printf("create key value failed.\n");
		}
		printf("Now the object is:\n");
		print_obj(obj);
	}
}

void test_json_deleteKV(void) {
	char* ret = handle_input();
	Obj* obj = parse_object(&ret);
	if (!obj) {
		printf("parse object failed.\n");
		return;
	}
	printf("Now the object is:\n");
	print_obj(obj);
	while (1) {
		char key[INIT_STR_SIZE];
		printf("input key: ");
		gets_s(key, INIT_STR_SIZE);
		bool ok  = del_by_key(obj,key);
		if (!ok) {
			printf("delete key value failed.\n");
		}
		printf("Now the object is:\n");
		print_obj(obj);
	}
}

void test_json_updateKV(void) {
	char* ret = handle_input();
	Obj* obj = parse_object(&ret);
	if (!obj) {
		printf("parse object failed.\n");
		return;
	}
	printf("Now the object is:\n");
	print_obj(obj);
	while (1) {
		char key[INIT_STR_SIZE];
		printf("input key: ");
		gets_s(key, INIT_STR_SIZE);
		bool ok =update_value(obj,key);
		if (!ok) {
			printf("update key value failed.\n");
		}
		printf("Now the object is:\n");
		print_obj(obj);
	}
}