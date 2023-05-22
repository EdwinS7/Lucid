#include "../framework.h"

void lucid_engine::ui::bullet(const char* title, color_t color) {
	vec2_t text_size = g_renderer->get_text_size(g_renderer->m_defualt_font, title);

	if (m_group->element_visible(m_group->m_elements_pos, text_size + vec2_t(8, 0))) {
		g_renderer->filled_circle(m_group->m_elements_pos + vec2_t(2, text_size.y * 0.5), 2, 100, 0, m_style->m_accent);
		g_renderer->text(g_renderer->m_defualt_font, title, m_group->m_elements_pos + vec2_t(8, 0), color);
	}

	m_group->m_elements_pos += vec2_t(0, text_size.y + m_style->m_group_spacing);
}