#pragma once

#include "../lucid.h"

namespace lucid_engine {
	class renderer : public singleton<renderer> {
	private:
		std::vector<draw_data_t> draw_data;
		IDirect3DVertexDeclaration9* vertex_declaration{ };
		IDirect3DVertexBuffer9* vertex_buffer{ };
		IDirect3DIndexBuffer9* index_buffer{ };

	public:
		compiled_draw_data_t get_draw_data();
		void render_draw_data(compiled_draw_data_t draw_data);
		void line(vec2_t from, vec2_t to, color_t color);
		void rectangle(vec2_t pos, vec2_t size, color_t color);
		void filled_rectangle(vec2_t pos, vec2_t size, color_t color);
	};
}