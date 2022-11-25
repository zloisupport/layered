#pragma once
#include "precomp.h"
#include "layered_obj.hpp"
#include "layered_texture.hpp"

class Brush : public Obj
{
public:
	Brush()
	{
		obj_type = obj_Brush;
	}

	Brush(Gdiplus::Brush* pointer)
	{
		obj_type = obj_Brush;
		gBrush = pointer;
	}

	~Brush()
	{
		switch (gBrush->GetType())
		{
		case Gdiplus::BrushTypeSolidColor:
			delete reinterpret_cast<Gdiplus::SolidBrush*>(gBrush);
			break;

		case Gdiplus::BrushTypeLinearGradient:
			delete reinterpret_cast<Gdiplus::LinearGradientBrush*>(gBrush);
			break;

		case Gdiplus::BrushTypeTextureFill:
			delete reinterpret_cast<Gdiplus::TextureBrush*>(gBrush);
			break;

		default:
			break;
		}
		
		//delete gBrush;
	}

	Gdiplus::Brush* gBrush;
};

Brush* call lSolidBrushCreate(DWORD argb);
int call lBrushDelete(Brush* brush);
