#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include <vector>
#include <chrono>
#include <format>
#include <map>
#include <unordered_map>
#include <cassert>
#include <shlobj.h>
#include <array>

// freetype
#include "include/ft2build.h"
#include FT_FREETYPE_H
#include FT_BITMAP_H

// data structures
#include "renderer/structures/color.h"
#include "renderer/structures/vector2d.h"
#include "renderer/structures/vector3d.h"
#include "renderer/structures/vertex.h"
#include "renderer/structures/font.h"
#include "renderer/structures/texture.h"
#include "renderer/structures/draw_data.h"
#include "renderer/structures/compiled_draw_data.h"

// library's
#include "math/math.h"
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

		float m_elapsed_time{ 0.f };

	};

	inline const auto g_io = std::make_unique< io >();
}