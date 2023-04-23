#include "../../lucid.h"

struct dragging_info {
	bool dragging;
	bool outside_bounds;
	vec2_t difference;
};

std::map<int, dragging_info> info;

vec2_t lucid_engine::ui::handle_dragging() {
	bool another_dragging = false;

	for (int i = info.size(); i > 0; i--) {
		if (window_id != i && info[i].dragging)
			another_dragging = true;
	}

	if (is_resizing() || another_dragging)
		return window_pos[window_id];

	bool held = input::get_instance().is_key_held(VK_LBUTTON);
	bool inside_bounds = input::get_instance().mouse_hovering_rect(window_pos[window_id], window_size[window_id]);

	if (!info[window_id].outside_bounds && held && !inside_bounds)
		info[window_id].outside_bounds = true;
	else if (info[window_id].outside_bounds && !held)
		info[window_id].outside_bounds = false;

	if (!info[window_id].dragging && !info[window_id].outside_bounds && held && inside_bounds) {
		info[window_id].difference = input::get_instance().mouse_pos - window_pos[window_id];
		info[window_id].dragging = true;
	}
	else if (info[window_id].dragging && !held)
		info[window_id].dragging = false;

	if (!info[window_id].dragging)
		return window_pos[window_id];

	vec2_t new_pos = input::get_instance().mouse_pos - info[window_id].difference;

	return new_pos;
}

bool lucid_engine::ui::is_dragging() {
	for (int i = 0; i < info.size(); i++) {
		if (info[i].dragging)
			return true;
	}

	return false;
}

bool lucid_engine::ui::is_this_dragging() {
	return info[window_id].dragging;
}