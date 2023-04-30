#include "input.h"

bool lucid_engine::input::button_behavior(int key, key_style style, vec2_t pos, vec2_t size) {
	if (m_mouse_pos.x >= pos.x && m_mouse_pos.x <= pos.x + size.x &&
		m_mouse_pos.y >= pos.y && m_mouse_pos.y <= pos.y + size.y ) {

		if (style == key_style::key_press )
			return is_key_pressed(key);
		else
			return is_key_held(key);
	}

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

	for (vec2_t& point : points)
		if (point_hovering_rect(point, _pos, _size))
			hovered = true;

	return hovered;
}

bool lucid_engine::input::point_hovering_rect(vec2_t point, vec2_t pos, vec2_t size) {
	if ((point.x >= pos.x && point.x <= pos.x + size.x) && (point.y >= pos.y && point.y <= pos.y + size.y))
		return true;

	return false;
}

bool lucid_engine::input::mouse_hovering_rect(vec2_t pos, vec2_t size) {
	if ( m_mouse_pos.x >= pos.x && m_mouse_pos.x <= pos.x + size.x && m_mouse_pos.y >= pos.y && m_mouse_pos.y <= pos.y + size.y )
		return true;

	return false;
}

bool lucid_engine::input::is_key_held(int key) {
	if (m_key_info.at(key).m_on && m_key_info.at(key).m_style == key_style::key_hold)
		return true;

	return false;
}

std::map<int, bool> old_frame_key_pressed{ };
bool lucid_engine::input::is_key_pressed(int key) {
	bool pressed = false;

	if (g_ui.get()->is_hovering_popup())
		return false;

	if (m_key_info.at(key).m_on && !old_frame_key_pressed.at(key)) {
		old_frame_key_pressed.at(key) = true;
		pressed = true;
	}
	else if (!m_key_info.at(key).m_on && old_frame_key_pressed.at(key))
		old_frame_key_pressed.at(key) = false;

	return pressed;
}