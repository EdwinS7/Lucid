#include "../framework.h"

void lucid_engine::ui::slider_int(const char* title, int min, int max, int* value) {
	m_slider_id++;

	bool another_in_use = std::ranges::any_of(m_slider_info, [this](const auto& item) {
		return m_slider_id != item.first && item.second.in_use;
	});

	vec2_t text_size = g_renderer->get_text_size(g_renderer->m_defualt_font, title).add(0, 2);
	vec2_t slider_size = vec2_t(m_group->m_size.x - m_style->m_group_padding * 4, 16);

	if (g_input->mouse_hovering_rect(m_group->m_elements_pos + vec2_t(0, text_size.y), slider_size) && g_input->is_key_held(VK_LBUTTON) && !another_in_use)
		m_slider_info[m_slider_id].in_use = true;
	else if ((m_slider_info[m_slider_id].in_use && !g_input->is_key_held(VK_LBUTTON)) || another_in_use)
		m_slider_info[m_slider_id].in_use = false;

	if (m_slider_info[m_slider_id].in_use) {
		*value = g_math->map(g_input->m_mouse_pos.x - m_group->m_elements_pos.x, 0.f, slider_size.x, min, max);

		m_hovering_element = true;
	}

	if (m_group->element_visible(m_group->m_elements_pos, slider_size)) {
		g_renderer->text(g_renderer->m_defualt_font, title, m_group->m_elements_pos - vec2_t(0, 2), m_style->m_text_inactive);

		g_renderer->filled_rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y), slider_size, m_style->m_element_inactive);
		g_renderer->filled_rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y), vec2_t(g_math->map(*value, min, max, 0, slider_size.x), slider_size.y), m_style->m_accent);
		g_renderer->rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y) + vec2_t(1, 1), slider_size - vec2_t(2, 2), m_style->m_element_in_outline);
		g_renderer->rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y), slider_size, m_style->m_element_outline);
	}

	m_group->m_elements_pos += vec2_t(0, text_size.y + slider_size.y + m_style->m_group_spacing);
}

void lucid_engine::ui::slider_float(const char* title, float min, float max, float* value) {
	m_slider_id++;

	bool another_in_use = std::ranges::any_of(m_slider_info, [this](const auto& item) {
		return m_slider_id != item.first && item.second.in_use;
	});

	vec2_t text_size = g_renderer->get_text_size(g_renderer->m_defualt_font, title).add(0, 2);
	vec2_t slider_size = vec2_t(m_group->m_size.x - m_style->m_group_padding * 4, 16);

	if (g_input->mouse_hovering_rect(m_group->m_elements_pos + vec2_t(0, text_size.y), slider_size) && g_input->is_key_held(VK_LBUTTON) && !another_in_use)
		m_slider_info[m_slider_id].in_use = true;
	else if ((m_slider_info[m_slider_id].in_use && !g_input->is_key_held(VK_LBUTTON)) || another_in_use)
		m_slider_info[m_slider_id].in_use = false;

	if (m_slider_info[m_slider_id].in_use) {
		*value = g_math->map(g_input->m_mouse_pos.x - m_group->m_elements_pos.x, 0.f, slider_size.x, min, max);

		m_hovering_element = true;
	}

	if (m_group->element_visible(m_group->m_elements_pos, slider_size)) {
		g_renderer->text(g_renderer->m_defualt_font, title, m_group->m_elements_pos - vec2_t(0, 2), m_style->m_text_inactive);

		g_renderer->filled_rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y), slider_size, m_style->m_element_inactive);
		g_renderer->filled_rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y), vec2_t(g_math->map(*value, min, max, 0, slider_size.x), slider_size.y), m_style->m_accent);
		g_renderer->rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y) + vec2_t(1, 1), slider_size - vec2_t(2, 2), m_style->m_element_in_outline);
		g_renderer->rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y), slider_size, m_style->m_element_outline);
	}

	m_group->m_elements_pos += vec2_t(0, text_size.y + slider_size.y + m_style->m_group_spacing);
}