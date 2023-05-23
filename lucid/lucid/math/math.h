#pragma once
#include "../lucid.h"

enum animation_type {
	lerp = 0,
	linear = 1
};

namespace lucid_engine {
	class math {
	public:
		float map(float x, float in_min, float in_max, float out_min, float out_max);
		float animate(animation_type animation, float from, float to, float speed_multiplier = 1.f);
		float bezier(float start, float control, float end, float parameter);
		vec2_t bezier_vec2_t(vec2_t start, vec2_t control, vec2_t end, float parameter);
	};

	inline const auto g_math = std::make_unique< math >();
}