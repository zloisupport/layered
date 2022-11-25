#include "layered_ui.hpp"

enum {
	LWS_BTNPUSH = 0x1, 
	LWS_BTNCHECK = 0x2, 
	LWS_PROGRESS = 0x4, 
	LWS_EDIT = 0x8,
};

enum { 
	L_FALSE = 0,
	L_TRUE = 1,

	L_POSX = -1,
	L_POSY = -2,
	L_POSCX = -3,
	L_POSCY = -4,

	L_EVENTCLICK = 1,
	L_EVENTONMOUSELEAVE = 2,
	L_EVENTONMOUSEENTER = 3,
	L_EVENTCHANGE = 13,

	L_BRUSHNORMAL = 4,
	L_BRUSHFOCUSED = 5,
	L_BRUSHDISABLED = 6,

	L_ENABLE = 7,
	L_VISIBILITY = 8,
	L_CHECKED = 9,

	L_ALIGNMENT = 10, 
	L_LINEALIGNMENT = 11,

	L_FONT = 12,

	L_PROGRESSVALUE = 1,
	L_PROGRESSMAX = 2,
	L_PROGRESSSTATE = 3
}; 

enum {
	L_NORMAL = 1,
	L_PAUSE = 2,
	L_ERROR = 3
};
std::wstring intToStr(int value);

LRESULT call lChildProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Win* This = reinterpret_cast<Win*>(GetWindowLong(hwnd, GWL_USERDATA));

	if (uMsg == WM_MOUSEMOVE)
	{
		POINT ptCursor;
		GetCursorPos(&ptCursor);
		if (WindowFromPoint(ptCursor) == hwnd)
			SetProp(This->scene->hWnd, L"CURRENT", (HANDLE)This);
		else
			SetProp(This->scene->hWnd, L"CURRENT", (HANDLE)0);
	}
	else if (uMsg == WM_MOUSELEAVE)
		SetProp(This->scene->hWnd, L"CURRENT", (HANDLE)0);

	return CallWindowProc((WNDPROC)This->nOldCProc, hwnd, uMsg, wParam, lParam);
}

Gdiplus::RectF mesuare_size(Win* win)
{
	Gdiplus::StringFormat format((Gdiplus::StringFormat*) Gdiplus::StringFormat::GenericTypographic());
	format.SetTrimming(Gdiplus::StringTrimmingCharacter);

	Gdiplus::RectF bounds;

	win->scene->graphics->MeasureString(win->Caption.c_str(), win->Caption.size() + 1, win->scene->pFont->gFont, Gdiplus::RectF(0, 0, 0, 0),
		&format, &bounds, 0, 0);

	return bounds;
};

std::wstring const sEditClass = L"lEditClass";

LRESULT WINAPI lChildEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Win* This = reinterpret_cast<Win*>(GetProp(hwnd, L"This"));

	switch (uMsg)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_CLICKACTIVE)
		{		
			This->bStartUpdate = true;
			if (This->Image)
				lImgSetVisiblePart(This->Image, 0, This->Texture->gTexture->GetHeight() / 2, This->Texture->gTexture->GetWidth(), This->Texture->gTexture->GetHeight() / 2);
			lSceneApply(This->scene);

			if (This->bFocused != 0)
			{
				Gdiplus::Color gpColor;

				if (((int)This->bFocused == -1) || ((int)This->bFocused == 0))
					This->scene->pBrush->gBrush->GetType() == Gdiplus::BrushTypeSolidColor ? ((Gdiplus::SolidBrush*)This->scene->pBrush->gBrush)->GetColor(&gpColor) : gpColor.MakeARGB(255, 0, 0, 0);
				else
					This->bFocused->gBrush->GetType() == Gdiplus::BrushTypeSolidColor ? ((Gdiplus::SolidBrush*)This->bFocused->gBrush)->GetColor(&gpColor) : gpColor.MakeARGB(255, 0, 0, 0);

				DWORD Color = gpColor.ToCOLORREF();
				SetProp(This->hChild, L"tColor", (HANDLE)Color);
				SetProp(This->hChild2, L"tColor", (HANDLE)Color);

				InvalidateRect(This->hChild, 0, TRUE);
				InvalidateRect(This->hChild2, 0, TRUE);
			}
		}
		else if (LOWORD(wParam) == WA_INACTIVE)
		{
			if (This)
				if ((This->bStartUpdate) && (This->Image) && (This->scene))
				{
					lImgSetVisiblePart(This->Image, 0, 0, This->Texture->gTexture->GetWidth(), This->Texture->gTexture->GetHeight() / 2);
					lSceneApply(This->scene);

					Gdiplus::Color gpColor;
					if (IsWindowEnabled(hwnd))
					{
						if (((int)This->bNormal == -1) || ((int)This->bNormal == 0))
							This->scene->pBrush->gBrush->GetType() == Gdiplus::BrushTypeSolidColor ? ((Gdiplus::SolidBrush*)This->scene->pBrush->gBrush)->GetColor(&gpColor) : gpColor.MakeARGB(255, 0, 0, 0);
						else
							This->bNormal->gBrush->GetType() == Gdiplus::BrushTypeSolidColor ? ((Gdiplus::SolidBrush*)This->bNormal->gBrush)->GetColor(&gpColor) : gpColor.MakeARGB(255, 0, 0, 0);
					}
					else
					{
						if (((int)This->bDisabled == -1) || ((int)This->bDisabled == 0))
							This->scene->pBrush->gBrush->GetType() == Gdiplus::BrushTypeSolidColor ? ((Gdiplus::SolidBrush*)This->scene->pBrush->gBrush)->GetColor(&gpColor) : gpColor.MakeARGB(255, 0, 0, 0);
						else
							This->bDisabled->gBrush->GetType() == Gdiplus::BrushTypeSolidColor ? ((Gdiplus::SolidBrush*)This->bDisabled->gBrush)->GetColor(&gpColor) : gpColor.MakeARGB(255, 0, 0, 0);
					}
					DWORD Color = gpColor.ToCOLORREF();
					SetProp(This->hChild, L"tColor", (HANDLE)Color);
					SetProp(This->hChild2, L"tColor", (HANDLE)Color);

					InvalidateRect(This->hChild, 0, TRUE);
					InvalidateRect(This->hChild2, 0, TRUE);
				}	
		}
	}
		break;

	case WM_NCCALCSIZE:
		//if (wParam == TRUE)
		//	return 0;
		break;

	case WM_ERASEBKGND:
		//return 1;
		break;

	case WM_CTLCOLOREDIT:
		{		
			DWORD bkColor = (DWORD)GetProp(hwnd, L"BKColor");
			DWORD tColor = (DWORD)GetProp(hwnd, L"tColor");
			
			SetBkColor((HDC)wParam, bkColor);
			SetTextColor((HDC)wParam, tColor);
			return (LRESULT)GetProp(hwnd, L"bkBrush");;
		}
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

