#pragma once

#include "../lucid.h"

#define CIRCLE_SEGMENTS 64

enum corner_flags {
	corner_none = 0 << 0,
	corner_top_left = 1 << 0,
	corner_top_right = 1 << 1,
	corner_bottom_left = 1 << 2,
	corner_bottom_right = 1 << 3,

	corner_top = corner_top_left | corner_top_right,
	corner_right = corner_top_right | corner_bottom_right,
	corner_bottom = corner_bottom_left | corner_bottom_right,
	corner_left = corner_top_left | corner_bottom_left,

	corner_all = corner_top_left | corner_top_right |
				 corner_bottom_left | corner_bottom_right
};

enum draw_list_t {
	background_draw_list = 0,
	default_draw_list,
	foreground_draw_list
};

namespace lucid_engine {
	class renderer {
	private:
		compiled_draw_data_t	 m_compiled_draw_data{ };
		std::vector<draw_data_t> m_default_draw_data{ },
								 m_background_draw_data{ },
			                     m_foreground_draw_data{ };

		IDirect3DVertexBuffer9*  m_vertex_buffer{ };
		IDirect3DIndexBuffer9*   m_index_buffer{ };

		LPDIRECT3DTEXTURE9		 m_font_texture{ };
		LPD3DXSPRITE			 m_font_sprite{ };

		clip_info_t              m_clip_info{ };

		draw_list_t              m_draw_list{ default_draw_list };

		std::vector<vec2_t> generate_circle_points(const vec2_t pos, const int radius, const int completion, const int rotation, int segments = -1);

	public:
		std::vector<font_t>		 m_fonts{ };
		font_t                   m_defualt_font{ };
		font_t                   m_logo_font{ };

		void create_objects();
		void destroy_objects();
		void render_draw_data();
		void set_draw_list(draw_list_t draw_list);
		void push_clip(const vec2_t pos, const vec2_t size);
		void pop_clip();
		font_t create_font(const std::string font_name, int radius, int weight = 400, const font_flags_t font_flags = font_flags_t());
		void write_vertex(const D3DPRIMITIVETYPE type, const std::vector<vertex_t>& vertices, bool anti_alias = false, const text_info_t& text_info = text_info_t());
		void line(const vec2_t from, const vec2_t to, const color_t color, const bool anti_alias = false);
		void polyline(const std::vector<vec2_t>& points, const color_t color, const bool anti_alias = false);
		void polygon(const std::vector<vec2_t>& points, const color_t color, const bool anti_alias = false);
		void rectangle(const vec2_t pos, const vec2_t size, const color_t color);
		void filled_rectangle(const vec2_t pos, const vec2_t size, const color_t color);
		void rounded_rectangle(const vec2_t pos, const vec2_t size, const color_t color, int radius, const corner_flags flags = corner_flags::corner_all);
		void filled_rounded_rectangle(const vec2_t pos, const vec2_t size, const color_t color, int radius, const corner_flags flags = corner_flags::corner_all);
		void gradient(const vec2_t pos, const vec2_t size, const color_t left, const color_t right, const bool vertical = false);
		void filled_gradient(const vec2_t pos, const vec2_t size, const color_t left, const color_t right, const bool vertical = false);
		void gradient_four(const vec2_t pos, const vec2_t size, const color_t top_left, const color_t top_right, const color_t bottom_right, const color_t bottom_left);
		void filled_gradient_four(const vec2_t pos, const vec2_t size, const color_t top_left, const color_t top_right, const color_t bottom_right, const color_t bottom_left);
		void triangle(const vec2_t pos, const vec2_t size, const color_t color);
		void filled_triangle(const vec2_t pos, const vec2_t size, const color_t color);
		void gradient_triangle(const vec2_t pos, const vec2_t size, const color_t color, const color_t color2);
		void circle(const vec2_t pos, int radius, int completion, int rotation, const color_t color);
		void filled_circle(const vec2_t pos, int radius, int completion, int rotation, const color_t color);
		void gradient_circle(const vec2_t pos, int radius, int completion, int rotation, const color_t color, const color_t color2);
		void text(const font_t font, const std::string string, const vec2_t pos, const color_t color);
		vec2_t get_text_size(const font_t font, const std::string string);
	};
	inline const auto g_renderer = std::make_unique< renderer >();
}