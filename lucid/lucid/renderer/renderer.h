#pragma once

#include "../lucid.h"

namespace lucid_engine {
	class renderer : public singleton<renderer> {
	private:


	public:
		void line(vec2_t from, vec2_t to, color_t color);
		void rectangle(vec2_t pos, vec2_t size, color_t color);
		void filled_rectangle(vec2_t pos, vec2_t size, color_t color);
	};
}