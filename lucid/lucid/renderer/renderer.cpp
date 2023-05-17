#include "renderer.h"

void lucid_engine::renderer::create_objects() {
	if (!g_graphics->m_direct_3d_device)
		throw std::runtime_error{ "create_objects error { device is not setup }" };

	create_font(&m_defualt_font, "Segoe UI", 9, FW_NORMAL, true);
	create_font(&m_logo_font, "Arial", 32, FW_NORMAL, true);

	for (font_t* font : m_fonts)
		build_font(*font);
}

void lucid_engine::renderer::destroy_objects() {
	if (!g_graphics->m_direct_3d_device)
		return;

	if (m_vertex_buffer) { m_vertex_buffer->Release(); m_vertex_buffer = nullptr; }
	if (m_index_buffer) { m_index_buffer->Release(); m_index_buffer = nullptr; }

	for (auto& font : m_font_map) {
		for (auto& character : font.second) {
			character.second.texture->Release();
			character.second.texture = nullptr;
		}
	}

	for (texture_t& texture : std::move(m_textures)) {
		texture.m_texture->Release();
		texture.m_texture = nullptr;
	}

	m_font_map.clear();
	m_fonts.clear();
}

void lucid_engine::renderer::build_font(font_t& font) {
	if (FT_Init_FreeType(&m_freetype)) throw std::runtime_error{ "failed to init freetype" };
	if (FT_New_Face(m_freetype, font.m_font_path.c_str(), 0, &m_freetype_face)) throw std::runtime_error{ "failed to init freetype" };

	D3DDEVICE_CREATION_PARAMETERS params;
	g_graphics->m_direct_3d_device->GetCreationParameters(&params);
	FT_Set_Char_Size(m_freetype_face, font.m_size * 64, 0, GetDpiForWindow(params.hFocusWindow), 0);
	FT_Select_Charmap(m_freetype_face, FT_ENCODING_UNICODE);

	unsigned int font_index = font.m_index;

	for (char i = ' '; i < 127; i++) {
		FT_Load_Char(m_freetype_face, i, font.m_antialiased ? FT_LOAD_RENDER : FT_LOAD_RENDER | FT_LOAD_TARGET_MONO);
		vec2_t size{ 0, 0 };

		size.x = m_freetype_face->glyph->bitmap.width ? m_freetype_face->glyph->bitmap.width : 16;
		size.y = m_freetype_face->glyph->bitmap.rows ? m_freetype_face->glyph->bitmap.rows : 16;

		if (g_graphics->m_direct_3d_device->CreateTexture(size.x, size.y, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8, D3DPOOL_DEFAULT, &m_font_map[font_index][i].texture, NULL) != D3D_OK)
			throw std::runtime_error{ "failed to create texture" };

		D3DLOCKED_RECT lock_rect;
		m_font_map[font_index][i].texture->LockRect(0, &lock_rect, nullptr, D3DLOCK_DISCARD);

		unsigned char* src_pixels = m_freetype_face->glyph->bitmap.buffer;
		unsigned char* dest_pixels = (unsigned char*)lock_rect.pBits;

		if (src_pixels && dest_pixels) {
			switch (m_freetype_face->glyph->bitmap.pixel_mode) {
			case FT_PIXEL_MODE_MONO:
				for (uint32_t y = 0; y < size.y; y++, src_pixels += m_freetype_face->glyph->bitmap.pitch, dest_pixels += lock_rect.Pitch)
				{
					uint8_t bits = 0;
					const uint8_t* bits_ptr = src_pixels;
					for (uint32_t x = 0; x < size.x; x++, bits <<= 1)
					{
						if ((x & 7) == 0)
							bits = *bits_ptr++;
						dest_pixels[x] = (bits & 0x80) ? 255 : 0;
					}
				}
				break;
			case FT_PIXEL_MODE_GRAY:
				for (UINT i = 0; i < size.y; ++i) {
					memcpy(dest_pixels, src_pixels, size.x);

					src_pixels += m_freetype_face->glyph->bitmap.pitch;
					dest_pixels += lock_rect.Pitch;
				}
				break;
			}
		}

		m_font_map[font_index][i].texture->UnlockRect(0);

		D3DSURFACE_DESC desc;
		m_font_map[font_index][i].texture->GetLevelDesc(0, &desc);
		m_font_map[font_index][i].size_x = size.x;
		m_font_map[font_index][i].size_y = size.y;
		m_font_map[font_index][i].bearing_x = m_freetype_face->glyph->bitmap_left;
		m_font_map[font_index][i].bearing_y = m_freetype_face->glyph->bitmap_top;
		m_font_map[font_index][i].advance = m_freetype_face->glyph->advance.x;
		m_font_map[font_index][i].exists = true;
	}

	FT_Done_Face(m_freetype_face);
	FT_Done_FreeType(m_freetype);
}

