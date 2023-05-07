#include "renderer.h"

void lucid_engine::renderer::create_objects() {
	if (!g_graphics.get()->m_direct_3d_device)
		throw std::runtime_error{ "create_objects error { device is not setup }" };

	D3DXCreateSprite(g_graphics.get()->m_direct_3d_device, &m_font_sprite);

	m_defualt_font = create_font("Segoe UI", 13, 400, font_flags_t(true, false, false));
	m_logo_font = create_font("Museo Sans Rounded", 46, 400, font_flags_t(true, false, false));
}

void lucid_engine::renderer::destroy_objects() {
	if (!g_graphics.get()->m_direct_3d_device)
		return;

	if (m_vertex_buffer) { m_vertex_buffer->Release(); m_vertex_buffer = nullptr; }
	if (m_index_buffer) { m_index_buffer->Release(); m_index_buffer = nullptr; }

	if (m_font_sprite) { m_font_sprite->Release(); m_font_sprite = nullptr; }

	for ( font_t& font : m_fonts) {
		if (&font) {
			font.m_dx_font->Release();
			font.m_dx_font = nullptr;
		}
	}

	m_fonts.clear();
}

void lucid_engine::renderer::render_draw_data() {
	m_screen_data = RECT(0, 0, g_window.get()->get_window_size().x, g_window.get()->get_window_size().y);
	g_graphics.get()->setup_render_states();
	compile_draw_data();

	if (!m_vertex_buffer || m_compiled_draw_data.m_total_vertex_count * sizeof(vertex_t) > m_vertex_buffer_size) {
		if (m_vertex_buffer) { m_vertex_buffer->Release(); m_vertex_buffer = nullptr; }

		m_vertex_buffer_size = m_compiled_draw_data.m_total_vertex_count + 5000;

		if (g_graphics.get()->m_direct_3d_device->CreateVertexBuffer(m_vertex_buffer_size * sizeof(vertex_t), D3DUSAGE_DYNAMIC |
			D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &m_vertex_buffer, nullptr) < 0)
			throw std::runtime_error{ "render_draw_data error (CreateVertexBuffer)" };
	}

	if (!m_index_buffer || m_compiled_draw_data.m_total_index_count * sizeof(std::uint32_t) > m_index_buffer_size) {
		if (m_index_buffer) { m_index_buffer->Release(); m_index_buffer = nullptr; }

		m_index_buffer_size = m_compiled_draw_data.m_total_index_count + 10000;

		if (g_graphics.get()->m_direct_3d_device->CreateIndexBuffer(m_index_buffer_size * sizeof(std::uint32_t), D3DUSAGE_DYNAMIC |
			D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_index_buffer, nullptr) < 0)
			throw std::runtime_error{ "render_draw_data error (CreateIndexBuffer)" };
	}

	IDirect3DStateBlock9* state_block{ };
	if (g_graphics.get()->m_direct_3d_device->CreateStateBlock(D3DSBT_ALL, &state_block) < 0)
		return;

	if (state_block->Capture() < 0) {
		throw std::runtime_error{ "render_draw_data error (state_block->Capture)" };
		state_block->Release();
		return;
	}

	vertex_t* vertex_data{ };
	std::uint32_t* index_data{ };
	if (m_vertex_buffer->Lock(0, (UINT)(m_compiled_draw_data.m_total_vertex_count * sizeof(vertex_t)),
		(void**)&vertex_data, D3DLOCK_DISCARD) < 0) {
		throw std::runtime_error{ "render_draw_data error (vtx_buffer->Lock)" };
		state_block->Release();
		return;
	}

	if (m_index_buffer->Lock(0, (UINT)(m_compiled_draw_data.m_total_index_count * sizeof(std::uint32_t)),
		(void**)&index_data, D3DLOCK_DISCARD) < 0) {
		throw std::runtime_error{ "render_draw_data error (idx_buffer->Lock)" };
		m_vertex_buffer->Unlock();
		state_block->Release();
		return;
	}

	memcpy(vertex_data, m_compiled_draw_data.m_vertices.data(), m_compiled_draw_data.m_total_vertex_count * sizeof(vertex_t));
	memcpy(index_data, m_compiled_draw_data.m_indices.data(), m_compiled_draw_data.m_total_index_count * sizeof(std::uint32_t));

	m_vertex_buffer->Unlock();
	m_index_buffer->Unlock();

	g_graphics.get()->m_direct_3d_device->SetStreamSource(0, m_vertex_buffer, 0, sizeof(vertex_t));
	g_graphics.get()->m_direct_3d_device->SetIndices(m_index_buffer);

	int start_vertex = 0, start_index = 0;
	for (int i = 0; i < 3; ++i) {
		for (auto& data : *get_draw_list(i)) {
			g_graphics->m_direct_3d_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, data.m_anti_alias);

			if (!data.m_clips.empty())
				g_graphics->m_direct_3d_device->SetScissorRect(&data.m_clips.back());
			else
				g_graphics->m_direct_3d_device->SetScissorRect(&m_screen_data);

			if (data.m_text_info.m_setup) {
				RECT rect = { data.m_text_info.m_pos.x, data.m_text_info.m_pos.y, 0, 0 };
				data.m_text_info.m_font.m_dx_font->DrawTextA(nullptr, data.m_text_info.m_string.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, color_t::translate(data.m_text_info.m_color));
			}
			else
				g_graphics->m_direct_3d_device->DrawIndexedPrimitive(data.m_draw_type, start_vertex, 0, data.m_vertex_count, start_index, data.m_index_count * 0.3333333333333333);

			start_vertex += data.m_vertex_count;
			start_index += data.m_index_count;
		}
	}

	state_block->Apply();
	state_block->Release();

	reset_draw_list();
}

