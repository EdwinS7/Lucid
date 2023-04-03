#include "renderer.h"

compiled_draw_data_t lucid_engine::renderer::get_draw_data() {
	compiled_draw_data_t compiled;
	compiled.vertices = draw_data;
	
	return compiled;
}

void lucid_engine::renderer::render_draw_data(compiled_draw_data_t draw_data) {
	static int vertex_buffer_size{ 5000 }, index_buffer_size{ 10000 };

	if (!vertex_buffer || vertex_buffer_size < draw_data.total_vertex_count) {
		if (vertex_buffer) {
			vertex_buffer->Release();
			vertex_buffer = nullptr;
		}

		vertex_buffer_size = draw_data.total_vertex_count + 5000;

		if (lucid_engine::graphics::get_instance().direct_3d_device->CreateVertexBuffer(vertex_buffer_size * sizeof(vertex_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &vertex_buffer, nullptr) < 0)
			throw std::runtime_error{ "CreateVertexBuffer error" };
	}

	if (!index_buffer || index_buffer_size < draw_data.total_index_count) {
		if (index_buffer) {
			index_buffer->Release();
			index_buffer = nullptr;
		}

		index_buffer_size = draw_data.total_index_count + 10000;

		if (lucid_engine::graphics::get_instance().direct_3d_device->CreateIndexBuffer(index_buffer_size * sizeof(std::uint32_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &index_buffer, nullptr) < 0)
			throw std::runtime_error{ "CreateIndexBuffer error" };
	}

	IDirect3DStateBlock9* d3d9_state_block{ };
	if (lucid_engine::graphics::get_instance().direct_3d_device->CreateStateBlock(D3DSBT_ALL, &d3d9_state_block) < 0)
		throw std::runtime_error{ "CreateStateBlock error" };

	D3DMATRIX last_world{ }, last_view{ }, last_projection{ };
	lucid_engine::graphics::get_instance().direct_3d_device->GetTransform(D3DTS_WORLD, &last_world);
	lucid_engine::graphics::get_instance().direct_3d_device->GetTransform(D3DTS_VIEW, &last_view);
	lucid_engine::graphics::get_instance().direct_3d_device->GetTransform(D3DTS_PROJECTION, &last_projection);

	vertex_t* vtx_dst{ };
	std::uint32_t* idx_dst{ };

	if (vertex_buffer->Lock(0, (UINT)(draw_data.total_vertex_count * sizeof(vertex_t)), (void**)&vtx_dst, D3DLOCK_DISCARD) < 0)
		throw std::runtime_error{ "vertex_buffer->Lock error" };
	if (index_buffer->Lock(0, (UINT)(draw_data.total_index_count * sizeof(std::uint32_t)), (void**)&idx_dst, D3DLOCK_DISCARD) < 0)
		throw std::runtime_error{ "index_buffer->Lock error" };

	vertex_buffer->Unlock();
	index_buffer->Unlock();
	lucid_engine::graphics::get_instance().direct_3d_device->SetStreamSource(0, vertex_buffer, 0, sizeof(vertex_t));
	lucid_engine::graphics::get_instance().direct_3d_device->SetIndices(index_buffer);
	lucid_engine::graphics::get_instance().direct_3d_device->SetVertexDeclaration(vertex_declaration);
	lucid_engine::graphics::get_instance().direct_3d_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
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

	lucid_engine::graphics::get_instance().direct_3d_device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vertices.size() - 1, vertices.data(), sizeof(vertex_t));
}