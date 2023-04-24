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
	renderer::get_instance().filled_rounded_rectangle(window_pos[window_id] + vec2_t(0, window_size[window_id].y - 45), vec2_t(window_size[window_id].x, 45), style->window_header, style->window_rounding, corner_bottom);
	renderer::get_instance().filled_rectangle(window_pos[window_id] + vec2_t(0, 26), vec2_t(window_size[window_id].x, 1), style->accent);
	renderer::get_instance().filled_rectangle(window_pos[window_id] + vec2_t(0, window_size[window_id].y - 46), vec2_t(window_size[window_id].x, 1), style->accent);
	renderer::get_instance().rounded_rectangle(window_pos[window_id], window_size[window_id], style->window_outline, style->window_rounding);

	vec2_t text_size = renderer::get_instance().get_text_size(renderer::get_instance().fonts.default_font, title);
	renderer::get_instance().text(renderer::get_instance().fonts.default_font, title, window_pos[window_id] + vec2_t((window_size[window_id].x / 2) - text_size.x / 2, 6), style->text_active);
}

void lucid_engine::ui::end_window() {
	window_size[window_id] = handle_resizing();
	window_pos[window_id] = handle_dragging();
}

vec2_t lucid_engine::ui::get_window_pos() {
	return window_pos[window_id];
}

vec2_t lucid_engine::ui::get_window_size() {
	return window_size[window_id];
}