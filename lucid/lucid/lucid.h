#pragma once

#include <d3d9.h>
#include <d3dx9.h>

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
#include "window/wnd.h"
#include "graphics/graphics.h"

#include "renderer/color/color.h"
#include "renderer/vector/vector2d.h"
#include "renderer/vector/vector3d.h"
#include "renderer/vector/vector4d.h"
#include "renderer/vertex/vertex.h"
#include "renderer/matrix/matrix.h"
#include "renderer/drawdata/draw_data.h"
#include "renderer/drawdata/compiled_draw_data.h"

#include "input/input.h"
#include "renderer/renderer.h"

namespace lucid_engine {
	class io : public singleton<io> {
	public:
		void create();
		void update();
		void reset();

		template<class T>
		constexpr const T& map(const T& x, const T& in_min, const T& in_max, const T& out_min, const T& out_max) {
			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}

		int random_int(int min, int max) {
			return rand() % max + min;
		}

		float random_float(float min, float max) {
			return static_cast<float>(rand() % static_cast<int>(max) + static_cast<int>(min));
		}

		float delta_time;
		float real_time;
		int frame_rate;
		int frame_rate_average;
	};
}