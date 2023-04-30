#include "../framework.h"

vec2_t check_box_size = { 10, 10 };
void lucid_engine::ui::check_box(const char* title, bool* state) {
	vec2_t text_size = g_renderer.get()->get_text_size(g_renderer.get()->m_defualt_font, title);

	// check if mouse is hovering the bounds of the check box.
	if (g_input.get()->mouse_hovering_rect(m_elements_pos, text_size + vec2_t(check_box_size.x, 0))) {
		// check if mouse1 is pressed and change the state of the checkbox.
		if (g_input.get()->is_key_pressed(VK_LBUTTON))
			*state = !(*state);

		// set this so we cannot drag our menu.
		m_hovering_element = true;
	}

	// check if check box is even visible and render if visible.
	if (g_input.get()->rect_clipping_rect(m_elements_pos, check_box_size,
		m_window_pos.at(m_window_id) + m_group_pos.at(m_group_id) + vec2_t(0, 26), m_group_size.at(m_group_id) - vec2_t(0, 26))) {
		g_renderer.get()->filled_rounded_rectangle(m_elements_pos, check_box_size, *state ? m_style->m_element_active : m_style->m_element_inactive, m_style->m_element_rounding);
		g_renderer.get()->rounded_rectangle(m_elements_pos, check_box_size, m_style->m_element_outline, m_style->m_element_rounding);

		g_renderer.get()->text(g_renderer.get()->m_defualt_font, title, m_elements_pos + vec2_t(check_box_size.x + 4, -2), *state ? m_style->m_text_active : m_style->m_text_inactive);
	}

	// apply new position.
	m_elements_pos += vec2_t(0, text_size.y + m_style->m_group_spacing);
}