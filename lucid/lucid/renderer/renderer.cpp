#include "renderer.h"

void lucid_engine::renderer::line(vec2_t from, vec2_t to, color_t color) {
	std::vector<vertex2d_t> vertices;

	vertices.emplace_back(vertex2d_t(from.x, from.y, color.translate()));
	vertices.emplace_back(vertex2d_t(to.x, to.y, color.translate()));

	lucid_engine::graphics::get_instance().direct_3d_device->DrawPrimitiveUP(D3DPT_LINELIST, vertices.size(), vertices.data(), sizeof(vertex2d_t));
}

void lucid_engine::renderer::rectangle(vec2_t pos, vec2_t size, color_t color) {
	std::vector<vertex2d_t> vertices;

	vertices.emplace_back(vertex2d_t(pos.x, pos.y, color.translate()));
	vertices.emplace_back(vertex2d_t(pos.x + size.x, pos.y, color.translate()));
	vertices.emplace_back(vertex2d_t(pos.x + size.x, pos.y + size.y, color.translate()));
	vertices.emplace_back(vertex2d_t(pos.x, pos.y + size.y, color.translate()));
	vertices.emplace_back(vertices.front());

	lucid_engine::graphics::get_instance().direct_3d_device->DrawPrimitiveUP(D3DPT_LINESTRIP, vertices.size() - 1, vertices.data(), sizeof(vertex2d_t));
}

void lucid_engine::renderer::filled_rectangle(vec2_t pos, vec2_t size, color_t color) {
	std::vector<vertex2d_t> vertices;

	vertices.emplace_back(vertex2d_t(pos.x, pos.y, color.translate()));
	vertices.emplace_back(vertex2d_t(pos.x + size.x, pos.y, color.translate()));
	vertices.emplace_back(vertex2d_t(pos.x + size.x, pos.y + size.y, color.translate()));
	vertices.emplace_back(vertex2d_t(pos.x, pos.y + size.y, color.translate()));
	vertices.emplace_back(vertices.front());

	lucid_engine::graphics::get_instance().direct_3d_device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vertices.size() - 1, vertices.data(), sizeof(vertex2d_t));
}