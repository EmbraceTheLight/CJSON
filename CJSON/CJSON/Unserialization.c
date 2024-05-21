#include"helper.h"
#include"Unserialization.h"
static char* get_json(char* file_path) {
	char temp[BUFFER_SIZE];
	char* json = (char*)calloc(INIT_STR_SIZE, sizeof(char));
	if (json == NULL) {
		printf("[Unserialization::get_json] INIT json string failed!\n");
		return NULL;
	}

	size_t jsize = INIT_STR_SIZE;
	size_t jlen = 0;

	FILE* stream = stdin;
	if (file_path != NULL) {
		stream = fopen(file_path, "r");
		if (stream == NULL) {
			perror("File opening failed");
			free(json);
			return NULL;
		}
	}
	while (fgets(temp, BUFFER_SIZE, stream) != NULL && temp[0] != '\n') {
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
				printf("[Unserialization::get_json] Realloc json string failed!\n");
				free(json);
				return NULL;
			}
		}
		strcat(json, temp);
		jlen += line_length;
	}
	return json;
}

char* handle_input(void) {
	char* json = NULL;

	printf("Input 1 to read json from file, 2 to input json string,oter input to exit:\n");
	int select = 0;
	int t;
	t = (scanf("%d", &select));
	while (getchar() != '\n')
		continue;
	if (t != 1 || (select != 1 && select != 2)) {
		printf("Bye.\n");
		return NULL;
	}
	else {
		if (select == 1) {
			printf("Input file path:\n");
			char file_path[BUFFER_SIZE];
			gets_s(file_path, BUFFER_SIZE);
			json = get_json(file_path);
		}
		else {
			json = get_json(NULL);
		}
	}
	return json;
}

// init_obj ��ʼ��Obj
Obj* init_obj() {
	Obj* ret = (Obj*)malloc(1 * sizeof(Obj));
	if (ret == NULL) {
		printf("[Unserialization::init_obj] Init Obj failed!\n");
		return NULL;
	}
	ret->nums = 0;
	ret->size = INIT_OBJ_NUMS;
	ret->kvs = (KeyValue*)malloc(INIT_OBJ_NUMS * sizeof(KeyValue));
	if (ret->kvs == NULL) {
		printf("[Unserialization::init_obj] Init KeyValue failed!\n");
		free(ret->kvs);
		ret->kvs = NULL;
		return NULL;
	}
	return ret;
}

// init_array ��ʼ��Array
Array* init_array() {
	Array* ret = (Array*)malloc(1 * sizeof(Array));
	if (ret == NULL) {
		printf("[Unserialization::init_array] Init Array failed!\n");
		return NULL;
	}
	ret->nums = 0;
	ret->size = INIT_ARRAY_NUMS;
	ret->jvs = (JsonValue*)malloc(INIT_ARRAY_NUMS * sizeof(JsonValue));
	if (ret->jvs == NULL) {
		printf("[Unserialization::init_array] Init KeyValue failed!\n");
		free(ret->jvs);
		ret->jvs = NULL;
		return NULL;
	}
	return ret;
}

// resize_objKvs ��obj��kvs��������
static Obj* resize_objKvs(Obj* obj, size_t size) {
	KeyValue* kvs = NULL;
	kvs = (KeyValue*)realloc(obj->kvs, size * sizeof(KeyValue));
	if (kvs == NULL) {
		printf("[Unserialization::resize_objKvs] Realloc KeyValue failed!\n");
		return NULL;
	}
	obj->kvs = kvs;
	return obj;
}

// check_ObjKvsSize ���obj��kvs�Ƿ���Ҫ����
static Obj* check_ObjKvsSize(Obj* obj) {
	if (obj->nums >= obj->size) { //��kvs��������
		Obj* tmp = NULL;
		tmp = resize_objKvs(obj, obj->size + INIT_OBJ_NUMS);
		if (tmp == NULL) {
			printf("[Unserialization::check_ObjKvsSize] Realloc KeyValue failed!\n");

			return NULL;
		}
		else {
			obj = tmp;
		}
	}
	return obj;
}

// resize_arrJvs ��array��jvs��������
static Array* resize_arrJvs(Array* arr, size_t size) {
	JsonValue* jvs = NULL;
	jvs = (JsonValue*)realloc(arr->jvs, size * sizeof(JsonValue));
	if (jvs == NULL) {
		printf("[Unserialization::resize_objKvs] Realloc KeyValue failed!\n");
		return NULL;
	}
	arr->jvs = jvs;
	return arr;
}

