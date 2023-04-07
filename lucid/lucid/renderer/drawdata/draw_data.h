#pragma once

struct draw_data_t {
	std::vector<vertex_t> vertices;
	std::vector<unsigned int> indices;
	int vertex_count;
	int index_count;
};