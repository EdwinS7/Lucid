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
	if (fonts.default_font.dx_font) { fonts.default_font.dx_font->Release(); fonts.default_font.dx_font = nullptr; }
}

void lucid_engine::renderer::render_draw_data() {
	static int vertex_buffer_size{ 5000 }, index_buffer_size{ 10000 };

	for (const draw_data_t& data : draw_data) {
		for (vertex_t vertex : data.vertices)
			compiled_draw_data.vertices.push_back(vertex);

		for (std::uint32_t index : data.indices)
			compiled_draw_data.indices.push_back(index);

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
			text_info.font.dx_font->DrawTextA(NULL, text_info.string.c_str(), -1, &rect, text_info.flags, text_info.color.translate());
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

void lucid_engine::renderer::write_vertex(D3DPRIMITIVETYPE type, std::vector<vertex_t> vertices, bool anti_alias, text_info_t text_info) {
	std::vector<unsigned int> indices(type == D3DPT_LINESTRIP ? vertices.size() * 3 - 1 : vertices.size() * 3);

	for (unsigned int i = 0; i < vertices.size(); i++)
		indices[i] = i;

	draw_data.emplace_back(draw_data_t{ type, vertices, indices, static_cast<int>(vertices.size()), static_cast<int>(indices.size()), anti_alias, text_info });
}

void lucid_engine::renderer::line(vec2_t from, vec2_t to, color_t color, bool anti_alias) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(from.x, from.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(to.x, to.y, 0.f, 1.f, color.translate()));

	write_vertex(D3DPT_LINELIST, vertices, anti_alias);
}

void lucid_engine::renderer::polyline(std::vector<vec2_t> points, color_t color, bool anti_alias) {
	std::vector<vertex_t> vertices;

	for (const vec2_t& point : points)
		vertices.emplace_back(vertex_t(point.x, point.y, 0.f, 1.f, color.translate()));

	write_vertex(D3DPT_LINESTRIP, vertices, anti_alias);
}

void lucid_engine::renderer::polygon(std::vector<vec2_t> points, color_t color, bool anti_alias) {
	std::vector<vertex_t> vertices;

	for (const vec2_t& point : points)
		vertices.emplace_back(vertex_t(point.x, point.y, 0.f, 1.f, color.translate()));

	write_vertex(D3DPT_TRIANGLEFAN, vertices, anti_alias);
}

void lucid_engine::renderer::rectangle(vec2_t pos, vec2_t size, color_t color) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertices.front());

	write_vertex(D3DPT_LINESTRIP, vertices);
}

void lucid_engine::renderer::filled_rectangle(vec2_t pos, vec2_t size, color_t color) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color.translate()));

	write_vertex(D3DPT_TRIANGLEFAN, vertices);
}

void lucid_engine::renderer::rounded_rectangle(vec2_t pos, vec2_t size, color_t color, int radius, corner_flags flags) {
	if (radius < 0.5f || flags == none)
		rectangle(pos, size, color);
	
	//Not used as of right now.
	/*const bool round_top_left = (flags & top_left) != 0;
	const bool round_top_right = (flags & top_right) != 0;
	const bool round_bottom_left = (flags & bottom_left) != 0;
	const bool round_bottom_right = (flags & bottom_right) != 0;*/

	line(vec2_t(pos.x + radius, pos.y), vec2_t(pos.x + size.x - radius, pos.y), color, true);
	line(vec2_t(pos.x + size.x, pos.y + radius), vec2_t(pos.x + size.x, pos.y + size.y - radius), color, true);
	line(vec2_t(pos.x + radius, pos.y + size.y), vec2_t(pos.x + size.x - radius, pos.y + size.y), color, true);
	line(vec2_t(pos.x, pos.y + radius), vec2_t(pos.x, pos.y + size.y - radius), color, true);

	circle(pos + vec2_t(radius, radius), radius, 25, 180, color);
	circle(pos + vec2_t(size.x - radius, radius), radius, 25, 270, color);
	circle(pos + vec2_t(size.x - radius, size.y - radius), radius, 25, 0, color);
	circle(pos + vec2_t(radius, size.y - radius), radius, 25, 90, color);
}

