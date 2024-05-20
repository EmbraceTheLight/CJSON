#include "print.h"

// make_value_string 将value为字符串的键值对转换为json字符串
char* make_value_string(char* key, char* value, bool is_obj_arr) {
	int vs_len = strlen(value) + strlen(key) + 6;//两个字符串的长度加上两对双引号的长度和冒号的长度，以及一个'\0'
	char* vs = (char*)calloc(vs_len, sizeof(char));
	if (vs == NULL) {
		printf("[print::make_value_string] make string failed!\n");
		return NULL;
	}
	is_obj_arr == true ? sprintf(vs, "\"%s\":%s", key, value) : sprintf(vs, "\"%s\"", value);
	return vs;
}

// make_value_number 将value为数字的键值对转换为json字符串
char* make_value_number(char* key, double value, bool is_obj_arr) {
	char* vs = (char*)malloc((strlen(key) + 64) * sizeof(char));
	if (vs == NULL) {
		printf("[print::make_value_number] make string failed!\n");
		return NULL;
	}
	if (isInteger(value)) {
		is_obj_arr == true ? sprintf(vs, "\"%s\":%d", key, (int)value) : sprintf(vs, "%d", (int)value);
	}
	else {
		is_obj_arr == true ? sprintf(vs, "\"%s\":%Lf", key, value): sprintf(vs, "%Lf", value);
	}
	return vs;
}

// make_value_bool 将value为布尔值的键值对转换为json字符串
char* make_value_bool(char* key, bool value, bool is_obj_arr) {
	int vs_len = 0;
	if (value == true)
		vs_len = is_obj_arr == true ? strlen(key) + 8 : 5;
	else
		vs_len = is_obj_arr == true ? strlen(key) + 9:6;

	char* vs = (char*)calloc(vs_len, sizeof(char));
	if (vs == NULL) {
		printf("[print::make_value_bool] make string failed!\n");
		return NULL;
	}
	vs_len = is_obj_arr == true ? sprintf(vs, "\"%s\":%s", key, value ? "true" : "false"): sprintf(vs, "%s", value ? "true" : "false");
	return vs;
}

// make_value_null 将value为null值的键值对转换为json字符串
char* make_value_null(char* key,bool is_obj_arr) {
	int vs_len = is_obj_arr==true?strlen(key) + 8:5;
	char* vs = (char*)calloc(vs_len, sizeof(char));
	if (vs == NULL) {
		printf("[print::make_value_null] make string failed!\n");
		return NULL;
	}
	is_obj_arr == true ? sprintf(vs, "\"%s\":%s", key, "null") : sprintf(vs, "%s", "null");
	return vs;
}

