#include "../../lucid.h"

group_box::group_box(const char* title, vec2_t pos, vec2_t size) {
	m_title = title;
	m_pos = pos;
	m_size = size;

	handle_input();
	handle_render();
}

void group_box::handle_render() {
	auto renderer = lucid_engine::g_renderer.get();
	auto style = lucid_engine::g_ui.get()->get_style();

	//background
	renderer->filled_rounded_rectangle(m_pos + vec2_t(0, style->m_group_header_size), m_size - vec2_t(0, style->m_group_header_size), style->m_group_background, style->m_group_rounding, corner_bottom);

	//header
	renderer->filled_rounded_rectangle(m_pos, vec2_t(m_size.x, style->m_group_header_size), style->m_group_header, style->m_group_rounding, corner_top);
	renderer->filled_rectangle(m_pos + vec2_t(0, style->m_group_header_size), vec2_t(m_size.x, 1), style->m_accent);

	//border
	renderer->rounded_rectangle(m_pos, m_size, style->m_group_outline, style->m_group_rounding);

	//title
	renderer->text(renderer->m_defualt_font, m_title, m_pos + vec2_t(4, 6), style->m_text_active);

	//clipping
	renderer->push_clip(m_pos + vec2_t(0, style->m_group_header_size + 1), m_size - vec2_t(0, style->m_group_header_size + 1));
}

std::map<int, double> m_scroll{}, m_scroll_lerp{};

void group_box::handle_input() {
	int& m_group_id = lucid_engine::g_ui.get()->m_group_id;
	auto style = lucid_engine::g_ui.get()->get_style();
	auto input = lucid_engine::g_input.get();

	//new group, assign new id.
	m_group_id++;

	//are we hovering our group box? (used for should scroll conditions).
	m_hovered = input->mouse_hovering_rect(m_pos, m_size);
	m_elements_pos = m_pos + vec2_t(style->m_group_padding * 2, style->m_group_header_size + style->m_group_padding + m_scroll_lerp[m_group_id]);
}

void group_box::destroy() {
	auto m_group_id = lucid_engine::g_ui.get()->m_group_id;
	auto style = lucid_engine::g_ui.get()->get_style();
	auto renderer = lucid_engine::g_renderer.get();
	auto input = lucid_engine::g_input.get();
	auto math = lucid_engine::g_math.get();
	auto ui = lucid_engine::g_ui.get();
	auto io = lucid_engine::g_io.get();

	//get the range for how much we can scroll and how how we need to get their from current scroll amount.
	double scoll_to_bottom = (m_pos.y + style->m_group_header_size + style->m_group_padding) + m_size.y - style->m_group_header_size - style->m_group_padding * 2;
	double scrollable = scoll_to_bottom - ((m_elements_pos.y - m_scroll_lerp[m_group_id]) + m_scroll[m_group_id]);

	//can we scroll? conditions & absolute value.
	if (m_elements_pos.y - scoll_to_bottom  >= 0 && !ui->is_dragging() && !ui->is_resizing() && m_hovered)
		m_scroll[m_group_id] = std::clamp(m_scroll[m_group_id] + (double)input->m_mouse_wheel_delta, scrollable + m_scroll[m_group_id], 0.0);

	//apply lerped value (animated).
	m_scroll_lerp[m_group_id] = math->animate(animation_type::lerp, m_scroll_lerp[m_group_id], m_scroll[m_group_id], 8);

	//pop clip from earlier. prevents elements from clipping outside of group box bounds.
	renderer->pop_clip();

	//dereference our group box.
	delete this;
}

bool group_box::element_visible(vec2_t pos, vec2_t size) {
	auto style = lucid_engine::g_ui.get()->get_style();
	auto input = lucid_engine::g_input.get();

	//Is every point (top left, top right, bottom right, bottom left) within the other rectangle. (true/false).
	return input->rect_clipping_rect(pos, size, m_pos + vec2_t(0, style->m_group_header_size), m_size - vec2_t(0, style->m_group_header_size));
}