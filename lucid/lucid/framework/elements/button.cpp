#include "../framework.h"

bool lucid_engine::ui::button(group_box* node, const char* title) {
	auto renderer = g_renderer.get();

	vec2_t text_size = renderer->get_text_size(renderer->m_defualt_font, title);
	vec2_t button_size = vec2_t(node->m_size.x - m_style->m_group_padding * 4, 16);
	bool pressed = false;

	if (g_input.get()->mouse_hovering_rect(node->m_elements_pos, button_size)) {
		if (g_input.get()->is_key_pressed(VK_LBUTTON))
			pressed = true;

		m_hovering_element = true;
	}

	if (node->element_visible(node->m_elements_pos, button_size)) {
		renderer->filled_rectangle(node->m_elements_pos, button_size, m_style->m_element_inactive);
		renderer->rectangle(node->m_elements_pos + vec2_t(1, 1), button_size - vec2_t(2, 2), m_style->m_element_in_outline);
		renderer->rectangle(node->m_elements_pos, button_size, pressed ? m_style->m_accent : m_style->m_element_outline);

		renderer->text(renderer->m_defualt_font, title, node->m_elements_pos + vec2_t((button_size.x - text_size.x) * 0.5, (button_size.y - text_size.y) * 0.5), pressed ? m_style->m_accent : m_style->m_text_inactive);
	}

	node->m_elements_pos += vec2_t(0, button_size.y + m_style->m_group_spacing);

	return pressed;
}