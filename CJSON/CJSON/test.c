#include"tests.h"
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
	char* str = "\"52255\",evgvss";
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