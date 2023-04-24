#include "../lucid.h"

void lucid_engine::ui::create_group(const char* title, vec2_t pos, vec2_t min_size, vec2_t default_size) {
	group_id++;

	if (!this_group_setup[group_id]) {
		group_pos[group_id] = pos;

		if (min_size > default_size)
			throw std::runtime_error{ "create_group error (default group size < min group size)" };

		group_min_size[group_id] = min_size;
		group_size[group_id] = default_size;
		this_group_setup[group_id] = true;
	}

	renderer::get_instance().filled_rounded_rectangle(window_pos[window_id] + group_pos[group_id], group_size[group_id], style->child_background, style->child_rounding);
	renderer::get_instance().filled_rounded_rectangle(window_pos[window_id] + group_pos[group_id], vec2_t(group_size[group_id].x, 25), style->child_header, style->child_rounding, corner_top);
	renderer::get_instance().filled_rectangle(window_pos[window_id] + group_pos[group_id] + vec2_t(0, 25), vec2_t(group_size[group_id].x, 1), style->accent);
	renderer::get_instance().rounded_rectangle(window_pos[window_id] + group_pos[group_id], group_size[group_id], style->child_outline, style->child_rounding);
	renderer::get_instance().text(renderer::get_instance().fonts.default_font, title, window_pos[window_id] + group_pos[group_id] + vec2_t(4, 6), style->text_active);
}

void lucid_engine::ui::end_group() {
	//group_size[group_id] = handle_resizing();
	//group_pos[group_id] = handle_dragging();
}