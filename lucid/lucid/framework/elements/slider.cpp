#include "../framework.h"

void lucid_engine::ui::slider_int(containers::group_box* node, const char* title, int min, int max, int* value) {
	auto renderer = g_renderer.get();
	m_slider_id++;

	bool another_in_use = std::ranges::any_of(m_slider_info, [this](const auto& item) {
		return m_slider_id != item.first && item.second.in_use;
	});

	vec2_t text_size = renderer->get_text_size(renderer->m_defualt_font, title).add(0, 2);
	vec2_t slider_size = vec2_t(node->m_size.x - m_style->m_group_padding * 4, 16);

	if (g_input.get()->mouse_hovering_rect(node->m_elements_pos + vec2_t(0, text_size.y), slider_size) && g_input.get()->is_key_held(VK_LBUTTON) && !another_in_use)
		m_slider_info[m_slider_id].in_use = true;
	else if ((m_slider_info[m_slider_id].in_use && !g_input.get()->is_key_held(VK_LBUTTON)) || another_in_use)
		m_slider_info[m_slider_id].in_use = false;

	if (m_slider_info[m_slider_id].in_use) {
		*value = g_math.get()->map(g_input.get()->m_mouse_pos.x - node->m_elements_pos.x, 0.f, slider_size.x, min, max);

		m_hovering_element = true;
	}

	if (node->element_visible(node->m_elements_pos, slider_size)) {
		renderer->text(renderer->m_defualt_font, title, node->m_elements_pos - vec2_t(0, 2), m_style->m_text_inactive);

		renderer->filled_rectangle(node->m_elements_pos + vec2_t(0, text_size.y), slider_size, m_style->m_element_inactive);
		renderer->filled_rectangle(node->m_elements_pos + vec2_t(0, text_size.y), vec2_t(g_math.get()->map(*value, min, max, 0, slider_size.x), slider_size.y), m_style->m_accent);
		renderer->rectangle(node->m_elements_pos + vec2_t(0, text_size.y) + vec2_t(1, 1), slider_size - vec2_t(2, 2), m_style->m_element_in_outline);
		renderer->rectangle(node->m_elements_pos + vec2_t(0, text_size.y), slider_size, m_style->m_element_outline);
	}

	node->m_elements_pos += vec2_t(0, text_size.y + slider_size.y + m_style->m_group_spacing);
}

void lucid_engine::ui::slider_float(containers::group_box* node, const char* title, float min, float max, float* value) {
	auto renderer = g_renderer.get();
	m_slider_id++;

	bool another_in_use = std::ranges::any_of(m_slider_info, [this](const auto& item) {
		return m_slider_id != item.first && item.second.in_use;
	});

	vec2_t text_size = renderer->get_text_size(renderer->m_defualt_font, title).add(0, 2);
	vec2_t slider_size = vec2_t(node->m_size.x - m_style->m_group_padding * 4, 16);

	if (g_input.get()->mouse_hovering_rect(node->m_elements_pos + vec2_t(0, text_size.y), slider_size) && g_input.get()->is_key_held(VK_LBUTTON) && !another_in_use)
		m_slider_info[m_slider_id].in_use = true;
	else if ((m_slider_info[m_slider_id].in_use && !g_input.get()->is_key_held(VK_LBUTTON)) || another_in_use)
		m_slider_info[m_slider_id].in_use = false;

	if (m_slider_info[m_slider_id].in_use) {
		*value = g_math.get()->map(g_input.get()->m_mouse_pos.x - node->m_elements_pos.x, 0.f, slider_size.x, min, max);

		m_hovering_element = true;
	}

	if (node->element_visible(node->m_elements_pos, slider_size)) {
		renderer->text(renderer->m_defualt_font, title, node->m_elements_pos - vec2_t(0, 2), m_style->m_text_inactive);

		renderer->filled_rectangle(node->m_elements_pos + vec2_t(0, text_size.y), slider_size, m_style->m_element_inactive);
		renderer->filled_rectangle(node->m_elements_pos + vec2_t(0, text_size.y), vec2_t(g_math.get()->map(*value, min, max, 0, slider_size.x), slider_size.y), m_style->m_accent);
		renderer->rectangle(node->m_elements_pos + vec2_t(0, text_size.y) + vec2_t(1, 1), slider_size - vec2_t(2, 2), m_style->m_element_in_outline);
		renderer->rectangle(node->m_elements_pos + vec2_t(0, text_size.y), slider_size, m_style->m_element_outline);
	}

	node->m_elements_pos += vec2_t(0, text_size.y + slider_size.y + m_style->m_group_spacing);
}