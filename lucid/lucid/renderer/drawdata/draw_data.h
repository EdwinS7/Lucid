#pragma once

struct draw_data_t {
	D3DPRIMITIVETYPE draw_type;
	std::vector<vertex_t> vertices;
	std::vector<unsigned int> indices;
	int vertex_count;
	int index_count;
	bool anti_alias;
	text_info_t text_info;
};