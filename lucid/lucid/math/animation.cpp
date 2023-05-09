#include "../lucid.h"

double func_linear(double x, double to, double speed_multiplier) {
	return std::clamp(x * lucid_engine::g_io.get()->m_delta_time * speed_multiplier, x, to);
}

double func_lerp(double from, double to, double speed_multiplier) {
	return from + (to - from) * lucid_engine::g_io.get()->m_delta_time * speed_multiplier;
}

double lucid_engine::math::animate(animation_type animation, double from, double to, double speed_multiplier) {
	double x = 0.0;

	switch (animation) {
	case animation_type::linear:
		x = func_linear(from, to, speed_multiplier);
		break;
	case animation_type::lerp:
		x = func_lerp(from, to, speed_multiplier);
		break;
	}

	return x;
}