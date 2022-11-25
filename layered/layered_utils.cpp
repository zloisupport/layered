#include "precomp.h"

std::wstring intToStr(int value)
{
	std::wstring result;
	wchar_t buf[16];
	wsprintf(buf, L"%i", value);
	return buf;
}

HWND hApplication = 0;
ITaskbarList3 *ptbl = 0;
HINSTANCE hIst = 0;

wchar_t * CDECL wcsstr_api(const wchar_t *s, const wchar_t *b)
{
	wchar_t *x;
	wchar_t *y;
	wchar_t *c;
	x = (wchar_t *)s;
	while (*x) {
		if (*x == *b) {
			y = x;
			c = (wchar_t *)b;
			while (*y && *c && *y == *c) {
				c++;
				y++;
			}
			if (!*c)
				return x;
		}
		x++;
	}
	return NULL;
}

BOOL CALLBACK EnFunc(HWND hWnd,
	LPARAM lParam
)
{
	DWORD pid, ispid;
	WCHAR cl[14];
	WCHAR app[] = L"TApplication";

	ispid = GetCurrentProcessId();
	GetWindowThreadProcessId(hWnd, &pid);
	GetClassNameW(hWnd, cl, 13);

	if (pid == ispid)
	{
		if (wcsstr_api(cl, app))
		{
			hApplication = hWnd;
			return FALSE;
		}
	}
	hApplication = 0;
	return TRUE;
}

void DLL_start()
{
	EnumWindows((WNDENUMPROC)EnFunc, 0);
	if (!hApplication)
		return;

	HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ptbl));
	if ((SUCCEEDED(hr)) && (ptbl))
	{
		ptbl->HrInit();
	}
}

void DLL_stop()
{
	if (ptbl)
		ptbl->Release();
	CoUninitialize();
}

BOOL IsWindows7()
{
	OSVERSIONINFO osver;
	osver.dwOSVersionInfoSize = sizeof(osver);
	if (GetVersionEx(&osver))
		return ((osver.dwMajorVersion == 6 && osver.dwMinorVersion >= 1) || (osver.dwMajorVersion > 6)) ? TRUE : FALSE;
	else
		return FALSE;
}

BOOL call DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (!IsWindows7())
		return TRUE;
	hIst = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DLL_start();
		break;

	case DLL_PROCESS_DETACH:
		DLL_stop();
		break;
	}
	return TRUE;
}

BOOL nNoProgress = false;
int nProgress = 0;

void call SetupPreview(HWND hWnd)
{
	SetWindowLongPtr(hWnd, -8, GetWindowLongPtr(GetWindow(hWnd, 4), -8));
}

void call SetTaskBarProgressValue(int value)
{
	nProgress = value;
	if (ptbl)
		ptbl->SetProgressValue(hApplication, (ULONGLONG)value, 100);
}

void call SetTaskBarProgressState(int value)
{
	nNoProgress = FALSE;

	switch (value)
	{
	case TBPF_NOPROGRESS:
	case TBPF_INDETERMINATE:
		nNoProgress = TRUE;
		break;
	case TBPF_NORMAL:
	case TBPF_ERROR:
	case TBPF_PAUSED:
		SetTaskBarProgressValue(nProgress);
		break;
	}

	if (ptbl)
		ptbl->SetProgressState(hApplication, (TBPFLAG)value);
}

void call SetTaskBarThumbnailTooltip(LPCWSTR hint)
{
	if (ptbl)
		ptbl->SetThumbnailTooltip(hApplication, hint);
}

void call SetTaskBarTitle(LPCWSTR hint)
{
	if (hApplication != 0)
		SetWindowText(hApplication, hint);
}

void call SetTaskBarOverlayIcon(HICON icon)
{
	if (ptbl)
		ptbl->SetOverlayIcon(hApplication, icon, NULL);
}

void call lMoving(HWND hWnd)
{
	ReleaseCapture();
	SendMessage(hWnd, WM_SYSCOMMAND, 0xF012, 0);
}