void lucid_engine::renderer::write_vertex(const D3DPRIMITIVETYPE type, const std::vector<vertex_t>& vertices, bool anti_alias, const text_info_t& text_info) {
	if (vertices.empty())
		throw std::runtime_error{ "write_vertex error { vertices is empty }" };

	std::vector<unsigned int> indices(type == D3DPT_LINESTRIP ? vertices.size() * 3 - 1 : vertices.size() * 3);

	for (unsigned int i = 0; i < vertices.size(); ++i)
		indices[i] = i;

	std::vector<draw_data_t>* draw_list = get_draw_list();
	draw_list->emplace_back(draw_data_t{ type, vertices, indices, static_cast<int>(vertices.size()), static_cast<int>(indices.size()), anti_alias, text_info, m_clip_info });
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
			m_compiled_draw_data.m_vertices.insert(m_compiled_draw_data.m_vertices.end(), data.m_vertices.begin(), data.m_vertices.end());
			m_compiled_draw_data.m_indices.insert(m_compiled_draw_data.m_indices.end(), data.m_indices.begin(), data.m_indices.end());

			m_compiled_draw_data.m_total_index_count += data.m_index_count;
			m_compiled_draw_data.m_total_vertex_count += data.m_vertex_count;
		}
	}
}

font_t lucid_engine::renderer::create_font(const std::string font_name, const int size, const int weight, const font_flags_t font_flags) {
	m_fonts.push_back(font_t(g_graphics.get()->m_direct_3d_device, font_name.c_str(), size, weight, font_flags));
	return m_fonts.back();
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
		vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
	};

	write_vertex(D3DPT_TRIANGLEFAN, vertices);
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

	for ( const std::tuple<vec2_t, vec2_t, int, bool>& corner_tuple : gen_points ) {
		vec2_t corner = std::get<0>( corner_tuple );
		vec2_t corner_rounded = std::get<1>( corner_tuple );
		int angle = std::get<2>( corner_tuple );
		bool should_round = std::get<3>( corner_tuple );
		if ( should_round ) {
			std::vector<vec2_t> corner_points = generate_circle_points( corner_rounded, radius, 25, angle );
			points.insert( points.end( ), corner_points.begin( ), corner_points.end( ) );
		}
		else {
			points.push_back( corner );
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

	for ( const std::tuple<vec2_t, vec2_t, int, bool>& corner_tuple : gen_points ) {
		vec2_t corner = std::get<0>( corner_tuple );
		vec2_t corner_rounded = std::get<1>( corner_tuple );
		int angle = std::get<2>( corner_tuple );
		bool should_round = std::get<3>( corner_tuple );
		if ( should_round ) {
			std::vector<vec2_t> corner_points = generate_circle_points( corner_rounded, radius, 25, angle );
			points.insert( points.end( ), corner_points.begin( ), corner_points.end( ) );
		}
		else {
			points.push_back( corner );
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

std::vector<vec2_t> lucid_engine::renderer::generate_circle_points(const vec2_t pos, const int radius, const int completion, const int rotation, int segments) {
	std::vector<vec2_t> points;

	double ang = static_cast<double>( rotation * D3DX_PI) / 180.0;
	double comp = (completion * 0.01) * D3DX_PI;

	if (segments == -1)
		segments = std::clamp(radius, 8, 128);

	for (int i = 0; i <= segments; ++i) {
		double theta = ang + 2.0 * comp * static_cast<double>(i) / static_cast<double>(segments);

		points.emplace_back(vec2_t(pos.x + radius * cosf(theta), pos.y + radius * sinf(theta)));
	}

	return points;
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

void lucid_engine::renderer::text(const font_t font, const std::string string, const vec2_t pos, const color_t color) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(color))
	};

	write_vertex(D3DPT_TRIANGLEFAN, vertices, true, text_info_t(font, string, pos, color));
}

vec2_t lucid_engine::renderer::get_text_size(const font_t font, const std::string string) {
	RECT text_clip; 

	font.m_dx_font->DrawTextA(0, string.c_str(),
		strlen(string.c_str()), &text_clip, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));

	return vec2_t(text_clip.right - text_clip.left, text_clip.bottom - text_clip.top);
}

void lucid_engine::renderer::push_clip(const vec2_t pos, const vec2_t size) {
	m_clip_info.push_back(RECT{ (LONG)pos.x, (LONG)pos.y, (LONG)pos.x + (LONG)size.x, (LONG)pos.y + (LONG)size.y });
}

void lucid_engine::renderer::pop_clip() {
	m_clip_info.pop_back();
}