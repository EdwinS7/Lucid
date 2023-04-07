#pragma once

struct vec4_t {
    float x, y, z, rhw;

    vec4_t() {}
    vec4_t(float _x, float _y, float _z, float _rhw)
        : x(_x), y(_y), z(_z), rhw(_rhw) {}
};