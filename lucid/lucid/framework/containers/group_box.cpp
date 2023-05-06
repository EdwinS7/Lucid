#include "../framework.h"

void lucid_engine::ui::create_group(const char* title, vec2_t pos, vec2_t min_size, vec2_t default_size) {
	m_group_id++;

	// check for invalid paramaters.
	if (!m_this_group_setup[m_group_id]) {
		if (min_size > default_size)
			throw std::runtime_error{ "create_group error (default group size < min group size)" };

		m_this_group_setup[m_group_id] = true;
	}

	// set group data.
	m_group_pos[m_group_id] = pos;
	m_group_min_size[m_group_id] = min_size;
	m_group_size[m_group_id] = default_size;

	// render our group box.
	g_renderer.get()->filled_rounded_rectangle(m_window_pos[m_window_id] + m_group_pos[m_group_id], m_group_size[m_group_id], m_style->m_group_background, m_style->m_group_rounding);
	g_renderer.get()->filled_rounded_rectangle(m_window_pos[m_window_id] + m_group_pos[m_group_id], vec2_t(m_group_size[m_group_id].x, 25), m_style->m_group_header, m_style->m_group_rounding, corner_top);
	g_renderer.get()->filled_rectangle(m_window_pos[m_window_id] + m_group_pos[m_group_id] + vec2_t(0, 25), vec2_t(m_group_size[m_group_id].x, 1), m_style->m_accent);
	g_renderer.get()->rounded_rectangle(m_window_pos[m_window_id] + m_group_pos[m_group_id], m_group_size[m_group_id], m_style->m_group_outline, m_style->m_group_rounding);

	g_renderer.get()->text(g_renderer.get()->m_defualt_font, title, m_window_pos[m_window_id] + m_group_pos[m_group_id] + vec2_t(4, 6), m_style->m_text_active);

	// handle scrolling data.
	m_groups[m_group_id].m_hovered = g_input.get()->mouse_hovering_rect(m_window_pos[m_window_id] + pos, m_group_size[m_group_id]);
	m_groups[m_group_id].m_original_pos = m_window_pos[m_window_id] + m_group_pos[m_group_id] + vec2_t(m_style->m_group_padding, 25 + m_style->m_group_padding);

	// apply origin for elements to render.
	m_elements_pos = m_window_pos[m_window_id] + m_group_pos[m_group_id] + vec2_t(m_style->m_group_padding * 2, 25 + m_style->m_group_padding + m_groups[m_group_id].m_scroll);

	g_renderer.get()->push_clip(m_window_pos[m_window_id] + m_group_pos[m_group_id] + vec2_t(0, 26), m_group_size[m_group_id] - vec2_t(0, 26));
}

void lucid_engine::ui::end_group() {
	g_renderer.get()->pop_clip();

	// get the amount that should be scrolled to get to the bottom from current scrolled amount.
	float left_over = m_groups[m_group_id].m_original_pos.y + m_group_size[m_group_id].y - 26 /* header */ - m_style->m_group_padding;
	float scroll_max_range = left_over - ((m_elements_pos.y - m_groups[m_group_id].m_scroll) + m_groups[m_group_id].m_scroll_abs);

	// apply values, the absolute and the lerped value.
	if (m_elements_pos.y - left_over >= 0 && !is_dragging() && !is_resizing())
		m_groups[m_group_id].m_scroll_abs = std::clamp(m_groups[m_group_id].m_scroll_abs + g_input.get()->m_mouse_wheel_delta, scroll_max_range + m_groups[m_group_id].m_scroll_abs, 0.f);

	m_groups[m_group_id].m_scroll = g_animations.get()->lerp(m_groups[m_group_id].m_scroll, m_groups[m_group_id].m_scroll_abs, g_io.get()->m_delta_time * 8);

	// prepare for next frame.
	m_elements_pos = { };
}