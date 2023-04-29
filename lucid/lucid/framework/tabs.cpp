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
		vec2_t title_size = g_renderer.get()->get_text_size(g_renderer.get()->m_fonts.at(fonts::font_default), m_tabs[i].m_title);
		vec2_t icon_size = g_renderer.get()->get_text_size(g_renderer.get()->m_fonts.at(fonts::font_primordial_icons), m_tabs[i].m_icon);

		// check for hovering affect and used for click detection.
		bool hovered = g_input.get()->mouse_hovering_rect(draw_position, area);

		// render our box and text(icon & bio).
		if (m_tab == i) {
			g_renderer.get()->filled_rectangle(draw_position, area - vec2_t(0, 2), m_style->m_window_background);
			g_renderer.get()->filled_rectangle(draw_position + vec2_t(0, area.x - 2), vec2_t(area.x, 2), m_style->m_accent);

			g_renderer.get()->text(g_renderer.get()->m_fonts.at(fonts::font_default), m_tabs[i].m_title, draw_position + vec2_t(-title_size.x / 2 + area.x / 2, area.x - 2 - title_size.y), m_style->m_text_active);
			g_renderer.get()->text(g_renderer.get()->m_fonts.at(fonts::font_primordial_icons), m_tabs[i].m_icon, draw_position + vec2_t(-icon_size.x / 2 + area.x / 2, area.x - 2 - title_size.y - icon_size.y), m_style->m_text_active);
		}
		else {
			if (hovered) {
				g_renderer.get()->filled_rectangle(draw_position, area - vec2_t(0, 2), m_style->m_window_background.override_alpha(150));
				g_renderer.get()->filled_rectangle(draw_position + vec2_t(0, area.x - 2), vec2_t(area.x, 2), m_style->m_accent.override_alpha(150));
			}

			g_renderer.get()->text(g_renderer.get()->m_fonts.at(fonts::font_default), m_tabs[i].m_title, draw_position + vec2_t(-title_size.x / 2 + area.x / 2, area.x - 2 - title_size.y), m_style->m_text_inactive);
			g_renderer.get()->text(g_renderer.get()->m_fonts.at(fonts::font_primordial_icons), m_tabs[i].m_icon, draw_position + vec2_t(-icon_size.x / 2 + area.x / 2, area.x - 2 - title_size.y - icon_size.y), m_style->m_text_inactive);
		}
		
		// check if mouse is hovering the bounds of this m_tab.
		if (hovered) {
			// check if mouse1 is pressed and change the m_tab to this m_tab.
			if (g_input.get()->is_key_pressed(VK_LBUTTON))
				m_tab = i;

			// set this so we cannot drag our menu.
			m_hovering_element = true;
		}
		
		// apply new position.
		draw_position.x += 45;
	}

	// clear vector for next window.
	m_tabs.clear();
