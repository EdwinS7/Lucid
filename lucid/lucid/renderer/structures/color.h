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

	color_t interpolate(color_t other, float t) {
		const float r = (1.0f - t) * r + t * other.r;
		const float g = (1.0f - t) * g + t * other.g;
		const float b = (1.0f - t) * b + t * other.b;
		const float a = (1.0f - t) * a + t * other.a;
		return color_t(r, g, b, a);
	}

	static color_t interpolate(color_t color, color_t color2, float t) {
		const float r = (1.0f - t) * color.r + t * color2.r;
		const float g = (1.0f - t) * color.g + t * color2.g;
		const float b = (1.0f - t) * color.b + t * color2.b;
		const float a = (1.0f - t) * color.a + t * color2.a;
		return color_t(r, g, b, a);
	}
};