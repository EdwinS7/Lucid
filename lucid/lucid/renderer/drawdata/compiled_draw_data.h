#pragma once

struct compiled_draw_data_t {
	std::vector<vertex_t> vertices;
	std::vector<std::uint32_t> indices;
	int total_vertex_count;
	int total_index_count;
};