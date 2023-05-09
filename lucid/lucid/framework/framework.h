#pragma once

#include "../lucid.h"
#include "containers/containers.h"

struct style_t {
	float	m_alpha{ 255.f };

	int		m_window_rounding{ 5 },
				m_group_rounding{ 5 },
				m_element_rounding{ 0 },
				m_group_header_size{ 25 },
				m_group_padding{ 10 },
				m_group_spacing{ 4 };

	color_t m_accent{ color_t(124, 110, 255) },

				m_window_background{ color_t(20, 20, 20) },
				m_window_outline{ color_t(35, 35, 35) },
				m_window_header{ color_t(28, 28, 28) },

				m_panel_background{ color_t(18, 18, 18) },

				m_group_background{ color_t(18, 18, 18) },
				m_group_outline{ color_t(35, 35, 35) },
				m_group_header{ color_t(28, 28, 28) },

				m_element_active{ m_accent },
				m_element_inactive{ color_t(20, 20, 20, 255) },
				m_element_outline{ color_t(0, 0, 0) },
				m_element_in_outline{ color_t(35, 35, 35) },

				m_text_active{ color_t(240, 240, 240) },
				m_text_hovered{ color_t(255, 255, 255) },
				m_text_inactive{ color_t(179, 179, 179) };
};

struct tab_info_t {
	const char* m_title{ };

	tab_info_t(const char* title)
		: m_title(title) {}
};

struct slider_info_t {
	bool in_use{ false };
};

struct combo_info_t {
	bool in_use{ false };
};

namespace lucid_engine {
	class ui {
	private:
		std::map<int, bool>					m_this_window_setup{ };
		std::map<int, vec2_t>				m_window_pos{ },
														m_window_min_size{ },
														m_window_size{ };

		std::vector<tab_info_t>			m_tabs;
		int											m_tab{ 0 };

		std::map<int, slider_info_t>		m_slider_info{ };
		int											m_slider_id{ 0 };

		std::map<int, combo_info_t>  m_combo_info{ },
														m_multi_combo_info{ };
		int											m_combo_id{ 0 },
														m_multi_combo_id{ 0 };

		bool											m_hovering_element{ false };
		bool											m_hovering_popup{ false };

		style_t*										m_style = new style_t;

	public:
		int	m_window_id{ -1 },
				m_group_id{ -1 };

		bool is_hovering_element();
		bool is_hovering_popup();

		vec2_t handle_dragging();
		bool is_dragging();
		bool is_this_dragging();

		vec2_t handle_resizing();
		bool is_resizing();
		bool is_this_resizing();

		void create_window(const char* title, vec2_t pos, vec2_t min_size, vec2_t default_size);
		void end_window();

		void set_tabs_pos(vec2_t pos);
		void add_tab(const char* title);
		void handle_tabs();
		int get_tab_index();

		void spacing(vec2_t spacing = vec2_t(0, 10));

		bool button(group_box* node, const char* title);
		void slider_int(group_box* node, const char* title, int min, int max, int* value);
		void slider_float(group_box* node, const char* title, float min, float max, float* value);
		void check_box(group_box* node, const char* title, bool* state);
		void label(group_box* node, const char* title, color_t color);
		void popup(const char* title, vec2_t pos, color_t color);
		void combo_box(group_box* node, const char* title, int* value, std::vector<const char*> options);
		void multi_combo_box(group_box* node, const char* title, std::vector<bool>* selected, std::vector<const char*> options);

		vec2_t get_window_pos();
		vec2_t get_window_size();

		style_t* get_style();

		void reset();
	};
	inline const auto g_ui = std::make_unique< ui >();
}