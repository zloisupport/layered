#include "layered_scene.hpp"

Scene* CurrentScene = 0;

Scene::Scene(HWND hWnd)
{
	this->hWnd = hWnd;
	count = 0;
	win_count = 0;
	 
	RECT rcWindow;
	GetWindowRect(hWnd, &rcWindow);

	cx = rcWindow.right - rcWindow.left;
	cy = rcWindow.bottom - rcWindow.top;

	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetProp(hWnd, L"HANDLE", (HANDLE)this);
	nOldProc = SetWindowLong(hWnd, GWL_WNDPROC, (long)WndProc);
	SetTimer(hWnd, 2014, 30, 0);

	hDC = GetDC(hWnd);
	hMemoryDC = CreateCompatibleDC(hDC);
	CreateContext();
	nSavedDc = SaveDC(hMemoryDC);

	pBrush = lSolidBrushCreate(0xff000000);
	hFont = CreateFont(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Lucida Console");
	pFont = lFontCreate(hFont);

	pOnpaint = 0;
	bMyFont = true;	
}

std::wstring intToStr(int value);

Scene::~Scene()
{
	//remove_windows(this);

	//for each (lElement* var in elements)
	//{
	//	obj_remove(var, var->type == IMG ? obj_Image : obj_String);
	//	delete var;
	//}

	for (int i = (int) windows.size() - 1; i >= 0; i--)
	{
		if (i >= 0)
		{			
			if (windows[i])
				lDestroyWindow(windows[i]);
			windows[i] = 0;

		}		
	}

	for (int i = (int) elements.size() - 1; i >= 0; i--)
	{
		if (elements[i])
			switch (elements[i]->type)
			{
			case IMG:
				lImgDelete(elements[i]);
				break;

			case STR: 
				lStrDelete(elements[i]);
				break;
				
			case DRAW:
				remove(elements[i]);
				delete elements[i];
				break;

				default:
					break;
			}
		//if ((elements.at(i)->type == IMG) || (elements.at(i)->type == STR))
		//	delete elements.at(i);
		elements[i] = 0;
	}

	if (bMyFont)
		DeleteObject(hFont);

	//lFontDelete(pFont);
	//lBrushDelete(pBrush);

	delete graphics;
	delete bitmap;

	KillTimer(hWnd, 2014);
	DeleteObject(hBitmap);
	SelectObject(hMemoryDC, hOldBitmap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd, hDC);

	SetWindowLong(hWnd, GWL_WNDPROC, nOldProc);
}

void Scene::BeginRender()
{
	CurrentScene = this;
	graphics->Clear(Gdiplus::Color(0, 0, 0, 0));
}

//BOOL isVista();

void Scene::EndRender(RECT* rcArea)
{
	if (CurrentScene)
	{
		HBITMAP hBitmap = 0;

		bitmap->GetHBITMAP(Gdiplus::Color(255, 0, 0, 0), &hBitmap);
		hOldBitmap = SelectObject(hMemoryDC, hBitmap);

		POINT ptSrc = { 0, 0 };
		SIZE szSrc = { cx, cy };
		BLENDFUNCTION blendFunc = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		/*
		if (isVista())
		{
		UPDATELAYEREDWINDOWINFO info;
		info.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
		info.dwFlags = ULW_ALPHA;
		info.hdcDst = hDC;
		info.hdcSrc = hMemoryDC;
		info.pblend = &blendFunc;
		info.pptDst = 0;
		info.pptSrc = &ptSrc;
		info.prcDirty = 0;
		info.psize = &szSrc;

		UpdateLayeredWindowIndirect(hWnd, &info);
		}
		else*/
		UpdateLayeredWindow(hWnd, hDC, 0, &szSrc,
			hMemoryDC, &ptSrc, 0, &blendFunc, ULW_ALPHA);

		RestoreDC(hMemoryDC, nSavedDc);

		DeleteObject(hBitmap);
		hBitmap = 0;
	}
	CurrentScene = 0;
}

void Scene::CreateContext()
{
	BITMAPINFO binfo;
	memset(&binfo, 0, sizeof(binfo));

	binfo.bmiHeader.biSize = sizeof(binfo.bmiHeader);
	binfo.bmiHeader.biBitCount = 32;
	binfo.bmiHeader.biWidth = cx;
	binfo.bmiHeader.biHeight = -cy;
	binfo.bmiHeader.biSizeImage = cx * cy * 4;
	binfo.bmiHeader.biCompression = BI_RGB;
	binfo.bmiHeader.biPlanes = 1;

	hBitmap = CreateDIBSection(hMemoryDC, &binfo, DIB_RGB_COLORS, &pPixels, 0, 0);

	bitmap = new Gdiplus::Bitmap(cx, cy, cx * 4, PixelFormat32bppPARGB, (BYTE*)pPixels);
	graphics = new Gdiplus::Graphics(bitmap);

	graphics->SetPageUnit(Gdiplus::UnitPixel);
	graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
}

LRESULT CALLBACK Scene::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Scene* This = reinterpret_cast<Scene*>(GetProp(hwnd, L"HANDLE"));

	switch (message)
	{
		case WM_SIZE:
		{
			RECT wrect;
			GetWindowRect(hwnd, &wrect);

			if (((wrect.right - wrect.left) != This->cx) & ((wrect.bottom - wrect.top) != This->cy))
			{
				This->cx = wrect.right - wrect.left;
				This->cy = wrect.bottom - wrect.top;

				DeleteObject(This->hBitmap);
				This->pPixels = 0;

				delete This->graphics;
				delete This->bitmap;

				This->CreateContext();
			}
		}
			break;

		case WM_NCHITTEST:
		{
			DWORD hittestx = (DWORD)GetProp(hwnd, L"HITTESTX");
			DWORD hittesty = (DWORD)GetProp(hwnd, L"HITTESTY");
			if ((hittestx) && (hittesty))
			{
				WORD left = LOWORD(hittestx);
				WORD right = HIWORD(hittestx);
				WORD top = LOWORD(hittesty);
				WORD bottom = HIWORD(hittesty);

				RECT wrect;
				GetWindowRect(hwnd, &wrect);

				int xPos = (int)(short)LOWORD(lParam);
				int yPos = (int)(short)HIWORD(lParam);

				if ((xPos >= wrect.left + left) && (xPos <= (wrect.right - right)) && (yPos >= wrect.top + top) && (yPos <= wrect.bottom - bottom))
					return HTCAPTION;
				else
					return HTCLIENT;
			}
		}
			break;
	}

	long result = lHandleUiProc(This, hwnd, message, wParam, lParam);
	if (!result)
		return CallWindowProc((WNDPROC)This->nOldProc, hwnd, message, wParam, lParam);
	else
		return result;
}

