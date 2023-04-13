#include "../lucid.h"

struct dragging_info {
	bool dragging;
	vec2_t difference;
};

std::map<int, dragging_info> info;

vec2_t lucid_engine::ui::handle_dragging() {
	if (!info[window_id].dragging && input::get_instance().button_behavior(VK_RETURN, hold, window_pos[window_id], window_size[window_id])) {
		info[window_id].difference = input::get_instance().mouse_pos - window_pos[window_id];
		info[window_id].dragging = true;
	}
	else if (info[window_id].dragging && !input::get_instance().is_key_held(VK_RETURN))
		info[window_id].dragging = false;

	if (!info[window_id].dragging)
		return window_pos[window_id];

	vec2_t new_pos = input::get_instance().mouse_pos - info[window_id].difference;

	return new_pos;
}