#include "utilities.h"
char* eat_space(char* cur_pos) {
	while (isspace(*cur_pos)) {
		cur_pos++;
	}
	return cur_pos;
}

bool isInteger(double num) {
	return fmod(num,1) == 0; //��num mod 1 == 0����numΪ����
}