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
	vec2_t text_size = g_renderer.get()->get_text_size(g_renderer.get()->m_defualt_font, title);

	// render our window.
	g_renderer.get()->filled_rounded_rectangle(m_window_pos[m_window_id], m_window_size[m_window_id], m_style->m_window_background, m_style->m_window_rounding);
	
	// side bar.
	g_renderer.get()->filled_rectangle(m_window_pos[m_window_id] + vec2_t(0, 45), vec2_t(150, m_window_size[m_window_id].y - 70), m_style->m_panel_background);
	g_renderer.get()->filled_rectangle(m_window_pos[m_window_id] + vec2_t(150, 45), vec2_t(1, m_window_size[m_window_id].y - 70), m_style->m_group_outline);

	// top bar.
	g_renderer.get()->filled_rounded_rectangle(m_window_pos[m_window_id], vec2_t(m_window_size[m_window_id].x, 45), m_style->m_window_header, m_style->m_window_rounding, corner_top);
	g_renderer.get()->filled_gradient(m_window_pos[m_window_id] + vec2_t(0, 45), vec2_t(m_window_size[m_window_id].x, 10), color_t(0, 0, 0, 150), color_t(0, 0, 0, 0), true);
	g_renderer.get()->filled_gradient(m_window_pos[m_window_id] + vec2_t(150, 45), vec2_t(m_window_size[m_window_id].x - 150, 1), m_style->m_accent.override_alpha(0), m_style->m_accent);

	// bottom bar.
	g_renderer.get()->filled_rounded_rectangle(m_window_pos[m_window_id] + vec2_t(0, m_window_size[m_window_id].y - 25), vec2_t(m_window_size[m_window_id].x, 25), m_style->m_window_header, m_style->m_window_rounding, corner_bottom);
	g_renderer.get()->filled_rectangle(m_window_pos[m_window_id] + vec2_t(0, m_window_size[m_window_id].y - 26), vec2_t(m_window_size[m_window_id].x, 1), m_style->m_accent);

	// outline.
	g_renderer.get()->rounded_rectangle(m_window_pos[m_window_id], m_window_size[m_window_id], m_style->m_window_outline, m_style->m_window_rounding);

	// title.
	g_renderer.get()->text(g_renderer.get()->m_logo_font, title, m_window_pos[m_window_id] + vec2_t(6, -1), m_style->m_text_active);
}

void lucid_engine::ui::end_window() {
	// handle dragging and resizing.
	m_window_size[m_window_id] = handle_resizing();
	m_window_pos[m_window_id] = handle_dragging();

	// prepare for next window/frame.
	m_hovering_element = false;
	m_hovering_popup = false;
}

vec2_t lucid_engine::ui::get_window_pos() {
	return m_window_pos[m_window_id];
}

vec2_t lucid_engine::ui::get_window_size() {
	return m_window_size[m_window_id];
}