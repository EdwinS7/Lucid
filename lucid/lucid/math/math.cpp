#include "../lucid.h"

float lucid_engine::math::map(float x, float in_min, float in_max, float out_min, float out_max) {
	return std::clamp((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min, out_min, out_max);
}

float lucid_engine::math::bezier(float start, float control, float end, float parameter) {
    float t1 = (1.f - parameter) * (1.f - parameter) * start;
    float t2 = 2.f * (1.f - parameter) * parameter * control;
    float t3 = parameter * parameter * end;

    float result = t1 + t2 + t3;

    return result;
}

vec2_t lucid_engine::math::bezier_vec2_t(vec2_t start, vec2_t control, vec2_t end, float parameter) {
    return vec2_t(bezier(start.x, control.x, end.x, parameter), bezier(start.y, control.y, end.y, parameter));
}