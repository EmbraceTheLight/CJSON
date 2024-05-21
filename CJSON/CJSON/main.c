#include"Serialization.h"
#include"Unserialization.h"
#include"tests.h"
int main(void) {
	//test_handle_input();  
	//test_parse_string();
	//test_parse_object();
	//test_parse_array();

	//test_json_string();
	test_json_find();
	test_json_addKV();
	test_json_updateKV();
	test_json_deleteKV();
	return 0;
}

