#include "precomp.h"
#include "layered_obj.hpp"

DWORD dGpToken = 0;
BOOL bLoad = false;

BOOL call lStartup()
{
	Gdiplus::GdiplusStartupInput input;
	Gdiplus::Status result = Gdiplus::GdiplusStartup(&dGpToken, &input, 0);

	return bLoad = true;
}

void unreg_class();

void call lShutdown()
{
	if (bLoad)
	{
		obj_delete(); 
		Gdiplus::GdiplusShutdown(dGpToken);
	}

	unreg_class();
}