void lucid_engine::renderer::filled_rounded_rectangle(vec2_t pos, vec2_t size, color_t color, int radius, corner_flags flags) {
	if (radius < 0.5f || flags == none)
		filled_rectangle(pos, size, color);

	//Not used as of right now.
	const bool round_top_left = (flags & corner_top_left) != 0;
	const bool round_top_right = (flags & corner_top_right) != 0;
	const bool round_bottom_left = (flags & corner_bottom_left) != 0;
	const bool round_bottom_right = (flags & corner_bottom_right) != 0;

	std::vector<vec2_t> corner_points = {
		vec2_t(pos.x + (round_top_left ? radius: 0), pos.y + (round_top_left ? radius : 0)),
		vec2_t(pos.x + radius, pos.y),
		vec2_t(pos.x + size.x - radius, pos.y),
		vec2_t(pos.x + size.x - (round_top_right ? radius : 0), pos.y + (round_top_right ? radius : 0)),
		vec2_t(pos.x + size.x, pos.y + radius),
		vec2_t(pos.x + size.x, pos.y + size.y - radius),
		vec2_t(pos.x + size.x - (round_bottom_right ? radius : 0), pos.y + size.y - (round_bottom_right ? radius : 0)),
		vec2_t(pos.x + size.x - radius, pos.y + size.y),
		vec2_t(pos.x + radius, pos.y + size.y),
		vec2_t(pos.x + (round_bottom_left ? radius : 0), pos.y + size.y - (round_bottom_left ? radius : 0)),
		vec2_t(pos.x, pos.y + size.y - radius),
		vec2_t(pos.x, pos.y + radius)
	};

	filled_circle(pos + vec2_t(radius, radius), radius, 25, 180, color);
	filled_circle(pos + vec2_t(radius, radius) + vec2_t(size.x - radius * 2, 0), radius, 25, 270, color);
	filled_circle(pos + vec2_t(radius, radius) + vec2_t(size.x - radius * 2, size.y - radius * 2), radius, 25, 0, color);
	filled_circle(pos + vec2_t(radius, radius) + vec2_t(0, size.y - radius * 2), radius, 25, 90, color);
	polygon(corner_points, color, true);
}

void lucid_engine::renderer::gradient(vec2_t pos, vec2_t size, color_t left, color_t right, bool vertical) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, left.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, vertical ? left.translate() : right.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, right.translate()));
	vertices.emplace_back(vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, vertical ? right.translate() : left.translate()));
	vertices.emplace_back(vertices.front());

	write_vertex(D3DPT_LINESTRIP, vertices);
}

void lucid_engine::renderer::filled_gradient(vec2_t pos, vec2_t size, color_t left, color_t right, bool vertical) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, left.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, vertical ? left.translate() : right.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, right.translate()));
	vertices.emplace_back(vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, vertical ? right.translate() : left.translate()));

	write_vertex(D3DPT_TRIANGLEFAN, vertices);
}

void lucid_engine::renderer::gradient_four(vec2_t pos, vec2_t size, color_t top_left, color_t top_right, color_t bottom_right, color_t bottom_left) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, top_left.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, top_right.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, bottom_right.translate()));
	vertices.emplace_back(vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, bottom_left.translate()));
	vertices.emplace_back(vertices.front());

	write_vertex(D3DPT_LINESTRIP, vertices);
}

void lucid_engine::renderer::filled_gradient_four(vec2_t pos, vec2_t size, color_t top_left, color_t top_right, color_t bottom_right, color_t bottom_left) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, top_left.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, top_right.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, bottom_right.translate()));
	vertices.emplace_back(vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, bottom_left.translate()));

	write_vertex(D3DPT_TRIANGLEFAN, vertices);
}