LRESULT WINAPI lEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Win* This = reinterpret_cast<Win*>(GetProp(hwnd, L"This"));

	switch (uMsg)
	{
		case WM_KEYUP:
		{
			wchar_t* caption = new wchar_t[512];
			GetWindowText(hwnd, caption, 512);
			SetWindowText(This->hWnd != hwnd ? hwnd : This->hWnd2, caption);
			delete caption;

			if (This->onChange)
				This->onChange((int)This);
		}
	    	break;

		case WM_RBUTTONDOWN:
		{
			return 0;
		}
	    	break;
	}

	return CallWindowProc((WNDPROC)This->nEOldProc, hwnd, uMsg, wParam, lParam);
}

static void reg_class(HWND hWnd)
{
	WNDCLASSEX wincl;

	wincl.cbSize = sizeof (WNDCLASSEX);
	wincl.hInstance = (HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE);
	wincl.lpszClassName = sEditClass.c_str();
	wincl.lpfnWndProc = lChildEditProc;
	wincl.style = CS_DBLCLKS;                 
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;               
	wincl.cbClsExtra = 0;                   
	wincl.cbWndExtra = 0;                   
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

	if (!RegisterClassEx(&wincl))
		return ;
}

void unreg_class()
{
	UnregisterClass(sEditClass.c_str(), 
		GetModuleHandle(0));
}

