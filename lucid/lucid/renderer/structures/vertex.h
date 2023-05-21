#pragma once

struct vertex_t {
    float x, y, z, rhw;
    DWORD color;

    /* texture coordinates */
    float u, v;

    vertex_t() {}
    vertex_t(float x, float y, float z, DWORD color, float u = 0.f, float v = 0.f, float rhw = 1.f)
        : x(x), y(y), z(z), color(color), u(u), v(v), rhw(rhw) {}
};