// check_ArrayJvsSize ���array��jvs�Ƿ���Ҫ����
static Array* check_ArrayJvsSize(Array* arr) {
	if (arr->nums >= arr->size) { //��kvs��������
		Array* tmp = NULL;
		tmp = resize_arrJvs(arr, arr->size + INIT_ARRAY_NUMS);
		if (tmp == NULL) {
			printf("[Unserialization::check_ObjKvsSize] Realloc KeyValue failed!\n");

			return NULL;
		}
		else {
			arr = tmp;
		}
	}
	return arr;
}

// parse_string ��json�е�ĳ���ַ�����ȡ����
char* parse_string(char** json_ptr) {
	char* start = (*json_ptr) + 1; //startָ���һ��˫���ź�ĵ�һ���ַ�
	char* end = find_token(start, '"');
	if (end == NULL) {
		printf("[Unserialization::parse_string] Invalid string format!\n");
		return NULL;
	}

	while (*(end - 1) == '\\') { //����ת���ַ�\",����
		if (end == NULL) {
			printf("[Unserialization::parse_string] Invalid string format!\n");
			return NULL;
		}
		end = find_token(end + 1, '"');
	}

	size_t len = end - start - num_of_escape_char(start, end) + 1;//�ַ������ȣ�����ת���ַ�
	char* ret = (char*)malloc((end - start + 1) * sizeof(char));
	if (ret == NULL) {
		printf("[Unserialization::parse_string] Malloc string failed!\n");
		return NULL;
	}

	char* tmp = my_strcpy(ret, start, end);
	if (tmp == NULL) {
		printf("[Unserialization::parse_string] Copy string failed!\n");
		free(ret);
		return NULL;
	}

	*json_ptr = end + 1;
	return ret;
}

// parse_object ��json�ַ�������ΪObj����
Obj* parse_object(char** json_ptr) {
	bool is_key = true;
	Obj* newObj = init_obj();

	char* mov = *json_ptr;
	mov = eat_space(mov + 1);//�����һ����ֵ�Եĵ�һ���ַ�

	while (*mov != '}' && *mov != '\0') {
		switch (*mov) {
		case '"':
			if (is_key) {
				char* key = parse_string(&mov);
				if (key == NULL) { //�������ʧ�ܵ������ִ����������Ȼ���˳�
					printf("[Unserialization::parse_object] parse object failed!\n");
					cleanup((void*)newObj, OBJECT);
					return NULL;
				}
				mov = eat_space(mov);
				if (*mov != ':') { //key����û��ƥ�䵽ð�ţ�˵��JSON��ʽ����
					printf("[Unserialization::parse_object] Invalid json format!\n");
					cleanup((void*)newObj, OBJECT);
					return NULL;
				}

				/*��key����obj��keyValue��key��*/
				newObj->kvs[newObj->nums].key = key;
				is_key = !is_key;
				mov = eat_space(mov + 1);//����ð�ţ�ƥ��value�ĵ�һ���ַ�
			}
			else {//ֵΪ�ַ���
				char* value = parse_string(&mov);
				if (value == NULL) {
					printf("[Unserialization::parse_object] parse object failed!\n");
					cleanup((void*)newObj, OBJECT);
					return NULL;
				}
				newObj->kvs[newObj->nums].value.type = STRING;
				newObj->kvs[newObj->nums].value.string = value;
				newObj->nums++;
				newObj = check_ObjKvsSize(newObj);
				is_key = !is_key;
			}
			break;
		case '{': //ƥ�䵽������ţ�˵������Ƕ��object
		{
			Obj* parsedObj = parse_object(&mov);
			if (parsedObj == NULL) {
				printf("[Unserialization::parse_object] parse object failed!\n");
				cleanup((void*)newObj, OBJECT);
				return NULL;
			}
			newObj->kvs[newObj->nums].value.type = OBJECT;
			newObj->kvs[newObj->nums].value.object = (struct Obj*)parsedObj;
			newObj->nums++;
			newObj = check_ObjKvsSize(newObj);
			is_key = !is_key;
			break;
		}
		case '['://ƥ�䵽�������ţ�˵������Ƕ������
		{
			Array* parsedArr = parse_array(&mov);
			if (parsedArr == NULL) {
				printf("[Unserialization::parse_object] parse array failed!\n");
				cleanup((void*)newObj, OBJECT);
				return NULL;
			}
			newObj->kvs[newObj->nums].value.type = ARRAY;
			newObj->kvs[newObj->nums].value.array = parsedArr;
			newObj->nums++;
			newObj = check_ObjKvsSize(newObj);
			is_key = !is_key;
			break;
		}
		default:
			if (isdigit(*mov) || *mov == '-') { //ƥ�䵽����
				double d = strtod(mov, &mov);
				mov = eat_space(mov);
				if (*mov == ',' || *mov == '}') {
					newObj->kvs[newObj->nums].value.type = NUMBER;
					newObj->kvs[newObj->nums].value.number = d;
					newObj->nums++;
					newObj = check_ObjKvsSize(newObj);
					is_key = !is_key;
				}
				else {
					printf("[Unserialization::parse_object] Invalid json format!\n");
					cleanup((void*)newObj, OBJECT);
					return NULL;
				}
			}
			else { //ƥ�䵽�����ַ���˵�������ǲ���ֵ��null
				Type ret = is_str_legal(mov);
				if (ret != UNDEFINED) {
					switch (ret) {
					case TRUE:
						newObj->kvs[newObj->nums].value.type = TRUE;
						newObj->kvs[newObj->nums].value.boolean = true;
						mov = mov + 4;
						break;
					case FALSE:
						newObj->kvs[newObj->nums].value.type = FALSE;
						newObj->kvs[newObj->nums].value.boolean = false;
						mov = mov + 5;
						break;
					case NULLTYPE:
						newObj->kvs[newObj->nums].value.type = NULLTYPE;
						newObj->kvs[newObj->nums].value.null = NULL;
						mov = mov + 4;
						break;
					default:
						break;
					}
					newObj->nums++;
					newObj = check_ObjKvsSize(newObj);
					is_key = !is_key;
				}
				else {
					printf("[Unserialization::parse_object] Invalid json format!\n");
					cleanup((void*)newObj, OBJECT);
					return NULL;
				}
			}
			break;
		}
		mov = next_token(mov);
	}

	if (*mov == '\0') { //û��ƥ�䵽�Ҵ�����
		printf("[Unserialization::parse_object] Invalid json format!\n");
		cleanup((void*)newObj, OBJECT);
		return NULL;
	}
	*json_ptr = mov + 1;
	return newObj;
}

