#include "layered_obj.hpp"
#include "layered_font.hpp"
#include "layered_brush.hpp"
#include "layered_texture.hpp"
#include "layered_scene.hpp"
#include "layered_ui.hpp"

std::vector<Obj*> obj;
std::wstring intToStr(int value);

void obj_pushback(Obj* pObj)
{
	obj.push_back(pObj);
}

void obj_remove(Obj* pObj)
{
	for (int i = (int) obj.size() - 1; i >= 0; i--)
	{
		if (i >= 0)
			if (obj[i])
				if (obj[i] == pObj)
					obj[i] = 0;
	}
}

void obj_delete()
{
	
	for (int i = obj.size() - 1; i >= 0; i--)
	{
		if ((i >= 0) && (obj[i]))
			if (obj[i]->obj_type == obj_Scene)
			{
				lSceneDelete((Scene*)obj[i]);
				obj[i] = 0;
			}	
	}

	for (int i = obj.size() - 1; i >= 0; i--)
	{
		if ((i >= 0) && (obj[i]))
			if (obj[i]->obj_type == obj_Brush)
			{
				lBrushDelete((Brush*)obj[i]);
				obj[i] = 0;
			}
	}
	
	for (int i = obj.size() - 1; i >= 0; i--)
	{
		if ((i >= 0) && (obj[i]))
			if (obj[i]->obj_type == obj_Texture)
			{
				lTextureDelete((Texture*)obj[i]);
				obj[i] = 0;
			}
	}

	for (int i = obj.size() - 1; i >= 0; i--)
	{
		if ((i >= 0) && (obj[i]))
			if (obj[i]->obj_type == obj_Font)
			{
				lFontDelete((Font*)obj[i]);
				obj[i] = 0;
			}
	}
}

std::wstring obj_type_to_name(int type)
{
#define CASE(t, n) case t: return n; break
	switch (type)
	{
		CASE(obj_Scene, L"Scene");
		CASE(obj_Texture, L"Texture");
		CASE(obj_Brush, L"Brush");
		CASE(obj_Font, L"Font");
		CASE(obj_Image, L"Image");
		CASE(obj_String, L"String");
		CASE(obj_Window, L"Window");
		default: return intToStr(type); break;
	}
#undef CASE
}
