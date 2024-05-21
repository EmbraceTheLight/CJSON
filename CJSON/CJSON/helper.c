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
static bool strncmp_ignore_case(const char* str1, const char* str2, size_t cmp_len) {
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
	if (tolower(*str) == 't') {
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

Type is_number(char* str, char** end, double* ret) {
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
	if (*mov == ',') {
		return eat_space(mov + 1);
	}
	return mov;
}

Type check_arr_or_obj(char* str) {
	while (*str != '\0') {
		if (*str == '{') {
			return OBJECT;
		}
		else if (*str == '[') {
			return ARRAY;
		}
		str++;
	}
}

const char* escape = "\"\\/bfnrt";

static char translate_escape(char c) {
	switch (c) {
	case '\"':
		return '\"';
	case '\\':
		return '\\';
	case '/':
		return '/';
	case 'b':
		return '\b';
	case 'f':
		return '\f';
	case 'n':
		return '\n';
	case 'r':
		return '\r';
	case 't':
		return '\t';
	}
}


// is_escape_legal 判断是否为合法的转义字符,若是，返回转义后字符，否则返回0
static char is_escape_legal(char c) {
	char* p = escape;
	while (*p != '\0') {
		if (c == *p) {
			return translate_escape(c);
		}
		p++;
	}
	return '\0';
}

size_t num_of_escape_char(char* start, char* end) {
	size_t ret = 0;
	for (char* p = start; p < end; p++) {
		if (*p == '\\') {
			ret++;
		}
	}
	return ret;
}

char* my_strcpy(char* dst,const char* start,const char* end) {
	char* cur = dst;
	while (start != end) {
		if (*start == '\\') {
			char ch = is_escape_legal(*(start + 1));
			if (ch) {
				*cur = ch;
				cur++;
				start += 2;
			}
			else {
				return NULL;
			}
		}
		else {
			*cur = *start;
			cur++;
			start++;
		}
	}
	*cur = '\0';
	return dst;
}