Array* parse_array(char** json_ptr) {
	Array* newArr = init_array();

	char* mov = *json_ptr;
	mov = eat_space(mov + 1);//�����һ��Ԫ�صĵ�һ���ַ�

	while (*mov != ']' && *mov != '\0') {
		switch (*mov) {
		case '"': //ƥ�䵽�ַ���
		{
			char* value = parse_string(&mov);
			if (value == NULL) {
				printf("[Unserialization::parse_array] parse array failed!\n");
				cleanup((void*)newArr, ARRAY);
				return NULL;
			}
			newArr->jvs[newArr->nums].type = STRING;
			newArr->jvs[newArr->nums].string = value;
			newArr->nums++;
			newArr = check_ArrayJvsSize(newArr);
			break;
		}
		case '{': //ƥ�䵽������ţ�˵������Ƕ��object
		{
			Obj* parsedObj = parse_object(&mov);
			if (parsedObj == NULL) {
				printf("[Unserialization::parse_array] parse object failed!\n");
				cleanup((void*)newArr, ARRAY);
				return NULL;
			}
			newArr->jvs[newArr->nums].type = OBJECT;
			newArr->jvs[newArr->nums].object = (struct Obj*)parsedObj;
			newArr->nums++;
			newArr = check_ArrayJvsSize(newArr);
			break;
		}
		case '['://ƥ�䵽�������ţ�˵������Ƕ������
		{
			Array* parsedArr = parse_array(&mov);
			if (parsedArr == NULL) {
				printf("[Unserialization::parse_array] parse array failed!\n");
				cleanup((void*)newArr, ARRAY);
				return NULL;
			}
			newArr->jvs[newArr->nums].type = ARRAY;
			newArr->jvs[newArr->nums].array = parsedArr;
			newArr->nums++;
			newArr = check_ArrayJvsSize(newArr);
			break;
		}
		default:
			if (isdigit(*mov) || *mov == '-') { //ƥ�䵽����
				double d = strtod(mov, &mov);
				mov = eat_space(mov);
				if (*mov == ',' || *mov == ']') {
					newArr->jvs[newArr->nums].type = NUMBER;
					newArr->jvs[newArr->nums].number = d;
					newArr->nums++;
					newArr = check_ArrayJvsSize(newArr);
				}
				else {
					printf("[Unserialization::parse_array] Invalid json format!\n");
					cleanup((void*)newArr, ARRAY);
					return NULL;
				}
			}
			else { //ƥ�䵽�����ַ���˵�������ǲ���ֵ��null
				Type ret = is_str_legal(mov);
				if (ret != UNDEFINED) {
					switch (ret) {
					case TRUE:
						newArr->jvs[newArr->nums].type = TRUE;
						newArr->jvs[newArr->nums].boolean = true;
						mov = mov + 4;
						break;
					case FALSE:
						newArr->jvs[newArr->nums].type = FALSE;
						newArr->jvs[newArr->nums].boolean = false;
						mov = mov + 5;
						break;
					case NULLTYPE:
						newArr->jvs[newArr->nums].type = NULLTYPE;
						newArr->jvs[newArr->nums].null = NULL;
						mov = mov + 4;
						break;
					default:
						break;
					}
					newArr->nums++;
					newArr = check_ArrayJvsSize(newArr);
				}
				else {
					printf("[Unserialization::parse_array] Invalid json format!\n");
					cleanup((void*)newArr, ARRAY);
					return NULL;
				}
			}
			break;
		}
		mov = next_token(mov);
	}

	if (*mov == '\0') { //û��ƥ�䵽��������
		printf("[Unserialization::parse_array] Invalid json format!\n");
		cleanup((void*)newArr, ARRAY);
		return NULL;
	}
	*json_ptr = mov + 1;
	return newArr;
}

