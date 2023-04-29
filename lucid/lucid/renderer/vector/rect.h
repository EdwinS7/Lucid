#pragma once

struct rect_t {
	int x, y, w, h;

	rect_t() {}
	rect_t(int x, int y, int w, int h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};