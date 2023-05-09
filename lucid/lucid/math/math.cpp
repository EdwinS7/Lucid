#include "../lucid.h"

double lucid_engine::math::map(float x, float in_min, float in_max, float out_min, float out_max) {
	return std::clamp((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min, out_min, out_max);
}