#include"Unserialization.h"

char* strptr;
char* handle_input(void) {
	char temp[BUFFER_SIZE];
	char* json = (char*)calloc(INIT_SIZE, sizeof(char));
	if (json == NULL) {
		printf("INIT json string failed!");
		return NULL;
	}

	size_t jsize = INIT_SIZE;
	size_t jlen = 0;

	puts("Input empty line to end inputing.");
	while (fgets(temp, BUFFER_SIZE, stdin) != NULL && temp[0] != '\n') {
		size_t line_length = strlen(temp);
		if (jlen + line_length >= jsize) {
			while (jlen + line_length >= jsize) { //¶Ô×Ö·û´®½øÐÐÀ©ÈÝ
				jsize *= 1.25;
			}
			char*t = (char*)realloc(json, jsize);
			if (t != NULL) {
				json = t;
			}else {
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


Obj* parse_object(char** json_ptr);

Array* parse_array(char** json_ptr);

JsonValue* parse_key_value(char** json_ptr, JSON* json);
