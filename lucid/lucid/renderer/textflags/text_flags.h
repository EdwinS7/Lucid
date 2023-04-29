#pragma once

#include "../../lucid.h"

enum text_alignment {
	alignment_left,
	alignment_right,
	alignment_center_x,
	alignment_center_y,
	alignment_center
};

struct text_flags_t {
	int m_alignment{ text_alignment::alignment_left };

	text_flags_t() {}
	text_flags_t(text_alignment alignment)
		: m_alignment(alignment) {}
};