#pragma once
#include "precomp.h"
#include "layered_font.hpp"
#include "layered_brush.hpp"
#include "layered_texture.hpp"

BOOL call lDrawImage(Texture* tex, int x, int y);
BOOL call lDrawImageRect(Texture* tex, int x, int y, int cx, int cy);
BOOL call lDrawImageRectRect(Texture* tex, int dstx, int dsty, int dstcx, int dstcy, int srcx, int srcy, int srccx, int srccy);

BOOL call lStringAlignment(int value);
BOOL call lStringLineAlignment(int value);
//BOOL call lDrawStringRect(const char* str, int x, int y, int cx, int cy, Font* font, Gdiplus::Brush* brush);
BOOL call lDrawStringRect(const wchar_t* str, int x, int y, int cx, int cy, Font* font, Brush* brush);
//BOOL call lDrawString(const char* str, int x, int y, int& cx, int& cy, Font* font, Gdiplus::Brush* brush);
BOOL call lDrawString(const wchar_t* str, int x, int y, int& cx, int& cy, Font* font, Brush* brush);
//BOOL call lMeasureString(const char* str, Font* font, int& cx, int& cy);
BOOL call lMeasureString(const wchar_t* str, Font* font, int& cx, int& cy);

int lGetStringAlignment();
int lGetStringLineAlignment();