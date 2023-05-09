#pragma once

struct rect_t {
	double	x = 0,
			y = 0,
			w = 0,
			h = 0;

	rect_t() { }
	rect_t(double x, double y, double w, double h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};