#pragma once

struct vertex3d_t {
	float x, y, z;
	DWORD color;

	vertex3d_t(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z) {}
};