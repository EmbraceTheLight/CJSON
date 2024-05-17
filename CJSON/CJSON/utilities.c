#include "utilities.h"
char* eat_space(char* cur_pos) {
	while (isspace(*cur_pos)) {
		cur_pos++;
	}
	return cur_pos;
}