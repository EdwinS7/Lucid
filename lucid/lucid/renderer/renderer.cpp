#include "renderer.h"

void lucid_engine::renderer::create_objects() {
	if (!lucid_engine::graphics::get_instance().direct_3d_device)
		throw std::runtime_error{ "create_objects error { device is not setup }" };

	D3DXCreateSprite(lucid_engine::graphics::get_instance().direct_3d_device, &font_sprite);
	fonts.default_font = create_font("Segoe UI", 13, 400, font_flags_t(true, false, false));
	fonts.primordial_icons = create_font("Primordial-Icons", 26, 400, font_flags_t(true, false, false));
}

void lucid_engine::renderer::destroy_objects() {
	if (!lucid_engine::graphics::get_instance().direct_3d_device)
		return;

	if (vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
	if (index_buffer) { index_buffer->Release(); index_buffer = nullptr; }

	if (font_sprite) { font_sprite->Release(); font_sprite = nullptr; }

	//fonts
	if (fonts.default_font.dx_font) { fonts.default_font.dx_font->Release(); fonts.default_font.dx_font = nullptr; }
	if (fonts.primordial_icons.dx_font) { fonts.primordial_icons.dx_font->Release(); fonts.primordial_icons.dx_font = nullptr; }
}

void lucid_engine::renderer::render_draw_data() {
	static int vertex_buffer_size{ 5000 }, index_buffer_size{ 10000 };

	for (const draw_data_t& data : draw_data) {
		for (vertex_t vertex : data.vertices)
			compiled_draw_data.vertices.emplace_back(vertex);

		for (std::uint32_t index : data.indices)
			compiled_draw_data.indices.emplace_back(index);

		compiled_draw_data.total_index_count += data.index_count;
		compiled_draw_data.total_vertex_count += data.vertex_count;
	}

	if (!vertex_buffer || compiled_draw_data.total_vertex_count * sizeof(vertex_t) > vertex_buffer_size) {
		if (vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }

		vertex_buffer_size = compiled_draw_data.total_vertex_count + 5000;

		if (lucid_engine::graphics::get_instance().direct_3d_device->CreateVertexBuffer(vertex_buffer_size * sizeof(vertex_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &vertex_buffer, nullptr) < 0)
			throw std::runtime_error{ "CreateVertexBuffer error" };
	}
	if (!index_buffer || compiled_draw_data.total_index_count * sizeof(std::uint32_t) > index_buffer_size) {
		if (index_buffer) { index_buffer->Release(); index_buffer = nullptr; }

		index_buffer_size = compiled_draw_data.total_index_count + 10000;

		if (lucid_engine::graphics::get_instance().direct_3d_device->CreateIndexBuffer(index_buffer_size * sizeof(std::uint32_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &index_buffer, nullptr) < 0)
			throw std::runtime_error{ "CreateIndexBuffer error" };
	}

	IDirect3DStateBlock9* state_block{ };
	if (lucid_engine::graphics::get_instance().direct_3d_device->CreateStateBlock(D3DSBT_ALL, &state_block) < 0)
		return;

	if (state_block->Capture() < 0) {
		throw std::runtime_error{ "state_block->Capture error" };
		state_block->Release();
		return;
	}

	D3DMATRIX last_world, last_view, last_projection;
	lucid_engine::graphics::get_instance().direct_3d_device->GetTransform(D3DTS_WORLD, &last_world);
	lucid_engine::graphics::get_instance().direct_3d_device->GetTransform(D3DTS_VIEW, &last_view);
	lucid_engine::graphics::get_instance().direct_3d_device->GetTransform(D3DTS_PROJECTION, &last_projection);

	vertex_t* vertex_data{ };
	std::uint32_t* index_data{ };
	if (vertex_buffer->Lock(0, (UINT)(compiled_draw_data.total_vertex_count * sizeof(vertex_t)), (void**)&vertex_data, D3DLOCK_DISCARD) < 0) {
		throw std::runtime_error{ "vtx_buffer->Lock error" };
		state_block->Release();
		return;
	}

	if (index_buffer->Lock(0, (UINT)(compiled_draw_data.total_index_count * sizeof(std::uint32_t)), (void**)&index_data, D3DLOCK_DISCARD) < 0) {
		throw std::runtime_error{ "idx_buffer->Lock error" };
		vertex_buffer->Unlock();
		state_block->Release();
		return;
	}

	memcpy(vertex_data, compiled_draw_data.vertices.data(), compiled_draw_data.total_vertex_count * sizeof(vertex_t));
	memcpy(index_data, compiled_draw_data.indices.data(), compiled_draw_data.total_index_count * sizeof(std::uint32_t));

	vertex_buffer->Unlock();
	index_buffer->Unlock();

	lucid_engine::graphics::get_instance().direct_3d_device->SetStreamSource(0, vertex_buffer, 0, sizeof(vertex_t));
	lucid_engine::graphics::get_instance().direct_3d_device->SetIndices(index_buffer);

	int start_vertex = 0;
	int start_index = 0;
	for (const draw_data_t& data : draw_data) {
		text_info_t text_info = data.text_info;

		lucid_engine::graphics::get_instance().direct_3d_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, data.anti_alias);

		if (text_info.setup) {
			RECT rect = { text_info.pos.x, text_info.pos.y, 0, 0 };
			text_info.font.dx_font->DrawTextA(NULL, text_info.string.c_str(), -1, &rect, text_info.flags, color_t::translate(text_info.color));
		}
		else
			lucid_engine::graphics::get_instance().direct_3d_device->DrawIndexedPrimitive(data.draw_type, start_vertex, 0, data.vertex_count, start_index, data.index_count / 3);

		start_vertex += data.vertex_count;
		start_index += data.index_count;
	}

	lucid_engine::graphics::get_instance().direct_3d_device->SetTransform(D3DTS_WORLD, &last_world);
	lucid_engine::graphics::get_instance().direct_3d_device->SetTransform(D3DTS_VIEW, &last_view);
	lucid_engine::graphics::get_instance().direct_3d_device->SetTransform(D3DTS_PROJECTION, &last_projection);

	state_block->Apply();
	state_block->Release();

	compiled_draw_data = {};
	draw_data.clear();
}

void lucid_engine::renderer::write_vertex(const D3DPRIMITIVETYPE type, const std::vector<vertex_t>& vertices, bool anti_alias, const text_info_t& text_info) {
	if (vertices.empty())
		throw std::runtime_error{ "write_vertex error { vertices is empty }" };

	std::vector<unsigned int> indices(type == D3DPT_LINESTRIP ? vertices.size() * 3 - 1 : vertices.size() * 3);

	for (unsigned int i = 0; i < vertices.size(); i++)
		indices[i] = i;

	draw_data.emplace_back(draw_data_t{ type, vertices, indices, static_cast<int>(vertices.size()), static_cast<int>(indices.size()), anti_alias, text_info });
}

void lucid_engine::renderer::line(const vec2_t from, const vec2_t to, const color_t color, const bool anti_alias) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(from.x, from.y, 0.f, 1.f, color_t::translate(color)));
	vertices.emplace_back(vertex_t(to.x, to.y, 0.f, 1.f, color_t::translate(color)));

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
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
		vertices.front()
	};

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
	if (radius < 0.5f || flags == none) {
		rectangle(pos, size, color);
		return;
	}

	const bool round_top_left = (flags & corner_top_left) != 0;
	const bool round_top_right = (flags & corner_top_right) != 0;
	const bool round_bottom_left = (flags & corner_bottom_left) != 0;
	const bool round_bottom_right = (flags & corner_bottom_right) != 0;

	std::vector<vec2_t> points;

	auto gen_points = {
		std::tuple{vec2_t(pos.x, pos.y), vec2_t(pos.x + radius, pos.y + radius), 180, round_top_left},
		std::tuple{vec2_t(pos.x + size.x, pos.y), vec2_t(pos.x + size.x - radius, pos.y + radius), 270, round_top_right},
		std::tuple{vec2_t(pos.x + size.x, pos.y + size.y), vec2_t(pos.x + size.x - radius, pos.y + size.y - radius), 0, round_bottom_right},
		std::tuple{vec2_t(pos.x, pos.y + size.y), vec2_t(pos.x + radius, pos.y + size.y - radius), 90, round_bottom_left}
	};

	for (const auto& [corner, corner_rounded, angle, should_round] : gen_points) {
		if (should_round) {
			auto corner_points = generate_circle_points(corner_rounded, radius, 25, angle);
			points.insert(points.end(), corner_points.begin(), corner_points.end());
		}
		else
			points.push_back(corner);
	}

	points.emplace_back(points.front());

	polyline(points, color);
}

