#pragma once
#include "precomp.h"
#include "layered_obj.hpp"

class Font : public Obj
{
public:
	Font(HFONT hfont)
	{
		obj_type = obj_Font;
		hDC = CreateCompatibleDC(0);
		SelectObject(hDC, (HGDIOBJ) hfont);
		gFont = new Gdiplus::Font(hDC);
	}

	Font() : hDC(0)
	{
		obj_type = obj_Font;
	}

	virtual ~Font()
	{
		delete gFont;
		if (hDC)
			DeleteDC(hDC);
	}

public:
	HDC hDC;
	Gdiplus::Font *gFont;
};

Font* call lFontCreate(HFONT hfont);
int call lFontDelete(Font* font);
Font* lFontCopy(Font* font);