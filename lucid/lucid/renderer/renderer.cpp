#include "renderer.h"

void lucid_engine::renderer::create_objects() {
	if (!lucid_engine::graphics::get_instance().direct_3d_device)
		return;
}

void lucid_engine::renderer::destroy_objects() {
	if (!lucid_engine::graphics::get_instance().direct_3d_device)
		return;

	if (vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
	if (index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
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
		lucid_engine::graphics::get_instance().direct_3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, start_vertex, 0, data.vertex_count, start_index, data.index_count / 3);
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

void lucid_engine::renderer::line(vec2_t from, vec2_t to, color_t color) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(from.x, from.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(to.x, to.y, 0.f, 1.f, color.translate()));

	lucid_engine::graphics::get_instance().direct_3d_device->DrawPrimitiveUP(D3DPT_LINELIST, vertices.size(), vertices.data(), sizeof(vertex_t));
}

void lucid_engine::renderer::rectangle(vec2_t pos, vec2_t size, color_t color) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertices.front());

	lucid_engine::graphics::get_instance().direct_3d_device->DrawPrimitiveUP(D3DPT_LINESTRIP, vertices.size() - 1, vertices.data(), sizeof(vertex_t));
}

void lucid_engine::renderer::filled_rectangle(vec2_t pos, vec2_t size, color_t color) {
	std::vector<vertex_t> vertices;

	vertices.emplace_back(vertex_t(pos.x, pos.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x + size.x, pos.y + size.y, 0.f, 1.f, color.translate()));
	vertices.emplace_back(vertex_t(pos.x, pos.y + size.y, 0.f, 1.f, color.translate()));

	std::vector<unsigned int> indices = {
		0, 1,
		2, 3,
		3, 4
	};

	draw_data.emplace_back(draw_data_t{ vertices, indices, static_cast<int>(vertices.size()), static_cast<int>(indices.size()) });
}