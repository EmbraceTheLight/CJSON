#include "print.h"
#include "helper.h"
#include "Serialization.h"

char* handle_json_string(char* json_str) {
	char* cur = json_str;
	char* ret = (char*)calloc(INIT_STR_SIZE,sizeof(char));
	if (!ret) {
		free(json_str);
		return NULL;
	}
	bool flag = false;
	size_t len = 0;
	size_t size = INIT_STR_SIZE;
	while (*cur != '\0') {
		if (!flag && (*cur == '\t' || *cur == '\n')) {
			cur++;
			continue;
		}
		if (len + 2 >= size) {
			char* t = (char*)realloc(ret, size * 2);
			if (!t) {
				perror("realloc failed:");
				free(ret);
				free(json_str);
				return NULL;
			}
			ret = t;
			size *= 2;
		}
		switch (*cur)
		{
		case '\"':
			if (!flag) {
				ret[len++]=*cur;
				flag = true;
			}
			else {
				if(*(cur+1)==','||*(cur+1)==':'||*(cur + 1) == '\n'){ //Óöµ½×Ö·û´®ÖÕÖ¹±ê¼Ç
					ret[len++] = *cur;
					flag = false;
				}
				else {
					ret[len++]='\\';
					ret[len++]=*cur;
				}
			}
			break;
		case '\\':
			ret[len++] = '\\';
			ret[len++] = '\\';
			break;
		case '\/':
			ret[len++] = '\\';
			ret[len++] = '/';
			break;
		case '\b':
			ret[len++] = '\\';
			ret[len++] = 'b';
			break;
		case '\f':
			ret[len++] = '\\';
			ret[len++] = 'f';
			break;
		case '\n':
			ret[len++] = '\\';
			ret[len++] = 'n';
			break;
		case '\r':
			ret[len++] = '\\';
			ret[len++] = 'r';
			break;
		case '\t':
			ret[len++] = '\\';
			ret[len++] = 't';
			break;
		default:
			ret[len++] = *cur;
			break;
		}
		cur++;
	}
	ret[len] = '\0';
	free(json_str);
	return ret;
}
static void ask_if_save_file(Obj* obj) {
	printf("Do you want to save this json?[y/n]");
	char c = getchar();
	while (getchar() != '\n');

	if (c == 'y' || c == 'Y') {
		printf("input the file path to save the json:");
		char path[INIT_STR_SIZE];
		gets_s(path, INIT_STR_SIZE);
		char* json_str = object2string(obj);
		char* final_str=handle_json_string(json_str);
		FILE* fp = fopen(path, "w");
		if (!fp) {
			perror("create file failed:");
			free(final_str);
			return;
		}
		fputs(final_str, fp);
		fclose(fp);
		free(final_str);
		printf("json object saved to %s success\n", path);
	}
}
static void ask_if_modify_JsonObj(Obj* obj) {
	printf("Do you want to modify this json object?[y/n]");
	char c = getchar();
	while (getchar() != '\n');

	if (c == 'y' || c == 'Y') {
		while (1) {
			int select;
			printf("1. add key-value pair\n2. modify key-value pair\n3. delete key-value pair\n4. exit\n");
			printf("select the item to modify:\n");
			scanf("%d", &select);
			while (getchar() != '\n');
			if (select == 4)
				break;
			switch (select) {
			case 1:
			{
				bool ok = create_key_value(obj);
				if (!ok) {
					fprintf(stderr, "create key-value pair failed.\n");
				}
				break;
			}
			case 2:
			{
				char key[INIT_STR_SIZE];
				printf("input the key to update:");
				gets_s(key, INIT_STR_SIZE);
				bool ok = update_value(obj, key);
				if (!ok) {
					fprintf(stderr, "modify key-value pair failed.\n");
				}
				break;
			}
			case 3:
			{
				char key[INIT_STR_SIZE];
				printf("input the key to delete:");
				gets_s(key, INIT_STR_SIZE);
				bool ok = del_by_key(obj, key);
				if (!ok) {
					fprintf(stderr, "delete key-value pair failed.\n");
				}
				break;
			}
			case 4:
				break;
			default:
				printf("invalid select.\n");
				break;
			}
			printf("Now the obj is:\n");
			print_obj(obj);
			printf("Enter y to continue modifying object:");
			char c = getchar();
			while (getchar() != '\n');
			if (c != 'y' && c != 'Y') {
				break;
			}

		}

	}
}

void main_menu() {
	printf("welcome to use this json tool.\n");
	printf("enter your select:\n");
	printf("1. create json object\n");
	printf("2. parse json string to object\n");
	printf("3. exit\n");
	int select;
	Obj* obj = NULL;
	scanf("%d", &select);
	while (getchar() != '\n');
	switch (select) {
	case 1:
	{
		obj = create_obj();
		if (!obj) {
			printf("create json object failed.\n");
			break;
		}
		ask_if_save_file(obj);
		break;
	}
	case 2:
	{
		char* res = handle_input();
		if (!res) {
			break;
		}
		char* del = res;
		obj = string2object(&res);
		if (!obj) {
			printf("parse json string to object failed.\n");
			free(del);
			break;
		}
		printf("parsed object is:\n");
		print_obj(obj);
		free(del);
		ask_if_modify_JsonObj(obj);
		ask_if_save_file(obj);
		break;
	}
	case 3:
		break;
	}
	if (obj)
		cleanup(obj, OBJECT);
	puts("bye");
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

void print_KeyValue(KeyValue* kv) {
	printf("key is %s:\n ", kv->key);
	switch (kv->value.type) {
	case STRING:
		printf("value type is string\n");
		printf("value is %s\n", kv->value.string);
		break;
	case NUMBER:
		printf("value type is number\n");
		printf("value is %f\n", kv->value.number);
		break;
	case TRUE:
		printf("value type is boolean-->true\n");
		break;
	case FALSE:
		printf("value type is boolean-->false\n");
		break;
	case NULLTYPE:
		printf("value type is null\n");
		break;
	case ARRAY:
		printf("value type is array\n");
		print_arr(kv->value.array);
		break;
	case OBJECT:
		printf("value type is object\n");
		print_obj(kv->value.object);
	}
}