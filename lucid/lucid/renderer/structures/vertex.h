#pragma once

struct vertex_t {
    float x, y, z, rhw;
    D3DCOLOR color;
    float u, v;

    vertex_t() {}
    vertex_t(float _x, float _y, float _z, float _rhw, D3DCOLOR _color, float _u = 0.f, float _v = 0.f)
        : x(_x), y(_y), z(_z), rhw(_rhw), color(_color), u(_u), v(_v) {}
};