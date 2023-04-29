#pragma once

struct draw_data_t {
	D3DPRIMITIVETYPE		   m_draw_type{ };
	std::vector<vertex_t>	   m_vertices{ };
	std::vector<std::uint32_t> m_indices{ };
	int						   m_vertex_count{ },
							   m_index_count{ };
	bool					   m_anti_alias{ };
	text_info_t				   m_text_info{ };
	clip_info_t                m_clip_info{ };
};