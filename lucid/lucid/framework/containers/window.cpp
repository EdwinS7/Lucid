#include "../framework.h"

std::map<int, bool>     window_setup{ };
std::map<int, vec2_t>	window_pos{ },
						window_size{ };

containers::window::window(const char* title, vec2_t pos, vec2_t min_size, vec2_t size, bool open) {
	auto& window_id = lucid_engine::g_ui.get()->m_window_id;
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

	lucid_engine::g_ui.get()->m_window = this;
}

void containers::window::handle_render() {
	auto& window_id = lucid_engine::g_ui.get()->m_window_id;
	auto style = lucid_engine::g_ui.get()->get_style();
	auto renderer = lucid_engine::g_renderer.get();

	vec2_t text_size = renderer->get_text_size(renderer->m_defualt_font, m_title);

	//background
	renderer->filled_rectangle(m_pos, m_size, style->m_window_background);

	//side bar
	renderer->filled_rectangle(m_pos, vec2_t(150, m_size.y), style->m_panel_background);
	renderer->filled_rectangle(m_pos + vec2_t(150, 0), vec2_t(1, m_size.y), style->m_group_outline);

	//header
	renderer->filled_rectangle(m_pos, vec2_t(m_size.x, 45), style->m_window_header);
	renderer->filled_rectangle(m_pos + vec2_t(0, 44), vec2_t(m_size.x, 1), style->m_window_outline);

	//border
	renderer->rectangle(m_pos, m_size, style->m_window_outline);

	//title
	renderer->text(renderer->m_logo_font, m_title, m_pos + vec2_t(6, -1), style->m_text_active);
}

void containers::window::destroy() {
	auto& window_id = lucid_engine::g_ui.get()->m_window_id;

	//handle dragging and resizing.
	window_size[window_id] = lucid_engine::g_ui.get()->handle_resizing(this);
	window_pos[window_id] = lucid_engine::g_ui.get()->handle_dragging(this);
	m_size = window_size[window_id];
	m_pos = window_pos[window_id];

	//reset references.
	lucid_engine::g_ui.get()->m_hovering_element = false;
	lucid_engine::g_ui.get()->m_hovering_popup = false;

	//dereference our window.
	delete this;
}