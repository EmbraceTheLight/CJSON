#include"tests.h"
#include"print.h"
void test_handle_input(void) {
	char* res = handle_input();
	if (res) {
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
	if (res) {
		printf("res: %s\n", res);
		printf("str: %s\n", str);
	}
	else {
		printf("parse string failed.");
	}
}

void test_parse_json_string(void) {
	char* res = handle_input();
	Type t = check_arr_or_obj(res);
	if (t == OBJECT) {
		Obj* obj = string2object(&res);
		if (obj) {
			print_obj(obj);
		}
		else {
			printf("parse object failed.");
		}
	}
	else if (t == ARRAY) {
		Array* arr = string2array(&res);
		if (arr) {
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
	if (!ret) {
		fprintf(stderr,"input failed.\n");
		return;
	}
	Obj*obj = string2object(&ret);
	if (!obj) {
		printf("parse object failed.\n");
		return;
	}
	printf("Now the object is:\n");
	print_obj(obj);
	for (int i = 0; i < 5; i++) {
		char key[INIT_STR_SIZE];
		printf("input key: ");
		gets_s(key, INIT_STR_SIZE);
		int idx;
		Obj* o = find_by_key(obj, key,&idx);
		if (!o) {
			printf("key not found.\n");
			continue;
		}
		print_KeyValue(&o->kvs[i]);
	}
	cleanup(obj, OBJECT);
}

void test_json_addKV(void) {
	char* ret = handle_input();
	if (!ret) {
		fprintf(stderr, "input failed.\n");
		return;
	}
	Obj* obj = string2object(&ret);
	if (!obj) {
		printf("parse object failed.\n");
		return;
	}
	printf("Now the object is:\n");
	print_obj(obj);
	for (int i = 0; i < 5; i++) {
		bool ok = create_key_value(obj);
		if (!ok) {
			printf("create key value failed.\n");
		}
		printf("Now the object is:\n");
		print_obj(obj);
	}
	cleanup(obj, OBJECT);
}

void test_json_deleteKV(void) {
	char* ret = handle_input();
	if (!ret) {
		fprintf(stderr, "input failed.\n");
		return;
	}
	Obj* obj = string2object(&ret);
	if (!obj) {
		printf("parse object failed.\n");
		return;
	}
	printf("Now the object is:\n");
	print_obj(obj);
	for(int i = 0; i < 5; i++) {
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
	cleanup(obj, OBJECT);
}

void test_json_updateKV(void) {
	char* ret = handle_input();
	if (!ret) {
		fprintf(stderr, "input failed.\n");
		return;
	}
	Obj* obj = string2object(&ret);
	if (!obj) {
		printf("parse object failed.\n");
		return;
	}
	printf("Now the object is:\n");
	print_obj(obj);
	for (int i = 0; i < 5; i++) {
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
	cleanup(obj, OBJECT);
}