texture_t lucid_engine::renderer::create_texture(BYTE texture[], vec2_t size) {
	m_textures.push_back(texture_t(g_graphics->m_direct_3d_device, texture, size));
	return m_textures.back();
}

texture_t lucid_engine::renderer::create_texture(std::string file_name, vec2_t size) {
	m_textures.push_back(texture_t(g_graphics->m_direct_3d_device, file_name, size));
	return m_textures.back();
}

void lucid_engine::renderer::create_font(font_t* font, const char* title, int size, int weight, bool anti_aliased) {
	*font = font_t(title, size, weight, anti_aliased);
	font->m_index = m_fonts.size();
	m_fonts.push_back(font);
}

void lucid_engine::renderer::write_vertex(const D3DPRIMITIVETYPE type, const std::vector<vertex_t>& vertices, bool anti_alias, texture_t texture) {
	if (vertices.empty())
		throw std::runtime_error{ "write_vertex error { vertices is empty }" };

	std::vector<unsigned int> indices(type == D3DPT_LINESTRIP ? vertices.size() * 3 - 1 : vertices.size() * 3);

	for (unsigned int i = 0; i < vertices.size(); ++i)
		indices[i] = i;

	get_draw_list()->emplace_back(draw_data_t{type, vertices, indices, static_cast<int>(vertices.size()), static_cast<int>(indices.size()), anti_alias, texture, m_clip_info});
}

std::vector<draw_data_t>* lucid_engine::renderer::get_draw_list(int id) {
	draw_list_t draw_list = draw_list_t(id);

	if (id == -1)
		draw_list = m_draw_list;

	switch (draw_list) {
	case background_draw_list:
		return &m_background_draw_data;
	case default_draw_list:
		return &m_default_draw_data;
	case foreground_draw_list:
		return &m_foreground_draw_data;
	}

	throw std::runtime_error{ "get_draw_list error (invalid draw_list)" };
}

void lucid_engine::renderer::set_draw_list(draw_list_t draw_list) {
	m_draw_list = draw_list;
}

void lucid_engine::renderer::reset_draw_list() {
	m_compiled_draw_data = {};
	m_foreground_draw_data.clear();
	m_background_draw_data.clear();
	m_default_draw_data.clear();
}

void lucid_engine::renderer::compile_draw_data() {
	for (int i = 0; i < 3; ++i) {
		std::vector<draw_data_t>* draw_data = get_draw_list(i);

		for (const draw_data_t& data : *draw_data) {
			m_compiled_draw_data.m_vertices.insert(m_compiled_draw_data.m_vertices.end(), 
				std::make_move_iterator(data.m_vertices.begin()), std::make_move_iterator(data.m_vertices.end())
			);

			m_compiled_draw_data.m_indices.insert(m_compiled_draw_data.m_indices.end(), 
				std::make_move_iterator(data.m_indices.begin()), std::make_move_iterator(data.m_indices.end())
			);

			m_compiled_draw_data.m_total_index_count += data.m_index_count;
			m_compiled_draw_data.m_total_vertex_count += data.m_vertex_count;
		}
	}
}

std::vector<vec2_t> lucid_engine::renderer::generate_circle_points(const vec2_t pos, const int radius, const int completion, const int rotation, int segments) {
	std::vector<vec2_t> points;

	double ang = static_cast<double>(rotation * D3DX_PI) / 180.0;
	double comp = (completion * 0.01) * D3DX_PI;

	if (segments == -1)
		segments = std::clamp(radius, 8, 128);

	for (int i = 0; i <= segments; ++i) {
		double theta = ang + 2.0 * comp * static_cast<double>(i) / static_cast<double>(segments);

		points.emplace_back(vec2_t(pos.x + radius * cosf(theta), pos.y + radius * sinf(theta)));
	}

	return points;
}

