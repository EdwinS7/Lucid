#pragma once

struct text_info_t {
	bool		m_setup{ false };

	font_t		m_font{ };
	std::string m_string{ };
	vec2_t		m_pos{ };
	color_t		m_color{ };

	text_info_t() {}
	text_info_t(font_t font, std::string string, vec2_t pos, color_t color)
		: m_font(font), m_string(string), m_pos(pos), m_color(color) { m_setup = true; }
};