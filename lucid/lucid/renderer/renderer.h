#pragma once

#include "../lucid.h"

namespace lucid_engine {
	class renderer : public singleton<renderer> {
	private:
		enum corner_flags {
			none = 0,

			top_left = 1 << 0,
			top_right = 1 << 1,
			bottom_left = 1 << 2,
			bottom_right = 1 << 3,

			top = top_left | top_right,
			right = top_right | bottom_right,
			bottom = bottom_left | bottom_right,
			left = top_left | bottom_left,

			all = top_left | top_right | bottom_left | bottom_right
		};

		compiled_draw_data_t compiled_draw_data{ };
		std::vector<draw_data_t> draw_data{ };

		IDirect3DVertexBuffer9* vertex_buffer{ };
		IDirect3DIndexBuffer9* index_buffer{ };

	public:
		void create_objects();
		void destroy_objects();
		void render_draw_data();
		void write_vertex(D3DPRIMITIVETYPE type, std::vector<vertex_t> vertices);
		void line(vec2_t from, vec2_t to, color_t color);
		void polyline(std::vector<vec2_t> points, color_t color);
		void polygon(std::vector<vec2_t> points, color_t color);
		void rectangle(vec2_t pos, vec2_t size, color_t color);
		void filled_rectangle(vec2_t pos, vec2_t size, color_t color);
		void rounded_rectangle(vec2_t pos, vec2_t size, color_t color, int radius, corner_flags flags = all);
		void filled_rounded_rectangle(vec2_t pos, vec2_t size, color_t color, int radius, corner_flags flags = all);
		void gradient(vec2_t pos, vec2_t size, color_t left, color_t right, bool vertical = false);
		void filled_gradient(vec2_t pos, vec2_t size, color_t left, color_t right, bool vertical = false);
		void gradient_four(vec2_t pos, vec2_t size, color_t top_left, color_t top_right, color_t bottom_right, color_t bottom_left);
		void filled_gradient_four(vec2_t pos, vec2_t size, color_t top_left, color_t top_right, color_t bottom_right, color_t bottom_left);
		void triangle(vec2_t pos, vec2_t size, color_t color);
		void filled_triangle(vec2_t pos, vec2_t size, color_t color);
		void circle(vec2_t pos, int size, int completion, int rotation, color_t color);
		void filled_circle(vec2_t pos, int size, int completion, int rotation, color_t color);
		void gradient_filled_circle(vec2_t pos, int size, int completion, int rotation, color_t color, color_t color2);
	};
}