void lucid_engine::renderer::render_draw_data() {
	m_screen_data = RECT(0, 0, (LONG)g_window->get_window_size().x, (LONG)g_window->get_window_size().y);
	g_graphics->setup_render_states();
	compile_draw_data();

	if (!m_vertex_buffer || m_compiled_draw_data.m_total_vertex_count * sizeof(vertex_t) > m_vertex_buffer_size) {
		if (m_vertex_buffer) { m_vertex_buffer->Release(); m_vertex_buffer = nullptr; }

		m_vertex_buffer_size = m_compiled_draw_data.m_total_vertex_count + 5000;

		if (g_graphics->m_direct_3d_device->CreateVertexBuffer(m_vertex_buffer_size * sizeof(vertex_t), D3DUSAGE_DYNAMIC |
			D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &m_vertex_buffer, nullptr) < 0)
			throw std::runtime_error{ "render_draw_data error (CreateVertexBuffer)" };
	}

	if (!m_index_buffer || m_compiled_draw_data.m_total_index_count * sizeof(std::uint32_t) > m_index_buffer_size) {
		if (m_index_buffer) { m_index_buffer->Release(); m_index_buffer = nullptr; }

		m_index_buffer_size = m_compiled_draw_data.m_total_index_count + 10000;

		if (g_graphics->m_direct_3d_device->CreateIndexBuffer(m_index_buffer_size * sizeof(std::uint32_t), D3DUSAGE_DYNAMIC |
			D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_index_buffer, nullptr) < 0)
			throw std::runtime_error{ "render_draw_data error (CreateIndexBuffer)" };
	}

	IDirect3DStateBlock9* state_block{ };
	if (g_graphics->m_direct_3d_device->CreateStateBlock(D3DSBT_ALL, &state_block) < 0)
		return;

	if (state_block->Capture() < 0)
		throw std::runtime_error{ "render_draw_data error (state_block->Capture)" };

	vertex_t* vertex_data{ };
	std::uint32_t* index_data{ };
	if (m_vertex_buffer->Lock(0, (UINT)(m_compiled_draw_data.m_total_vertex_count * sizeof(vertex_t)), (void**)&vertex_data, D3DLOCK_DISCARD) < 0)
		throw std::runtime_error{ "render_draw_data error (vtx_buffer->Lock)" };

	if (m_index_buffer->Lock(0, (UINT)(m_compiled_draw_data.m_total_index_count * sizeof(std::uint32_t)), (void**)&index_data, D3DLOCK_DISCARD) < 0)
		throw std::runtime_error{ "render_draw_data error (idx_buffer->Lock)" };

	memcpy(vertex_data, m_compiled_draw_data.m_vertices.data(), m_compiled_draw_data.m_total_vertex_count * sizeof(vertex_t));
	memcpy(index_data, m_compiled_draw_data.m_indices.data(), m_compiled_draw_data.m_total_index_count * sizeof(std::uint32_t));

	m_vertex_buffer->Unlock();
	m_index_buffer->Unlock();

	g_graphics->m_direct_3d_device->SetStreamSource(0, m_vertex_buffer, 0, sizeof(vertex_t));
	g_graphics->m_direct_3d_device->SetIndices(m_index_buffer);

	int start_vertex = 0, start_index = 0;
	for (int i = 0; i <= draw_list_t::foreground_draw_list; ++i) {
		for (auto& data : *get_draw_list(i)) {
			g_graphics->m_direct_3d_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, data.m_anti_alias);
			g_graphics->m_direct_3d_device->SetTexture(0, data.m_texture.m_texture);

			if (!data.m_clips.empty() && i == draw_list_t::default_draw_list)
				g_graphics->m_direct_3d_device->SetScissorRect(&data.m_clips.back());
			else
				g_graphics->m_direct_3d_device->SetScissorRect(&m_screen_data);

			g_graphics->m_direct_3d_device->DrawIndexedPrimitive(data.m_draw_type, start_vertex, 0, data.m_vertex_count, start_index, data.m_index_count * 0.3333333333333333);
			
			start_vertex += data.m_vertex_count;
			start_index += data.m_index_count;
		}
	}

	state_block->Apply();
	state_block->Release();
	reset_draw_list();
}

