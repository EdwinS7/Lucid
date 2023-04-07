#include "renderer.h"

void lucid_engine::renderer::create_objects() {
	if (!lucid_engine::graphics::get_instance().direct_3d_device)
		return;

	if (!vertex_declaration) {
		constexpr D3DVERTEXELEMENT9 elements[] {
				{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
				{ 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
				{ 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
				D3DDECL_END()
		};

		lucid_engine::graphics::get_instance().direct_3d_device->CreateVertexDeclaration(elements, &vertex_declaration);
	}
}

void lucid_engine::renderer::destroy_objects() {
	if (!lucid_engine::graphics::get_instance().direct_3d_device)
		return;

	if (vertex_declaration) { vertex_declaration->Release(); vertex_declaration = nullptr; }
	if (vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
	if (index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
}

void lucid_engine::renderer::render_draw_data() {
	static int vertex_buffer_size{ 5000 }, index_buffer_size{ 10000 };
	
	//compile our draw data.
	for (const draw_data_t& draw_data : draw_data) {
		compiled_draw_data.total_index_count += draw_data.index_count;
		compiled_draw_data.total_vertex_count += draw_data.vertex_count;
	}

	//create vertex buffer.
	if (!vertex_buffer || compiled_draw_data.total_vertex_count * sizeof(vertex_t) > vertex_buffer_size) {
		if (vertex_buffer) {
			vertex_buffer->Release();
			vertex_buffer = nullptr;
		}

		vertex_buffer_size = compiled_draw_data.total_vertex_count + 5000;

		if (lucid_engine::graphics::get_instance().direct_3d_device->CreateVertexBuffer(vertex_buffer_size * sizeof(vertex_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &vertex_buffer, nullptr) < 0)
			throw std::runtime_error{ "CreateVertexBuffer error" };
	}

	//create index buffer.
	if (!index_buffer || compiled_draw_data.total_index_count * sizeof(std::uint32_t) > index_buffer_size) {
		if (index_buffer) {
			index_buffer->Release();
			index_buffer = nullptr;
		}

		index_buffer_size = compiled_draw_data.total_index_count + 10000;

		if (lucid_engine::graphics::get_instance().direct_3d_device->CreateIndexBuffer(index_buffer_size * sizeof(std::uint32_t), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &index_buffer, nullptr) < 0)
			throw std::runtime_error{ "CreateIndexBuffer error" };
	}

	//create state block
	IDirect3DStateBlock9* d3d9_state_block{ };
	if (lucid_engine::graphics::get_instance().direct_3d_device->CreateStateBlock(D3DSBT_ALL, &d3d9_state_block) < 0)
		throw std::runtime_error{ "CreateStateBlock error" };

	//camera
	D3DMATRIX last_world{ }, last_view{ }, last_projection{ };
	lucid_engine::graphics::get_instance().direct_3d_device->GetTransform(D3DTS_WORLD, &last_world);
	lucid_engine::graphics::get_instance().direct_3d_device->GetTransform(D3DTS_VIEW, &last_view);
	lucid_engine::graphics::get_instance().direct_3d_device->GetTransform(D3DTS_PROJECTION, &last_projection);

	vertex_t* vertex_data{ };
	std::uint32_t* index_data{ };
	if (vertex_buffer->Lock(0, (UINT)(compiled_draw_data.total_vertex_count * sizeof(vertex_t)), (void**)&vertex_data, D3DLOCK_DISCARD) < 0)
		throw std::runtime_error{ "vtx_buffer->Lock error" };

	if (index_buffer->Lock(0, (UINT)(compiled_draw_data.total_index_count * sizeof(std::uint32_t)), (void**)&index_data, D3DLOCK_DISCARD) < 0)
		throw std::runtime_error{ "idx_buffer->Lock error" };

	for (const draw_data_t& draw_data : draw_data) {
		memcpy(vertex_data, draw_data.vertices.data(), draw_data.vertex_count * sizeof(vertex_t));
		memcpy(index_data, draw_data.indices.data(), draw_data.index_count * sizeof(std::uint32_t));
	}

	vertex_buffer->Unlock();
	index_buffer->Unlock();

	//set stream source and indices.
	lucid_engine::graphics::get_instance().direct_3d_device->SetIndices(index_buffer);
	lucid_engine::graphics::get_instance().direct_3d_device->SetStreamSource(0, vertex_buffer, 0, sizeof(vertex_t));
	//lucid_engine::graphics::get_instance().direct_3d_device->SetVertexDeclaration(vertex_declaration);

	for (const draw_data_t& draw_data : draw_data) {
		lucid_engine::graphics::get_instance().direct_3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, draw_data.vertex_count, 0, draw_data.index_count / 3);
	}

	//restore camera
	lucid_engine::graphics::get_instance().direct_3d_device->SetTransform(D3DTS_WORLD, &last_world);
	lucid_engine::graphics::get_instance().direct_3d_device->SetTransform(D3DTS_VIEW, &last_view);
	lucid_engine::graphics::get_instance().direct_3d_device->SetTransform(D3DTS_PROJECTION, &last_projection);

	//apply state block
	d3d9_state_block->Apply();
	d3d9_state_block->Release();

	//clear
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