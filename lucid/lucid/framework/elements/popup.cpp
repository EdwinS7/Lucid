#include "../framework.h"

void lucid_engine::ui::popup(const char* title, vec2_t pos, color_t color) {
	vec2_t text_size = g_renderer->get_text_size(g_renderer->m_defualt_font, title);

	// set draw list to foreground.
	g_renderer->set_draw_list(draw_list_t::foreground_draw_list);

	// render our element.
	g_renderer->filled_rectangle(pos, vec2_t(4 + text_size.x, 2 + text_size.y), m_style->m_element_inactive);
	g_renderer->rectangle(pos + vec2_t(1, 1), vec2_t(2 + text_size.x, text_size.y), m_style->m_element_in_outline);
	g_renderer->rectangle(pos, vec2_t(4 + text_size.x, 2 + text_size.y), m_style->m_element_outline);
	g_renderer->text(g_renderer->m_defualt_font, title, pos + vec2_t(2, 0), color);

	// reset draw list to default.
	g_renderer->set_draw_list(draw_list_t::default_draw_list);
}