void lucid_engine::renderer::line(const vec2_t from, const vec2_t to, const color_t color, const bool anti_alias) {
	std::vector<vertex_t> vertices = {
		vertex_t(from.x, from.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(to.x, to.y, 0.f, 1.f, color_t::translate(color))
	};

	write_vertex(D3DPT_LINELIST, vertices, anti_alias);
}

void lucid_engine::renderer::polyline(const std::vector<vec2_t>& points, const color_t color, const bool anti_alias) {
	std::vector<vertex_t> vertices;

	for (const vec2_t& point : points)
		vertices.emplace_back(vertex_t(point.x, point.y, 0.f, 1.f, color_t::translate(color)));

	write_vertex(D3DPT_LINESTRIP, vertices, anti_alias);
}

void lucid_engine::renderer::polygon(const std::vector<vec2_t>& points, const color_t color, const bool anti_alias) {
	std::vector<vertex_t> vertices;

	for (const vec2_t& point : points)
		vertices.emplace_back(vertex_t(point.x, point.y, 0.f, 1.f, color_t::translate(color)));

	write_vertex(D3DPT_TRIANGLEFAN, vertices, anti_alias);
}

void lucid_engine::renderer::rectangle(const vec2_t pos, const vec2_t size, const color_t color) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color))
	};

	vertices.emplace_back(vertices.front());

	write_vertex(D3DPT_LINESTRIP, vertices);
}

void lucid_engine::renderer::filled_rectangle(const vec2_t pos, const vec2_t size, const color_t color) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(color), 0.f, 0.f),
		vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, color_t::translate(color), 1.f, 0.f),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color), 1.f, 1.f),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color), 0.f, 1.f)
	};

	write_vertex(D3DPT_TRIANGLEFAN, vertices);
}

void lucid_engine::renderer::texture(texture_t texture, const vec2_t pos, const vec2_t size, const color_t color) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(color), 0.f, 0.f),
		vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, color_t::translate(color), 1.f, 0.f),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color), 1.f, 1.f),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color), 0.f, 1.f)
	};

	write_vertex(D3DPT_TRIANGLEFAN, vertices, false, texture);
}

void lucid_engine::renderer::rounded_rectangle(const vec2_t pos, const vec2_t size, const color_t color, const int radius, const corner_flags flags) {
	if (radius < 0.5f || (flags & corner_none)) {
		rectangle(pos, size, color);
		return;
	}

	const bool round_top_left = (flags & corner_top_left);
	const bool round_top_right = (flags & corner_top_right);
	const bool round_bottom_left = (flags & corner_bottom_left);
	const bool round_bottom_right = (flags & corner_bottom_right);

	std::vector<vec2_t> points;

	std::initializer_list<std::tuple<vec2_t, vec2_t, int, bool>> gen_points = {
		std::tuple{vec2_t(pos.x, pos.y), vec2_t(pos.x + radius, pos.y + radius), 180, round_top_left},
		std::tuple{vec2_t(pos.x + size.x, pos.y), vec2_t(pos.x + size.x - radius, pos.y + radius), 270, round_top_right},
		std::tuple{vec2_t(pos.x + size.x, pos.y + size.y), vec2_t(pos.x + size.x - radius, pos.y + size.y - radius), 0, round_bottom_right},
		std::tuple{vec2_t(pos.x, pos.y + size.y), vec2_t(pos.x + radius, pos.y + size.y - radius), 90, round_bottom_left}
	};

	for (const std::tuple<vec2_t, vec2_t, int, bool>& corner_tuple : gen_points) {
		vec2_t corner = std::get<0>(corner_tuple);
		vec2_t corner_rounded = std::get<1>(corner_tuple);
		int angle = std::get<2>(corner_tuple);
		bool should_round = std::get<3>(corner_tuple);

		if (should_round) {
			std::vector<vec2_t> corner_points = generate_circle_points(corner_rounded, radius, 25, angle);
			points.insert(points.end(), corner_points.begin(), corner_points.end());
		}
		else {
			points.push_back(corner);
		}
	}

	points.emplace_back(points.front());

	polyline(points, color);
}

