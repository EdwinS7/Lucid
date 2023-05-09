#include "../framework.h"

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
	g_renderer.get()->filled_rectangle(m_window_pos[m_window_id], m_window_size[m_window_id], m_style->m_window_background);

	// side bar.
	g_renderer.get()->filled_rectangle(m_window_pos[m_window_id], vec2_t(150, m_window_size[m_window_id].y), m_style->m_panel_background);
	g_renderer.get()->filled_rectangle(m_window_pos[m_window_id] + vec2_t(150, 0), vec2_t(1, m_window_size[m_window_id].y), m_style->m_group_outline);

	// top bar.
	g_renderer.get()->filled_rectangle(m_window_pos[m_window_id], vec2_t(m_window_size[m_window_id].x, 45), m_style->m_window_header);
	g_renderer.get()->filled_rectangle(m_window_pos[m_window_id] + vec2_t(0, 44), vec2_t(m_window_size[m_window_id].x, 1), m_style->m_window_outline);

	// outline.
	g_renderer.get()->rectangle(m_window_pos[m_window_id], m_window_size[m_window_id], m_style->m_window_outline);

	// text.
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