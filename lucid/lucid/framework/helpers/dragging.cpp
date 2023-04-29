#include "../../lucid.h"

struct dragging_info {
	bool dragging;
	bool outside_bounds;
	vec2_t difference;
};

std::map<int, dragging_info> info;

vec2_t lucid_engine::ui::handle_dragging() {
	bool another_dragging = std::ranges::any_of(info, [this](const auto& item) {
		return window_id != item.first && item.second.dragging;
	});

	if (is_resizing() || another_dragging)
		return window_pos[window_id];

	bool held = g_input.get( )->is_key_held(VK_LBUTTON);
	bool inside_bounds = g_input.get( )->mouse_hovering_rect(window_pos[window_id], window_size[window_id]);

	auto& window_info = info[window_id];
	if (!window_info.outside_bounds && ((held && !inside_bounds) || hovering_element))
		window_info.outside_bounds = true;
	else if (window_info.outside_bounds && !held)
		window_info.outside_bounds = false;

	if (!window_info.dragging && !window_info.outside_bounds && held && inside_bounds) {
		window_info.difference = g_input.get( )->mouse_pos - window_pos[window_id];
		window_info.dragging = true;
	}
	else if (window_info.dragging && !held)
		window_info.dragging = false;

	if (!window_info.dragging)
		return window_pos[window_id];

	vec2_t new_pos = g_input.get( )->mouse_pos - window_info.difference;

	return new_pos;
}

bool lucid_engine::ui::is_dragging() {
	bool another_dragging = std::ranges::any_of(info, [this](const auto& item) {
		return item.second.dragging;
	});

	return another_dragging;
}

bool lucid_engine::ui::is_this_dragging() {
	return info[window_id].dragging;
}