#include "input.h"

bool lucid_engine::input::button_behavior(int key, key_style style, vec2_t pos, vec2_t size) {
	if ( key_info[key].on && key_info[key].style == style &&
		 mouse_pos.x >= pos.x && mouse_pos.x <= pos.x + size.x &&
		 mouse_pos.y >= pos.y && mouse_pos.y <= pos.y + size.y
		)
		return true;

	return false;
}

bool lucid_engine::input::rect_clipping_rect(vec2_t pos, vec2_t size, vec2_t _pos, vec2_t _size) {
	bool hovered = false;

	std::vector<vec2_t> points = {
		{pos.x, pos.y},
		{pos.x + size.x, pos.y},
		{pos.x, pos.y + size.y},
		{pos.x + size.x, pos.y + size.y}
	};

	for (int i = 0; i < points.size(); i++)
		if (point_hovering_rect(points[i], _pos, _size))
			hovered = true;

	return hovered;
}

bool lucid_engine::input::point_hovering_rect(vec2_t point, vec2_t pos, vec2_t size) {
	if ((point.x >= pos.x && point.x <= pos.x + size.x) && (point.y >= pos.y && point.y <= pos.y + size.y))
		return true;

	return false;
}

bool lucid_engine::input::mouse_hovering_rect(vec2_t pos, vec2_t size) {
	if ( mouse_pos.x >= pos.x && mouse_pos.x <= pos.x + size.x && mouse_pos.y >= pos.y && mouse_pos.y <= pos.y + size.y )
		return true;

	return false;
}

bool lucid_engine::input::is_key_held(int key) {
	if (key_info[key].on && key_info[key].style == key_style::hold)
		return true;

	return false;
}

bool lucid_engine::input::is_key_pressed(int key) {
	if (key_info[key].on && key_info[key].style == key_style::press)
		return true;

	return false;
}