/*��json�ڴ�����CRUD*/
static int find_by_key_ret_index(Obj* obj, char* key) {
	for (unsigned int i = 0; i < obj->nums; i++) {
		if (strcmp(obj->kvs[i].key, key) == 0) {
			return (int)i;
		}
	}
	return -1;
}

bool create_key_value(Obj* obj) {
	printf("Please input key of key-values:\n");
	char* key = (char*)malloc(INIT_STR_SIZE * sizeof(char));
	if (key == NULL) {
		printf("[Unserialization::create_key_value] Malloc key failed!\n");
		return false;
	}
	gets_s(key, 1024);
	obj->kvs[obj->nums].key = key;

	printf("Please set value type:\n");
	printf("1 is string\t\t2 is number\t\t3 is true\t\t4 is false\n\t\t5 is null\t\t6 is array\t\t7 is object\t\t0 is quit\n");
	int select;
	while ((scanf("%d", &select)) != 1 || (select < 0 || select > 7)) {
		printf("Your choice is invalid.Please input number again:\n");
		while (getchar() != '\n')
			continue;
	}

	switch (select) {
		case STRING: //string
		{
			printf("Please input string value:\n");
			char* value = (char*)malloc(INIT_STR_SIZE * sizeof(char));
			if (value == NULL) {
				printf("[Unserialization::create_key_value] Malloc value failed!\n");
				free(key);
				return false;
			}
			gets_s(value, 1024);
			obj->kvs[obj->nums].value.type = STRING;
			obj->kvs[obj->nums].value.string = value;
			obj->nums++;
			obj = check_ObjKvsSize(obj);
			break;
		}
		case NUMBER: //number
		{
			printf("Please input number value:\n");
			double num;
			while ((scanf("%lf", &num)) != 1) {
				printf("Your input is invalid.Please input number again:\n");
				while (getchar() != '\n')
					continue;
			}
			obj->kvs[obj->nums].value.type = NUMBER;
			obj->kvs[obj->nums].value.number = num;
			obj->nums++;
			obj = check_ObjKvsSize(obj);
			break;
		}
		case TRUE: //true
			obj->kvs[obj->nums].value.type = TRUE;
			obj->kvs[obj->nums].value.boolean = true;
			obj->nums++;
			obj = check_ObjKvsSize(obj);
			break;
		case FALSE: //false
			obj->kvs[obj->nums].value.type = FALSE;
			obj->kvs[obj->nums].value.boolean = false;
			obj->nums++;
			obj = check_ObjKvsSize(obj);
			break;
		case NULLTYPE:
			obj->kvs[obj->nums].value.type = NULLTYPE;
			obj->kvs[obj->nums].value.null = NULL;
			obj->nums++;
			obj = check_ObjKvsSize(obj);
			break;
		case ARRAY:
		{
			char* json_array = get_json(NULL);
			char* del = json_array;
			Array* parse_arr = parse_array(&json_array);
			obj->kvs[obj->nums].value.type = ARRAY;
			obj->kvs[obj->nums].value.array = parse_arr;
			obj->nums++;
			free(json_array);
			obj = check_ObjKvsSize(obj);
			break;
		}
		case OBJECT:
		{
			char* json_object = get_json(NULL);
			char* del = json_object;
			Obj* parse_obj = parse_object(&json_object);
			obj->kvs[obj->nums].value.type = OBJECT;
			obj->kvs[obj->nums].value.object = (struct Obj*)parse_obj;
			obj->nums++;
			obj = check_ObjKvsSize(obj);
			break;
		}
		case 0:
			free(key);
			return false;
		}
	return true;
}

