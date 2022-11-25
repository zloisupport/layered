#include "layered_font.hpp"

Font* call lFontCreate(HFONT hfont)
{
	Font* font = new Font(hfont);

	obj_pushback(font);
	return font;
}

Font* lFontCopy(Font* font)
{
	if (!font)
		return 0;

	Font* f = new Font();
	f->gFont = font->gFont->Clone();

	obj_pushback(f);
	return f;
}

int call lFontDelete(Font* font)
{	
	obj_remove(font);
	delete font;
	return 0;
}
