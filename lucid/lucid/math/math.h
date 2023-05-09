#pragma once
#include "../lucid.h"

enum animation_type {
	lerp = 0,
	linear = 1
};

namespace lucid_engine {
	class math {
	public:
		double map(float x, float in_min, float in_max, float out_min, float out_max);
		double animate(animation_type animation, double from, double to, double speed_multiplier = 1.f);

	};

	inline const auto g_math = std::make_unique< math >();
}