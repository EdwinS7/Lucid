#include "../lucid.h"

void lucid_engine::ui::create_window(const char* title, vec2_t pos, vec2_t min_size, vec2_t default_size) {
	window_id++;
	if (!this_window_setup[window_id]) {
		window_pos[window_id] = pos;

		if (min_size > default_size)
			throw std::runtime_error{ "create_window error (default window size < min window size)" };

		window_min_size[window_id] = min_size;
		window_size[window_id] = default_size;
		this_window_setup[window_id] = true;
	}

	renderer::get_instance().filled_rounded_rectangle(window_pos[window_id], window_size[window_id], style->window_background, style->window_rounding);
	renderer::get_instance().filled_rounded_rectangle(window_pos[window_id], vec2_t(window_size[window_id].x, 25), style->window_header, style->window_rounding, corner_top);
	renderer::get_instance().rounded_rectangle(window_pos[window_id], window_size[window_id], style->window_outline, style->window_rounding);
	renderer::get_instance().filled_gradient(window_pos[window_id] + vec2_t(0, 25), vec2_t(window_size[window_id].x, 1), color_t(0, 0, 0, 0), style->accent);
	renderer::get_instance().text(renderer::get_instance().fonts.default_font, title, window_pos[window_id] + vec2_t(4, 4), style->text_active);
}

void lucid_engine::ui::end_window() {
	window_size[window_id] = handle_resizing();
	window_pos[window_id] = handle_dragging();
}