void lucid_engine::renderer::triangle(vec2_t pos, vec2_t size, color_t color) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x + size.x / 2, pos.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertices.front());

	write_vertex(D3DPT_LINESTRIP, vertices);
}

void lucid_engine::renderer::filled_triangle(vec2_t pos, vec2_t size, color_t color) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x + size.x / 2, pos.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color.translate()));

	write_vertex(D3DPT_TRIANGLEFAN, vertices);
}

void lucid_engine::renderer::gradient_triangle(vec2_t pos, vec2_t size, color_t color, color_t color2) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x + size.x / 2, pos.y, 0.f, 1.f, color2.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x / 2, pos.y + size.y / 2, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color2.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x / 2, pos.y + size.y / 2, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color2.translate()));

	write_vertex(D3DPT_TRIANGLESTRIP, vertices);
}

void lucid_engine::renderer::circle(vec2_t pos, int size, int completion, int rotation, color_t color) {
	std::vector<vertex_t> vertices;

	float angle = rotation * D3DX_PI / 180;
	float _completion = (completion / 100.f) * D3DX_PI;

	for (int i = 0; i < 64 + 1; i++) {
		float calc = static_cast<float>(i) / 64;

		float x = pos.x + size * cos(2 * _completion * calc + angle);
		float y = pos.y + size * sin(2 * _completion * calc + angle);

		vertices.emplace_back(vertex_t(x, y, 0.f, 1.f, color.translate()));
	}

	write_vertex(D3DPT_LINESTRIP, vertices, true);
}

void lucid_engine::renderer::filled_circle(vec2_t pos, int size, int completion, int rotation, color_t color) {
	std::vector<vertex_t> vertices;

	float angle = rotation * D3DX_PI / 180;
	float _completion = (completion / 100.f) * D3DX_PI;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, color.translate()));

	for (int i = 0; i < 64 + 1; i++) {
		float calc = static_cast<float>(i) / 64;

		float x = pos.x + size * cos(2 * _completion * calc + angle);
		float y = pos.y + size * sin(2 * _completion * calc + angle);

		vertices.emplace_back(vertex_t(x, y, 0.f, 1.f, color.translate()));
	}

	write_vertex(D3DPT_TRIANGLEFAN, vertices, true);
}

void lucid_engine::renderer::gradient_circle(vec2_t pos, int size, int completion, int rotation, color_t color, color_t color2) {
	std::vector<vertex_t> vertices;

	float angle = rotation * D3DX_PI / 180;
	float _completion = (completion / 100.f) * D3DX_PI;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, color.translate()));

	for (int i = 0; i < 64 + 1; i++) {
		float calc = static_cast<float>(i) / 64;

		float x = pos.x + size * cos(2 * _completion * calc + angle);
		float y = pos.y + size * sin(2 * _completion * calc + angle);

		vertices.emplace_back(vertex_t(x, y, 0.f, 1.f, color2.translate()));
	}

	write_vertex(D3DPT_TRIANGLEFAN, vertices, true);
}

font_t lucid_engine::renderer::create_font(std::string font_name, int size, int weight, font_flags_t font_flags) {
	return font_t(lucid_engine::graphics::get_instance().direct_3d_device, font_name.c_str(), size, weight, font_flags);
}

void lucid_engine::renderer::text(font_t font, std::string string, vec2_t pos, color_t color, text_flags_t flags) {
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

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, color.translate()));
	write_vertex(D3DPT_TRIANGLEFAN, vertices, true, text_info_t(font, string, pos, color, text_flags));
}

vec2_t lucid_engine::renderer::get_text_size(font_t font, std::string string) {
	RECT text_clip; 
	font.dx_font->DrawTextA(0, string.c_str(), strlen(string.c_str()), &text_clip, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));

	return vec2_t(text_clip.right - text_clip.left, text_clip.bottom - text_clip.top);
}