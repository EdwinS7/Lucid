#pragma once

struct vec3_t {
    double x, y, z;

    vec3_t() {}
    vec3_t(double _x, double _y, double _z)
        : x(_x), y(_y), z(_z) {}
};