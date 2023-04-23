#pragma once

struct color_t {
	int r, g, b, a;

	color_t() {}
	color_t(int _r, int _g, int _b, int _a = 255)
		: r(_r), g(_g), b(_b), a(_a) {}

	D3DCOLOR translate() {
		return D3DCOLOR_RGBA(r, g, b, a);
	}
};