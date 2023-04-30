#include "../framework.h"

void lucid_engine::ui::combo_box(const char* title, int* value, std::vector<const char*> options) {
	m_combo_id++;

	// calculate sizes for group and text.
	vec2_t combo_size = vec2_t(m_group_size[m_group_id].x - m_style->m_group_padding * 2, 16);
	vec2_t text_size = g_renderer.get()->get_text_size(g_renderer.get()->m_defualt_font, title);

	// check if mouse is hovering the bounds of the combo box.
	if (g_input.get()->mouse_hovering_rect(m_elements_pos, combo_size)) {
		// check if mouse1 is pressed and change the state of the combo box.
		if (g_input.get()->is_key_pressed(VK_LBUTTON))
			m_combo_info[m_combo_id].in_use = !m_combo_info[m_combo_id].in_use;

		// set this so we cannot drag our menu.
		m_hovering_element = true;
	}

	// check if combo box is even visible and render if visible.
	if (g_input.get()->rect_clipping_rect(m_elements_pos, combo_size, m_window_pos[m_window_id] + m_group_pos[m_group_id] + vec2_t(0, 26), m_group_size[m_group_id] - vec2_t(0, 26))) {
		g_renderer.get()->filled_rounded_rectangle(m_elements_pos, combo_size, m_style->m_element_inactive, m_style->m_element_rounding, m_combo_info[m_combo_id].in_use ? corner_top : corner_all);
		g_renderer.get()->rounded_rectangle(m_elements_pos, combo_size, m_style->m_element_outline, m_style->m_element_rounding, m_combo_info[m_combo_id].in_use ? corner_top : corner_all);

		// render our element.
		g_renderer.get()->text(g_renderer.get()->m_defualt_font, title, m_elements_pos + vec2_t(3, 1), m_style->m_text_active);

		// apply foreground to draw list type.
		g_renderer.get()->set_draw_list(foreground_draw_list);

		// if the combo is opened then lopp through elements and handle them.
		if (m_combo_info[m_combo_id].in_use) {
			// render background & outline.
			g_renderer.get()->filled_rounded_rectangle(m_elements_pos + vec2_t(0, combo_size.y), vec2_t(combo_size.x, combo_size.y * options.size()), m_style->m_element_inactive, m_style->m_element_rounding, corner_bottom);
			g_renderer.get()->rounded_rectangle(m_elements_pos + vec2_t(0, combo_size.y), vec2_t(combo_size.x, combo_size.y * options.size()), m_style->m_element_outline, m_style->m_element_rounding, corner_bottom);

			for (int i = 0; i < options.size(); i++) {
				vec2_t bbox = m_elements_pos + vec2_t(0, combo_size.y * (i + 1));

				// check for new selected & set hover state.
				if (g_input.get()->mouse_hovering_rect(bbox, combo_size)) {
					// check if mouse1 is pressed and change the value of the combo box.
					if (g_input.get()->is_key_pressed(VK_LBUTTON)) {
						//close combo box menu.
						m_combo_info[m_combo_id].in_use = false;

						// change value of current selected object.
						*value = i;
					}

					// set this so we cannot drag our menu.
					m_hovering_popup = true;
				}

				g_renderer.get()->text(g_renderer.get()->m_defualt_font, options.at(i), bbox + vec2_t(3, 1), *value == i ? m_style->m_accent : m_style->m_text_inactive);
			}
		}

		// restore draw list to default.
		g_renderer.get()->set_draw_list(default_draw_list);
	}

	// apply new position.
	m_elements_pos += vec2_t(0, combo_size.y + m_style->m_group_spacing);
}