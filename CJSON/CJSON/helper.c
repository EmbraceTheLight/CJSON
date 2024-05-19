#include"helper.h"
char* find_token(char* str, char token) {
	char* ret = strchr(str, token);
	if (ret == NULL) {
		return NULL;
	}
	return ret;
}