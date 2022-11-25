#pragma once 
#include "precomp.h"
#include "layered_obj.hpp"

class Texture : public Obj
{
public:
	Texture(Gdiplus::Image* pointer)
	{
		obj_type = obj_Texture;
		gTexture = pointer;
	}

	~Texture()
	{
		delete gTexture;
	}

	Gdiplus::Image* gTexture;
};

//Gdiplus::Image* call lTextureCreateA(const char* filename);
Texture* call lTextureCreateW(const wchar_t* filename);
unsigned int call lTextureWidth(Texture* tex);
unsigned int call lTextureHeight(Texture* tex);
int call lTextureDelete(Texture* tex);
