#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <DirectXMath.h>

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
#include "renderer/color/color.h"
#include "renderer/vector/rect.h"
#include "renderer/vector/vector2d.h"
#include "renderer/vector/vector3d.h"
#include "renderer/vector/vector4d.h"
#include "renderer/vertex/vertex.h"
#include "renderer/matrix/matrix.h"
#include "renderer/clip/clip.h"
#include "renderer/font/font.h"
#include "renderer/textflags/text_flags.h"
#include "renderer/textinfo/text_info.h"
#include "renderer/drawdata/draw_data.h"
#include "renderer/drawdata/compiled_draw_data.h"

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
	};
	inline const auto g_animations = std::make_unique< animations >();
}