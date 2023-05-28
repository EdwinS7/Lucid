#include "input.h"

bool lucid_engine::input::button_behavior(int key, key_style style, vec2_t pos, vec2_t size) {
	if (const auto [minX, minY, maxX, maxY] = std::tuple{ pos.x, pos.y, pos.x + size.x, pos.y + size.y };
		m_mouse_pos.x >= minX && m_mouse_pos.x <= maxX &&
		m_mouse_pos.y >= minY && m_mouse_pos.y <= maxY) {

		if (style == key_style::key_press)
			return is_key_pressed(key);
		else
			return is_key_held(key);
	}

	return false;
}

bool lucid_engine::input::rect_clipping_rect(vec2_t pos, vec2_t size, vec2_t _pos, vec2_t _size) {
	const std::array<vec2_t, 4> points = {
		vec2_t{ pos.x, pos.y },
		vec2_t{ pos.x + size.x, pos.y },
		vec2_t{ pos.x, pos.y + size.y },
		vec2_t{ pos.x + size.x, pos.y + size.y }
	};

	for (const vec2_t& point : points) {
		if (point_hovering_rect(point, _pos, _size))
			return true;
	}

	return false;
}

bool lucid_engine::input::point_hovering_rect(vec2_t point, vec2_t pos, vec2_t size) {
	return (point.x >= pos.x && point.x <= pos.x + size.x && point.y >= pos.y && point.y <= pos.y + size.y);
}

bool lucid_engine::input::mouse_hovering_rect(vec2_t pos, vec2_t size) {
	return point_hovering_rect(m_mouse_pos, pos, size);
}

bool lucid_engine::input::is_key_held(int key) {
	if (g_ui->is_hovering_popup())
		return false;

	return m_key_info[key] == key_style::key_down;
}

bool lucid_engine::input::is_key_pressed(int key) {
	if (g_ui->is_hovering_popup())
		return false;

	if (m_key_info[key] == key_style::key_press) {
		m_key_info[key] = key_style::key_up;
		return true;
	}

	return false;
}