Scene* call lSceneCreate(HWND hWnd)
{
	Scene* scene = new Scene(hWnd);
	scene->obj_type = obj_Scene;
	obj_pushback(scene);
	return scene;
}

int call lSceneDelete(Scene* scene)
{
	obj_remove(scene);
	delete scene;
	return 0;
}

int call lSceneAddlayer(Scene* scene)
{
	lElement* draw = new lElement;
	draw->type = DRAW;
	draw->index = scene->elements.size();
	return scene->add(draw);
}

void call lSceneApply(Scene* scene)
{
	scene->BeginRender();

	for each (lElement* elem in scene->elements)
	{

	//for (int i = 0; i != scene->elements.size(); i++)
	//{
		if (elem)
			switch (elem->type)
			{
			case IMG:
				{
					if (elem->bVisible)
					{
						BOOL bDrawResult = FALSE;

							if (elem->bPart)
								bDrawResult = lDrawImageRectRect(elem->pTexture,
									elem->nLeft, elem->nTop, elem->nWidth, elem->nHeight,
									elem->nLeft_part, elem->nTop_part, elem->nWidth_part, elem->nHeight_part);
							else
								bDrawResult = lDrawImageRect(elem->pTexture,
									elem->nLeft, elem->nTop, elem->nWidth, elem->nHeight);

					}

				}
				break;

			case STR:
				{
					if (elem->bVisible)
					{
						int a = lGetStringAlignment();
						int la = lGetStringLineAlignment();

						lStringAlignment(elem->nAlignment);
						lStringLineAlignment(elem->nLineAlignment);

						BOOL bDrawResult = FALSE;
					
						if (elem->bAutoSize)
							bDrawResult = lDrawString(elem->sCaption.c_str(), elem->nLeft, elem->nTop, elem->nWidth, elem->nHeight,
								elem->pFont, elem->pBrush);
						else
							bDrawResult = lDrawStringRect(elem->sCaption.c_str(), elem->nLeft, elem->nTop, elem->nWidth, elem->nHeight,
								elem->pFont, elem->pBrush);

						lStringAlignment(a);
						lStringLineAlignment(la);
					}
				}
				break;

			case DRAW:
				{
					if (scene->pOnpaint)
						scene->pOnpaint(elem->index);
				}
				break;

			default: 
				break;
			}

	}

	scene->EndRender(0);
}

enum { 
	SSL_PAINTEVENT = 1, 
	SSL_BRUSH, 
	SSL_FONT, 
	SSL_HITTEST_TITLEX, 
	SSL_HITTEST_TITLEY
};

void call lSceneSetLong(Scene* scene, int index, void* newlong)
{
	switch (index)
	{
		case SSL_PAINTEVENT:
		{
			if (newlong)
				scene->pOnpaint = static_cast<Scene::pProc>(newlong);
		}
		break;

		case SSL_BRUSH:
		{
			if ((int)newlong == -1)
				return ;

			//lBrushDelete(scene->pBrush);
			scene->pBrush = reinterpret_cast<Brush*> (newlong);
			//lBrushCopy(reinterpret_cast<Brush*>(newlong));
		}
		break;

		case SSL_FONT:
		{
			if ((int)newlong == -1)
				return;

			if (scene->bMyFont)
				DeleteObject(scene->hFont);		
			//lFontDelete(scene->pFont);

			scene->bMyFont = FALSE;
			scene->pFont = reinterpret_cast<Font*>(newlong);
			//lFontCopy(reinterpret_cast<Font*>(newlong));
		}
		break;

		case SSL_HITTEST_TITLEX:
		{
			if (newlong)
				SetProp(scene->hWnd, L"HITTESTX", (HANDLE)newlong);
		}
		break;

		case SSL_HITTEST_TITLEY:
		{
			if (newlong)
				SetProp(scene->hWnd, L"HITTESTY", (HANDLE)newlong);
		}
		break;

	default:
		break;
	}
}
