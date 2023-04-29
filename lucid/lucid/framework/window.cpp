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
	vec2_t text_size = g_renderer.get_text_size(g_renderer.fonts.default_font, title);

	// render our window.
	g_renderer.filled_rounded_rectangle(window_pos[window_id], window_size[window_id], style->window_background, style->window_rounding);
	g_renderer.filled_rounded_rectangle(window_pos[window_id], vec2_t(window_size[window_id].x, 25), style->window_header, style->window_rounding, corner_top);
	g_renderer.filled_rounded_rectangle(window_pos[window_id] + vec2_t(0, window_size[window_id].y - 45), vec2_t(window_size[window_id].x, 45), style->window_header, style->window_rounding, corner_bottom);
	g_renderer.filled_rectangle(window_pos[window_id] + vec2_t(0, 26), vec2_t(window_size[window_id].x, 1), style->accent);
	g_renderer.filled_rectangle(window_pos[window_id] + vec2_t(0, window_size[window_id].y - 46), vec2_t(window_size[window_id].x, 1), style->accent);
	g_renderer.rounded_rectangle(window_pos[window_id], window_size[window_id], style->window_outline, style->window_rounding);
	g_renderer.text(g_renderer.fonts.default_font, title, window_pos[window_id] + vec2_t((window_size[window_id].x / 2) - text_size.x / 2, 6), style->text_active);
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