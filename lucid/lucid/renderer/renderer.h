#pragma once
#include "../lucid.h"

// settings
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

struct character_t {
	LPDIRECT3DTEXTURE9	m_texture = nullptr;
	vec2_t				m_size, m_bearing;
	int					m_advance;
};

namespace lucid_engine {
	class renderer {
	public:
		std::vector<texture_t>						m_textures{ };

		std::vector<font_t*>						m_fonts{ };
		std::map<int, std::map<char, character_t>>	m_font_map;
		font_t										m_defualt_font{ };
		font_t										m_logo_font{ };

		void create_objects();
		void destroy_objects();
		void build_font(font_t& font);
		texture_t create_texture(BYTE texture[], vec2_t size);
		texture_t create_texture(std::string file_name, vec2_t size);
		void create_font(font_t* font, const char* title, int size, int weight = FW_NORMAL, bool anti_aliased = false);
		void write_vertex(const D3DPRIMITIVETYPE type, const std::vector<vertex_t>& vertices, bool anti_alias = false, texture_t texture = texture_t());
		std::vector<draw_data_t>* get_draw_list(draw_list_t draw_list);
		void set_draw_list(draw_list_t draw_list);
		void render_draw_data();

		void line(const vec2_t from, const vec2_t to, const color_t color, const bool anti_alias = false);
		void polyline(const std::vector<vec2_t>& points, const color_t color, const bool anti_alias = false);
		void polygon(const std::vector<vec2_t>& points, const color_t color, const bool anti_alias = false);
		void rectangle(const vec2_t pos, const vec2_t size, const color_t color);
		void filled_rectangle(const vec2_t pos, const vec2_t size, const color_t color);
		void texture(texture_t texture, const vec2_t pos, const vec2_t size, const color_t color);
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
		void text(font_t font, const std::string string, const vec2_t pos, const color_t color);
		vec2_t get_text_size(font_t font, const std::string string);
		void push_clip(const vec2_t pos, const vec2_t size);
		void pop_clip();

	private:
		int							m_vertex_buffer_size{ 5000 },
									m_index_buffer_size{ 10000 };

		IDirect3DVertexBuffer9*		m_vertex_buffer{ };
		IDirect3DIndexBuffer9*		m_index_buffer{ };

		compiled_draw_data_t		m_compiled_draw_data{ };
		std::vector<draw_data_t>	m_default_draw_data{ },
									m_background_draw_data{ },
									m_foreground_draw_data{ };

		LPDIRECT3DTEXTURE9			m_font_texture{ };

		std::vector<RECT>			m_clip_info{ };

		draw_list_t					m_draw_list{ default_draw_list };

		RECT                        m_screen_data{ };

		FT_Library					m_freetype;
		FT_Face						m_freetype_face;

		std::vector<vec2_t> generate_arc_points(const vec2_t pos, const int radius, const int completion, const int rotation, int segments = -1);
		void compile_draw_data();
		void reset_draw_list();

	};

	inline const auto g_renderer = std::make_unique< renderer >();
}