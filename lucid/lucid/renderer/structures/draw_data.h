#pragma once

struct draw_command_t {
	bool						m_anti_aliased{ };
	texture_t					m_texture{ };
	std::vector<RECT>			m_clips{ };
};

struct draw_data_t {
	D3DPRIMITIVETYPE			m_draw_type{ };

	std::vector<vertex_t>		m_vertices{ };

	std::vector<std::uint32_t>	m_indices{ };

	int							m_vertex_count{ },
								m_index_count{ };

	draw_command_t				m_command{ };
};