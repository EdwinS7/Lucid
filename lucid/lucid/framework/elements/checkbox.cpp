#include "../framework.h"

void lucid_engine::ui::check_box(const char* title, bool* state) {
	vec2_t check_box_size = { 9, 9 };
	vec2_t text_size = g_renderer.get_text_size(g_renderer.fonts.default_font, title);

	// check if mouse is hovering the bounds of the check box.
	if (g_input.mouse_hovering_rect(elements_pos, text_size + vec2_t(check_box_size.x, 0))) {
		// check if mouse1 is pressed and change the state of the checkbox.
		if (g_input.is_key_pressed(VK_LBUTTON))
			*state = !(*state);

		// set this so we cannot drag our menu.
		hovering_element = true;
	}

	// check if check box is even visible and render if visible.
	if (g_input.rect_clipping_rect(elements_pos, check_box_size, window_pos[window_id] + group_pos[group_id] + vec2_t(0, 26), group_size[group_id] - vec2_t(0, 26))) {
		g_renderer.filled_rounded_rectangle(elements_pos, check_box_size, *state ? style->element_active : style->element_inactive, style->element_rounding);
		g_renderer.rounded_rectangle(elements_pos, check_box_size, style->element_outline, style->element_rounding);

		g_renderer.text(g_renderer.fonts.default_font, title, elements_pos + vec2_t(check_box_size.x + 4, -2), *state ? style->text_active : style->text_inactive);
	}

	// apply new position.
	elements_pos += vec2_t(0, text_size.y + style->group_spacing);
}