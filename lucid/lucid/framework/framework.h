#pragma once

#include "../lucid.h"

struct style_t {
	float alpha = 255.f;

	int window_rounding = 5;
	int group_rounding = 5;
	int element_rounding = 3;
	int group_padding = 10;
	int group_spacing = 4;

	//colors (r, g, b, a = 255)
	color_t accent = color_t(207, 157, 173);

	color_t window_background = color_t(20, 20, 20);
	color_t window_outline = color_t(0, 0, 0);
	color_t window_header = color_t(28, 28, 28);

	color_t panel_background = color_t(18, 18, 18);

	color_t group_background = color_t(18, 18, 18);
	color_t group_outline = color_t(0, 0, 0);
	color_t group_header = color_t(28, 28, 28);

	color_t element_active = accent;
	color_t element_inactive = color_t(28, 28, 28, 255); //unused
	color_t element_outline = color_t(0, 0, 0);

	color_t text_active = color_t(240, 240, 240);
	color_t text_hovered = color_t(255, 255, 255);
	color_t text_inactive = color_t(179, 179, 179);
};

struct tab_info {
	const char* icon;
	const char* title;

	tab_info(const char* _icon, const char* _title)
		: icon(_icon), title(_title) {}
};

struct group_info {
	bool hovered;
	float scroll_abs;
	float scroll;

	vec2_t og_elements_pos{ };

	group_info() { }
	group_info(bool _hovered, float _scroll_abs, float _scroll, vec2_t _og_elements_pos)
		: hovered(_hovered), scroll_abs(_scroll_abs), scroll(_scroll), og_elements_pos(_og_elements_pos) {}
};

namespace lucid_engine {
	class ui : public singleton<ui> {
	private:
		int window_id = -1;
		int group_id = -1;

		std::map<int, bool> this_window_setup{ };
		std::map<int, vec2_t> window_pos, window_min_size, window_size{ };

		std::map<int, bool> this_group_setup{ };
		std::map<int, vec2_t> group_pos, group_min_size, group_size{ };
		std::map<int, group_info> groups;

		std::vector<tab_info> tabs;
		int tab = 0;

		bool hovering_element = false;
		vec2_t elements_pos{ };

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

		void create_group(const char* title, vec2_t pos, vec2_t min_size, vec2_t default_size);
		void end_group();

		void set_tabs_pos(vec2_t pos);
		void add_tab(const char* icon, const char* title);
		void handle_tabs();

		void check_box(const char* title, bool* state);

		vec2_t get_window_pos();
		vec2_t get_window_size();

		void reset();
	};
}