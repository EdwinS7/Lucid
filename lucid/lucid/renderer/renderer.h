#pragma once

#include "../lucid.h"

enum corner_flags {
	none = 0,

	corner_top_left = 1 << 0,
	corner_top_right = 1 << 1,
	corner_bottom_left = 1 << 2,
	corner_bottom_right = 1 << 3,

	corner_top = corner_top_left | corner_top_right,
	corner_right = corner_top_right | corner_bottom_right,
	corner_bottom = corner_bottom_left | corner_bottom_right,
	corner_left = corner_top_left | corner_bottom_left,

	corner_all = corner_top_left | corner_top_right | corner_bottom_left | corner_bottom_right
};

namespace lucid_engine {
	class renderer : public singleton<renderer> {
	private:
		compiled_draw_data_t compiled_draw_data{ };
		std::vector<draw_data_t> draw_data{ };

		IDirect3DVertexBuffer9* vertex_buffer{ };
		IDirect3DIndexBuffer9* index_buffer{ };

		LPDIRECT3DTEXTURE9 font_texture{ };
		LPD3DXSPRITE font_sprite{ };

	public:
		struct fonts {
			font_t default_font{ };
			font_t primordial_icons{ };
		} fonts;

		void create_objects();
		void destroy_objects();
		void render_draw_data();
		void write_vertex(D3DPRIMITIVETYPE type, std::vector<vertex_t> vertices, bool anti_alias = false, text_info_t text_info = text_info_t());
		void line(vec2_t from, vec2_t to, color_t color, bool anti_alias = false);
		void polyline(std::vector<vec2_t> points, color_t color, bool anti_alias = false);
		void polygon(std::vector<vec2_t> points, color_t color, bool anti_alias = false);
		void rectangle(vec2_t pos, vec2_t size, color_t color);
		void filled_rectangle(vec2_t pos, vec2_t size, color_t color);
		void rounded_rectangle(vec2_t pos, vec2_t size, color_t color, int radius, corner_flags flags = corner_all);
		void filled_rounded_rectangle(vec2_t pos, vec2_t size, color_t color, int radius, corner_flags flags = corner_all);
		void gradient(vec2_t pos, vec2_t size, color_t left, color_t right, bool vertical = false);
		void filled_gradient(vec2_t pos, vec2_t size, color_t left, color_t right, bool vertical = false);
		void gradient_four(vec2_t pos, vec2_t size, color_t top_left, color_t top_right, color_t bottom_right, color_t bottom_left);
		void filled_gradient_four(vec2_t pos, vec2_t size, color_t top_left, color_t top_right, color_t bottom_right, color_t bottom_left);
		void triangle(vec2_t pos, vec2_t size, color_t color);
		void filled_triangle(vec2_t pos, vec2_t size, color_t color);
		void gradient_triangle(vec2_t pos, vec2_t size, color_t color, color_t color2);
		void circle(vec2_t pos, int size, int completion, int rotation, color_t color);
		void filled_circle(vec2_t pos, int size, int completion, int rotation, color_t color);
		void gradient_circle(vec2_t pos, int size, int completion, int rotation, color_t color, color_t color2);
		font_t create_font(std::string font_name, int size, int weight = 400, font_flags_t font_flags = font_flags_t());
		void text(font_t font, std::string string, vec2_t pos, color_t color, text_flags_t flags = text_flags_t());
		vec2_t get_text_size(font_t font, std::string string);
	};
}