#include "../../lucid.h"

struct dragging_info {
	bool dragging;
	bool outside_bounds;
	vec2_t difference;
};

std::map<int, dragging_info> info;

vec2_t lucid_engine::ui::handle_dragging(containers::window* node) {
	bool another_dragging = std::ranges::any_of(info, [this](const auto& item) {
		return m_window_id != item.first && item.second.dragging;
	});

	if (is_resizing() || another_dragging)
		return node->m_pos;

	bool held = g_input->is_key_held(VK_LBUTTON);
	bool inside_bounds = g_input->mouse_hovering_rect(node->m_pos, node->m_size);

	dragging_info& window_info = info[m_window_id];
	if (!window_info.outside_bounds && ((held && !inside_bounds) || m_hovering_element || is_hovering_popup()))
		window_info.outside_bounds = true;
	else if (window_info.outside_bounds && !held)
		window_info.outside_bounds = false;

	if (!window_info.dragging && !window_info.outside_bounds && held && inside_bounds) {
		window_info.difference = g_input->m_mouse_pos - node->m_pos;
		window_info.dragging = true;
	}
	else if (window_info.dragging && !held)
		window_info.dragging = false;

	if (!window_info.dragging)
		return node->m_pos;

	vec2_t new_pos = g_input->m_mouse_pos - window_info.difference;

	return new_pos;
}

bool lucid_engine::ui::is_dragging() {
	bool another_dragging = std::ranges::any_of(info, [this](const auto& item) {
		return item.second.dragging;
	});

	return another_dragging;
}

bool lucid_engine::ui::is_this_dragging() {
	return info[m_window_id].dragging;
}