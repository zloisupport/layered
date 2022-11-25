#pragma once
#include "precomp.h"
#include "layered_scene.hpp"
#include "layered_element.hpp"

class Scene;
class Win;

Win* call lCreateWindow(Scene* scene, DWORD style,
	int nLeft, int nTop, int nWidth, int nHeight, const wchar_t* sCaption, Texture* pTexture);
int call lDestroyWindow(Win* win);
void call lSetWindowLong(Win* win, int index, void* newlong);
void* call lGetWindowLong(Win* win, int index);
void call lSetWindowText(Win* win, const wchar_t* sCaption);
void call lGetWindowText(Win* win, wchar_t* sCaption);
void call lSetWindowPos(Win* win, int nLeft, int nTop);
void call lGetWindowPos(Win* win, int& nLeft, int& nTop, int& nWidth, int& nHeight);

LRESULT lHandleUiProc(Scene* scene, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
