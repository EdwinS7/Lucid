#pragma once

struct rect_t {
	float x, y, w, h;

	rect_t() {}
	rect_t(float x, float y, float w, float h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};