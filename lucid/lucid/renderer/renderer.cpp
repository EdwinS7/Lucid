#include "renderer.h"


void lucid_engine::renderer::create_objects() {
	if (!g_graphics.get()->m_direct_3d_device)
		throw std::runtime_error{ "create_objects error { device is not setup }" };

	D3DXCreateSprite(g_graphics.get()->m_direct_3d_device, &m_font_sprite);

	m_defualt_font = create_font("Segoe UI", 13, 400, font_flags_t(true, false, false));
	m_primordial_font = create_font("Primordial-Icons", 26, 400, font_flags_t(true, false, false));
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
	static int vertex_buffer_size{ 5000 }, index_buffer_size{ 10000 };

	for (int i = 0; i < 3; ++i) {
		std::vector<draw_data_t> draw_data_ptr;
		
		switch (i) {
		case background_draw_list:
			if (m_background_draw_data.empty())
				break;
			else
				draw_data_ptr = m_background_draw_data;

			break;
		case default_draw_list:
			if (m_default_draw_data.empty())
				break;
			else
				draw_data_ptr = m_default_draw_data;

			break;
		case foreground_draw_list:
			if (m_foreground_draw_data.empty())
				break;
			else
				draw_data_ptr = m_foreground_draw_data;

			break;
		}

		for (const draw_data_t& data : draw_data_ptr) {
			for (vertex_t vertex : data.m_vertices)
				m_compiled_draw_data.m_vertices.emplace_back(vertex);

			for (std::uint32_t index : data.m_indices)
				m_compiled_draw_data.m_indices.emplace_back(index);

			m_compiled_draw_data.m_total_index_count += data.m_index_count;
			m_compiled_draw_data.m_total_vertex_count += data.m_vertex_count;
		}
	}

	if (!m_vertex_buffer || m_compiled_draw_data.m_total_vertex_count * sizeof(vertex_t) > vertex_buffer_size) {
		if (m_vertex_buffer) { m_vertex_buffer->Release(); m_vertex_buffer = nullptr; }

		vertex_buffer_size = m_compiled_draw_data.m_total_vertex_count + 5000;

		if (g_graphics.get()->m_direct_3d_device->CreateVertexBuffer(vertex_buffer_size * sizeof(vertex_t), D3DUSAGE_DYNAMIC |
			D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &m_vertex_buffer, nullptr) < 0)
			throw std::runtime_error{ "CreateVertexBuffer error" };
	}
	if (!m_index_buffer || m_compiled_draw_data.m_total_index_count * sizeof(std::uint32_t) > index_buffer_size) {
		if (m_index_buffer) { m_index_buffer->Release(); m_index_buffer = nullptr; }

		index_buffer_size = m_compiled_draw_data.m_total_index_count + 10000;

		if (g_graphics.get()->m_direct_3d_device->CreateIndexBuffer(index_buffer_size * sizeof(std::uint32_t), D3DUSAGE_DYNAMIC |
			D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_index_buffer, nullptr) < 0)
			throw std::runtime_error{ "CreateIndexBuffer error" };
	}

	IDirect3DStateBlock9* state_block{ };
	if (g_graphics.get()->m_direct_3d_device->CreateStateBlock(D3DSBT_ALL, &state_block) < 0)
		return;

	if (state_block->Capture() < 0) {
		throw std::runtime_error{ "state_block->Capture error" };
		state_block->Release();
		return;
	}

	D3DMATRIX last_world, last_view, last_projection;
	g_graphics.get()->m_direct_3d_device->GetTransform(D3DTS_WORLD, &last_world);
	g_graphics.get()->m_direct_3d_device->GetTransform(D3DTS_VIEW, &last_view);
	g_graphics.get()->m_direct_3d_device->GetTransform(D3DTS_PROJECTION, &last_projection);

	vertex_t* vertex_data{ };
	std::uint32_t* index_data{ };
	if (m_vertex_buffer->Lock(0, (UINT)(m_compiled_draw_data.m_total_vertex_count * sizeof(vertex_t)),
		(void**)&vertex_data, D3DLOCK_DISCARD) < 0) {
		throw std::runtime_error{ "vtx_buffer->Lock error" };
		state_block->Release();
		return;
	}

	if (m_index_buffer->Lock(0, (UINT)(m_compiled_draw_data.m_total_index_count * sizeof(std::uint32_t)),
		(void**)&index_data, D3DLOCK_DISCARD) < 0) {
		throw std::runtime_error{ "idx_buffer->Lock error" };
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

	int start_vertex = 0;
	int start_index = 0;
	for (int i = 0; i < 3; ++i) {
		std::vector<draw_data_t> draw_data_ptr;

		switch (i) {
		case background_draw_list:
			if (m_background_draw_data.empty())
				break;
			else
				draw_data_ptr = m_background_draw_data;

			break;
		case default_draw_list:
			if (m_default_draw_data.empty())
				break;
			else
				draw_data_ptr = m_default_draw_data;

			break;
		case foreground_draw_list:
			if (m_foreground_draw_data.empty())
				break;
			else
				draw_data_ptr = m_foreground_draw_data;

			break;
		}

		for (const draw_data_t& data : draw_data_ptr) {
			RECT clip = { data.m_clip_info.m_clip.x, data.m_clip_info.m_clip.y, data.m_clip_info.m_clip.x + data.m_clip_info.m_clip.w, data.m_clip_info.m_clip.y + data.m_clip_info.m_clip.h };
			text_info_t text_info = data.m_text_info;

			g_graphics.get()->m_direct_3d_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, data.m_anti_alias);
			g_graphics.get()->m_direct_3d_device->SetRenderState(D3DRS_SCISSORTESTENABLE, (i == default_draw_list) && data.m_clip_info.m_clipping);
			g_graphics.get()->m_direct_3d_device->SetScissorRect(&clip);

			if (text_info.m_setup) {
				RECT rect = { text_info.m_pos.x, text_info.m_pos.y, 0, 0 };
				text_info.m_font.m_dx_font->DrawTextA(NULL, text_info.m_string.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, color_t::translate(text_info.m_color));
			}
			else
				g_graphics.get()->m_direct_3d_device->DrawIndexedPrimitive(data.m_draw_type, start_vertex, 0, data.m_vertex_count, start_index, data.m_index_count / 3);

			start_vertex += data.m_vertex_count;
			start_index += data.m_index_count;
		}
	}

	g_graphics.get()->m_direct_3d_device->SetTransform(D3DTS_WORLD, &last_world);
	g_graphics.get()->m_direct_3d_device->SetTransform(D3DTS_VIEW, &last_view);
	g_graphics.get()->m_direct_3d_device->SetTransform(D3DTS_PROJECTION, &last_projection);

	state_block->Apply();
	state_block->Release();

	m_compiled_draw_data = {};
	m_foreground_draw_data.clear();
	m_background_draw_data.clear();
	m_default_draw_data.clear();

	m_clip_info.clear();
}

