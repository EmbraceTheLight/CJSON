#include"Unserialization.h"
#include"print.h"
static char* get_json(char* file_path) {
	FILE* stream = stdin;
	if (file_path) {
		stream = fopen(file_path, "r");
		if (!stream) {
			perror("File opening failed");
			return NULL;
		}
	}
	char* json = read_string(stream);
	if (stream != stdin) {
		fclose(stream);
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
			char file_path[INIT_STR_SIZE];
			gets_s(file_path, INIT_STR_SIZE);
			json = get_json(file_path);
		}
		else {
			printf("Please input json string,Input Ctrl+Z to end:\n");
			json = get_json(NULL);
		}
	}
	return json;
}

// resize_objKvs 对obj的kvs进行扩容
static Obj* resize_objKvs(Obj* obj, size_t size) {
	KeyValue* kvs = NULL;
	kvs = (KeyValue*)realloc(obj->kvs, size * sizeof(KeyValue));
	if (!kvs) {
		printf("[Unserialization::resize_objKvs] Realloc KeyValue failed!\n");
		return NULL;
	}
	obj->size = size;
	obj->kvs = kvs;
	return obj;
}

// check_ObjKvsSize 检查obj的kvs是否需要扩容
static Obj* check_ObjKvsSize(Obj* obj) {
	if (obj->nums >= obj->size) { //对kvs进行扩容
		Obj* tmp = NULL;
		tmp = resize_objKvs(obj, obj->size + INIT_OBJ_NUMS);
		if (!tmp) {
			printf("[Unserialization::check_ObjKvsSize] Realloc KeyValue failed!\n");
			cleanup((void*)obj, OBJECT);
			return NULL;
		}
		else {
			obj = tmp;
		}
	}
	return obj;
}

// resize_arrJvs 对array的jvs进行扩容
static Array* resize_arrJvs(Array* arr, size_t size) {
	JsonValue* jvs = NULL;
	jvs = (JsonValue*)realloc(arr->jvs, size * sizeof(JsonValue));
	if (!jvs) {
		printf("[Unserialization::resize_objKvs] Realloc KeyValue failed!\n");
		return NULL;
	}
	arr->size = size;
	arr->jvs = jvs;
	return arr;
}

// check_ArrayJvsSize 检查array的jvs是否需要扩容
static Array* check_ArrayJvsSize(Array* arr) {
	if (arr->nums >= arr->size) { //对kvs进行扩容
		Array* tmp = NULL;
		tmp = resize_arrJvs(arr, arr->size + INIT_ARRAY_NUMS);
		if (!tmp) {
			printf("[Unserialization::check_ObjKvsSize] Realloc KeyValue failed!\n");
			cleanup((void*)arr, ARRAY);
			return NULL;
		}
		else {
			arr = tmp;
		}
	}
	return arr;
}

char* parse_string(char** json_ptr) {
	char* start = (*json_ptr) + 1; //start指向第一个双引号后的第一个字符
	char* end = find_token(start, '"');
	if (!end) {
		printf("[Unserialization::parse_string] Invalid string format!\n");
		return NULL;
	}

	while (*(end - 1) == '\\') { //遇到转义字符\",跳过
		if (!end) {
			printf("[Unserialization::parse_string] Invalid string format!\n");
			return NULL;
		}
		end = find_token(end + 1, '"');
	}

	char* ret = (char*)calloc((end - start + 1), sizeof(char));
	if (!ret) {
		printf("[Unserialization::parse_string] Malloc string failed!\n");
		return NULL;
	}

	char* tmp = my_strcpy(ret, start, end);
	if (!tmp) {
		printf("[Unserialization::parse_string] Parse string failed!\n");
		free(ret);
		return NULL;
	}

	*json_ptr = end + 1;
	return ret;
}

