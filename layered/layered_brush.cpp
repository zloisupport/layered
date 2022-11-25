#include "layered_brush.hpp"

Brush* call lSolidBrushCreate(DWORD argb)
{
	Brush* brush = new Brush(new Gdiplus::SolidBrush(Gdiplus::Color(argb)));
	obj_pushback(brush);
	return brush;
}

int call lBrushDelete(Brush* brush)
{
	obj_remove(brush);
	delete brush;
	return 0;
}