Win* call lCreateWindow(Scene* scene, DWORD style, 
	int nLeft, int nTop, int nWidth, int nHeight, const wchar_t* sCaption, Texture* pTexture)
{
	Win* win = new Win;
	win->scene = scene;
	win->index = scene->windows.size();
	win->Caption = sCaption ? sCaption : L"\0";
	win->Caption += L"\0";
	win->Texture = pTexture;
	win->nLeft = nLeft;
	win->nTop = nTop;
	win->nHeight = nHeight;
	win->nWidth = nWidth;
	win->style = style;

	auto create_button = [] (Win* win, int mod, bool cb) -> bool
	{
		win->hWnd = CreateWindow(L"BUTTON", win->Caption.c_str(), WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
			win->nLeft, win->nTop, win->nWidth, win->nHeight, 
			win->scene->hWnd, 0, (HINSTANCE)GetWindowLong(win->scene->hWnd, GWL_HINSTANCE), 0);
		SetWindowLong(win->hWnd, GWL_USERDATA, (long) win);
		win->nOldCProc = SetWindowLong(win->hWnd, GWL_WNDPROC, (long)lChildProc);

		win->Image = lImgLoad(win->scene, win->Texture, win->nLeft, win->nTop, win->nWidth, win->nHeight);
		lImgSetVisiblePart(win->Image, 0, 0, win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / mod);

		win->Text = cb ? lStrLoad(win->scene, win->Caption.c_str(), win->nLeft + win->nWidth + 5, win->nTop, (int) mesuare_size(win).Width + 10, win->nHeight, FALSE) :
			lStrLoad(win->scene, win->Caption.c_str(), win->nLeft + 5, win->nTop, win->nWidth - 10, win->nHeight, FALSE);

		if (!cb) lStrSetAlignment(win->Text, 1);
		lStrSetLineAlignment(win->Text, 1);

		std::wstring prop = L"l" + intToStr((int)win);
		SetProp(win->scene->hWnd, prop.c_str(), (HANDLE)win);

		return true;
	};	

	if ((LWS_BTNPUSH & style ) == LWS_BTNPUSH)
	{
		if (!create_button(win, 4, false))
			goto go_break;
	}
	else if ((LWS_BTNCHECK & style) == LWS_BTNCHECK)
	{
		if (!create_button(win, 8, true))
			goto go_break;
	}
	else if ((LWS_PROGRESS & style) == LWS_PROGRESS)
	{
		win->Image = lImgLoad(win->scene, win->Texture, win->nLeft, win->nTop, win->nWidth, win->nHeight);
		lImgSetVisiblePart(win->Image, 0, 0, win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 4);

		win->PB = lImgLoad(win->scene, win->Texture, win->nLeft, win->nTop, win->nWidth, win->nHeight);
		lImgSetPosition(win->PB, win->nLeft, win->nTop, win->nWidth * (100 * win->nPBValue / win->nPBMax) / 100, win->nHeight);
		lImgSetVisiblePart(win->PB, 0,
			win->Texture->gTexture->GetHeight() / 4,
			win->Texture->gTexture->GetWidth(),
			win->Texture->gTexture->GetHeight() / 4);
		
		std::wstring prop = L"l" + intToStr(reinterpret_cast<int>(win));
		SetProp(win->scene->hWnd, prop.c_str(), (HANDLE)win);
	}
	else if ((LWS_EDIT & style) == LWS_EDIT)
	{
		reg_class(win->scene->hWnd);
		win->hChild = CreateWindow(sEditClass.c_str(), L"", WS_POPUPWINDOW | WS_VISIBLE,
			win->nLeft, win->nTop, win->nWidth - 10, int(mesuare_size(win).Height), win->scene->hWnd,
			0, (HINSTANCE)GetWindowLong(win->scene->hWnd, GWL_HINSTANCE), 0);

		win->hChild2 = CreateWindow(sEditClass.c_str(), L"", WS_POPUPWINDOW | WS_VISIBLE,
			win->nLeft, win->nTop, win->nWidth - 10, int(mesuare_size(win).Height), win->scene->hWnd,
			0, (HINSTANCE)GetWindowLong(win->scene->hWnd, GWL_HINSTANCE), 0);

		SetProp(win->hChild, L"This", (HANDLE)win);
		SetProp(win->hChild2, L"This", (HANDLE)win);

		win->Image = lImgLoad(win->scene, win->Texture, win->nLeft, win->nTop, win->nWidth, win->nHeight);
		lImgSetVisiblePart(win->Image, 0, 0, win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 2);

		LOGFONT logFont;
		logFont.lfQuality = DEFAULT_QUALITY;
		win->scene->pFont->gFont->GetLogFontW(win->scene->graphics, &logFont);
		win->hFont = CreateFontIndirectW(&logFont);

		win->hWnd = CreateWindow(L"EDIT", win->Caption.c_str(), WS_CHILD | WS_VISIBLE, 0, 0, win->nWidth, int(mesuare_size(win).Height), win->hChild, 0, 0, 0);
		SendMessage(win->hWnd, WM_SETFONT, (WPARAM)win->hFont, MAKELPARAM(TRUE, 0));
		win->hWnd2 = CreateWindow(L"EDIT", win->Caption.c_str(), WS_CHILD | WS_VISIBLE, 0, 0, win->nWidth, int(mesuare_size(win).Height), win->hChild2, 0, 0, 0);
		SendMessage(win->hWnd2, WM_SETFONT, (WPARAM)win->hFont, MAKELPARAM(TRUE, 0));

		SetProp(win->hWnd, L"This", (HANDLE) win);
		SetProp(win->hWnd2, L"This", (HANDLE) win);

		win->nEOldProc = SetWindowLong(win->hWnd, GWL_WNDPROC, (long)lEditProc);

		lSceneApply(win->scene);

		Gdiplus::Color gpColor;
		win->scene->bitmap->GetPixel(win->nLeft + win->nWidth / 2, win->nTop + win->nHeight / 2, &gpColor);
		DWORD Color = gpColor.ToCOLORREF();
		SetProp(win->hChild, L"BKColor", (HANDLE)Color);
		SetProp(win->hChild2, L"BKColor", (HANDLE)Color);
		HBRUSH hBrush = CreateSolidBrush(Color);
		SetProp(win->hChild, L"bkBrush", (HANDLE)hBrush);
		SetProp(win->hChild2, L"bkBrush", (HANDLE)hBrush);

		SetWindowLong(win->hChild, GWL_EXSTYLE,
			GetWindowLong(win->hChild, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(win->hChild, 0, 70, LWA_ALPHA);

		SetWindowLong(win->hChild2, GWL_EXSTYLE,
			GetWindowLong(win->hChild2, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(win->hChild2, Color, 0, LWA_COLORKEY);

		win->scene->pBrush->gBrush->GetType() == Gdiplus::BrushTypeSolidColor ? ((Gdiplus::SolidBrush*)win->scene->pBrush->gBrush)->GetColor(&gpColor) : gpColor.MakeARGB(255, 0, 0, 0);
		Color = gpColor.ToCOLORREF();
		SetProp(win->hChild, L"tColor", (HANDLE)Color);
		SetProp(win->hChild2, L"tColor", (HANDLE)Color);	

		RECT rcWindow;
		GetWindowRect(win->scene->hWnd, &rcWindow);

		win->nMod = (win->nHeight - (int) mesuare_size(win).Height) / 2;
		SetWindowPos(win->hChild2, 0, rcWindow.left + win->nLeft + 5, rcWindow.top + win->nTop + win->nMod, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
		SetWindowPos(win->hChild, HWND_TOP, rcWindow.left + win->nLeft + 5, rcWindow.top + win->nTop + win->nMod, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);

		std::wstring prop = L"l" + intToStr((int)win);
		SetProp(win->scene->hWnd, prop.c_str(), (HANDLE)win);
	}
	else
	{
go_break:
		delete win;
		return 0;
	}

	scene->add(win);
	return win;
}

int call lDestroyWindow(Win* win)
{
	Scene* scene = win->scene;
	std::wstring prop = L"l" + intToStr(reinterpret_cast<int>(win));
	RemoveProp(scene->hWnd, prop.c_str());

	if (win->Text) lStrDelete(win->Text);
	if (win->PB) lImgDelete(win->PB);
	if (win->Image) lImgDelete(win->Image);

	scene->remove(win);
	delete win;
	return 0;
}

auto update_prevbtn = [](Win*prev)->void
{
	prev->bEnter = FALSE;

	if ((prev->Text) && (!prev->bEnter))
	{
		lStrSetBrush(prev->Text, prev->bNormal == 0 ? (Brush*)(-1) : prev->bNormal);
		lSceneApply(prev->scene);
	}

	int l, t, w, h;
	lImgGetVisiblePart(prev->Image, l, t, w, h);

	if (prev->style == LWS_BTNPUSH)
	{
		if (t != 0)
		{
			lImgSetVisiblePart(prev->Image, 0, 0, prev->Texture->gTexture->GetWidth(), prev->Texture->gTexture->GetHeight() / 4);
			lSceneApply(prev->scene);
			if (prev->onLeave)
				prev->onLeave((int)prev);
		}
	}
	else
	{
		if ((t != 0) || (t != prev->nHeight * 4))
		{
			lImgSetVisiblePart(prev->Image, 0, prev->bChecked ? 0 : prev->Texture->gTexture->GetHeight() / 8 * 4, prev->Texture->gTexture->GetWidth(), prev->Texture->gTexture->GetHeight() / 8);
			lSceneApply(prev->scene);
			if (prev->onLeave)
				prev->onLeave((int)prev);
		}
	}
};

void call lSetWindowLong(Win* win, int index, void* newlong)
{
	switch (index)
	{
		case L_EVENTCLICK:
			if (win->style == LWS_PROGRESS)
			{
				int old = win->nPBValue;
				lSetWindowLong(win, L_PROGRESSSTATE, (void*)L_NORMAL);
				win->nPBValue = reinterpret_cast<int> (newlong);
				if ((win->nPBValue >= 0) && (win->nPBValue <= win->nPBMax))
				{
					int res = win->nWidth * (100 * win->nPBValue / win->nPBMax) / 100;
					lImgSetPosition(win->PB, win->nLeft, win->nTop, res > win->nWidth ? win->nWidth : res, win->nHeight);

					lImgSetVisiblePart(win->PB, 0,
						win->Texture->gTexture->GetHeight() / 4,
						win->Texture->gTexture->GetWidth(),
						win->Texture->gTexture->GetHeight() / 4);
				}
				else
					win->nPBValue = old;
			}
			else
				win->onClick = reinterpret_cast<pEventProc> (newlong);
			break;

		case L_EVENTONMOUSEENTER:	
			if (win->style == LWS_PROGRESS)
			{					
				win->nPBState = reinterpret_cast<int> (newlong);

				switch (win->nPBState)
				{
				case L_NORMAL:
					lImgSetVisiblePart(win->PB, 0,
						win->Texture->gTexture->GetHeight() / 4,
						win->Texture->gTexture->GetWidth(),
						win->Texture->gTexture->GetHeight() / 4);
					break;

				case L_PAUSE:
					lImgSetVisiblePart(win->PB, 0,
						win->Texture->gTexture->GetHeight() / 4 * 2,
						win->Texture->gTexture->GetWidth(),
						win->Texture->gTexture->GetHeight() / 4);
					break;

				case L_ERROR:
					lImgSetVisiblePart(win->PB, 0,
						win->Texture->gTexture->GetHeight() / 4 * 3,
						win->Texture->gTexture->GetWidth(),
						win->Texture->gTexture->GetHeight() / 4);
					break;

				default:
					break;
				}			
			}
			else
				win->onEnter = reinterpret_cast<pEventProc> (newlong);
			break;

		case L_EVENTCHANGE:
			{
				win->onChange = reinterpret_cast<pEventProc> (newlong);
			}
			break;

		case L_EVENTONMOUSELEAVE:
			if (win->style == LWS_PROGRESS)
				win->nPBMax = reinterpret_cast<int> (newlong);
			else
				win->onLeave = reinterpret_cast<pEventProc> (newlong);
			break;

		case L_BRUSHNORMAL:
		{
			win->bNormal = reinterpret_cast<Brush*> (newlong);
			if (win->Text)
				lStrSetBrush(win->Text, win->bNormal == 0 ? (Brush*)(-1) : win->bNormal);		
		}
			break;

		case L_BRUSHFOCUSED:
		{
			win->bFocused = reinterpret_cast<Brush*> (newlong);
		}
			break;

		case L_BRUSHDISABLED:
		{
			win->bDisabled = reinterpret_cast<Brush*> (newlong);
		}
			break;

		case L_ENABLE:
		{
			win->bEnabled = reinterpret_cast<BOOL>(newlong);

			if (win->style == LWS_EDIT)
			{
				if (win->hChild)
					EnableWindow(win->hChild, win->bEnabled);
				if (win->hChild2)
					EnableWindow(win->hChild2, win->bEnabled);

				//lImgSetVisiblePart(win->Image, 0, 0, win->nWidth, win->nHeight);
				lImgSetVisiblePart(win->Image, 0, 0, win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 2);
				lSceneApply(win->scene);

				if (win->bEnabled)
				{
					Gdiplus::Color gpColor;
					if (((int)win->bNormal == -1) || ((int)win->bNormal == 0))
						win->scene->pBrush->gBrush->GetType() == Gdiplus::BrushTypeSolidColor ? ((Gdiplus::SolidBrush*)win->scene->pBrush->gBrush)->GetColor(&gpColor) : gpColor.MakeARGB(255, 0, 0, 0);
					else
						win->bNormal->gBrush->GetType() == Gdiplus::BrushTypeSolidColor ? ((Gdiplus::SolidBrush*)win->bNormal->gBrush)->GetColor(&gpColor) : gpColor.MakeARGB(255, 0, 0, 0);
					DWORD Color = gpColor.ToCOLORREF();
					SetProp(win->hChild, L"tColor", (HANDLE)Color);
					SetProp(win->hChild2, L"tColor", (HANDLE)Color);					
				}
				else
				{
					Gdiplus::Color gpColor;
					if (((int)win->bDisabled == -1) || ((int)win->bDisabled == 0))
						win->scene->pBrush->gBrush->GetType() == Gdiplus::BrushTypeSolidColor ? ((Gdiplus::SolidBrush*)win->scene->pBrush->gBrush)->GetColor(&gpColor) : gpColor.MakeARGB(255, 0, 0, 0);
					else
						win->bDisabled->gBrush->GetType() == Gdiplus::BrushTypeSolidColor ? ((Gdiplus::SolidBrush*)win->bDisabled->gBrush)->GetColor(&gpColor) : gpColor.MakeARGB(255, 0, 0, 0);
					DWORD Color = gpColor.ToCOLORREF();
					SetProp(win->hChild, L"tColor", (HANDLE)Color);
					SetProp(win->hChild2, L"tColor", (HANDLE)Color);
				}

				InvalidateRect(win->hChild, 0, TRUE);
				InvalidateRect(win->hChild2, 0, TRUE);

			}
			else
			{	
				if (!win->bEnabled)
				{
					if (win->Text)
						lStrSetBrush(win->Text, win->bDisabled == 0 ? (Brush*)(-1) : win->bDisabled);
					win->style == LWS_BTNPUSH ? lImgSetVisiblePart(win->Image, 0, win->Texture->gTexture->GetHeight() / 4 * 3, win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 4) :
						lImgSetVisiblePart(win->Image, 0, win->bChecked ? win->Texture->gTexture->GetHeight() / 8 * 3 : win->Texture->gTexture->GetHeight() / 8 * 7, win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 8);
					lSceneApply(win->scene);
				}
				else
				{
					if (win->Text)
						lStrSetBrush(win->Text, win->bNormal == 0 ? (Brush*)(-1) : win->bNormal);
					win->style == LWS_BTNPUSH ? lImgSetVisiblePart(win->Image, 0, 0, win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 4) :
						lImgSetVisiblePart(win->Image, 0, win->bChecked ? 0 : win->Texture->gTexture->GetHeight() / 8 * 4, win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 8);
					lSceneApply(win->scene);
				}
			}

		}
			break;

		case L_VISIBILITY:
		{
			if (reinterpret_cast<BOOL>(newlong) == win->bVisable)
				return;

			win->bVisable = reinterpret_cast<BOOL>(newlong);	

			if ((win->style == LWS_BTNPUSH) || (win->style == LWS_BTNCHECK))
				update_prevbtn(win);
			if (win->Text)
				lStrSetVisibility(win->Text, win->bVisable);	
			lImgSetVisibility(win->Image, win->bVisable);

			if (win->PB)
				lImgSetVisibility(win->PB, win->bVisable);
			if ((win->hWnd) && (win->style != LWS_EDIT))
				ShowWindow(win->hWnd, win->bVisable ? SW_SHOWNORMAL : SW_HIDE);
			if (win->hChild)
				ShowWindow(win->hChild, win->bVisable ? SW_SHOWNORMAL : SW_HIDE);
			if (win->hChild2)
				ShowWindow(win->hChild2, win->bVisable ? SW_SHOWNORMAL : SW_HIDE);
		}
			break;

		case L_CHECKED:
		{
			if (win->style == LWS_BTNCHECK)
			{
				win->bChecked = reinterpret_cast<BOOL>(newlong);
				win->bEnabled ? lImgSetVisiblePart(win->Image, 0, win->bChecked ? 0 : win->Texture->gTexture->GetHeight() / 8 * 4, 
					win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 8) :
					lImgSetVisiblePart(win->Image, 0, win->bChecked ? win->Texture->gTexture->GetHeight() / 8 * 3 : win->Texture->gTexture->GetHeight() / 8 * 7, 
					win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 8);
				//lSceneApply(win->scene);
			}
			//if ((win->style == LWS_BTNCHECK) || (win->style == LWS_BTNPUSH))
				//if (win->onClick)
					//win->onClick((int)win);
		}
			break;

		case L_ALIGNMENT:
		{
			if (win->style == LWS_BTNPUSH)
				lStrSetAlignment(win->Text, (int)newlong);
		}
			break;

		case L_LINEALIGNMENT:
		{
			if (win->style == LWS_BTNPUSH)
				lStrSetLineAlignment(win->Text, (int)newlong);
		}
			break;

		case L_FONT:
		{
			if ((win->style == LWS_BTNCHECK) || (win->style == LWS_BTNPUSH) || (win->style == LWS_EDIT))
			{
				Font* font = reinterpret_cast<Font*>(newlong);

				if ((int)font != -1)
						return;

				if (win->style != LWS_EDIT)
				{
					lStrSetFont(win->Text, font);
					if (win->style != LWS_BTNPUSH)
					{
						int l, t, w, h;
						lStrGetPosition(win->Text, l, t, w, h);
						lStrSetPosition(win->Text, l, t, (int)mesuare_size(win).Width, (int)mesuare_size(win).Height);
					}				
					if (win->style == LWS_BTNCHECK)
						lStrSetPosition(win->Text, win->nLeft + win->nWidth + 5, win->nTop, (int)mesuare_size(win).Width, win->nHeight);
				}						
				else
				{
					if (win->hFont) DeleteObject(win->hFont);
					LOGFONT logFont;
					(int)font == -1 ? win->scene->pFont->gFont->GetLogFontW(win->scene->graphics, &logFont) : 
						font->gFont->GetLogFontW(win->scene->graphics, &logFont);
					win->hFont = CreateFontIndirectW(&logFont);
					SendMessage(win->hWnd, WM_SETFONT, (WPARAM)win->hFont, MAKELPARAM(TRUE, 0));
					SendMessage(win->hWnd2, WM_SETFONT, (WPARAM)win->hFont, MAKELPARAM(TRUE, 0));

					win->nMod = (win->nHeight - (int) mesuare_size(win).Height) / 2;

					RECT rcWindow;
					GetWindowRect(win->scene->hWnd, &rcWindow);

					SetWindowPos(win->hChild2, 0, rcWindow.left + win->nLeft + 5, rcWindow.top + win->nTop + win->nMod, win->nWidth - 10, (int)mesuare_size(win).Height, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
					SetWindowPos(win->hChild, 0, rcWindow.left + win->nLeft + 5, rcWindow.top + win->nTop + win->nMod, win->nWidth - 10, (int)mesuare_size(win).Height, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

					SetWindowPos(win->hWnd, 0, 0, 0, win->nWidth - 10, (int)mesuare_size(win).Height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					SetWindowPos(win->hWnd2, 0, 0, 0, win->nWidth - 10, (int)mesuare_size(win).Height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
				}
			}
		}
			break;

		default:
			break;
	}
}

void* call lGetWindowLong(Win* win, int index)
{
	switch (index)
	{
		case L_ENABLE:
			return (void*)win->bEnabled;
			break;

		case L_VISIBILITY:
			return (void*)win->bVisable;
			break;

		case L_CHECKED:
			return (void*)win->bChecked;
			break;

		case L_PROGRESSVALUE:
			return (void*)win->nPBValue;
			break;

		case L_PROGRESSSTATE:
			return (void*)win->nPBState;
			break;

		case L_PROGRESSMAX:
			return (void*)win->nPBMax;
			break;

		case L_POSX:
			return (void*)win->nLeft;
			break;

		case L_POSY:
			return (void*)win->nTop;
			break;

		case L_POSCX:
			return (void*)win->nWidth;
			break;

		case L_POSCY:
			return (void*)win->nHeight;
			break;

		default:
			break;
	}
	return 0;
}

void call lSetWindowText(Win* win, const wchar_t* sCaption)
{
	switch (win->style)
	{
	case LWS_BTNCHECK:
	case LWS_BTNPUSH:
	case LWS_EDIT:
		win->Caption = sCaption ? sCaption : L"\0";
		win->Caption += L"\0";
		if (win->Text)
		{
			lStrSetCaption(win->Text, sCaption);
			if (win->style == LWS_BTNCHECK)
			{
				int l, t, w, h;
				lStrGetPosition(win->Text, l, t, w, h);
				lStrSetPosition(win->Text, l, t, (int)mesuare_size(win).Width, h);
			}					
		}
		SetWindowText(win->hWnd, win->Caption.c_str());
		if (win->hWnd2) 
			SetWindowText(win->hWnd2, win->Caption.c_str());
		if (win->onChange)
			win->onChange((int)win);
		lSceneApply(win->scene);
		break;
	default:
		return ;
		break;
	}
}

void call lGetWindowText(Win* win, wchar_t* sCaption)
{
	switch (win->style)
	{
	case LWS_BTNCHECK:
	case LWS_BTNPUSH:
	case LWS_EDIT:
		GetWindowText(win->hWnd, sCaption, 512);
		break;
	default:
		sCaption = L"\0";
		break;
	}
}

int call lGetWindowTextLength(Win* win)
{
	return win->Caption.size();
}

void call lSetWindowPos(Win* win, int nLeft, int nTop)
{
	win->nLeft = nLeft;
	win->nTop = nTop;

	switch (win->style)
	{
		case LWS_BTNPUSH:
		{
			lImgSetPosition(win->Image, nLeft, nTop, win->nWidth, win->nHeight);
			lStrSetPosition(win->Text, nLeft + 5, nTop, win->nWidth - 10, win->nHeight);
			SetWindowPos(win->hWnd, 0, nLeft, nTop, win->nWidth, win->nHeight, SWP_NOZORDER);
		}
		break;

		case LWS_BTNCHECK:
		{
			lImgSetPosition(win->Image, nLeft, nTop, win->nWidth, win->nHeight);
			lStrSetPosition(win->Text, nLeft + win->nWidth + 5, nTop, win->nWidth - 10, win->nHeight);
			SetWindowPos(win->hWnd, 0, nLeft, nTop, win->nWidth, win->nHeight, SWP_NOZORDER);
		}
		break;

		case LWS_PROGRESS:
		{
			lImgSetPosition(win->Image, nLeft, nTop, win->nWidth, win->nHeight);
			int l, t, w, h;
			lImgGetPosition(win->PB, l, t, w, h);
			lImgSetPosition(win->PB, nLeft, nTop, w, win->nHeight);
		}
		break;

		case LWS_EDIT:
		{
			RECT rcWindow;
			GetWindowRect(win->scene->hWnd, &rcWindow);
			lImgSetPosition(win->Image, nLeft, nTop, win->nWidth, win->nHeight);
			SetWindowPos(win->hChild, 0, rcWindow.left + win->nLeft + 5, rcWindow.top + win->nTop + 5, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
			SetWindowPos(win->hChild2, 0, rcWindow.left + win->nLeft + 5, rcWindow.top + win->nTop + 5, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
		}
		break;

		default:
		break;
	}		
}

void call lGetWindowPos(Win* win, int& nLeft, int& nTop, int& nWidth, int& nHeight)
{
	nLeft = win->nLeft; nTop = win->nTop;
	if (win->style == LWS_BTNCHECK)
	{
		int l, t, w, h;
		lStrGetPosition(win->Text, l, t, w, h);
		nWidth = win->nWidth + 5 + w;
		nHeight = win->nHeight;
	}
	else
	{
		nWidth = win->nWidth; 
		nHeight = win->nHeight;
	}
}

LRESULT lHandleUiProc(Scene* scene, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_MOUSEMOVE:
		{
			//POINT ptCursor;
			//GetCursorPos(&ptCursor);
			//if (WindowFromPoint(ptCursor) == hwnd)
			//{
				Win* win = reinterpret_cast<Win*>(GetProp(hwnd, L"CURRENT"));
				if (win)
					SetProp(hwnd, L"CURRENT", (HANDLE)0);
			//}
			//else
			//	SetProp(hwnd, L"CURRENT", (HANDLE)GetWindowLong(WindowFromPoint(ptCursor), GWL_USERDATA));
		}
		break;

		case WM_MOVE:
		{
			int xPos = (int)(short)LOWORD(lParam);
			int yPos = (int)(short)HIWORD(lParam);

			for each (Win* var in scene->windows)
			{
				if (var)
					if (var->style == LWS_EDIT)
					{
						SetWindowPos(var->hChild, 0, xPos + var->nLeft + 5, yPos + var->nTop + var->nMod, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
						SetWindowPos(var->hChild2, 0, xPos + var->nLeft + 5, yPos + var->nTop + var->nMod, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
					}
			}
		}
			break;

	case WM_TIMER:
		if (wParam == 2014)
		{
			if (!IsWindowEnabled(hwnd))
				return 0;

			Win* win = reinterpret_cast<Win*>(GetProp(hwnd, L"CURRENT"));
			if (win != 0)
			{
				Win* prev = reinterpret_cast<Win*>(GetProp(hwnd, L"PREV"));
				if (prev) prev->bUpdate = true;
				if ((prev) && (prev != win) && (prev->bEnabled) && (prev->bVisable))
				{
					if ((prev->style == LWS_BTNCHECK) || (prev->style == LWS_BTNPUSH))
						update_prevbtn(prev);
				}
				SetProp(hwnd, L"PREV", (HANDLE)win);
			}
			
			if ((win) && (win->bEnabled) && (win->bVisable) && 
				((win->style == LWS_BTNCHECK) || (win->style == LWS_BTNPUSH)))
			{
				if (!win->bEnter)
					if (win->onEnter)
						win->onEnter((int)win);

				if ((!win->bEnter) && (win->style == LWS_BTNPUSH))
				{
					lStrSetBrush(win->Text, win->bFocused == 0 ? (Brush*)(-1) : win->bFocused);
					lSceneApply(win->scene);
				}

				win->bEnter = TRUE;

				if (HIBYTE(GetAsyncKeyState(VK_LBUTTON))) 
				{
					int l, t, w, h;
					lImgGetVisiblePart(win->Image, l, t, w, h);

					if (win->style == LWS_BTNPUSH)
					{
						if (t != win->nHeight * 2)
						{
							lImgSetVisiblePart(win->Image, 0, win->Texture->gTexture->GetHeight() / 4 * 2, win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 4);
							lSceneApply(win->scene);
						}
					}
					else
					{
						if ((t != win->nHeight * 2) || (t != win->nHeight * 6))
						{
							lImgSetVisiblePart(win->Image, 0, win->bChecked ? win->Texture->gTexture->GetHeight() / 8 * 2 : win->Texture->gTexture->GetHeight() / 8 * 6, 
								win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 8);
							lSceneApply(win->scene);
						}
					}
					if (win) win->bOnce = TRUE;
				}
				else
				{
					if ((win) && (win->bOnce))
					{
					int l, t, w, h;
					lImgGetVisiblePart(win->Image, l, t, w, h);

					if (win->style == LWS_BTNPUSH)
					{
						if (t != win->nHeight)
						{
							lImgSetVisiblePart(win->Image, 0, win->Texture->gTexture->GetHeight() / 4, win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 4);
							lSceneApply(win->scene);

							if (t == win->Texture->gTexture->GetHeight() / 4 * 2)
							{
								if (win->onClick)
									win->onClick((int)win);					
							}
						}
					}
					else
					{
						if ((t != win->nHeight) || (t != win->nHeight * 5))
						{
							lImgSetVisiblePart(win->Image, 0, win->bChecked ? win->Texture->gTexture->GetHeight() / 8 : win->Texture->gTexture->GetHeight() / 8 * 5, 
								win->Texture->gTexture->GetWidth(), win->Texture->gTexture->GetHeight() / 8);
							lSceneApply(win->scene);
							
							if ((t == win->Texture->gTexture->GetHeight() / 8 * 2) || (t == win->Texture->gTexture->GetHeight() / 8 * 6))
							{
								win->bChecked = win->bChecked ? FALSE : TRUE;
								if (win->onClick)
									win->onClick((int)win);								
							}
						}
					}
					if (win) win->bOnce = FALSE;
				}
				}
			}
			else
			{
				Win* prev = reinterpret_cast<Win*>(GetProp(hwnd, L"PREV"));
				if ((prev) && (prev->bUpdate) && (prev->bEnabled) && (prev->bVisable))
					if ((prev->style == LWS_BTNCHECK) || (prev->style == LWS_BTNPUSH))
					{
						prev->bUpdate = false;
						update_prevbtn(prev);
					}
					if (prev) prev->bOnce = TRUE;
			}
		}
		break;

	default: break;
	}

	return 0;
}
