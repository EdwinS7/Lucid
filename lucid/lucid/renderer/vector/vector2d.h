#pragma once

struct vec2_t {
    float x, y;

    vec2_t() {}
    vec2_t(float _x, float _y)
        : x(_x), y(_y) {}

    vec2_t operator + (const vec2_t& other) {
        return vec2_t(x + other.x, y + other.y);
    }

    vec2_t operator - (const vec2_t& other) {
        return vec2_t(x - other.x, y - other.y);
    }
};