#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")

#include <Windows.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <format>
#include <map>

template <typename T>
class singleton
{
protected:
	singleton() { }
	~singleton() { }

	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;

	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;
public:
	static T& get_instance() {
		static T inst { };

		return inst;
	}
};

/* lucid engine backend */
#include "renderer/color/color.h"
#include "renderer/vector/vector2d.h"
#include "renderer/vector/vector3d.h"
#include "renderer/vector/vector4d.h"
#include "renderer/vertex/vertex.h"
#include "renderer/matrix/matrix.h"
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
	class io : public singleton<io> {
	public:
		void create();
		void update();
		void reset();
		void demo_window(bool open);

		float delta_time;
		float real_time;
		int frame_rate;
		int frame_rate_average;
	};
}