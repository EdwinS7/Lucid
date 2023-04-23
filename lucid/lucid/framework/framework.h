#pragma once

#include "../lucid.h"

struct style_t {
	float alpha = 255.f;
	int padding = 8;

	int window_rounding = 5;
	int child_rounding = 10;
	int element_rounding = 5;

	//colors (r, g, b, a = 255)
	color_t accent = color_t(166, 147, 243);

	color_t window_background = color_t(20, 20, 20);
	color_t window_outline = color_t(33, 33, 33);
	color_t window_header = color_t(28, 28, 28);

	color_t panel_background = color_t(18, 18, 18);

	color_t child_background = color_t(18, 18, 18);
	color_t child_outline = color_t(33, 33, 33);
	color_t child_header = color_t(28, 28, 28);

	color_t element_active = accent;
	color_t element_inactive = color_t(0, 0, 0, 0); //unused
	color_t element_outline = color_t(0, 0, 0);

	color_t text_active = color_t(240, 240, 240);
	color_t text_hovered = color_t(255, 255, 255);
	color_t text_inactive = color_t(74, 74, 74);
};

namespace lucid_engine {
	class ui : public singleton<ui> {
	private:
		int window_id = -1;

		std::map<int, bool> this_window_setup{ };
		std::map<int, vec2_t> window_pos, window_min_size, window_size{ };

		style_t* style = new style_t;

	public:
		vec2_t handle_dragging();
		bool is_dragging();
		bool is_this_dragging();

		vec2_t handle_resizing();
		bool is_resizing();
		bool is_this_resizing();

		void create_window(const char* title, vec2_t pos, vec2_t min_size, vec2_t default_size);
		void end_window();

		void reset();
	};
}