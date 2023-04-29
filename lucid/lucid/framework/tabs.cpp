#include "framework.h"

vec2_t draw_position{ };
void lucid_engine::ui::set_tabs_pos(vec2_t pos) {
	// used only for setting where you want the tabs to go.
	draw_position = pos;
}

void lucid_engine::ui::add_tab(const char* icon, const char* title) {
	// add a new tab to our vector.
	tabs.emplace_back(tab_info(icon, title));
}

void lucid_engine::ui::handle_tabs() {
	// size for each tab.
	vec2_t area = {45, 45};

	// get the draw position and fix the draw position to work with the amount of tabs. (center)
	draw_position.x = (window_pos[window_id].x + (window_size[window_id].x / 2)) - (area.x * tabs.size()) / 2;
	draw_position.y = window_pos[window_id].y + window_size[window_id].y - area.y;

	// each tab in our vector.
	for (int i = 0; i < tabs.size(); i++) {
		// calculate text sizes.
		vec2_t title_size = g_renderer.get( )->get_text_size(g_renderer.get( )->fonts.at( default_font ), tabs[i].title);
		vec2_t icon_size = g_renderer.get( )->get_text_size(g_renderer.get( )->fonts.at( primordial_icons), tabs[i].icon);

		// check for hovering affect and used for click detection.
		bool hovered = g_input.get( )->mouse_hovering_rect(draw_position, area);

		// render our box and text(icon & bio).
		if (tab == i) {
			g_renderer.get( )->filled_rectangle(draw_position, area - vec2_t(0, 2), style->window_background);
			g_renderer.get( )->filled_rectangle(draw_position + vec2_t(0, area.x - 2), vec2_t(area.x, 2), style->accent);

			g_renderer.get( )->text(g_renderer.get( )->fonts.at( default_font ), tabs[i].title, draw_position + vec2_t(-title_size.x / 2 + area.x / 2, area.x - 2 - title_size.y), style->text_active);
			g_renderer.get( )->text(g_renderer.get( )->fonts.at( primordial_icons), tabs[i].icon, draw_position + vec2_t(-icon_size.x / 2 + area.x / 2, area.x - 2 - title_size.y - icon_size.y), style->text_active);
		}
		else {
			if (hovered) {
				g_renderer.get( )->filled_rectangle(draw_position, area - vec2_t(0, 2), style->window_background.override_alpha(150));
				g_renderer.get( )->filled_rectangle(draw_position + vec2_t(0, area.x - 2), vec2_t(area.x, 2), style->accent.override_alpha(150));
			}

			g_renderer.get( )->text(g_renderer.get( )->fonts.at( default_font ), tabs[i].title, draw_position + vec2_t(-title_size.x / 2 + area.x / 2, area.x - 2 - title_size.y), style->text_inactive);
			g_renderer.get( )->text(g_renderer.get( )->fonts.at( primordial_icons), tabs[i].icon, draw_position + vec2_t(-icon_size.x / 2 + area.x / 2, area.x - 2 - title_size.y - icon_size.y), style->text_inactive);
		}
		
		// check if mouse is hovering the bounds of this tab.
		if (hovered) {
			// check if mouse1 is pressed and change the tab to this tab.
			if (g_input.get( )->is_key_pressed(VK_LBUTTON))
				tab = i;

			// set this so we cannot drag our menu.
			hovering_element = true;
		}
		
		// apply new position.
		draw_position.x += 45;
	}

	// clear vector for next window.
	tabs.clear();
}