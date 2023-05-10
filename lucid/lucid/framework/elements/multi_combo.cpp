#include "../framework.h"

void lucid_engine::ui::multi_combo_box(const char* title, std::vector<bool>* selected, std::vector<const char*> options) {
	auto renderer = g_renderer.get();
	m_multi_combo_id++;

	vec2_t combo_size = vec2_t(m_group->m_size.x - m_style->m_group_padding * 4, 16);
	vec2_t text_size = renderer->get_text_size(renderer->m_defualt_font, title).add(0, 2);

	if (g_input.get()->mouse_hovering_rect(m_group->m_elements_pos + vec2_t(0, text_size.y), combo_size)) {
		if (g_input.get()->is_key_pressed(VK_LBUTTON))
			m_multi_combo_info[m_multi_combo_id].in_use = !m_multi_combo_info[m_multi_combo_id].in_use;

		m_hovering_element = true;
	}

	if (m_group->element_visible(m_group->m_elements_pos + vec2_t(0, text_size.y), combo_size)) {
		renderer->text(renderer->m_defualt_font, title, m_group->m_elements_pos - vec2_t(0, 2), m_style->m_text_inactive);

		renderer->filled_rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y), combo_size, m_style->m_element_inactive);
		renderer->rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y) + vec2_t(1, 1), combo_size - vec2_t(2, 2), m_style->m_element_in_outline);
		renderer->rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y), combo_size, m_style->m_element_outline);

		std::string active{ "empty" };
		selected->resize(options.size());
		for (int i = 0; i < options.size(); i++) {
			if (selected->at(i)) {
				if (active == "empty")
					active.clear();

				active.append(active.empty() ? std::string(options.at(i)) : ", " + std::string(options.at(i)));
			}
		}

		renderer->text(renderer->m_defualt_font, active.c_str(), m_group->m_elements_pos + vec2_t(3, 1 + text_size.y), m_style->m_text_active);

		renderer->set_draw_list(foreground_draw_list);

		if (m_multi_combo_info[m_multi_combo_id].in_use) {
			selected->resize(options.size());

			renderer->filled_rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y) + vec2_t(0, combo_size.y), vec2_t(combo_size.x, combo_size.y * options.size()), m_style->m_element_inactive);
			renderer->rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y) + vec2_t(1, combo_size.y + 1), vec2_t(combo_size.x - 2, -2 + combo_size.y * options.size()), m_style->m_element_in_outline);
			renderer->rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y) + vec2_t(0, combo_size.y), vec2_t(combo_size.x, combo_size.y * options.size()), m_style->m_element_outline);

			for (int i = 0; i < options.size(); i++) {
				vec2_t bbox = m_group->m_elements_pos + vec2_t(0, combo_size.y * (i + 1)) + vec2_t(0, text_size.y);

				if (g_input.get()->mouse_hovering_rect(bbox, combo_size)) {
					if (g_input.get()->is_key_pressed(VK_LBUTTON))
						selected->at(i) = !selected->at(i);

					m_hovering_popup = true;
				}

				renderer->text(renderer->m_defualt_font, options.at(i), bbox + vec2_t(3, 1), selected->at(i) ? m_style->m_accent : m_style->m_text_inactive);
			}

			if (g_input.get()->is_key_pressed(VK_LBUTTON))
				m_multi_combo_info[m_multi_combo_id].in_use = false;
		}

		renderer->set_draw_list(default_draw_list);
	}

	m_group->m_elements_pos += vec2_t(0, text_size.y + combo_size.y + m_style->m_group_spacing);
}