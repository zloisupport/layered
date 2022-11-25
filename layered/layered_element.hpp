#pragma once
#include "precomp.h"
#include "layered_obj.hpp"
#include "layered_font.hpp"
#include "layered_brush.hpp"
#include "layered_texture.hpp"
#include "layered_draw.hpp"
#include "layered_scene.hpp"

class Scene;
class lElement;

lElement* call lImgLoad(Scene* scene, Texture* tex,
	int nLeft, int nTop, int nWidth, int nHeight);
int call lImgDelete(lElement* img);
void call lImgSetVisibility(lElement* img, BOOL nValue);
BOOL call lImgGetVisibility(lElement* img);
void call lImgSetVisiblePart(lElement* img, int nLeft, int nTop, int nWidth, int nHeight);
void call lImgGetVisiblePart(lElement* img, int& nLeft, int& nTop, int& nWidth, int& nHeight);
void call lImgSetPosition(lElement* img, int nLeft, int nTop, int nWidth, int nHeight);
void call lImgGetPosition(lElement* img, int& nLeft, int& nTop, int& nWidth, int& nHeight);

lElement* call lStrLoad(Scene* scene, const wchar_t* caption,
	int nLeft, int nTop, int nWidth, int nHeight, BOOL bAutoSize);
int call lStrDelete(lElement* str);
void call lStrSetCaption(lElement* str, const wchar_t* sCaption);
void call lStrGetCaption(lElement* str, wchar_t* sCaption);
int call lStrLength(lElement* str);
void call lStrSetVisibility(lElement* str, BOOL nValue);
BOOL call lStrGetVisibility(lElement* str);
void call lStrSetPosition(lElement* str, int nLeft, int nTop, int nWidth, int nHeight);
void call lStrGetPosition(lElement* str, int& nLeft, int& nTop, int& nWidth, int& nHeight);
void call lStrSetBrush(lElement* str, Brush* brush);
void call lStrSetFont(lElement* str, Font* font);
void call lStrSetAlignment(lElement* str, int nValue);
void call lStrSetLineAlignment(lElement* str, int nValue);
