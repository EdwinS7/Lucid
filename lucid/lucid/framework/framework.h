#pragma once

#include "../lucid.h"
#include "containers/containers.h"

struct style_t {
	float		m_alpha{ 255.f };

	int			m_window_rounding{ 5 },
				m_group_rounding{ 5 },
				m_element_rounding{ 0 },
				m_group_header_size{ 25 },
				m_group_padding{ 10 },
				m_group_spacing{ 4 };

	color_t		m_accent{ color_t(255, 150, 150) },

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

struct text_box_info_t {
	bool in_use{ false };
	float press_time{ 0.f };

};

namespace lucid_engine {
	class ui {
	public:
		containers::group_box*	m_group{ };
		containers::window*		m_window{ };

		int			m_window_id{ -1 },
					m_group_id{ -1 };

		bool		m_hovering_element{ false },
					m_hovering_popup{ false };

		bool is_hovering_element();
		bool is_hovering_popup();

		vec2_t handle_dragging(containers::window* node);
		bool is_dragging();
		bool is_this_dragging();

		vec2_t handle_resizing(containers::window* node);
		bool is_resizing();
		bool is_this_resizing();

		void set_tabs_pos(vec2_t pos);
		void add_tab(const char* title);
		void handle_tabs();
		int get_tab_index();

		bool button(const char* title);
		void slider_int(const char* title, int min, int max, int* value);
		void slider_float(const char* title, float min, float max, float* value);
		void check_box(const char* title, bool* state);
		void label(const char* title, color_t color);
		void bullet(const char* title, color_t color);
		void popup(const char* title, vec2_t pos, color_t color);
		void combo_box(const char* title, int* value, std::vector<const char*> options);
		void multi_combo_box(const char* title, std::vector<bool>* selected, std::vector<const char*> options);
		bool text_box(const char* title, std::string& data);

		vec2_t get_window_pos();
		vec2_t get_window_size();

		style_t* get_style();

		void reset();

	private:
		std::vector<tab_info_t>			m_tabs;
		int								m_tab{ 0 };

		std::map<int, slider_info_t>	m_slider_info{ };
		int								m_slider_id{ 0 };

		std::map<int, combo_info_t>		m_combo_info{ },
										m_multi_combo_info{ };

		std::map<int, text_box_info_t>	m_text_box_info{ };

		int								m_combo_id{ 0 },
										m_multi_combo_id{ 0 },
										m_text_box_id{ 0 };

		style_t*						m_style = new style_t;

	};

	inline const auto g_ui = std::make_unique< ui >();
}