KeyValue* find_by_key(Obj* obj, char* key) {
	int index = find_by_key_ret_index(obj, key);
	if (index != -1)
		return &(obj->kvs[index]);
	else
		return NULL;
}

static void help_cleaner(JsonValue jv) { //��ԭ�����ڴ��ͷŵ�
	switch (jv.type) {
		case STRING:
			cleanup((void*)jv.string, STRING);
			break;
		case OBJECT:
			cleanup((void*)jv.object, OBJECT);
			break;
		case ARRAY:
			cleanup((void*)jv.array, ARRAY);
			break;
	}
}
bool update_value(Obj* obj,char*key) {
	int idx = find_by_key_ret_index(obj, key);
	if (idx == -1) {
		fprintf(stderr, "No such key:%s\n", key);
		return false;
	}

	printf("Please set value type:\n");
	printf("1 is string\t\t2 is number\t\t3 is true\t\t4 is false\n\t\t5 is null\t\t6 is array\t\t7 is object\t\t0 is quit\n");
	int select;
	while ((scanf("%d", &select)) != 1 || (select < 0 || select > 7)) {
		printf("Your choice is invalid.Please input number again:\n");
		while (getchar() != '\n')
			continue;
	}
	switch (select) { //ע��ԭ���ڴ�ռ���ͷ�
		case STRING:
		{
			char* newStr = (char*)malloc(INIT_STR_SIZE*sizeof(char));
			if (!newStr) {
				printf("[Unserialization::update_value] Malloc newStr failed!\n");
				return false;
			}
			gets_s(newStr, INIT_STR_SIZE);
			help_cleaner(obj->kvs[idx].value);
			obj->kvs[idx].value.type = STRING;
			obj->kvs[idx].value.string = newStr;
			break;
		}
		case NUMBER:
			help_cleaner(obj->kvs[idx].value);
			printf("Please input number value:\n");
			double d = 0;
			while ((scanf("%lf", &d)) != 1) {
				printf("Your input is invalid.Please input number again:\n");
				while (getchar() != '\n')
					continue;
			}
			obj->kvs[idx].value.type = NUMBER;
			obj->kvs[idx].value.number = d;
			break;
		case TRUE:
			help_cleaner(obj->kvs[idx].value);
			obj->kvs[idx].value.type = TRUE;
			obj->kvs[idx].value.boolean = true;
			break;
		case FALSE:
			help_cleaner(obj->kvs[idx].value);
			obj->kvs[idx].value.type = FALSE;
			obj->kvs[idx].value.boolean = false;
			break;
		case NULLTYPE:
			help_cleaner(obj->kvs[idx].value);
			obj->kvs[idx].value.type = NULLTYPE;
			obj->kvs[idx].value.null = NULL;
			break;
		case OBJECT:
		{
			char* value = get_json(NULL);
			char* del = value;
			Obj* parse_obj = parse_object(&value);
			if (parse_obj == NULL) {
				fprintf(stderr, "[Unserialization::update_value] parse object failed!\n");
				free(del);
				return false;
			}
			help_cleaner(obj->kvs[idx].value);
			free(del);
			obj->kvs[idx].value.type = OBJECT;
			obj->kvs[idx].value.object = (struct Obj*)parse_obj;
			break;
		}
		case ARRAY:
		{
			char* value = get_json(NULL);
			char* del = value;
			Array* arr = parse_array(&value);
			if (arr == NULL) {
				fprintf(stderr, "[Unserialization::update_value] parse array failed!\n");
				free(del);
				return false;
			}
			help_cleaner(obj->kvs[idx].value);
			free(del);
			obj->kvs[idx].value.type = ARRAY;
			obj->kvs[idx].value.array = arr;
			break;
		}
	}
	return true;
}

bool del_by_key(Obj* obj, char* key) {
	int index = find_by_key_ret_index(obj, key);
	if (index != -1) {
		printf("No such key:%s\n", key);
		return false;
	}
	else {
		for (int i = index; i < obj->nums - 1; i++) {
			obj->kvs[i] = obj->kvs[i + 1];
		}
		obj->nums--;
		return true;
	}
}

