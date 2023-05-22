#include "../lucid.h"

float lucid_engine::math::map(float x, float in_min, float in_max, float out_min, float out_max) {
	return std::clamp((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min, out_min, out_max);
}

float lucid_engine::math::bezier(float p0, float p1, float p2, float t) {
	return (1.f - t) * (1.f - t) * p0 + 2.f * (1.f - t) * t * p1 + t * t * p2;
}