void lucid_engine::renderer::set_draw_list(draw_list_t draw_list) {
	m_draw_list = draw_list;
}

void lucid_engine::renderer::push_clip(const vec2_t pos, const vec2_t size) {
	m_clip_info.m_old_clips.push(std::make_pair(m_clip_info.m_clipping, m_clip_info.m_clip));

	m_clip_info.m_clipping = true;
	m_clip_info.m_clip = { pos.x, pos.y, size.x, size.y };
}

void lucid_engine::renderer::pop_clip() {
	if (!m_clip_info.m_old_clips.empty()) {
		m_clip_info.m_clipping = m_clip_info.m_old_clips.top().first;
		m_clip_info.m_clip = m_clip_info.m_old_clips.top().second;
		m_clip_info.m_old_clips.pop();
	}
	else
		m_clip_info.clear();
}

font_t lucid_engine::renderer::create_font(const std::string font_name, const int size, const int weight, const font_flags_t font_flags) {
	m_fonts.push_back(font_t(g_graphics.get()->m_direct_3d_device, font_name.c_str(), size, weight, font_flags));
	return m_fonts.back();
}

void lucid_engine::renderer::write_vertex(const D3DPRIMITIVETYPE type, const std::vector<vertex_t>& vertices, bool anti_alias, const text_info_t& text_info) {
	if (vertices.empty())
		throw std::runtime_error{ "write_vertex error { vertices is empty }" };

	std::vector<unsigned int> indices(type == D3DPT_LINESTRIP ? vertices.size() * 3 - 1 : vertices.size() * 3);

	for (unsigned int i = 0; i < vertices.size(); ++i)
		indices[i] = i;

	switch (m_draw_list) {
	case default_draw_list:
		m_default_draw_data.emplace_back(draw_data_t{ type, vertices, indices, static_cast<int>(vertices.size()), static_cast<int>(indices.size()), anti_alias, text_info, m_clip_info });
		break;
	case background_draw_list:
		m_background_draw_data.emplace_back(draw_data_t{ type, vertices, indices, static_cast<int>(vertices.size()), static_cast<int>(indices.size()), anti_alias, text_info, m_clip_info });
		break;
	case foreground_draw_list:
		m_foreground_draw_data.emplace_back(draw_data_t{ type, vertices, indices, static_cast<int>(vertices.size()), static_cast<int>(indices.size()), anti_alias, text_info, m_clip_info });
		break;
	}
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
		vertex_t(pos.x + size.x / 2, pos.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
	};

	vertices.emplace_back(vertices.front());

	write_vertex(D3DPT_LINESTRIP, vertices);
}

void lucid_engine::renderer::filled_triangle(const vec2_t pos, const vec2_t size, const color_t color) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x + size.x / 2, pos.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
	};

	write_vertex(D3DPT_TRIANGLEFAN, vertices);
}

void lucid_engine::renderer::gradient_triangle(const vec2_t pos, const vec2_t size, const color_t color, const color_t color2) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x + size.x / 2, pos.y, 0.f, 1.f, color_t::translate(color2)),
		vertex_t(pos.x + size.x / 2, pos.y + size.y / 2, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color2)),
		vertex_t(pos.x + size.x / 2, pos.y + size.y / 2, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color2))
	};

	write_vertex(D3DPT_TRIANGLESTRIP, vertices);
}

std::vector<vec2_t> lucid_engine::renderer::generate_circle_points(const vec2_t pos, const int radius, const int completion, const int rotation, int segments) {
	std::vector<vec2_t> points;

	double ang = static_cast<double>( rotation * D3DX_PI ) / 180.0;
	double c = (completion / 100.f) * D3DX_PI;

	if (segments == -1)
		segments = std::clamp(radius, 8, 128);

	for (int i = 0; i < segments + 1; ++i) {
		double current_point = static_cast<double>(i) / segments;

		double x = pos.x + radius * cos(ang + 2 * c * current_point);
		double y = pos.y + radius * sin(ang + 2 * c * current_point);

		points.emplace_back(vec2_t(x, y));
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