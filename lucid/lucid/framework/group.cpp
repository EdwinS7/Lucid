#include "framework.h"

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
	g_renderer.filled_rounded_rectangle(window_pos[window_id] + group_pos[group_id], group_size[group_id], style->group_background, style->group_rounding);
	g_renderer.filled_rounded_rectangle(window_pos[window_id] + group_pos[group_id], vec2_t(group_size[group_id].x, 25), style->group_header, style->group_rounding, corner_top);
	g_renderer.filled_rectangle(window_pos[window_id] + group_pos[group_id] + vec2_t(0, 25), vec2_t(group_size[group_id].x, 1), style->accent);
	g_renderer.rounded_rectangle(window_pos[window_id] + group_pos[group_id], group_size[group_id], style->group_outline, style->group_rounding);
	g_renderer.text(g_renderer.fonts.default_font, title, window_pos[window_id] + group_pos[group_id] + vec2_t(4, 6), style->text_active);

	// handle scrolling data.
	groups[group_id].hovered = g_input.mouse_hovering_rect(window_pos[window_id] + pos, group_size[group_id]);
	groups[group_id].og_elements_pos = window_pos[window_id] + group_pos[group_id] + vec2_t(style->group_padding, 25 + style->group_padding);

	// apply origin for elements to render.
	m_elements_pos = m_window_pos[m_window_id] + m_group_pos[m_group_id] +
		vec2_t(m_style->m_group_padding, 25 + m_style->m_group_padding + m_groups[m_group_id].m_scroll);
}

void lucid_engine::ui::end_group() {
	//m_group_size[m_group_id] = handle_resizing();
	//m_group_pos[m_group_id] = handle_dragging();

	// get the amount that should be scrolled to get to the bottom from current scrolled amount.
	float left_over = m_groups[m_group_id].m_original_pos.y +
		m_group_size[m_group_id].y - 26 /* header */ - m_style->m_group_padding;
	float scroll_max_range = left_over -
		((m_elements_pos.y - m_groups[m_group_id].m_scroll) + m_groups[m_group_id].m_scroll_abs);

	// apply values, the absolute and the lerped value.
	groups[group_id].scroll_abs = std::clamp(groups[group_id].scroll_abs + g_input.mouse_wheel_delta, scroll_max_range + groups[group_id].scroll_abs, 0.f);
	groups[group_id].scroll = g_animations.lerp(groups[group_id].scroll, groups[group_id].scroll_abs, g_io.delta_time * 8);

	// prepare for next frame.
	m_elements_pos = { };
}