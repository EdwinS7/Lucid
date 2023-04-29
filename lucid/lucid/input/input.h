#pragma once

#include "../lucid.h"

enum key_style { hold, press };
struct key_data {
	key_style style;
	bool on;
};

namespace lucid_engine {
	class input {
	public:
		vec2_t mouse_pos;
		float mouse_wheel_delta;
		std::map<int, key_data> key_info;

		//used for cursor
		HCURSOR cursor_style;

		bool button_behavior(int key, key_style style, vec2_t pos, vec2_t size);
		bool rect_clipping_rect(vec2_t pos, vec2_t size, vec2_t _pos, vec2_t _size);
		bool point_hovering_rect(vec2_t point, vec2_t pos, vec2_t size);
		bool mouse_hovering_rect(vec2_t pos, vec2_t size);
		bool is_key_held(int key);
		bool is_key_pressed(int key);
	};
	inline const auto g_input = std::make_unique< input >( );
}