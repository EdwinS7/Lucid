#pragma once

struct color_t {
	int r = 255, 
		g = 255,
		b = 255, 
		a = 255;

	color_t() { }
	color_t(int _r, int _g, int _b, int _a = 255)
		: r(_r), g(_g), b(_b), a(_a) {}

	D3DCOLOR translate() {
		return D3DCOLOR_RGBA(r, g, b, a);
	}

	static D3DCOLOR translate(color_t clr) {
		return D3DCOLOR_RGBA(clr.r, clr.g, clr.b, clr.a);
	}

	color_t override_alpha(int alpha) {
		return color_t(r, g, b, alpha);
	}
};