#include "../framework.h"

bool lucid_engine::ui::button(const char* title) {
	vec2_t text_size = g_renderer.get()->get_text_size(g_renderer.get()->m_defualt_font, title);
	vec2_t button_size = vec2_t(m_group_size[m_group_id].x - m_style->m_group_padding * 2, 16);
	bool pressed = false;

	// check if mouse is hovering the bounds of our button.
	if (g_input.get()->mouse_hovering_rect(m_elements_pos, button_size)) {
		// check if mouse1 is pressed and change the state of the button.
		if (g_input.get()->is_key_pressed(VK_LBUTTON))
			pressed = true;

		// set this so we cannot drag our menu.
		m_hovering_element = true;
	}

	// check if our button is even visible and render if visible.
	if (g_input.get()->rect_clipping_rect(m_elements_pos, button_size, m_window_pos[m_window_id] + m_group_pos[m_group_id] + vec2_t(0, 26), m_group_size[m_group_id] - vec2_t(0, 26))) {
		g_renderer.get()->filled_rounded_rectangle(m_elements_pos, button_size, m_style->m_element_inactive, m_style->m_element_rounding);
		g_renderer.get()->rounded_rectangle(m_elements_pos, button_size, pressed ? m_style->m_accent : m_style->m_element_outline, m_style->m_element_rounding);

		g_renderer.get()->text(g_renderer.get()->m_defualt_font, title, m_elements_pos + vec2_t((button_size.x - text_size.x) * 0.5, (button_size.y - text_size.y) * 0.5), pressed ? m_style->m_accent : m_style->m_text_inactive);
	}

	// apply new position.
	m_elements_pos += vec2_t(0, button_size.y + m_style->m_group_spacing);

	return pressed;
}