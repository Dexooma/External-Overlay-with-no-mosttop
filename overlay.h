#pragma once
#include "d3x.h"
/*
Github: https://github.com/Dexooma
by Dexooma, cheers
*/
struct d3colors
{
	unsigned long Red = D3DCOLOR_RGBA(255, 0, 0, 255);
	unsigned long Green = D3DCOLOR_RGBA(0, 255, 0, 255);
	unsigned long Blue = D3DCOLOR_RGBA(0, 0, 255, 255);
};

struct D3DTLVERTEX {
	float x, y, z, rhw;
	unsigned long color;
};

#ifndef overlay_HPP
class Overlay
{
public:
	Overlay() { };
	int ScreenSize[2];
	void window_d3x_setup();
	bool rendering();
	void Begin();
	void End();
	RECT get_text_dimensions(std::string_view text, LPD3DXFONT font);
	auto draw_text(std::string_view text, float x, float y, unsigned long color, bool center, bool outline)-> void;
	auto DrawBox(float x, float y, float width, float height, D3DCOLOR color)-> void;

};
#endif // include guard