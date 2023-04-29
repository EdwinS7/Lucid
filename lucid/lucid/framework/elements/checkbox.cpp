#include "../framework.h"

void lucid_engine::ui::check_box(const char* title, bool* state) {
	vec2_t check_box_size = { 9, 9 };
	vec2_t text_size = lucid_engine::renderer::get_instance().get_text_size(lucid_engine::renderer::get_instance().fonts.default_font, title);

	// check if mouse is hovering the bounds of the check box.
	if (lucid_engine::input::get_instance().mouse_hovering_rect(elements_pos, text_size + vec2_t(check_box_size.x, 0))) {
		// check if mouse1 is pressed and change the state of the checkbox.
		if (lucid_engine::input::get_instance().is_key_pressed(VK_LBUTTON))
			*state = !(*state);

		// set this so we cannot drag our menu.
		hovering_element = true;
	}

	// check if check box is even visible and render if visible.
	if (lucid_engine::input::get_instance().rect_clipping_rect(elements_pos, check_box_size, window_pos[window_id] + group_pos[group_id] + vec2_t(0, 26), group_size[group_id] - vec2_t(0, 26))) {
		lucid_engine::renderer::get_instance().filled_rounded_rectangle(elements_pos, check_box_size, *state ? style->element_active : style->element_inactive, style->element_rounding);
		lucid_engine::renderer::get_instance().rounded_rectangle(elements_pos, check_box_size, style->element_outline, style->element_rounding);

		lucid_engine::renderer::get_instance().text(lucid_engine::renderer::get_instance().fonts.default_font, title, elements_pos + vec2_t(check_box_size.x + 4, -2), *state ? style->text_active : style->text_inactive);
	}

	// apply new position.
	elements_pos += vec2_t(0, text_size.y + style->group_spacing);
}