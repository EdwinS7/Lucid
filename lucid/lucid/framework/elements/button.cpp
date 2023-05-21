#include "../framework.h"

bool lucid_engine::ui::button(const char* title) {
	vec2_t text_size = g_renderer->get_text_size(g_renderer->m_defualt_font, title);
	vec2_t button_size = vec2_t(m_group->m_size.x - m_style->m_group_padding * 4, 16);

	if (!m_group->element_visible(m_group->m_elements_pos, button_size)) {
		m_group->m_elements_pos += vec2_t(0, button_size.y + m_style->m_group_spacing);
		return false;
	}

	bool pressed = false;

	if (g_input->mouse_hovering_rect(m_group->m_elements_pos, button_size)) {
		if (g_input->is_key_pressed(VK_LBUTTON))
			pressed = true;

		m_hovering_element = true;
	}

	g_renderer->filled_rectangle(m_group->m_elements_pos, button_size, m_style->m_element_inactive);
	g_renderer->rectangle(m_group->m_elements_pos + vec2_t(1, 1), button_size - vec2_t(2, 2), m_style->m_element_in_outline);
	g_renderer->rectangle(m_group->m_elements_pos, button_size, pressed ? m_style->m_accent : m_style->m_element_outline);

	g_renderer->text(g_renderer->m_defualt_font, title, m_group->m_elements_pos + vec2_t((button_size.x - text_size.x) * 0.5, (button_size.y - text_size.y) * 0.5), pressed ? m_style->m_accent : m_style->m_text_inactive);

	m_group->m_elements_pos += vec2_t(0, button_size.y + m_style->m_group_spacing);

	return pressed;
}