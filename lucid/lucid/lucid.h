#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")

#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>
#include <chrono>
#include <format>
#include <map>

/* lucid engine backend */
#include "renderer/structures/color.h"
#include "renderer/structures/rect.h"
#include "renderer/structures/vector2d.h"
#include "renderer/structures/vector3d.h"
#include "renderer/structures/vector4d.h"
#include "renderer/structures/vertex.h"
#include "renderer/structures/matrix.h"
#include "renderer/structures/clip.h"
#include "renderer/structures/font.h"
#include "renderer/structures/text_flags.h"
#include "renderer/structures/text_info.h"
#include "renderer/structures/draw_data.h"
#include "renderer/structures/compiled_draw_data.h"

#include "window/wnd.h"
#include "input/input.h"
#include "graphics/graphics.h"
#include "renderer/renderer.h"

#include "framework/framework.h"

namespace lucid_engine {
	class io {
	public:
		void create();
		void update();
		void reset();
		void demo_window(bool open);

		float m_delta_time{ 0.f },
			  m_real_time{ 0.f };
		int	  m_frame_rate{ 0 },
			  m_frame_rate_average{ 0 };
	};
	inline const auto g_io = std::make_unique< io >();

	class animations {
	public:
		float linear(float x, float to, float delta) {
			return x < to ? x + (1 * delta) : x - (1 * delta);
		}

		float lerp(float x, float to, float delta) {
			return x + (to - x) * delta;
		}

		vec2_t vec2_lerp(vec2_t x, vec2_t to, float delta) {
			return vec2_t(lerp(x.x, to.x, delta), lerp(x.y, to.y, delta));
		}

		float in(float x, float delta) {
			return x * (x * delta);
		}

	    float map(float x, float in_min, float in_max, float out_min, float out_max) {
			return std::clamp((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min, out_min, out_max);
		}
	};
	inline const auto g_animations = std::make_unique< animations >();
}