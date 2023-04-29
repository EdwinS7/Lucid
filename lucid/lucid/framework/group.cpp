#include "framework.h"

void lucid_engine::ui::create_group(const char* title, vec2_t pos, vec2_t min_size, vec2_t default_size) {
	group_id++;

	// check for invalid paramaters.
	if (!this_group_setup[group_id]) {
		if (min_size > default_size)
			throw std::runtime_error{ "create_group error (default group size < min group size)" };

		this_group_setup[group_id] = true;
	}

	// set group data.
	group_pos[group_id] = pos;
	group_min_size[group_id] = min_size;
	group_size[group_id] = default_size;

	// render our group box.
	g_renderer.get( )->filled_rounded_rectangle(window_pos[window_id] + group_pos[group_id], group_size[group_id], style->group_background, style->group_rounding);
	g_renderer.get( )->filled_rounded_rectangle(window_pos[window_id] + group_pos[group_id], vec2_t(group_size[group_id].x, 25), style->group_header, style->group_rounding, corner_top);
	g_renderer.get( )->filled_rectangle(window_pos[window_id] + group_pos[group_id] + vec2_t(0, 25), vec2_t(group_size[group_id].x, 1), style->accent);
	g_renderer.get( )->rounded_rectangle(window_pos[window_id] + group_pos[group_id], group_size[group_id], style->group_outline, style->group_rounding);
	g_renderer.get( )->text(g_renderer.get( )->fonts.at( default_font ), title, window_pos[window_id] + group_pos[group_id] + vec2_t(4, 6), style->text_active);

	// handle scrolling data.
	groups[group_id].hovered = g_input.get( )->mouse_hovering_rect(window_pos[window_id] + pos, group_size[group_id]);
	groups[group_id].og_elements_pos = window_pos[window_id] + group_pos[group_id] + vec2_t(style->group_padding, 25 + style->group_padding);

	// apply origin for elements to render.
	elements_pos = window_pos[window_id] + group_pos[group_id] + vec2_t(style->group_padding, 25 + style->group_padding + groups[group_id].scroll);
}

void lucid_engine::ui::end_group() {
	//group_size[group_id] = handle_resizing();
	//group_pos[group_id] = handle_dragging();

	// get the amount that should be scrolled to get to the bottom from current scrolled amount.
	float left_over = groups[group_id].og_elements_pos.y + group_size[group_id].y - 26 /* header */ - style->group_padding;
	float scroll_max_range = left_over - ((elements_pos.y - groups[group_id].scroll) + groups[group_id].scroll_abs);

	// apply values, the absolute and the lerped value.
	groups[group_id].scroll_abs = std::clamp(groups[group_id].scroll_abs + g_input.get( )->mouse_wheel_delta, scroll_max_range + groups[group_id].scroll_abs, 0.f);
	groups[group_id].scroll = g_animations.get( )->lerp(groups[group_id].scroll, groups[group_id].scroll_abs, g_io.get( )->delta_time * 8);

	// prepare for next frame.
	elements_pos = { };
}