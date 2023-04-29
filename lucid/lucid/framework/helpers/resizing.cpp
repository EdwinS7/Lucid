#include "../../lucid.h"

struct resizing_info {
	bool resizing;
	bool outside_bounds;
	vec2_t difference;
};

std::map<int, resizing_info> info;

vec2_t lucid_engine::ui::handle_resizing() {
	bool another_resizing = std::any_of(info.begin(), info.end(), [this](const auto& item) {
		return window_id != item.first && item.second.resizing;
	});

	if (is_dragging() || another_resizing)
		return window_size[window_id];

	vec2_t area{ 10, 10 };

	bool held = input::get_instance().is_key_held(VK_LBUTTON);
	bool inside_bounds = input::get_instance().mouse_hovering_rect(window_pos[window_id] + window_size[window_id] - area, area);

	if (!info[window_id].outside_bounds && ((held && !inside_bounds) || hovering_element))
		info[window_id].outside_bounds = true;
	else if (info[window_id].outside_bounds && !held)
		info[window_id].outside_bounds = false;

	if (!info[window_id].resizing && !info[window_id].outside_bounds && held && inside_bounds) {
		info[window_id].difference = input::get_instance().mouse_pos - (window_pos[window_id] + window_size[window_id]);
		info[window_id].resizing = true;
	}
	else if (info[window_id].resizing && !held)
		info[window_id].resizing = false;

	if (!info[window_id].resizing)
		return window_size[window_id];

	vec2_t new_size = (input::get_instance().mouse_pos - window_pos[window_id]) - info[window_id].difference;

	if (new_size.x < window_min_size[window_id].x) new_size.x = window_min_size[window_id].x;
	if (new_size.y < window_min_size[window_id].y) new_size.y = window_min_size[window_id].y;

	lucid_engine::input::get_instance().cursor_style = LoadCursor(NULL, IDC_SIZENWSE);

	return new_size;
}

bool lucid_engine::ui::is_resizing() {
	for (int i = 0; i < info.size(); i++) {
		if (info[i].resizing)
			return true;
	}

	return false;
}

bool lucid_engine::ui::is_this_resizing() {
	return info[window_id].resizing;
}