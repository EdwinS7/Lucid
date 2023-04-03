#pragma once

struct vertex2d_t {
	float x, y;
	DWORD color;

	vertex2d_t(float _x, float _y, DWORD _color)
		: x(_x), y(_y), color(_color) {}
};