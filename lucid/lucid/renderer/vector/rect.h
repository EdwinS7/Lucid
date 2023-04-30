#pragma once

struct rect_t {
	double x, y, w, h;

	rect_t() {}
	rect_t(double x, double y, double w, double h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};