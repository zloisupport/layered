#include "layered_element.hpp"

// Images

lElement* call lImgLoad(Scene* scene, Texture* tex,
	int nLeft, int nTop, int nWidth, int nHeight)
{
	lElement* image = new lElement;
	image->scene = scene;
	image->index = scene->elements.size();
	image->type = IMG;
	image->pTexture = tex;
	image->bPart = FALSE;
	image->bVisible = TRUE;
	image->nOpasity = 100;
	image->nLeft = nLeft;
	image->nTop = nTop;
	image->nWidth = nWidth;
	image->nHeight = nHeight;
	image->nLeft_part = 0;
	image->nTop_part = 0;
	image->nWidth_part = (int)tex->gTexture->GetWidth();
	image->nHeight_part = (int)tex->gTexture->GetHeight();
	image->obj_type = obj_Image;

	scene->add(image);
	return image;
}

int call lImgDelete(lElement* img)
{
	Scene* scene = img->scene;
	scene->remove(img);
	delete img;
	return 0;
}

void call lImgSetVisibility(lElement* img, BOOL nValue)
{
	img->bVisible = nValue;
}

BOOL call lImgGetVisibility(lElement* img)
{
	return img->bVisible;
}

void call lImgSetVisiblePart(lElement* img, int nLeft, int nTop, int nWidth, int nHeight)
{
	img->bPart = TRUE;
	img->nLeft_part = nLeft;
	img->nTop_part = nTop;
	img->nWidth_part = nWidth;
	img->nHeight_part = nHeight;
}

void call lImgGetVisiblePart(lElement* img, int& nLeft, int& nTop, int& nWidth, int& nHeight)
{
	nLeft = img->nLeft_part;
	nTop = img->nTop_part;
	nWidth = img->nWidth_part;
	nHeight = img->nHeight_part;
}

void call lImgSetPosition(lElement* img, int nLeft, int nTop, int nWidth, int nHeight)
{
	img->nLeft = nLeft;
	img->nTop = nTop;
	if (((nWidth != img->nWidth) || (nHeight != img->nHeight)) && ((nWidth > 0) || (nHeight > 0)))
	{
		img->bAutoSize = FALSE;
		img->nWidth = nWidth;
		img->nHeight = nHeight;
	}
}

void call lImgGetPosition(lElement* img, int& nLeft, int& nTop, int& nWidth, int& nHeight)
{
	nLeft = img->nLeft;
	nTop = img->nTop;
	nWidth = img->nWidth;
	nHeight = img->nHeight;
}

const std::wstring to_wide(const std::string text);

// Strings

lElement* call lStrLoad(Scene* scene, const wchar_t* caption,
	int nLeft, int nTop, int nWidth, int nHeight, BOOL bAutoSize)
{
	lElement* string = new lElement;
	string->scene = scene;
	string->index = scene->elements.size();
	string->type = STR;
	string->pBrush = scene->pBrush;
	string->pFont = scene->pFont;
	string->bAutoSize = bAutoSize;
	string->bVisible = TRUE;
	string->nLeft = nLeft;
	string->nTop = nTop;
	string->nWidth = nWidth;
	string->nHeight = nHeight;
	string->nAlignment = 0;
	string->nLineAlignment = 0;
	string->sCaption = caption ? caption : L"\0";
	string->sCaption += L"\0";
	string->obj_type = obj_String;

	scene->add(string);
	return string;
}

int call lStrDelete(lElement* str)
{
	Scene* scene = str->scene;
	scene->remove(str);
	delete str;
	return 0;
}

void call lStrSetCaption(lElement* str, const wchar_t* sCaption)
{
	str->sCaption = sCaption ? sCaption : L"\0";
}

void call lStrGetCaption(lElement* str, wchar_t* sCaption)
{
	wcscpy_s(sCaption, 512, str->sCaption.c_str());
}

int call lStrLength(lElement* str)
{
	return str->sCaption.size();
}

void call lStrSetVisibility(lElement* str, BOOL nValue)
{
	str->bVisible = nValue;
}

BOOL call lStrGetVisibility(lElement* str)
{
	return str->bVisible;
}

void call lStrSetPosition(lElement* str, int nLeft, int nTop, int nWidth, int nHeight)
{
	str->nLeft = nLeft;
	str->nTop = nTop;
	if ((nWidth != str->nWidth) || (nHeight != str->nHeight))
	{
		str->bAutoSize = FALSE;
		str->nWidth = nWidth;
		str->nHeight = nHeight;
	}
}

void call lStrGetPosition(lElement* str, int& nLeft, int& nTop, int& nWidth, int& nHeight)
{
	nLeft = str->nLeft;
	nTop = str->nTop;
	nWidth = str->nWidth;
	nHeight = str->nHeight;
}

void call lStrSetBrush(lElement* str, Brush* brush)
{
	str->pBrush = (int)brush == -1 ? str->scene->pBrush : brush;
}

void call lStrSetFont(lElement* str, Font* font)
{
	str->pFont = (int)font == -1 ? str->scene->pFont : font;
}

void call lStrSetAlignment(lElement* str, int nValue)
{
	str->nAlignment = nValue;
}

void call lStrSetLineAlignment(lElement* str, int nValue)
{
	str->nLineAlignment = nValue;
}
