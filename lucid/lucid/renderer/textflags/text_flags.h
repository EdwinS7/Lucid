#pragma once

#include "../../lucid.h"

enum text_alignment {
	left,
	right,
	center_x,
	center_y,
	center
};

struct text_flags_t {
	int alignment = text_alignment::left;

	text_flags_t() {}
	text_flags_t(text_alignment _alignment)
		: alignment(_alignment) {}
};