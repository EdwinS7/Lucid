#pragma once

struct vec2_t {
    float x, y;

    vec2_t() {}
    vec2_t(float _x, float _y)
        : x(_x), y(_y) {}

    vec2_t& operator + (const vec2_t& other) {
        x += other.x;
        y += other.y;

        return *this;
    }

    vec2_t& operator - (const vec2_t& other) {
        x -= other.x;
        y -= other.y;

        return *this;
    }
};