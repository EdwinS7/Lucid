#pragma once

struct vec2_t {
    int x, y;

    vec2_t() {}
    vec2_t(int _x, int _y)
        : x(_x), y(_y) {}

    vec2_t operator + (const vec2_t& other) {
        return vec2_t(x + other.x, y + other.y);
    }

    vec2_t operator - (const vec2_t& other) {
        return vec2_t(x - other.x, y - other.y);
    }

    void operator += (const vec2_t& other) {
        x = x + other.x;
        y = y + other.y;
    }

    vec2_t operator -= (const vec2_t& other) {
        x = x - other.x;
        y = y - other.y;
    }

    bool operator < (const vec2_t& other) {
        return (x < other.x && y < other.y);
    }

    bool operator > (const vec2_t& other) {
        return (x > other.x && y > other.y);
    }

    bool operator <= (const vec2_t& other) {
        return (x <= other.x && y <= other.y);
    }

    bool operator >= (const vec2_t& other) {
        return (x >= other.x && y >= other.y);
    }
};