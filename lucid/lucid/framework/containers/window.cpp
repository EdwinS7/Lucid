#include "../framework.h"

std::map<int, bool>     window_setup{ };
std::map<int, vec2_t>	window_pos{ },
						window_size{ };

containers::window::window(const char* title, vec2_t pos, vec2_t min_size, vec2_t size, bool open) {
	auto& window_id = lucid_engine::g_ui->m_window_id;
	window_id++;

	m_title = title;
	m_pos = pos;
	m_size = size;
	m_min_size = min_size;
	m_open = open;

	if (!window_setup[window_id]) {
		window_pos[window_id] = m_pos;
		window_size[window_id] = m_size;
		window_setup[window_id] = true;
	}
	else {
		m_pos = window_pos[window_id];
		m_size = window_size[window_id];
	}

	handle_render();

	lucid_engine::g_ui->m_window = this;
}

void containers::window::handle_render()
{
	auto& window_id = lucid_engine::g_ui->m_window_id;
	auto style = lucid_engine::g_ui->get_style();

	vec2_t text_size = lucid_engine::g_renderer->get_text_size(lucid_engine::g_renderer->m_defualt_font, m_title);

	// background
	lucid_engine::g_renderer->filled_rectangle(m_pos, m_size, style->m_window_background);

	// resize triangle
	lucid_engine::g_renderer->polygon({m_pos + m_size - vec2_t(10, 0), m_pos + m_size - vec2_t(0, 10), m_pos + m_size}, style->m_accent);

	// side bar
	lucid_engine::g_renderer->filled_rectangle(m_pos, vec2_t(150, m_size.y), style->m_panel_background);
	lucid_engine::g_renderer->filled_rectangle(m_pos + vec2_t(150, 0), vec2_t(1, m_size.y), style->m_group_outline);

	// header
	lucid_engine::g_renderer->filled_rectangle(m_pos, vec2_t(m_size.x, 45), style->m_window_header);
	lucid_engine::g_renderer->filled_rectangle(m_pos + vec2_t(0, 44), vec2_t(m_size.x, 1), style->m_window_outline);

	// border
	lucid_engine::g_renderer->rectangle(m_pos, m_size, style->m_window_outline);

	// title
	lucid_engine::g_renderer->text(lucid_engine::g_renderer->m_logo_font, m_title, m_pos + vec2_t(6, 2), style->m_accent);
}

void containers::window::destroy() {
	auto& window_id = lucid_engine::g_ui->m_window_id;

	// handle dragging and resizing.
	window_size[window_id] = lucid_engine::g_ui->handle_resizing(this);
	window_pos[window_id] = lucid_engine::g_ui->handle_dragging(this);
	m_size = window_size[window_id];
	m_pos = window_pos[window_id];

	// reset references.
	lucid_engine::g_ui->m_hovering_element = false;
	lucid_engine::g_ui->m_hovering_popup = false;

	// dereference our window.
	delete this;
}