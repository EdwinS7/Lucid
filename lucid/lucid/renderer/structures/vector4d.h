#pragma once

struct vec4_t {
    double  x = 0,
            y = 0,
            z = 0,
            rhw = 0;

    vec4_t() { }
    vec4_t(double _x, double _y, double _z, double _rhw)
        : x(_x), y(_y), z(_z), rhw(_rhw) {}
};