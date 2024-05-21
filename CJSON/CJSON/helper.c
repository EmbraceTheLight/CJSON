#include"helper.h"
static char* true_str = "true";
static char* false_str = "false";
static char* null_str = "null";

const char* escape = "\"\\/bfnrt";

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

char* make_value_string(char* key, char* value, bool is_obj_arr) {
	size_t vs_len = strlen(value) + strlen(key) + 6;//两个字符串的长度加上两对双引号的长度和冒号的长度，以及一个'\0'
	char* vs = (char*)calloc(vs_len, sizeof(char));
	if (vs == NULL) {
		printf("[print::make_value_string] make string failed!\n");
		return NULL;
	}
	is_obj_arr == true ? sprintf(vs, "\"%s\":\"%s\"", key, value) : sprintf(vs, "\"%s\"", value);
	return vs;
}

char* make_value_number(char* key, double value, bool is_obj_arr) {
	char* vs = (char*)malloc((strlen(key) + 64) * sizeof(char));
	if (vs == NULL) {
		printf("[print::make_value_number] make string failed!\n");
		return NULL;
	}
	if (isInteger(value)) {
		is_obj_arr == true ? sprintf(vs, "\"%s\":%lld", key, (long long)value) : sprintf(vs, "%d", (int)value);
	}
	else {
		is_obj_arr == true ? sprintf(vs, "\"%s\":%Lf", key, value) : sprintf(vs, "%Lf", value);
	}
	return vs;
}

char* make_value_bool(char* key, bool value, bool is_obj_arr) {
	size_t vs_len = 0;
	if (value == true)
		vs_len = is_obj_arr == true ? strlen(key) + 8 : 5;
	else
		vs_len = is_obj_arr == true ? strlen(key) + 9 : 6;

	char* vs = (char*)calloc(vs_len, sizeof(char));
	if (vs == NULL) {
		printf("[print::make_value_bool] make string failed!\n");
		return NULL;
	}
	vs_len = is_obj_arr == true ? sprintf(vs, "\"%s\":%s", key, value ? "true" : "false") : sprintf(vs, "%s", value ? "true" : "false");
	return vs;
}

char* make_value_null(char* key, bool is_obj_arr) {
	size_t vs_len = is_obj_arr == true ? strlen(key) + 8 : 5;
	char* vs = (char*)calloc(vs_len, sizeof(char));
	if (vs == NULL) {
		printf("[print::make_value_null] make string failed!\n");
		return NULL;
	}
	is_obj_arr == true ? sprintf(vs, "\"%s\":%s", key, "null") : sprintf(vs, "%s", "null");
	return vs;
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
	return UNDEFINED;
}

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
	return '\0';
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

size_t num_of_escape_char(const char* start,const char* end) {
	size_t ret = 0;
	for (const char* p = start; p < end; p++) {
		if (*p == '\\') {
			ret++;
		}
	}
	return ret;
}

char* my_strcpy(char* dst, const char* start, const char* end) {
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

static void free_object(Obj* obj);
static void free_array(Array* arr) {
	for (unsigned int i = 0; i < arr->nums; i++) {
		JsonValue val = arr->jvs[i];
		if (val.type == ARRAY) {
			free_array(val.array);
		}
		else if (val.type == OBJECT) {
			free_object(val.object);
		}
		else if (val.type == STRING) {
			free(val.string);
		}
	}
	free(arr->jvs);
	free(arr);
	printf("[helper::free_array] free array success!\n");
}

static void free_object(Obj* obj) {
	for (unsigned int i = 0; i < obj->nums; i++) {
		KeyValue kv = obj->kvs[i];
		if (kv.value.type == ARRAY) {
			free_array(kv.value.array);
		}
		else if (kv.value.type == OBJECT) {
			free_object(kv.value.object);
		}
		else if (kv.value.type == STRING) {
			free(kv.value.string);
		}
		free(kv.key);
	}
	free(obj->kvs);
	free(obj);
	printf("[helper::free_object] free object success!\n");
}

void cleanup(void* json, Type type) {
	if (type == ARRAY) {
		free_array((Array*)json);
	}
	else if (type == OBJECT) {
		free_object((Obj*)json);
	}
	else if (type == STRING) {
		free((char*)json);
	}
}