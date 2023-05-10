#include "../framework.h"

vec2_t check_box_size = { 10, 10 };
void lucid_engine::ui::check_box(const char* title, bool* state) {
	auto renderer = g_renderer.get();

	vec2_t text_size = renderer->get_text_size(renderer->m_defualt_font, title);

	if (g_input.get()->mouse_hovering_rect(m_group->m_elements_pos, text_size + vec2_t(check_box_size.x, 0))) {
		if (g_input.get()->is_key_pressed(VK_LBUTTON))
			*state = !(*state);

		m_hovering_element = true;
	}

	if (m_group->element_visible(m_group->m_elements_pos, check_box_size)) {
		renderer->filled_rectangle(m_group->m_elements_pos, check_box_size, *state ? m_style->m_element_active : m_style->m_element_inactive);
		renderer->rectangle(m_group->m_elements_pos + vec2_t(1, 1), check_box_size - vec2_t(2, 2), m_style->m_element_in_outline);
		renderer->rectangle(m_group->m_elements_pos, check_box_size, m_style->m_element_outline);

		renderer->text(renderer->m_defualt_font, title, m_group->m_elements_pos + vec2_t(check_box_size.x + 4, -2), *state ? m_style->m_text_active : m_style->m_text_inactive);
	}

	m_group->m_elements_pos += vec2_t(0, text_size.y + m_style->m_group_spacing);
}