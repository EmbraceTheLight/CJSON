#include"helper.h"
#include"Unserialization.h"

char* strptr;
char* handle_input(void) {
	char temp[BUFFER_SIZE];
	char* json = (char*)calloc(INIT_STR_SIZE, sizeof(char));
	if (json == NULL) {
		printf("INIT json string failed!");
		return NULL;
	}

	size_t jsize = INIT_STR_SIZE;
	size_t jlen = 0;

	puts("Input empty line to end inputing.");
	while (fgets(temp, BUFFER_SIZE, stdin) != NULL && temp[0] != '\n') {
		size_t line_length = strlen(temp);
		if (jlen + line_length >= jsize) {
			while (jlen + line_length >= jsize) { //���ַ�����������
				jsize *= 1.25;
			}
			char* t = (char*)realloc(json, jsize);
			if (t != NULL) {
				json = t;
			}
			else {
				printf("Realloc json string failed!");
				free(json);
				return NULL;
			}
		}
		strcat(json, temp);
		jlen += line_length;
	}
	return json;
}

Obj* init_obj() {
	Obj* ret = (Obj*)malloc(1*sizeof(Obj));
	if (ret == NULL) {
		printf("Init Obj failed!");
		return NULL;
	}
	ret->nums = 0;
	ret->size = INIT_OBJ_NUMS;
	ret->kvs = (KeyValue*)malloc(INIT_OBJ_NUMS * sizeof(KeyValue));
	if (ret->kvs == NULL) {
		printf("[Unserialization::init_obj] Init KeyValue failed!");
		free(ret->kvs);
		ret->kvs = NULL;
		return NULL;
	}
	return ret;
}
Array* init_array() {
	Array* ret = (Array*)malloc(1*sizeof(Array));
	if (ret == NULL) {
		printf("Init Array failed!");
		return NULL;
	}
	ret->nums = 0;
	ret->nums = INIT_ARRAY_NUMS;
	ret->jvs = (JsonValue*)malloc(INIT_ARRAY_NUMS * sizeof(JsonValue));
	if (ret->jvs == NULL) {
		printf("[Unserialization::init_obj] Init KeyValue failed!");
		free(ret->jvs);
		ret->jvs = NULL;
		return NULL;
	}
	return ret;
}

Obj* parse_object(char** json_ptr) {
	bool is_key = true;
	char* mov = *json_ptr;
	Obj* newObj = init_obj();
	while (*mov != '}' && *mov != '\0') {
		mov = eat_space(mov);
		switch (*mov) {
		case '"':
			if (is_key) { //��������key
				char* key = parse_string(json_ptr);
				if (key == NULL) { //�������ʧ�ܵ������ִ����������Ȼ���˳�

				}
				is_key = !is_key;
			}
			break;
		case ':': 
			if (is_key) {  //ƥ�䵽ð�ţ���֮ǰ�Ѿ���������key��˵����һ��Ѱ��value
				is_key = !is_key;
			}
			else { //����˵��JSON��ʽ����
				printf("Invalid json format!");
				exit(EXIT_FAILURE);
			}
			break;
		case '{': //ƥ�䵽������ţ�˵������Ƕ��object
			Obj * parsedObj = parse_object(json_ptr);
			is_key = !is_key;
			break;
		case '['://ƥ�䵽�������ţ�˵������Ƕ������
			Array * parsedArr = parse_array(json_ptr);
			is_key = !is_key;
			break;
		default:
			break;
		}
	}
	if (*mov == '\0') { //û��ƥ�䵽�Ҵ�����
		printf("Invalid json format!");
		exit(EXIT_FAILURE);
	}
	*json_ptr = mov + 1;
	return newObj;
}

char* parse_string(char** json_ptr) {
	char* start = (*json_ptr) + 1;
	char* end = find_token(start, '"');
	if (end == NULL) {
		printf("Invalid string format!");
		return NULL;
	}
	char* ret = (char*)malloc((end - start + 1) * sizeof(char));
	if (ret == NULL) {
		printf("Malloc string failed!");
		return NULL;
	}
	strncpy(ret, start, end - start);
	ret[end - start] = '\0';
	*json_ptr = end + 1;
	return ret;
}

Array* parse_array(char** json_ptr) {

}

