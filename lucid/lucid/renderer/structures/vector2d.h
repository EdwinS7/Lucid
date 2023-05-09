#pragma once

struct vec2_t {
    double  x = 0,
            y = 0;

    vec2_t() { }
    vec2_t(double _x, double _y)
        : x(_x), y(_y) {}

    vec2_t add(double x, double y) {
        this->x += x;
        this->y += y;
        return *this;
    }

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