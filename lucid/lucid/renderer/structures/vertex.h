#pragma once

struct vertex_t {
	float x, y, z, rhw;
	D3DCOLOR color;

	vertex_t() { }
	vertex_t(float _x, float _y, float _z, float _rhw, D3DCOLOR _color)
		: x(_x), y(_y), z(_z), rhw(_rhw), color(_color) {}
};