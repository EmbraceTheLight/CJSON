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