char* object2string(Obj* obj) {
	char* json = (char*)calloc(INIT_STR_SIZE, sizeof(char));
	if (json == NULL) {
		printf("[print::make_object_string] object to json string failed!\n");
		return NULL;
	}

	size_t jsize = INIT_STR_SIZE;
	size_t jlen = 0;
	json[jlen++] = '{';
	json[jlen++] = '\n';
	json[jlen++] = '\t';

	for (int numsOfElements = 0; numsOfElements < obj->nums; numsOfElements++) {
		KeyValue kv = obj->kvs[numsOfElements];
		switch (kv.value.type) {
		case STRING:
		{
			char* line = make_value_string(kv.key, kv.value.string,true);
			size_t line_length = strlen(line);
			if (jlen + line_length + 3 >= jsize) {
				while (jlen + line_length + 3 >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t != NULL) {
					json = t;
				}
				else {
					printf("[print::make_object_string]  Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			json[jlen++] = '\t';
			free(line);
			break;
		}
		case NUMBER:
		{
			char* line = make_value_number(kv.key, kv.value.number,true);
			size_t line_length = strlen(line);
			if (jlen + line_length + 3 >= jsize) {
				while (jlen + line_length + 3 >= jsize) {
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t != NULL) {
					json = t;
				}
				else {
					printf("[print::make_object_string] Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			json[jlen++] = '\t';
			free(line);
			break;
		}
		case TRUE:
		case FALSE:
		{
			char* line = make_value_bool(kv.key, kv.value.type == TRUE ? true : false,true);
			size_t line_length = strlen(line);
			if (jlen + line_length + 3 >= jsize) {
				while (jlen + line_length + 3 >= jsize) {
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t != NULL) {
					json = t;
				}
				else {
					printf("[print::make_object_string]  Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			json[jlen++] = '\t';
			free(line);
			break;
		}
		case NULLTYPE:
		{
			char* line = make_value_null(kv.key, true);
			size_t line_length = strlen(line);
			if (jlen + line_length + 3 >= jsize) {
				while (jlen + line_length + 3 >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t != NULL) {
					json = t;
				}
				else {
					printf("[print::make_object_string]  Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			json[jlen++] = '\t';
			free(line);
			break;
		}
		case ARRAY:
		{
			json[jlen++] = '"';
			strcpy(json + jlen, kv.key);
			jlen += strlen(kv.key);
			json[jlen++] = '"';
			json[jlen++] = ':';

			char* line = array2string(kv.value.object);
			size_t line_length = strlen(line);
			if (jlen + line_length + 3 >= jsize) {
				while (jlen + line_length + 3 >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t != NULL) {
					json = t;
				}
				else {
					printf("[print::make_object_string] Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			json[jlen++] = '\t';
			free(line);
			break;
		}
		case OBJECT:
		{
			json[jlen++] = '"';
			strcpy(json + jlen, kv.key);
			jlen += strlen(kv.key);
			json[jlen++] = '"';
			json[jlen++] = ':';

			char* line = object2string(kv.value.object);
			size_t line_length = strlen(line);
			if (jlen + line_length + 3 >= jsize) {
				while (jlen + line_length + 3 >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t != NULL) {
					json = t;
				}
				else {
					printf("[print::make_object_string] Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			json[jlen++] = '\t';
			free(line);
			break;
		}
		break;
		default:
			break;
		}
	}
	json[jlen - 1] = '}';
	json[jlen - 3] = '\b';
	return json;
}

char* array2string(Array* arr) {
	char* json = (char*)calloc(INIT_STR_SIZE, sizeof(char));
	if (json == NULL) {
		printf("[print::make_array_string] array to json string failed!\n");
		return NULL;
	}

	size_t jsize = INIT_STR_SIZE;
	size_t jlen = 0;
	json[jlen++] = '[';
	json[jlen++] = '\n';
	json[jlen++] = '\t';

	for (int numsOfElements = 0; numsOfElements < arr->nums; numsOfElements++) {
		JsonValue v = arr->jvs[numsOfElements];
		switch (v.type) {
		case STRING:
		{
			char* line = make_value_string("", v.string,false);
			size_t line_length = strlen(line);
			if (jlen + line_length + 3 >= jsize) {
				while (jlen + line_length + 3 >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t != NULL) {
					json = t;
				}
				else {
					printf("[print::make_array_string]  Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			json[jlen++] = '\t';
			free(line);
			break;
		}
		case NUMBER:
		{
			char* line = make_value_number("", v.number,false);
			size_t line_length = strlen(line);
			if (jlen + line_length + 3 >= jsize) {
				while (jlen + line_length + 3 >= jsize) {
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t != NULL) {
					json = t;
				}
				else {
					printf("[print::make_array_string] Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			json[jlen++] = '\t';
			free(line);
			break;
		}
		case TRUE:
		case FALSE:
		{
			char* line = make_value_bool("", v.type == TRUE ? true : false, false);
			size_t line_length = strlen(line);
			if (jlen + line_length + 3 >= jsize) {
				while (jlen + line_length + 3 >= jsize) {
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t != NULL) {
					json = t;
				}
				else {
					printf("[print::make_array_string]  Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			json[jlen++] = '\t';
			free(line);
			break;
		}
		case NULLTYPE:
		{
			char* line = make_value_null("",false);
			size_t line_length = strlen(line);
			if (jlen + line_length + 3 >= jsize) {
				while (jlen + line_length + 3 >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t != NULL) {
					json = t;
				}
				else {
					printf("[print::make_array_string]  Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			json[jlen++] = '\t';
			free(line);
			break;
		}
		case ARRAY:
		{
			char* line = array2string(v.array);
			size_t line_length = strlen(line);
			if (jlen + line_length + 3 >= jsize) {
				while (jlen + line_length + 3 >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t != NULL) {
					json = t;
				}
				else {
					printf("[print::make_array_string]  Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			json[jlen++] = '\t';
			free(line);
			break;
		}
		case OBJECT:
		{
			char* line = object2string(v.object);
			size_t line_length = strlen(line);
			if (jlen + line_length + 3 >= jsize) {
				while (jlen + line_length + 3 >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t != NULL) {
					json = t;
				}
				else {
					printf("[print::make_array_string]  Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			json[jlen++] = '\t';
			free(line);
			break;
		}
		default:
			break;
		}
	}
	json[jlen - 1] = ']';
	json[jlen - 3] = '\b';
	return json;
}

void print_obj(Obj* obj) {
	char* json_str = object2string(obj);
	if (json_str != NULL) {
		printf("%s\n", json_str);
	}
	else {
		printf("parse object to json string failed.");
	}
	free(json_str);
}

void print_arr(Array* arr) {
	char* json_str = array2string(arr);
	if (json_str != NULL) {
		printf("%s\n", json_str);
	}
	else {
		printf("parse array to json string failed.");
	}
	free(json_str);
}