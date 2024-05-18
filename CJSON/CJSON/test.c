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