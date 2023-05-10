#include "../framework.h"

void lucid_engine::ui::label(containers::group_box* node, const char* title, color_t color) {
	auto renderer = g_renderer.get();

	vec2_t text_size = renderer->get_text_size(renderer->m_defualt_font, title);

	if (node->element_visible(node->m_elements_pos, text_size))
		renderer->text(renderer->m_defualt_font, title, node->m_elements_pos, color);

	node->m_elements_pos += vec2_t(0, text_size.y + m_style->m_group_spacing);
}