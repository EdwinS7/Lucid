#include "../framework.h"

void lucid_engine::ui::label(const char* title, color_t color) {
	vec2_t text_size = g_renderer.get()->get_text_size(g_renderer.get()->m_defualt_font, title);

	// render our element.
	if (g_input.get()->rect_clipping_rect(m_elements_pos, text_size, m_window_pos.at(m_window_id) + m_group_pos.at(m_group_id) + vec2_t(0, 26), m_group_size.at(m_group_id) - vec2_t(0, 26)))
		g_renderer.get()->text(g_renderer.get()->m_defualt_font, title, m_elements_pos, color);

	// apply new position.
	m_elements_pos += vec2_t(0, text_size.y + m_style->m_group_spacing);
}