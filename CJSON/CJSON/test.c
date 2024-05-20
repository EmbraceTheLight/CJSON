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

void test_is_number(void) {
	char* str = "-1230e-3abcvaunhfi nliaun g ";
	double res = 0;
	printf("original: %s\n", str);
	bool isNum = is_number(str, &str, &res);
	if (isNum != 0) {
		printf("is number: %f\n", res);
	}
	else {
		printf("is not number.");
	}
	printf("now,str: %s\n", str);
}

void test_parse_object(void) {
	char* res = handle_input();
	printf("original: %s\n", res);

	Obj* obj = parse_object(&res);
	if (obj != NULL) {
		print_obj(obj);
	}
	else {
		printf("parse object failed.");
	}
}

void test_parse_number_object(void) {
	char* res = handle_input();
}

void test_parse_bool_object(void){
	char* res = handle_input();
}

void test_parse_null_object(void) {
	char* res = handle_input();
}