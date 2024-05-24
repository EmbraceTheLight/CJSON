#include"Serialization.h"
#define ADDITIONAL_SPACE 5

char* object2string(Obj* obj) {
	char* json = (char*)calloc(INIT_STR_SIZE, sizeof(char));
	if (!json) {
		printf("[print::make_object_string] object to json string failed!\n");
		return NULL;
	}

	size_t jsize = INIT_STR_SIZE;
	size_t jlen = 0;
	json[jlen++] = '{';
	json[jlen++] = '\n';
	json[jlen++] = '\t';

	for (unsigned int numsOfElements = 0; numsOfElements < obj->nums; numsOfElements++) {
		KeyValue kv = obj->kvs[numsOfElements];
		switch (kv.value.type) {
		case STRING:
		{
			char* line = make_value_string(kv.key, kv.value.string, true);
			if (!line) {
				fprintf(stderr, "[Serialization::object2string] make_value_bool failed!\n");
				free(json);
				return NULL;
			}
			size_t line_length = strlen(line);
			if (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
				while (jlen + line_length + ADDITIONAL_SPACE >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t) {
					json = t;
				}
				else {
					printf("[print::make_object_string]  Realloc json string failed!\n");
					free(json);
					free(line);
					return NULL;
				}
			}
			strcpy(json + jlen, line);
			jlen += line_length;
			free(line);
			break;
		}
		case NUMBER:
		{
			char* line = make_value_number(kv.key, kv.value.number, true);
			if (!line) {
				fprintf(stderr, "[Serialization::object2string] make_value_bool failed!\n");
				free(json);
				return NULL;
			}
			size_t line_length = strlen(line);
			if (!line) {
				fprintf(stderr, "[Serialization::object2string] make_value_bool failed!\n");
				free(json);
				return NULL;
			}
			if (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
				while (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t) {
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
			free(line);
			break;
		}
		case TRUE:
		case FALSE:
		{
			char* line = make_value_bool(kv.key, kv.value.type == TRUE ? true : false, true);
			if (!line) {
				fprintf(stderr, "[Serialization::object2string] make_value_bool failed!\n");
				free(json);
				return NULL;
			}
			size_t line_length = strlen(line);

			if (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
				while (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t) {
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
			free(line);
			break;
		}
		case NULLTYPE:
		{
			char* line = make_value_null(kv.key, true);
			if (!line) {
				fprintf(stderr, "[Serialization::object2string] make_value_bool failed!\n");
				free(json);
				return NULL;
			}
			size_t line_length = strlen(line);
			if (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
				while (jlen + line_length + ADDITIONAL_SPACE >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t) {
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

			char* line = array2string(kv.value.array);
			if (!line) {
				fprintf(stderr, "[Serialization::object2string] make_value_bool failed!\n");
				free(json);
				return NULL;
			}
			size_t line_length = strlen(line);
			if (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
				while (jlen + line_length + ADDITIONAL_SPACE >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t) {
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
			if (!line) {
				fprintf(stderr, "[Serialization::object2string] make_value_bool failed!\n");
				free(json);
				return NULL;
			}
			size_t line_length = strlen(line);
			if (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
				while (jlen + line_length + ADDITIONAL_SPACE >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t) {
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
			free(line);
			break;
		}
		break;
		default:
			break;
		}
		if (numsOfElements != obj->nums - 1) {
			json[jlen++] = ',';
		}
		json[jlen++] = '\n';
		json[jlen++] = '\t';
	}
	json[jlen - 1] = '}';
	return json;
}

char* array2string(Array* arr) {
	char* json = (char*)calloc(INIT_STR_SIZE, sizeof(char));
	unsigned int  cnt = 1;//打印时每行的元素个数
	if (!json) {
		printf("[print::make_array_string] array to json string failed!\n");
		return NULL;
	}

	size_t jsize = INIT_STR_SIZE;
	size_t jlen = 0;
	json[jlen++] = '[';
	json[jlen++] = ' ';

	for (int numsOfElements = 0; numsOfElements < arr->nums; numsOfElements++) {
		JsonValue v = arr->jvs[numsOfElements];
		switch (v.type) {
		case STRING:
		{
			char* line = make_value_string("", v.string, false);
			size_t line_length = strlen(line);
			if (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
				while (jlen + line_length + ADDITIONAL_SPACE >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t) {
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
			free(line);
			break;
		}
		case NUMBER:
		{
			char* line = make_value_number("", v.number, false);
			size_t line_length = strlen(line);
			if (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
				while (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t) {
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
			free(line);
			break;
		}
		case TRUE:
		case FALSE:
		{
			char* line = make_value_bool("", v.type == TRUE ? true : false, false);
			size_t line_length = strlen(line);
			if (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
				while (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t) {
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
			free(line);
			break;
		}
		case NULLTYPE:
		{
			char* line = make_value_null("", false);
			size_t line_length = strlen(line);
			if (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
				while (jlen + line_length + ADDITIONAL_SPACE >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t) {
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
			free(line);
			break;
		}
		case ARRAY:
		{
			char* line = array2string(v.array);
			size_t line_length = strlen(line);
			if (jlen + line_length + ADDITIONAL_SPACE >= jsize) {
				while (jlen + line_length + ADDITIONAL_SPACE >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t) {
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
			free(line);
			break;
		}
		case OBJECT:
		{
			char* line = object2string(v.object);
			size_t line_length = strlen(line);
			if (jlen + line_length + 5 >= jsize) {
				while (jlen + line_length + 5 >= jsize) { //对字符串进行扩容
					jsize *= 1.25;
				}
				char* t = (char*)realloc(json, jsize);
				if (t) {
					json = t;
				}
				else {
					printf("[print::make_array_string]  Realloc json string failed!\n");
					free(json);
					return NULL;
				}
			}
			json[jlen++] = '\n';
			json[jlen++] = ' ';
			json[jlen++] = ' ';
			strcpy(json + jlen, line);
			jlen += line_length;
			json[jlen++] = ',';
			json[jlen++] = '\n';
			free(line);
			break;
		}
		default:
			break;
		}
		cnt++;
		if (numsOfElements != arr->nums - 1 && v.type != OBJECT)
			json[jlen++] = ',';

		if (cnt % 10 == 0)
			json[jlen++] = '\n';
		json[jlen++] = ' ';
	}
	json[jlen] = ']';
	json[jlen - 1] = ' ';
	return json;
}

Obj* create_obj() {
	printf("create json object:\n");
	Obj* obj = init_obj();
	if (!obj) {
		printf("[Serialization::create_obj] init json object failed!\n");
		return NULL;
	}
	printf("now the object is:");
	print_obj(obj);
	while (1) {
		printf("input y to add key-value pair, any other key to exit:");
		char c = getchar();
		while (getchar() != '\n');
		if (c != 'y' && c != 'Y') {
			printf("exit create object.\n");
			break;
		}
		bool ok = create_key_value(obj);
		if (!ok) {
			cleanup(obj, OBJECT);
			return NULL;
		}
		printf("now the object is:");
		print_obj(obj);
	}
	printf("[Serialization::create_obj] create json object success.\n");
	return obj;
}

