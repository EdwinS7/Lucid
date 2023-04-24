#include "../lucid.h"

vec2_t draw_position{ };
void lucid_engine::ui::set_tabs_pos(vec2_t pos) {
	draw_position = pos;
}

void lucid_engine::ui::add_tab(const char* icon, const char* title) {
	tabs.emplace_back(tab_info(icon, title));
}

void lucid_engine::ui::handle_tabs() {
	vec2_t area = {45, 45};

	draw_position.x = (window_pos[window_id].x + (window_size[window_id].x / 2)) - (area.x * tabs.size()) / 2;
	draw_position.y = window_pos[window_id].y + window_size[window_id].y - area.y;

	for (int i = 0; i < tabs.size(); i++) {
		if (tab == i) {
			lucid_engine::renderer::get_instance().filled_rectangle(draw_position, area - vec2_t(0, 2), style->window_background);
			lucid_engine::renderer::get_instance().filled_rectangle(draw_position + vec2_t(0, area.x - 2), vec2_t(area.x, 2), style->accent);
		}

		vec2_t title_size = lucid_engine::renderer::get_instance().get_text_size(lucid_engine::renderer::get_instance().fonts.default_font, tabs[i].title);
		vec2_t icon_size = lucid_engine::renderer::get_instance().get_text_size(lucid_engine::renderer::get_instance().fonts.primordial_icons, tabs[i].icon);
		lucid_engine::renderer::get_instance().text(lucid_engine::renderer::get_instance().fonts.default_font, tabs[i].title, draw_position + vec2_t(-title_size.x / 2 + area.x / 2, area.x - 2 - title_size.y), style->text_inactive);
		lucid_engine::renderer::get_instance().text(lucid_engine::renderer::get_instance().fonts.primordial_icons, tabs[i].icon, draw_position + vec2_t(-icon_size.x / 2 + area.x / 2, area.x - 2 - title_size.y - icon_size.y), style->text_inactive);
		
		if (lucid_engine::input::get_instance().button_behavior(VK_LBUTTON, press, draw_position, area))
			tab = i;
		
		draw_position.x += 45;
	}

	tabs.clear();
}