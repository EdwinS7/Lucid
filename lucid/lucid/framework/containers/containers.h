#pragma once
#include "../../lucid.h"

class group_box {
public:
	group_box(const char* title, vec2_t pos, vec2_t size);

	void handle_render();
	void handle_input();
	void destroy();

	bool element_visible(vec2_t pos, vec2_t size);

public:
	const char*	m_title;
	vec2_t			m_pos = {},
						m_size = {},
						m_elements_pos{};

	bool				m_hovered{ false };
};