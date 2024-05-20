#include"helper.h"
static char* true_str = "true";
static char* false_str = "false";
static char* null_str = "null";

char* find_token(char* str, char token) {
	char* ret = strchr(str, token);
	if (ret == NULL) {
		return NULL;
	}
	return ret;
}

//忽略大小写的strncmp版本
static bool strncmp_ignore_case(const char* str1,const char* str2,size_t cmp_len) {
	while (cmp_len-- && (*str1 && *str2)) {
		if (tolower(*str1) != tolower(*str2)) {
			return false;
		}
		str1++;
		str2++;
	}
	return true;
}

Type is_str_legal(char* str) {
	if (tolower(*str) == 't'){
		if (strncmp_ignore_case(str, true_str, 4) == true) {
			return TRUE;
		}
		else {
			return UNDEFINED;
		}		
	}
	else if (tolower(*str) == 'f') {
		if (strncmp_ignore_case(str, false_str, 5) == true) {
			return FALSE;
		}
		else {
			return UNDEFINED;
		}
	}
	else if (tolower(*str) == 'n') {
		if (strncmp_ignore_case(str, null_str, 4) == true) {
			return NULLTYPE;
		}
		else {
			return UNDEFINED;
		}
	}
	else
		return UNDEFINED;
}

Type is_number(char* str, char** end,double*ret) {
	char* start = str;
	*ret = strtod(start, end);
	if (start != end) {
		return NUMBER;
	}
	else {
		return UNDEFINED;
	}
}

char* next_token(char* mov) {
	mov = eat_space(mov);
	if (*mov == ',' ) {
		return eat_space(mov + 1);
	}
	return mov;
}
