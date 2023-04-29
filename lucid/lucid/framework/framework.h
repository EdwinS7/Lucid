#pragma once

#include "../lucid.h"

struct style_t {
	float	m_alpha{ 255.f };

	int		m_window_rounding{ 5 },
			m_group_rounding{ 5 },
			m_element_rounding{ 3 },
			m_group_padding{ 10 },
			m_group_spacing{ 4 };

	color_t m_accent{ color_t(207, 157, 173) },

			m_window_background{ color_t(20, 20, 20) },
			m_window_outline{ color_t(0, 0, 0) },
			m_window_header{ color_t(28, 28, 28) },

			m_panel_background{ color_t(18, 18, 18) },

			m_group_background{ color_t(18, 18, 18) },
			m_group_outline{ color_t(0, 0, 0) },
			m_group_header{ color_t(28, 28, 28) },

			m_element_active{ m_accent },
			m_element_inactive{ color_t(28, 28, 28, 255) },
			m_element_outline{ color_t(0, 0, 0) },

			m_text_active{ color_t(240, 240, 240) },
			m_text_hovered{ color_t(255, 255, 255) },
			m_text_inactive{ color_t(179, 179, 179) };
};

struct tab_info_t {
	const char* m_icon{ },
			  * m_title{ };

	tab_info_t(const char* icon, const char* title)
		: m_icon(icon), m_title(title) {}
};

struct group_info_t {
	bool  m_hovered{ false };
	float m_scroll_abs{ 0.f },
		  m_scroll{ 0.f };

	vec2_t m_original_pos{ };

	group_info_t() { }
	group_info_t(bool hovered, float scroll_abs,
		float scroll, vec2_t original_pos)
		: m_hovered(hovered), m_scroll_abs(scroll_abs),
		  m_scroll(scroll), m_original_pos(original_pos) {}
};

namespace lucid_engine {
	class ui {
	private:
		int							m_window_id{ -1 },
									m_group_id{ -1 };

		std::map<int, bool>			m_this_window_setup{ };
		std::map<int, vec2_t>		m_window_pos{ },
									m_window_min_size{ },
									m_window_size{ };

		std::map<int, bool>			m_this_group_setup{ };
		std::map<int, vec2_t>		m_group_pos{ },
									m_group_min_size{ },
									m_group_size{ };
		std::map<int, group_info_t> m_groups;

		std::vector<tab_info_t>		m_tabs;
		int							m_tab{ 0 };

		bool						m_hovering_element{ false };
		vec2_t						m_elements_pos{ };

		style_t*					m_style = new style_t;

	public:
		vec2_t handle_dragging();
		bool is_dragging();
		bool is_this_dragging();

		vec2_t handle_resizing();
		bool is_resizing();
		bool is_this_resizing();

		void create_window(const char* title,
			vec2_t pos, vec2_t min_size, vec2_t default_size);
		void end_window();

		void create_group(const char* title,
			vec2_t pos, vec2_t min_size, vec2_t default_size);
		void end_group();

		void set_tabs_pos(vec2_t pos);
		void add_tab(const char* icon, const char* title);
		void handle_tabs();

		void check_box(const char* title, bool* state);

		vec2_t get_window_pos();
		vec2_t get_window_size();

		void reset();
	};
	inline const auto g_ui = std::make_unique< ui >();
}