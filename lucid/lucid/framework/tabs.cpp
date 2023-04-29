#include "framework.h"

vec2_t draw_position{ };
void lucid_engine::ui::set_tabs_pos(vec2_t pos) {
	// used only for setting where you want the m_tabs to go.
	draw_position = pos;
}

void lucid_engine::ui::add_tab(const char* icon, const char* title) {
	// add a new m_tab to our vector.
	m_tabs.emplace_back(tab_info_t(icon, title));
}

void lucid_engine::ui::handle_tabs() {
	// size for each m_tab.
	vec2_t area = {45, 45};

	// get the draw position and fix the draw position to work with the amount of m_tabs. (center)
	draw_position.x = (m_window_pos[m_window_id].x + (m_window_size[m_window_id].x / 2)) - (area.x * m_tabs.size()) / 2;
	draw_position.y = m_window_pos[m_window_id].y + m_window_size[m_window_id].y - area.y;

	// each m_tab in our vector.
	for (int i = 0; i < m_tabs.size(); i++) {
		// calculate text sizes.
		vec2_t title_size = g_renderer.get_text_size(g_renderer.fonts.default_font, tabs[i].title);
		vec2_t icon_size = g_renderer.get_text_size(g_renderer.fonts.primordial_icons, tabs[i].icon);

		// check for hovering affect and used for click detection.
		bool hovered = g_input.mouse_hovering_rect(draw_position, area);

		// render our box and text(icon & bio).
		if (tab == i) {
			g_renderer.filled_rectangle(draw_position, area - vec2_t(0, 2), style->window_background);
			g_renderer.filled_rectangle(draw_position + vec2_t(0, area.x - 2), vec2_t(area.x, 2), style->accent);

			g_renderer.text(g_renderer.fonts.default_font, tabs[i].title, draw_position + vec2_t(-title_size.x / 2 + area.x / 2, area.x - 2 - title_size.y), style->text_active);
			g_renderer.text(g_renderer.fonts.primordial_icons, tabs[i].icon, draw_position + vec2_t(-icon_size.x / 2 + area.x / 2, area.x - 2 - title_size.y - icon_size.y), style->text_active);
		}
		else {
			if (hovered) {
				g_renderer.filled_rectangle(draw_position, area - vec2_t(0, 2), style->window_background.override_alpha(150));
				g_renderer.filled_rectangle(draw_position + vec2_t(0, area.x - 2), vec2_t(area.x, 2), style->accent.override_alpha(150));
			}

			g_renderer.text(g_renderer.fonts.default_font, tabs[i].title, draw_position + vec2_t(-title_size.x / 2 + area.x / 2, area.x - 2 - title_size.y), style->text_inactive);
			g_renderer.text(g_renderer.fonts.primordial_icons, tabs[i].icon, draw_position + vec2_t(-icon_size.x / 2 + area.x / 2, area.x - 2 - title_size.y - icon_size.y), style->text_inactive);
		}
		
		// check if mouse is hovering the bounds of this m_tab.
		if (hovered) {
			// check if mouse1 is pressed and change the tab to this tab.
			if (g_input.is_key_pressed(VK_LBUTTON))
				tab = i;

			// set this so we cannot drag our menu.
			m_hovering_element = true;
		}
		
		// apply new position.
		draw_position.x += 45;
	}

	// clear vector for next window.
	m_tabs.clear();
}