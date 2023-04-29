#pragma once

#include "../lucid.h"

enum key_style {
	key_hold, key_press
};

struct key_data {
	key_style m_style{ };
	bool	  m_on{ false };
};

namespace lucid_engine {
	class input {
	public:
		vec2_t					m_mouse_pos{ };
		float					m_mouse_wheel_delta{ 0.f };
		std::map<int, key_data> m_key_info{ };
		HCURSOR					m_cursor_style{ };

		bool button_behavior(int key,
			key_style style, vec2_t pos, vec2_t size);
		bool rect_clipping_rect(vec2_t pos
			, vec2_t size, vec2_t _pos, vec2_t _size);
		bool point_hovering_rect(vec2_t point, vec2_t pos, vec2_t size);
		bool mouse_hovering_rect(vec2_t pos, vec2_t size);
		bool is_key_held(int key);
		bool is_key_pressed(int key);
	};
	inline const auto g_input = std::make_unique< input >();
}