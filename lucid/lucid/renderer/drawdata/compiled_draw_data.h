#pragma once

struct compiled_draw_data_t {
	std::vector<draw_data_t> vertices;
	int total_vertex_count;
	int total_index_count;
};