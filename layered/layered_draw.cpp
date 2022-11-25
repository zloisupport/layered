#include "layered_draw.hpp"
#include "layered_brush.hpp"
#include "layered_scene.hpp"

extern Scene* CurrentScene;

// Image draw

unsigned int image_opasity = 100;

BOOL DrawImage(Gdiplus::Image* tex, Gdiplus::Rect rect1, Gdiplus::Rect rect2)
{
	Gdiplus::ColorMatrix colrMatrix;

	memset(&colrMatrix.m, 0, sizeof(colrMatrix.m));
	for (int g = 0; g < 5; g++)
		colrMatrix.m[g][g] = 1;
	colrMatrix.m[3][3] = float( (float)image_opasity / 100);

	Gdiplus::ImageAttributes imgAttributes;
	if (!imgAttributes.SetColorMatrix(&colrMatrix) == Gdiplus::Ok)
		return false;

	return (CurrentScene->graphics->DrawImage(tex, rect1,
		rect2.GetLeft(), rect2.GetTop(), rect2.Width, rect2.Height, 
		Gdiplus::UnitPixel, &imgAttributes, 0, 0) == Gdiplus::Ok);
}

BOOL call lDrawImage(Texture* tex, int x, int y)
{
	if (DrawImage(tex->gTexture, Gdiplus::Rect(x, y, tex->gTexture->GetWidth(), tex->gTexture->GetHeight()), 
		Gdiplus::Rect(0, 0, tex->gTexture->GetWidth(), tex->gTexture->GetHeight())))
		return true;
	else
		return false;
}

BOOL call lDrawImageRect(Texture* tex, int x, int y, int cx, int cy)
{
	if (DrawImage(tex->gTexture, Gdiplus::Rect(x, y, cx, cy), 
		Gdiplus::Rect(0, 0, tex->gTexture->GetWidth(), tex->gTexture->GetHeight())))
		return true;
	else
		return false;
}

BOOL call lDrawImageRectRect(Texture* tex, int dstx, int dsty, int dstcx, int dstcy, int srcx, int srcy, int srccx, int srccy)
{
	if (DrawImage(tex->gTexture, Gdiplus::Rect(dstx, dsty, dstcx, dstcy), Gdiplus::Rect(srcx, srcy, srccx, srccy)))
		return true;
	else
		return false;
}

// String draw

int aligment = Gdiplus::StringAlignmentNear;
int line_aligment = Gdiplus::StringAlignmentNear;

BOOL call lStringAlignment(int value)
{
	aligment = value;
	return TRUE;
}

BOOL call lStringLineAlignment(int value)
{
	line_aligment = value;
	return TRUE;
}

int lGetStringAlignment()
{
	return aligment;
}

int lGetStringLineAlignment()
{
	return line_aligment;
}

//const std::wstring to_wide(const std::string text);

#define CREATEFORMAT\
	Gdiplus::StringFormat format((Gdiplus::StringFormat*) Gdiplus::StringFormat::GenericTypographic());\
	format.SetTrimming(Gdiplus::StringTrimmingCharacter);

BOOL DrawString(const std::wstring wstr, Gdiplus::RectF rect, Gdiplus::Font* font, Gdiplus::Brush* brush)
{
	CREATEFORMAT
	format.SetAlignment((Gdiplus::StringAlignment)aligment);
	format.SetLineAlignment((Gdiplus::StringAlignment)line_aligment);

	return (CurrentScene->graphics->DrawString(wstr.c_str(),
		wstr.size(),
		font,
		rect,
		&format,
		brush) == Gdiplus::Ok);
}

BOOL call lDrawStringRect(const wchar_t* str, int x, int y, int cx, int cy, Font* font, Brush* brush)
{
	std::wstring wstr = str ? str : L"\0";

	if (!DrawString(wstr, Gdiplus::RectF(float(x), float(y), float(cx), float(cy)), 
		(int) font == -1 ? CurrentScene->pFont->gFont : font->gFont,
		(int) brush == -1 ? CurrentScene->pBrush->gBrush : brush->gBrush))
	{
		return false;
	}

	return true;
}

BOOL MeasureString(const std::wstring wstr, Font* font, int& cx, int& cy, Gdiplus::RectF& bounds)
{
	CREATEFORMAT
	
	if (!CurrentScene->graphics->MeasureString(wstr.c_str(),
		wstr.size(),
		font->gFont,
		Gdiplus::RectF(0, 0, 0, 0),
		&format,
		&bounds,
		0,
		0) == Gdiplus::Ok)
		return false;
	
	cx = int(bounds.Width);
	cy = int(bounds.Height);
	
	return true;
}

BOOL call lDrawString(const wchar_t* str, int x, int y, int& cx, int& cy, Font* font, Brush* brush)
{
	std::wstring wstr = str ? str : L"\0";
	Gdiplus::RectF bounds;

	int _x, _y;

	if (!MeasureString(wstr, (int)font == -1 ? CurrentScene->pFont : font, _x, _y, bounds))
	{
		return false;
	}

	bounds.X = float(x);
	bounds.Y = float(y);

	cx = _x;
	cy = _y;

	return DrawString(wstr, bounds, 
		(int)font == -1 ? CurrentScene->pFont->gFont : font->gFont,
		(int)brush == -1 ? CurrentScene->pBrush->gBrush : brush->gBrush);
}

BOOL call lMeasureString(const wchar_t* str, Font* font, int& cx, int& cy)
{
	Brush* brush = 0;

	std::wstring wstr = str;
	Gdiplus::RectF bounds;

	if (!MeasureString(wstr, font, cx, cy, bounds))
	{
		return false;
	}

	return true;
}
