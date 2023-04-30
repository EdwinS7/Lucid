#include "../framework.h"

void lucid_engine::ui::label(const char* title, color_t color) {
	vec2_t text_size = g_renderer.get()->get_text_size(g_renderer.get()->m_defualt_font, title);

	// render our element.
	g_renderer.get()->text(g_renderer.get()->m_defualt_font, title, m_elements_pos, color);

	// apply new position.
	m_elements_pos += vec2_t(0, text_size.y + m_style->m_group_spacing);
}