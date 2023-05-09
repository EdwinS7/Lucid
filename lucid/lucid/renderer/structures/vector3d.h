#pragma once

struct vec3_t {
    double  x = 0,
            y = 0,
            z = 0;

    vec3_t() { }
    vec3_t(double _x, double _y, double _z)
        : x(_x), y(_y), z(_z) {}
};