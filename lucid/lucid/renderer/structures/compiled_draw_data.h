#pragma once

struct compiled_draw_data_t {
	std::vector<vertex_t>	   m_vertices{ };
	std::vector<std::uint32_t> m_indices{ };

	int						   m_total_vertex_count{ },
							   m_total_index_count{ };
};