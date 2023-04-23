#pragma once

struct text_info_t {
	bool setup = false;

	font_t font;
	std::string string;
	vec2_t pos;
	color_t color;
	DWORD flags;

	text_info_t() {}
	text_info_t(font_t _font, std::string _string, vec2_t _pos, color_t _color, DWORD _flags)
		: font(_font), string(_string), pos(_pos), color(_color), flags(_flags) { setup = true; }
};