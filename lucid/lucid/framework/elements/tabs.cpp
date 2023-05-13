#include "../framework.h"

vec2_t draw_position{ };
void lucid_engine::ui::set_tabs_pos(vec2_t pos) {
	// used only for setting where you want the m_tabs to go.
	draw_position = pos;
}

void lucid_engine::ui::add_tab(const char* title) {
	// add a new m_tab to our vector.
	m_tabs.emplace_back(tab_info_t(title));
}

void lucid_engine::ui::handle_tabs() {
	// get the draw position and fix the draw position to work with the amount of m_tabs. (center)
	draw_position.x = m_window->m_pos.x;
	draw_position.y = m_window->m_pos.y + 56;

	// each m_tab in our vector.
	for (int i = 0; i < m_tabs.size(); i++) {
		// calculate text sizes.
		vec2_t text_size = g_renderer->get_text_size(g_renderer->m_defualt_font, m_tabs[i].m_title);

		// check for hovering affect and used for click detection.
		bool hovered = g_input->mouse_hovering_rect(draw_position, {148, 30});

		// render our box and text(icon & bio).
		if (m_tab == i) {
			g_renderer->filled_gradient(draw_position + vec2_t(2, 0), vec2_t(146, 30), m_style->m_window_outline, m_style->m_window_outline.override_alpha(0));
			g_renderer->filled_rectangle(draw_position, vec2_t(2, 30), m_style->m_accent);

			g_renderer->text(g_renderer->m_defualt_font, m_tabs[i].m_title, draw_position + vec2_t(15, (30 * 0.5) - text_size.y * 0.5), m_style->m_accent);
		}
		else {
			if (hovered) {
				g_renderer->filled_gradient(draw_position + vec2_t(2, 0), vec2_t(146, 30), m_style->m_window_outline.override_alpha(150), m_style->m_window_outline.override_alpha(0));
				g_renderer->filled_rectangle(draw_position, vec2_t(2, 30), m_style->m_accent.override_alpha(150));
			}

			g_renderer->text(g_renderer->m_defualt_font, m_tabs[i].m_title, draw_position + vec2_t(15, (30 * 0.5) - text_size.y * 0.5), m_style->m_text_inactive);
		}

		// check if mouse is hovering the bounds of this m_tab.
		if (hovered) {
			// check if mouse1 is pressed and change the m_tab to this m_tab.
			if (g_input->is_key_pressed(VK_LBUTTON))
				m_tab = i;

			// set this so we cannot drag our menu.
			m_hovering_element = true;
		}

		// apply new position.
		draw_position.y += 30;
	}

	// clear vector for next window.
	m_tabs.clear();
}

int lucid_engine::ui::get_tab_index() {
	return m_tab;
}