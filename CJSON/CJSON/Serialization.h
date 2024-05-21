#pragma once
#include"utilities.h"
#include"define.h"


// object2string 将object转换为json字符串
char* object2string(Obj* obj);

// array2string 将array转换为json字符串
char* array2string(Array* arr);