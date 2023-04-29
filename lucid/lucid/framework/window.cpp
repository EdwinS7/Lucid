#include "framework.h"

void lucid_engine::ui::create_window(const char* title, vec2_t pos, vec2_t min_size, vec2_t default_size) {
	m_window_id++;

	// check for invalid paramaters and set paramaters.
	if (!m_this_window_setup[m_window_id]) {
		m_window_pos[m_window_id] = pos;

		if (min_size > default_size)
			throw std::runtime_error{ "create_window error (default window size < min window size)" };

		m_window_min_size[m_window_id] = min_size;
		m_window_size[m_window_id] = default_size;
		m_this_window_setup[m_window_id] = true;
	}

	// calculate text size for title.
	vec2_t text_size = g_renderer.get()->get_text_size(g_renderer.get()->m_fonts.at(fonts::font_default), title);

	// render our window.
	g_renderer.get()->filled_rounded_rectangle(m_window_pos[m_window_id], m_window_size[m_window_id], m_style->m_window_background, m_style->m_window_rounding);
	g_renderer.get()->filled_rounded_rectangle(m_window_pos[m_window_id], vec2_t(m_window_size[m_window_id].x, 25), m_style->m_window_header, m_style->m_window_rounding, corner_top);
	g_renderer.get()->filled_rounded_rectangle(m_window_pos[m_window_id] + vec2_t(0, m_window_size[m_window_id].y - 45), vec2_t(m_window_size[m_window_id].x, 45), m_style->m_window_header, m_style->m_window_rounding, corner_bottom);
	g_renderer.get()->filled_rectangle(m_window_pos[m_window_id] + vec2_t(0, 26), vec2_t(m_window_size[m_window_id].x, 1), m_style->m_accent);
	g_renderer.get()->filled_rectangle(m_window_pos[m_window_id] + vec2_t(0, m_window_size[m_window_id].y - 46), vec2_t(m_window_size[m_window_id].x, 1), m_style->m_accent);
	g_renderer.get()->rounded_rectangle(m_window_pos[m_window_id], m_window_size[m_window_id], m_style->m_window_outline, m_style->m_window_rounding);

	g_renderer.get()->text(g_renderer.get()->m_fonts.at(fonts::font_default), title, m_window_pos[m_window_id] + vec2_t((m_window_size[m_window_id].x / 2) - text_size.x / 2, 6), m_style->m_text_active);
}

void lucid_engine::ui::end_window() {
	// handle dragging and resizing.
	m_window_size[m_window_id] = handle_resizing();
	m_window_pos[m_window_id] = handle_dragging();

	// prepare for next window/frame.
	m_hovering_element = false;
}

vec2_t lucid_engine::ui::get_window_pos() {
	return m_window_pos[m_window_id];
}

vec2_t lucid_engine::ui::get_window_size() {
	return m_window_size[m_window_id];
}