void lucid_engine::renderer::filled_rounded_rectangle(const vec2_t pos, const vec2_t size, const color_t color, const int radius, const corner_flags flags) {
	if (radius < 0.5f || flags == none) {
		filled_rectangle(pos, size, color);
		return;
	}

	const bool round_top_left = (flags & corner_top_left) != 0;
	const bool round_top_right = (flags & corner_top_right) != 0;
	const bool round_bottom_left = (flags & corner_bottom_left) != 0;
	const bool round_bottom_right = (flags & corner_bottom_right) != 0;

	std::vector<vec2_t> points;

	auto gen_points = {
		std::tuple{vec2_t(pos.x, pos.y), vec2_t(pos.x + radius, pos.y + radius), 180, round_top_left},
		std::tuple{vec2_t(pos.x + size.x, pos.y), vec2_t(pos.x + size.x - radius, pos.y + radius), 270, round_top_right},
		std::tuple{vec2_t(pos.x + size.x, pos.y + size.y), vec2_t(pos.x + size.x - radius, pos.y + size.y - radius), 0, round_bottom_right},
		std::tuple{vec2_t(pos.x, pos.y + size.y), vec2_t(pos.x + radius, pos.y + size.y - radius), 90, round_bottom_left}
	};

	for (const auto& [corner, corner_rounded, angle, should_round] : gen_points) {
		if (should_round) {
			auto corner_points = generate_circle_points(corner_rounded, radius, 25, angle);
			points.insert(points.end(), corner_points.begin(), corner_points.end());
		}
		else
			points.push_back(corner);
	}

	polygon(points, color);
}

