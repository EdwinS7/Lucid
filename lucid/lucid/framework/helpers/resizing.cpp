#include "../../lucid.h"

struct resizing_info {
	bool resizing;
	bool outside_bounds;
	vec2_t difference;
};

std::map<int, resizing_info> info;

vec2_t lucid_engine::ui::handle_resizing() {
	bool another_resizing = std::ranges::any_of(info, [this](const auto& item) {
		return window_id != item.first && item.second.resizing;
	});

	if (is_dragging() || another_resizing)
		return window_size[window_id];

	vec2_t area{ 10, 10 };

	bool held = g_input.get( )->is_key_held(VK_LBUTTON);
	bool inside_bounds = g_input.get( )->mouse_hovering_rect(window_pos[window_id] + window_size[window_id] - area, area);

	auto& window_info = info[window_id];
	if (!window_info.outside_bounds && ((held && !inside_bounds) || hovering_element))
		window_info.outside_bounds = true;
	else if (window_info.outside_bounds && !held)
		window_info.outside_bounds = false;

	if (!window_info.resizing && !window_info.outside_bounds && held && inside_bounds) {
		window_info.difference = g_input.get( )->mouse_pos - (window_pos[window_id] + window_size[window_id]);
		window_info.resizing = true;
	}
	else if (window_info.resizing && !held)
		window_info.resizing = false;

	if (!window_info.resizing)
		return window_size[window_id];

	vec2_t new_size = (g_input.get( )->mouse_pos - window_pos[window_id]) - window_info.difference;

	new_size = vec2_t{ std::max(new_size.x, window_min_size[window_id].x), std::max(new_size.y, window_min_size[window_id].y) };

	g_input.get( )->cursor_style = LoadCursor(NULL, IDC_SIZENWSE);

	return new_size;
}

bool lucid_engine::ui::is_resizing() {
	bool another_resizing = std::ranges::any_of(info, [this](const auto& item) {
		return item.second.resizing;
	});

	return another_resizing;
}

bool lucid_engine::ui::is_this_resizing() {
	return info[window_id].resizing;
}