void lucid_engine::renderer::filled_rounded_rectangle(const vec2_t pos, const vec2_t size, const color_t color, const int radius, const corner_flags flags) {
	if (radius < 0.5f || (flags & corner_none)) {
		filled_rectangle(pos, size, color);
		return;
	}

	const bool round_top_left = (flags & corner_top_left);
	const bool round_top_right = (flags & corner_top_right);
	const bool round_bottom_left = (flags & corner_bottom_left);
	const bool round_bottom_right = (flags & corner_bottom_right);

	std::vector<vec2_t> points;

	std::initializer_list<std::tuple<vec2_t, vec2_t, int, bool>> gen_points = {
		std::tuple{vec2_t(pos.x, pos.y), vec2_t(pos.x + radius, pos.y + radius), 180, round_top_left},
		std::tuple{vec2_t(pos.x + size.x, pos.y), vec2_t(pos.x + size.x - radius, pos.y + radius), 270, round_top_right},
		std::tuple{vec2_t(pos.x + size.x, pos.y + size.y), vec2_t(pos.x + size.x - radius, pos.y + size.y - radius), 0, round_bottom_right},
		std::tuple{vec2_t(pos.x, pos.y + size.y), vec2_t(pos.x + radius, pos.y + size.y - radius), 90, round_bottom_left}
	};

	for (const std::tuple<vec2_t, vec2_t, int, bool>& corner_tuple : gen_points) {
		vec2_t corner = std::get<0>(corner_tuple);
		vec2_t corner_rounded = std::get<1>(corner_tuple);
		int angle = std::get<2>(corner_tuple);
		bool should_round = std::get<3>(corner_tuple);

		if (should_round) {
			std::vector<vec2_t> corner_points = generate_circle_points(corner_rounded, radius, 25, angle);
			points.insert(points.end(), corner_points.begin(), corner_points.end());
		}
		else {
			points.push_back(corner);
		}
	}

	polygon(points, color);
}

void lucid_engine::renderer::gradient(const vec2_t pos, const vec2_t size, const color_t left, const color_t right, const bool vertical) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(left)),
		vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, vertical ? color_t::translate(left) : color_t::translate(right)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(right)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, vertical ? color_t::translate(right) : color_t::translate(left)),
	};

	vertices.emplace_back(vertices.front());

	write_vertex(D3DPT_LINESTRIP, vertices);
}

void lucid_engine::renderer::filled_gradient(const vec2_t pos, const vec2_t size, const color_t left, const color_t right, const bool vertical) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(left)),
		vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, vertical ? color_t::translate(left) : color_t::translate(right)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(right)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, vertical ? color_t::translate(right) : color_t::translate(left)),
	};

	write_vertex(D3DPT_TRIANGLEFAN, vertices);
}

void lucid_engine::renderer::gradient_four(const vec2_t pos, const vec2_t size, const color_t top_left, const color_t top_right, const color_t bottom_right, const color_t bottom_left) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(top_left)),
		vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, color_t::translate(top_right)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(bottom_right)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(bottom_left)),
	};

	vertices.emplace_back(vertices.front());

	write_vertex(D3DPT_LINESTRIP, vertices);
}

void lucid_engine::renderer::filled_gradient_four(const vec2_t pos, const vec2_t size, const color_t top_left, const color_t top_right, const color_t bottom_right, const color_t bottom_left) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(top_left)),
		vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, color_t::translate(top_right)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(bottom_right)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(bottom_left))
	};

	write_vertex(D3DPT_TRIANGLEFAN, vertices);
}

void lucid_engine::renderer::triangle(const vec2_t pos, const vec2_t size, const color_t color) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x + size.x * 0.5, pos.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
	};

	vertices.emplace_back(vertices.front());

	write_vertex(D3DPT_LINESTRIP, vertices);
}

void lucid_engine::renderer::filled_triangle(const vec2_t pos, const vec2_t size, const color_t color) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x + size.x * 0.5, pos.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
	};

	write_vertex(D3DPT_TRIANGLEFAN, vertices);
}