void lucid_engine::renderer::gradient(const vec2_t pos, const vec2_t size, const color_t left, const color_t right, const bool vertical) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(left)),
		vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, vertical ? color_t::translate(left) : color_t::translate(right)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(right)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, vertical ? color_t::translate(right) : color_t::translate(left)),
		vertices.front()
	};

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
		vertices.front()
	};

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
		vertices.front()
	};

	write_vertex(D3DPT_LINESTRIP, vertices);
}

void lucid_engine::renderer::filled_triangle(const vec2_t pos, const vec2_t size, const color_t color) {
	std::vector<vertex_t> vertices = {
		vertex_t(pos.x + size.x / 2, pos.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
		vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color_t::translate(color)),
		vertices.front()
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

	float ang = rotation * D3DX_PI / 180;
	float c = (completion / 100.f) * D3DX_PI;

	if (segments == -1)
		segments = std::clamp(radius, 8, 128);

	for (int i = 0; i < segments + 1; i++) {
		float current_point = static_cast<float>(i) / segments;

		float x = pos.x + radius * cos(2 * c * current_point + ang);
		float y = pos.y + radius * sin(2 * c * current_point + ang);

		points.emplace_back(vec2_t(x, y));
	}

	return points;
}

void lucid_engine::renderer::circle(const vec2_t pos, int radius, int completion, int rotation, const color_t color) {
	std::vector<vec2_t> points = generate_circle_points(pos, radius, completion, rotation, CIRCLE_SEGMENTS);
	std::vector<vertex_t> vertices;

	for (const auto& point : points)
		vertices.emplace_back(vertex_t(point.x, point.y, 0.f, 1.f, color_t::translate(color)));

	write_vertex(D3DPT_LINESTRIP, vertices, true);
}

void lucid_engine::renderer::filled_circle(const vec2_t pos, int radius, int completion, int rotation, const color_t color) {
	std::vector<vec2_t> points = generate_circle_points(pos, radius, completion, rotation, CIRCLE_SEGMENTS);
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(color)));

	for (const auto& point : points)
		vertices.emplace_back(vertex_t(point.x, point.y, 0.f, 1.f, color_t::translate(color)));

	write_vertex(D3DPT_TRIANGLEFAN, vertices, true);
}

void lucid_engine::renderer::gradient_circle(const vec2_t pos, int radius, int completion, int rotation, const color_t color, const color_t color2) {
	std::vector<vec2_t> points = generate_circle_points(pos, radius, completion, rotation, CIRCLE_SEGMENTS);
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(color)));

	for (const auto& point : points)
		vertices.emplace_back(vertex_t(point.x, point.y, 0.f, 1.f, color_t::translate(color2)));

	write_vertex(D3DPT_TRIANGLEFAN, vertices, true);
}

font_t lucid_engine::renderer::create_font(const std::string font_name, const int size, const int weight, const font_flags_t font_flags) {
	return font_t(lucid_engine::graphics::get_instance().direct_3d_device, font_name.c_str(), size, weight, font_flags);
}

void lucid_engine::renderer::text(const font_t font, const std::string string, const vec2_t pos, const color_t color, const text_flags_t flags) {
	std::vector<vertex_t> vertices;
	DWORD text_flags = {};

	switch (flags.alignment) {
	case text_alignment::left:
		text_flags = DT_LEFT | DT_NOCLIP;

		break;
	case text_alignment::right:
		text_flags = DT_RIGHT | DT_NOCLIP;

		break;
	case text_alignment::center_x:
		text_flags = DT_CENTER | DT_NOCLIP;

		break;
	case text_alignment::center_y:
		text_flags = DT_VCENTER | DT_NOCLIP;

		break;
	case text_alignment::center:
		text_flags = DT_CENTER | DT_VCENTER | DT_NOCLIP;
		break;
	}

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, color_t::translate(color)));
	write_vertex(D3DPT_TRIANGLEFAN, vertices, true, text_info_t(font, string, pos, color, text_flags));
}

vec2_t lucid_engine::renderer::get_text_size(const font_t font, const std::string string) {
	RECT text_clip; 

	font.dx_font->DrawTextA(0, string.c_str(), strlen(string.c_str()), &text_clip, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));

	return vec2_t(text_clip.right - text_clip.left, text_clip.bottom - text_clip.top);
}