#include "../framework.h"

void lucid_engine::ui::label(const char* title, color_t color) {
	vec2_t text_size = g_renderer->get_text_size(g_renderer->m_defualt_font, title);

	if (m_group->element_visible(m_group->m_elements_pos, text_size))
		g_renderer->text(g_renderer->m_defualt_font, title, m_group->m_elements_pos, color);

	m_group->m_elements_pos += vec2_t(0, text_size.y + m_style->m_group_spacing);
}