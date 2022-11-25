#pragma once
#include "precomp.h"
#include "layered_obj.hpp"
#include "layered_font.hpp"
#include "layered_brush.hpp"
#include "layered_texture.hpp"
#include "layered_draw.hpp"
#include "layered_element.hpp"
#include "layered_ui.hpp"

enum EType { IMG = 1, STR = 2, DRAW = 3 };

class Scene;

class lElement : public Obj
{
public:
	Scene* scene;
	unsigned int index;
	EType type;

	std::wstring sCaption;
	int nLeft, nTop, nWidth, nHeight,
		nLeft_part, nTop_part, nWidth_part, nHeight_part;
	Texture* pTexture;
	Brush* pBrush;
	Font* pFont;
	BOOL bVisible, bAutoSize, bPart;
	int nAlignment, nLineAlignment;
	unsigned int nOpasity;
};

typedef VOID(WINAPI* pEventProc)(int);

std::wstring intToStr(int value);

class Win : public Obj
{
public:
	Win() :
		bEnter(FALSE), bStartUpdate(false), nMod(5),
		bEnabled(TRUE), bVisable(TRUE), bChecked(TRUE), bUpdate(TRUE), bOnce(TRUE),
		hWnd(0), Image(0), Text(0), PB(0),
		onClick(0), onEnter(0), onLeave(0), onChange(0),
		bNormal(0), bFocused(0), bDisabled(0),
		nPBValue(0), nPBMax(100), nPBState(1)
	{
		obj_type = obj_Window;
		nOldCProc = 0;
		nEOldProc = 0;
		hChild = 0;
		hChild = 0;
	}

	~Win()
	{
		if (nEOldProc) SetWindowLong(hWnd, GWL_WNDPROC, nEOldProc);
		if (hWnd2) DestroyWindow(hWnd2);
		if (hWnd) DestroyWindow(hWnd);

		if (hChild2) DestroyWindow(hChild2);
		if (hChild) DestroyWindow(hChild);

		if (hFont) DeleteObject(hFont);
	}

	Scene* scene;
	int index;
	DWORD style;
	std::wstring Caption;
	Texture* Texture;
	int nLeft, nTop, nWidth, nHeight;
	BOOL bEnter, bEnabled, bVisable, bChecked, bUpdate, bOnce;

	int nPBValue, nPBMax, nPBState;

	HWND hWnd, hWnd2, hChild, hChild2; 

	lElement* Text;
	lElement* Image;
	lElement* PB;
	long nOldCProc, nEOldProc;
	HFONT hFont;
	bool bStartUpdate;
	int nMod;

	pEventProc onClick;
	pEventProc onEnter;
	pEventProc onLeave;
	pEventProc onChange;

	Brush* bNormal;
	Brush* bFocused;
	Brush* bDisabled;
};

class Scene : public Obj
{
public:

	typedef VOID(WINAPI* pProc)(int);

	Scene(HWND hWnd);
	virtual ~Scene();

	void BeginRender();
	void EndRender(RECT* rcArea);

	int add(lElement* element)
	{
		count++;
		elements.push_back(element);
		return count - 1;
	}

	void remove(lElement* element)
	{
		elements.at(element->index) = 0;
		//elements.erase(elements.begin() + element->index);
		//std::vector<lElement*>(elements).swap(elements);
	}

	int add(Win* win)
	{
		win_count++;
		windows.push_back(win);
		return win_count - 1;
	}

	void remove(Win* win)
	{
		windows.at(win->index) = 0;
	}

public:

	Brush* pBrush;
	Font* pFont;
	HFONT hFont;
	BOOL bMyFont;
	pProc pOnpaint;

	std::vector<lElement*> elements;
	std::vector<Win*> windows;

private:

	HDC hDC, hMemoryDC;
	HBITMAP hBitmap;
	HGDIOBJ hOldBitmap;
	void* pPixels;
	int nSavedDc, nOldProc;
	int count, win_count;

public:

	HWND hWnd;
	int cx, cy;
	Gdiplus::Graphics *graphics;
	Gdiplus::Bitmap *bitmap;
	float Scale;

protected:

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void CreateContext();
};

Scene* call lSceneCreate(HWND hWnd);
int call lSceneDelete(Scene* scene);
void call lSceneApply(Scene* scene);
int call lSceneAddlayer(Scene* scene);
void call lSceneSetLong(Scene* scene, int index, void* newlong);