Obj* string2object(char** json_ptr) {
	bool is_key = true;

	char* mov = *json_ptr;
	mov = eat_space(mov);//到达第一个键值对的第一个字符
	if (*mov != '{') {
		printf("[Unserialization::string2object] Invalid json format!\n");
		return NULL;
	}
	mov = eat_space(mov + 1);//到达第一个键值对的第一个字符

	Obj* newObj = init_obj();
	while (*mov != '}' && *mov != '\0') {
		switch (*mov) {
		case '"':
			if (is_key) {
				char* key = parse_string(&mov);
				if (!key) { //处理解析失败的情况：执行清理工作，然后退出
					printf("[Unserialization::parse_object] parse object failed!\n");
					cleanup((void*)newObj, OBJECT);
					return NULL;
				}
				mov = eat_space(mov);
				if (*mov != ':') { //key后面没有匹配到冒号，说明JSON格式错误
					printf("[Unserialization::parse_object] Invalid json format!\n");
					cleanup((void*)newObj, OBJECT);
					return NULL;
				}

				/*将key放入obj的keyValue的key中*/
				newObj->kvs[newObj->nums].key = key;
				is_key = !is_key;
				mov = eat_space(mov + 1);//跳过冒号，匹配value的第一个字符
			}
			else {//值为字符串
				char* value = parse_string(&mov);
				if (!value) {
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
		case '{': //匹配到左大括号，说明发现嵌套object
		{
			Obj* parsedObj = string2object(&mov);
			if (!parsedObj) {
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
		case '['://匹配到左中括号，说明发现嵌套数组
		{
			Array* parsedArr = string2array(&mov);
			if (!parsedArr) {
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
			if (isdigit(*mov) || *mov == '-') { //匹配到数字
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
			else { //匹配到其他字符，说明可能是布尔值或null
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

	if (*mov == '\0') { //没有匹配到右大括号
		printf("[Unserialization::parse_object] Invalid json format!\n");
		cleanup((void*)newObj, OBJECT);
		return NULL;
	}
	*json_ptr = mov + 1;
	return newObj;
}

Array* string2array(char** json_ptr) {
	Array* newArr = init_array();

	char* mov = *json_ptr;
	mov = eat_space(mov);
	if (*mov != '[') {
		printf("[Unserialization::string2array] Invalid json format!\n");
		return NULL;
	}
	mov = eat_space(mov + 1);//到达第一个元素的第一个字符

	while (*mov != ']' && *mov != '\0') {
		switch (*mov) {
		case '"': //匹配到字符串
		{
			char* value = parse_string(&mov);
			if (!value) {
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
		case '{': //匹配到左大括号，说明发现嵌套object
		{
			Obj* parsedObj = string2object(&mov);
			if (!parsedObj) {
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
		case '['://匹配到左中括号，说明发现嵌套数组
		{
			Array* parsedArr = string2array(&mov);
			if (!parsedArr) {
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
			if (isdigit(*mov) || *mov == '-') { //匹配到数字
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
			else { //匹配到其他字符，说明可能是布尔值或null
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

	if (*mov == '\0') { //没有匹配到右中括号
		printf("[Unserialization::parse_array] Invalid json format!\n");
		cleanup((void*)newArr, ARRAY);
		return NULL;
	}
	*json_ptr = mov + 1;
	return newArr;
}

bool create_key_value(Obj* obj) {
	printf("Please input key of key-values:\n");
	char* key = (char*)malloc(INIT_STR_SIZE * sizeof(char));
	if (!key) {
		printf("[Unserialization::create_key_value] Malloc key failed!\n");
		return false;
	}
	char* delk = key;
	key[0] = '"';
	gets_s(key+1, INIT_STR_SIZE-2);
	size_t len = strlen(key);
	key[len++] = '"';
	key[len] = '\0';

	int i;
	Obj*find=find_by_key(obj, key, &i);
	if (find) {
		printf("The key already exists, please input another key:\n");
		free(key);
		return false;
	}

	char*nk=parse_string(&key);
	if (!nk) {
		printf("[Unserialization::create_key_value] parse string failed!\n");
		free(delk);
		return false;
	}
	obj->kvs[obj->nums].key = nk;
	free(delk);

	printf("Please set value type:\n");
	printf("1 is string\n2 is number\n3 is true\n4 is false\n5 is null\n6 is array\n7 is object\n0 is quit\n");
	int select;
	scanf("%d", &select);
	while (getchar() != '\n');
	while (select < 0 || select > 7) {
		printf("Your choice is invalid.Please input number again:\n");
		scanf("%d", &select);
		while (getchar() != '\n');
	}

	switch (select) {
	case STRING: //string
	{
		printf("Please input string value:\n");

		char* value = (char*)malloc(INIT_STR_SIZE * sizeof(char));
		if (!value) {
			printf("[Unserialization::create_key_value] Malloc key failed!\n");
			free(key);
			return false;
		}
		char* delv = value;
		value[0] = '"';
		gets_s(value + 1, INIT_STR_SIZE - 2);
		size_t l = strlen(value);
		value[l++] = '"';
		value[l] = '\0';

		char*nv = parse_string(&value);
		if (!nv) {
			printf("[Unserialization::create_key_value] parse string failed!\n");
			free(delv);
			free(nk);
			return false;
		}
		obj->kvs[obj->nums].value.type = STRING;
		obj->kvs[obj->nums].value.string = nv;
		obj->nums++;
		obj = check_ObjKvsSize(obj);
		free(delv);
		break;
	}
	case NUMBER: //number
	{
		printf("Please input number value:\n");
		double num;
		scanf("%lf", &num);
		while (getchar() != '\n')
			continue;
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
		Array* parse_arr = string2array(&json_array);
		if (!parse_arr) {
			printf("[Unserialization::create_key_value] parse array failed!\n");
			free(del);
			free(nk);
			return false;
		}
		obj->kvs[obj->nums].value.type = ARRAY;
		obj->kvs[obj->nums].value.array = parse_arr;
		obj->nums++;
		free(del);
		obj = check_ObjKvsSize(obj);
		break;
	}
	case OBJECT:
	{
		char* json_object = get_json(NULL);
		char* del = json_object;
		Obj* parse_obj = string2object(&json_object);
		if (!parse_obj) {
			printf("[Unserialization::create_key_value] parse object failed!\n");
			free(del);
			free(nk);
			return false;
		}
		obj->kvs[obj->nums].value.type = OBJECT;
		obj->kvs[obj->nums].value.object = (struct Obj*)parse_obj;
		obj->nums++;
		free(del);
		obj = check_ObjKvsSize(obj);
		break;
	}
	case 0:
		free(nk);
		return false;
	}
	printf("create key-value success\n");
	return true;
}

Obj* find_by_key(Obj* obj, char* key, int* index) {
	char* cur = strtok(key, ".");
	Obj* curObj = obj;
	Obj* ret = NULL;
	/*KeyValue* kv = NULL;*/

	while (cur) {
		bool is_find = false;  //表明当前对象是否有找到key
		unsigned int i = 0;
		//遍历当前对象
		for (i = 0; i < curObj->nums; i++) {
			if (strcmp(curObj->kvs[i].key, cur) == 0) {
				ret = curObj;
				curObj = curObj->kvs[i].value.object;
				*index = i;
				is_find = true;
				break;
			}
		}
		if (!is_find) {
			return NULL;
		}
		cur = strtok(NULL, ".");

	}
	return ret;
}

//将原来的内存释放掉
static void help_cleaner(JsonValue jv) {
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
bool update_value(Obj* obj, char* key) {
	int i;
	Obj* object = find_by_key(obj, key, &i);
	if (!object) {
		fprintf(stderr, "No such key:%s\n", key);
		return false;
	}

	printf("Please set value type:\n");
	printf("1 is string\n2 is number\n3 is true\n4 is false\n5 is null\n6 is array\n7 is object\n0 is quit\n");
	int select;
	scanf("%d", &select);
	while (getchar() != '\n');

	switch (select) { //注意原来内存空间的释放
	case STRING:
	{
		char* newStr = (char*)malloc(INIT_STR_SIZE * sizeof(char));
		puts("Please input string value:\n");
		if (!newStr) {
			printf("[Unserialization::update_value] Malloc newStr failed!\n");
			return false;
		}
		gets_s(newStr, INIT_STR_SIZE);
		help_cleaner(object->kvs[i].value);
		object->kvs[i].value.type = STRING;
		object->kvs[i].value.string = newStr;
		break;
	}
	case NUMBER:
	{
		help_cleaner(object->kvs[i].value);
		printf("Please input number value:\n");
		double d = 0;
		scanf("%lf", &d);
		while (getchar() != '\n');
		object->kvs[i].value.type = NUMBER;
		object->kvs[i].value.number = d;
		break;
	}
	case TRUE:
		help_cleaner(object->kvs[i].value);
		object->kvs[i].value.type = TRUE;
		object->kvs[i].value.boolean = true;
		break;
	case FALSE:
		help_cleaner(object->kvs[i].value);
		object->kvs[i].value.type = FALSE;
		object->kvs[i].value.boolean = false;
		break;
	case NULLTYPE:
		help_cleaner(object->kvs[i].value);
		object->kvs[i].value.type = NULLTYPE;
		object->kvs[i].value.null = NULL;
		break;
	case OBJECT:
	{
		char* value = get_json(NULL);
		char* del = value;
		Obj* parse_obj = string2object(&value);
		if (!parse_obj) {
			fprintf(stderr, "[Unserialization::update_value] parse object failed!\n");
			free(del);
			return false;
		}
		help_cleaner(object->kvs[i].value);
		free(del);
		object->kvs[i].value.type = OBJECT;
		object->kvs[i].value.object = (struct Obj*)parse_obj;
		break;
	}
	case ARRAY:
	{
		char* value = get_json(NULL);
		char* del = value;
		Array* arr = string2array(&value);
		if (!arr) {
			fprintf(stderr, "[Unserialization::update_value] parse array failed!\n");
			free(del);
			return false;
		}
		help_cleaner(object->kvs[i].value);
		free(del);
		object->kvs[i].value.type = ARRAY;
		object->kvs[i].value.array = arr;
		break;
	}
	}
	return true;
}

bool del_by_key(Obj* obj, char* key) {
	int idx;
	Obj* object = find_by_key(obj, key, &idx);
	if (!object) {
		printf("No such key:%s\n", key);
		return false;
	}
	else {
		help_cleaner(object->kvs[idx].value);
		free(object->kvs[idx].key);
		for (int i = idx; i < object->nums - 1; i++) {
			obj->kvs[i] = obj->kvs[i + 1];
		}
		obj->nums--;
		return true;
	}
}

