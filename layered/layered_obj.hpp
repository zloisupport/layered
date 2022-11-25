#pragma once
#include "precomp.h"

#define obj_Scene 1
#define obj_Texture 2
#define obj_Brush 3
#define obj_Font 4
#define obj_Image 5
#define obj_String 6
#define obj_Window 7

class Obj
{	
public:
	int obj_type;
};

void obj_pushback(Obj* pObj);
void obj_remove(Obj* pObj);
void obj_delete();
//int handle_obj_type(void* pObj);
std::wstring obj_type_to_name(int type);