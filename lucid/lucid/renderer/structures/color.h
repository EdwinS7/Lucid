#pragma once

struct color_t {
	uint8_t r = 255, 
			g = 255,
			b = 255, 
			a = 255;

	color_t() { }
	color_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
		: r(r), g(g), b(b), a(a) {}

	DWORD translate() {
		return D3DCOLOR_RGBA(r, g, b, a);
	}

	static DWORD translate(color_t clr) {
		return D3DCOLOR_RGBA(clr.r, clr.g, clr.b, clr.a);
	}

	color_t override_alpha(uint8_t alpha) {
		return color_t(r, g, b, alpha);
	}
};