void lucid_engine::renderer::gradient_triangle(const vec2_t pos, const vec2_t size, const color_t color, const color_t color2) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x + size.x * 0.5, pos.y, 0.f, 1.f, color_t::translate(color2)),
		vertex_t(pos.x + size.x * 0.5, pos.y + size.y * 0.5, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color2)),
		vertex_t(pos.x + size.x * 0.5, pos.y + size.y * 0.5, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color2))
	};

	write_vertex(D3DPT_TRIANGLESTRIP, vertices);
}

void lucid_engine::renderer::circle(const vec2_t pos, int radius, int completion, int rotation, const color_t color) {
	std::vector<vec2_t> points = generate_circle_points(pos, radius, completion, rotation, CIRCLE_SEGMENTS);
	std::vector<vertex_t> vertices;

	for (const vec2_t& point : points)
		vertices.emplace_back(vertex_t(point.x, point.y, 0.f, 1.f, color_t::translate(color)));

	write_vertex(D3DPT_LINESTRIP, vertices, true);
}

void lucid_engine::renderer::filled_circle(const vec2_t pos, int radius, int completion, int rotation, const color_t color) {
	std::vector<vec2_t> points = generate_circle_points(pos, radius, completion, rotation, CIRCLE_SEGMENTS);
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(color)));

	for (const vec2_t& point : points)
		vertices.emplace_back(vertex_t(point.x, point.y, 0.f, 1.f, color_t::translate(color)));

	write_vertex(D3DPT_TRIANGLEFAN, vertices, true);
}

void lucid_engine::renderer::gradient_circle(const vec2_t pos, int radius, int completion, int rotation, const color_t color, const color_t color2) {
	std::vector<vec2_t> points = generate_circle_points(pos, radius, completion, rotation, CIRCLE_SEGMENTS);
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(color)));

	for (const vec2_t& point : points)
		vertices.emplace_back(vertex_t(point.x, point.y, 0.f, 1.f, color_t::translate(color2)));

	write_vertex(D3DPT_TRIANGLEFAN, vertices, true);
}

void lucid_engine::renderer::text(font_t font, const std::string string, const vec2_t pos, const color_t color) {
	auto& char_set = m_font_map[font.m_index];
	const auto bounds = get_text_size(font, string);
	auto clr = color_t::translate(color);

	float add = 0.f;
	for (const auto letter : string) {
		if (!isprint(letter) || letter == ' ') {
			add += char_set[letter].advance / 64.f;
			continue;
		}

		const auto& glyph = char_set[letter];
		const float w = static_cast<float>(glyph.size_x) + 0.5f;
		const float h = static_cast<float>(glyph.size_y) + 0.5f;
		const float x_pos = static_cast<float>(pos.x + add + glyph.bearing_x) - 0.5f;
		const float y_pos = static_cast<float>(pos.y + (bounds.y * 0.75f) - glyph.bearing_y) - 0.5f;

		const std::vector<vertex_t> vertices = {
			{x_pos, y_pos, 0.f, 1.f, clr, 0.f, 0.f},
			{x_pos + w, y_pos, 0.f, 1.f, clr, 1.f, 0.f},
			{x_pos + w, y_pos + h, 0.f, 1.f, clr, 1.f, 1.f},
			{x_pos, y_pos + h, 0.f, 1.f, clr, 0.f, 1.f}
		};

		write_vertex(D3DPT_TRIANGLEFAN, vertices, false, glyph.texture);
		add += glyph.advance / 64.f;
	}
}

vec2_t lucid_engine::renderer::get_text_size(const font_t font, const std::string string) {
	vec2_t pos{ 0, 0 };

	std::map<char, character_t>& char_set = m_font_map[font.m_index];
	D3DSURFACE_DESC desc;

	for (size_t i = 0; i < string.size(); i++) {
		character_t& glyph = char_set[string[i]];

		glyph.texture->GetLevelDesc(0, &desc);
		pos.y = std::max(pos.y, static_cast<double>(font.m_size * 1.5f));
		pos.x += (float)(glyph.advance / 64);
	}

	return pos;
}

void lucid_engine::renderer::push_clip(const vec2_t pos, const vec2_t size) {
	m_clip_info.push_back(RECT{ (LONG)pos.x, (LONG)pos.y, (LONG)pos.x + (LONG)size.x, (LONG)pos.y + (LONG)size.y });
}

void lucid_engine::renderer::pop_clip() {
	m_clip_info.pop_back();
}