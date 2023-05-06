#include "../framework.h"

void lucid_engine::ui::slider_int(const char* title, int min, int max, int* value) {
	m_slider_id++;

	bool another_in_use = std::ranges::any_of(m_slider_info, [this](const auto& item) {
		return m_slider_id != item.first && item.second.in_use;
	});

	// calculate sizes for group and text.
	vec2_t text_size = g_renderer.get()->get_text_size(g_renderer.get()->m_defualt_font, title).add(0, 2);
	vec2_t slider_size = vec2_t(m_group_size[m_group_id].x - m_style->m_group_padding * 4, 16);

	// check if mouse is hovering the bounds of the slider and mouse1 is down, set in_use to true.
	if (g_input.get()->mouse_hovering_rect(m_elements_pos + vec2_t(0, text_size.y), slider_size) && g_input.get()->is_key_held(VK_LBUTTON) && !another_in_use)
		m_slider_info[m_slider_id].in_use = true;
	else if ((m_slider_info[m_slider_id].in_use && !g_input.get()->is_key_held(VK_LBUTTON)) || another_in_use)
		m_slider_info[m_slider_id].in_use = false;

	if (m_slider_info[m_slider_id].in_use) {
		// set the value of our slider based on min, max, mouse pos.
		*value = g_animations.get()->map(g_input.get()->m_mouse_pos.x - m_elements_pos.x, 0.f, slider_size.x, min, max);

		// set this so we cannot drag our menu.
		m_hovering_element = true;
	}

	// check if slider is even visible and render if visible.
	if (g_input.get()->rect_clipping_rect(m_elements_pos, slider_size, m_window_pos[m_window_id] + m_group_pos[m_group_id] + vec2_t(0, 26), m_group_size[m_group_id] - vec2_t(0, 26))) {
		g_renderer.get()->text(g_renderer.get()->m_defualt_font, title, m_elements_pos, m_style->m_text_inactive);

		g_renderer.get()->filled_rectangle(m_elements_pos + vec2_t(0, text_size.y), slider_size, m_style->m_element_inactive);
		g_renderer.get()->filled_rectangle(m_elements_pos + vec2_t(0, text_size.y), vec2_t(g_animations.get()->map(*value, min, max, 0, slider_size.x), slider_size.y), m_style->m_accent);
		g_renderer.get()->rectangle(m_elements_pos + vec2_t(0, text_size.y) + vec2_t(1, 1), slider_size - vec2_t(2, 2), m_style->m_element_in_outline);
		g_renderer.get()->rectangle(m_elements_pos + vec2_t(0, text_size.y), slider_size, m_style->m_element_outline);
	}

	// apply new position.
	m_elements_pos += vec2_t(0, text_size.y + slider_size.y + m_style->m_group_spacing);
}

void lucid_engine::ui::slider_float(const char* title, float min, float max, float* value) {
	m_slider_id++;

	bool another_in_use = std::ranges::any_of(m_slider_info, [this](const auto& item) {
		return m_slider_id != item.first && item.second.in_use;
	});

	// calculate sizes for group and text.
	vec2_t text_size = g_renderer.get()->get_text_size(g_renderer.get()->m_defualt_font, title).add(0, 2);
	vec2_t slider_size = vec2_t(m_group_size[m_group_id].x - m_style->m_group_padding * 4, 16);

	// check if mouse is hovering the bounds of the slider and mouse1 is down, set in_use to true.
	if (g_input.get()->mouse_hovering_rect(m_elements_pos + vec2_t(0, text_size.y), slider_size) && g_input.get()->is_key_held(VK_LBUTTON) && !another_in_use)
		m_slider_info[m_slider_id].in_use = true;
	else if ((m_slider_info[m_slider_id].in_use && !g_input.get()->is_key_held(VK_LBUTTON)) || another_in_use)
		m_slider_info[m_slider_id].in_use = false;

	if (m_slider_info[m_slider_id].in_use) {
		// set the value of our slider based on min, max, mouse pos.
		*value = g_animations.get()->map(g_input.get()->m_mouse_pos.x - m_elements_pos.x, 0.f, slider_size.x, min, max);

		// set this so we cannot drag our menu.
		m_hovering_element = true;
	}

	// check if slider is even visible and render if visible.
	if (g_input.get()->rect_clipping_rect(m_elements_pos, slider_size, m_window_pos[m_window_id] + m_group_pos[m_group_id] + vec2_t(0, 26), m_group_size[m_group_id] - vec2_t(0, 26))) {
		g_renderer.get()->text(g_renderer.get()->m_defualt_font, title, m_elements_pos, m_style->m_text_inactive);

		g_renderer.get()->filled_rectangle(m_elements_pos + vec2_t(0, text_size.y), slider_size, m_style->m_element_inactive);
		g_renderer.get()->filled_rectangle(m_elements_pos + vec2_t(0, text_size.y), vec2_t(g_animations.get()->map(*value, min, max, 0, slider_size.x), slider_size.y), m_style->m_accent);
		g_renderer.get()->rectangle(m_elements_pos + vec2_t(0, text_size.y) + vec2_t(1, 1), slider_size - vec2_t(2, 2), m_style->m_element_in_outline);
		g_renderer.get()->rectangle(m_elements_pos + vec2_t(0, text_size.y), slider_size, m_style->m_element_outline);
	}

	// apply new position.
	m_elements_pos += vec2_t(0, text_size.y + slider_size.y + m_style->m_group_spacing);
}