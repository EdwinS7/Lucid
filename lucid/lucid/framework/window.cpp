#include "../lucid.h"

void lucid_engine::ui::create_window(const char* title, vec2_t pos, vec2_t size) {
	window_id++;
	if (!this_window_setup[window_id]) {
		window_pos[window_id] = pos;
		window_size[window_id] = size;
		this_window_setup[window_id] = true;
	}

	renderer::get_instance().shadow_filled_rounded_rectangle(window_pos[window_id], window_size[window_id], color_t(34, 108, 199), 25);
}

void lucid_engine::ui::end_window() {
	window_pos[window_id] = handle_dragging();
}