#include"helper.h"
#include"Unserialization.h"

char* handle_input(void) {
	char temp[BUFFER_SIZE];
	char* json = (char*)calloc(INIT_STR_SIZE, sizeof(char));
	if (json == NULL) {
		printf("[Unserialization::handle_input] INIT json string failed!\n");
		return NULL;
	}

	size_t jsize = INIT_STR_SIZE;
	size_t jlen = 0;

	puts("Input empty line to end inputing.");
	while (fgets(temp, BUFFER_SIZE, stdin) != NULL && temp[0] != '\n') {
		size_t line_length = strlen(temp);
		if (jlen + line_length >= jsize) {
			while (jlen + line_length >= jsize) { //对字符串进行扩容
				jsize *= 1.25;
			}
			char* t = (char*)realloc(json, jsize);
			if (t != NULL) {
				json = t;
			}
			else {
				printf("[Unserialization::handle_input]  Realloc json string failed!\n");
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

KeyValue* init_keyValue(char* str) {
	KeyValue* kv = (KeyValue*)malloc(1 * sizeof(KeyValue));
	if (kv == NULL) {
		printf("[Unserialization::init_keyValue] Malloc KeyValue failed!\n");
		return NULL;
	}
	kv->key = str;
	kv->value.type = UNDEFINED;
	return kv;
}

JsonValue* init_jsonValue(enum ValueType type) {
	JsonValue* jv = (JsonValue*)malloc(1 * sizeof(JsonValue));
	if (jv == NULL) {
		printf("[Unserialization::init_jsonValue] Malloc JsonValue failed!\n");
		return NULL;
	};
	jv->type = type;
	return jv;
}

// 对obj的kvs进行扩容
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

static Obj* check_ObjKvsSize(Obj* obj) {
	if (obj->nums >= obj->size) { //对kvs进行扩容
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

// 对array的jvs进行扩容
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

static Array* check_ArrayJvsSize(Array* arr) {
	if (arr->nums >= arr->size) { //对kvs进行扩容
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

Obj* parse_object(char** json_ptr) {
	bool is_key = true;
	Obj* newObj = init_obj();

	char* mov = *json_ptr;
	mov = eat_space(mov + 1);//到达第一个键值对的第一个字符

	while (*mov != '}' && *mov != '\0') {
		switch (*mov) {
		case '"':
			if (is_key) {
				char* key = parse_string(&mov);
				if (key == NULL) { //处理解析失败的情况：执行清理工作，然后退出
					printf("[Unserialization::parse_object] parse object failed!\n");

					return NULL;
				}
				mov = eat_space(mov);
				if (*mov != ':') { //key后面没有匹配到冒号，说明JSON格式错误
					printf("[Unserialization::parse_object] Invalid json format!\n");

					return NULL;
				}

				/*将key放入obj的keyValue的key中*/
				newObj->kvs[newObj->nums].key = key;
				is_key = !is_key;
				mov = eat_space(mov + 1);//跳过冒号，匹配value的第一个字符
			}
			else {//值为字符串
				char* value = parse_string(&mov);
				if (value == NULL) {
					printf("[Unserialization::parse_object] parse object failed!\n");

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
			Obj* parsedObj = parse_object(&mov);
			if (parsedObj == NULL) {
				printf("[Unserialization::parse_object] parse object failed!\n");

				return NULL;
			}
			newObj->kvs[newObj->nums].value.type = OBJECT;
			newObj->kvs[newObj->nums].value.object = parsedObj;
			newObj->nums++;
			newObj = check_ObjKvsSize(newObj);
			is_key = !is_key;
			break;
		}
		case '['://匹配到左中括号，说明发现嵌套数组
		{
			Array* parsedArr = parse_array(&mov);
			if (parsedArr == NULL) {
				printf("[Unserialization::parse_object] parse array failed!\n");

				return NULL;
			}
			newObj->kvs[newObj->nums].value.type = ARRAY;
			newObj->kvs[newObj->nums].value.object = parsedArr;
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

					return NULL;
				}
			}
			break;
		}
		mov = next_token(mov);
	}

	if (*mov == '\0') { //没有匹配到右大括号
		printf("[Unserialization::parse_object] Invalid json format!\n");

		return NULL;
	}
	*json_ptr = mov + 1;
	return newObj;
}

char* parse_string(char** json_ptr) {
	char* start = (*json_ptr) + 1; //start指向第一个双引号后的第一个字符
	char* end = find_token(start, '"');
	if (end == NULL) {
		printf("[Unserialization::parse_string] Invalid string format!\n");
		return NULL;
	}
	while (*(end - 1) == '\\') { //遇到转义字符\"
		if (end == NULL) {
			printf("[Unserialization::parse_string] Invalid string format!\n");
			return NULL;
		}
		 end = find_token(end+1, '"');
	}

	size_t len = end - start + 1 - num_of_escape_char(start,end);//字符串长度，计算转义字符
	char* ret = (char*)malloc((end - start + 1) * sizeof(char));
	if (ret == NULL) {
		printf("[Unserialization::parse_string] Malloc string failed!\n");
		return NULL;
	}

	char*tmp = my_strcpy(ret, start, end);
	if (tmp == NULL) {
		printf("[Unserialization::parse_string] Copy string failed!\n");
		free(ret);
		return NULL;
	}
	*json_ptr = end + 1;
	return ret;
}

Array* parse_array(char** json_ptr) {
	Array* newArr = init_array();

	char* mov = *json_ptr;
	mov = eat_space(mov + 1);//到达第一个元素的第一个字符

	while (*mov != ']' && *mov != '\0') {
		switch (*mov) {
		case '"': //匹配到字符串
		{
			char* value = parse_string(&mov);
			if (value == NULL) {
				printf("[Unserialization::parse_array] parse array failed!\n");

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
			Obj* parsedObj = parse_object(&mov);
			if (parsedObj == NULL) {
				printf("[Unserialization::parse_array] parse object failed!\n");

				return NULL;
			}
			newArr->jvs[newArr->nums].type = OBJECT;
			newArr->jvs[newArr->nums].object = parsedObj;
			newArr->nums++;
			newArr = check_ArrayJvsSize(newArr);
			break;
		}
		case '['://匹配到左中括号，说明发现嵌套数组
		{
			Array* parsedArr = parse_array(&mov);
			if (parsedArr == NULL) {
				printf("[Unserialization::parse_array] parse array failed!\n");

				return NULL;
			}
			newArr->jvs[newArr->nums].type = ARRAY;
			newArr->jvs[newArr->nums].object = parsedArr;
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

					return NULL;
				}
			}
			break;
		}
		mov = next_token(mov);
	}

	if (*mov == '\0') { //没有匹配到右中括号
		printf("[Unserialization::parse_array] Invalid json format!\n");

		return NULL;
	}
	*json_ptr = mov + 1;
	return newArr;
}

