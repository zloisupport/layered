#include "layered_texture.hpp"

Texture* call lTextureCreateW(const wchar_t* filename)
{
	Texture* image = new Texture(new Gdiplus::Image(filename));
	obj_pushback(image);
	return image;
}

IStream* pStream = 0;

unsigned int call lTextureWidth(Texture* tex)
{
	return tex->gTexture->GetWidth();
}

unsigned int call lTextureHeight(Texture* tex)
{
	return tex->gTexture->GetHeight();
}

int call lTextureDelete(Texture* tex)
{
	obj_remove(tex);
	delete tex;
	return 0;
}
