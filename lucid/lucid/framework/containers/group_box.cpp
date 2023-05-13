#include "../../lucid.h"

containers::group_box::group_box(const char* title, vec2_t pos, vec2_t size) {
	lucid_engine::g_ui->m_group_id++;

	m_title = title;
	m_pos = pos;
	m_size = size;

	handle_input();
	handle_render();

	lucid_engine::g_ui->m_group = this;
}

void containers::group_box::handle_render() {
	auto style = lucid_engine::g_ui->get_style();

	// background
	lucid_engine::g_renderer->filled_rectangle(m_pos + vec2_t(0, style->m_group_header_size), m_size - vec2_t(0, style->m_group_header_size), style->m_group_background);

	// header
	lucid_engine::g_renderer->filled_rectangle(m_pos, vec2_t(m_size.x, style->m_group_header_size), style->m_group_header);
	lucid_engine::g_renderer->filled_rectangle(m_pos + vec2_t(0, style->m_group_header_size), vec2_t(m_size.x, 1), style->m_accent);

	// border
	lucid_engine::g_renderer->rectangle(m_pos, m_size, style->m_group_outline);

	// title
	lucid_engine::g_renderer->text(lucid_engine::g_renderer->m_defualt_font, m_title, m_pos + vec2_t(4, 6), style->m_text_active);

	// clipping
	lucid_engine::g_renderer->push_clip(m_pos + vec2_t(0, style->m_group_header_size + 1), m_size - vec2_t(0, style->m_group_header_size + 1));
}

std::map<int, double> m_scroll{}, m_scroll_lerp{};

void containers::group_box::handle_input() {
	auto group_id = lucid_engine::g_ui->m_group_id;
	auto style = lucid_engine::g_ui->get_style();

	// are we hovering our group box? (used for should scroll conditions).
	m_hovered = lucid_engine::g_input->mouse_hovering_rect(m_pos, m_size);
	m_elements_pos = m_pos + vec2_t(style->m_group_padding * 2, style->m_group_header_size + style->m_group_padding + m_scroll_lerp[group_id]);
}

void containers::group_box::destroy() {
	auto group_id = lucid_engine::g_ui->m_group_id;
	auto style = lucid_engine::g_ui->get_style();

	// get the range for how much we can scroll and how how we need to get their from current scroll amount.
	double scoll_to_bottom = (m_pos.y + style->m_group_header_size + style->m_group_padding) + m_size.y - style->m_group_header_size - style->m_group_padding * 2;
	double scrollable = scoll_to_bottom - ((m_elements_pos.y - m_scroll_lerp[group_id]) + m_scroll[group_id]);

	// can we scroll? conditions & absolute value.
	if (m_elements_pos.y - scoll_to_bottom  >= 0 && !lucid_engine::g_ui->is_dragging() && !lucid_engine::g_ui->is_resizing() && m_hovered)
		m_scroll[group_id] = std::clamp(m_scroll[group_id] + (double)lucid_engine::g_input->m_mouse_wheel_delta, scrollable + m_scroll[group_id], 0.0);

	// apply lerped value (animated).
	m_scroll_lerp[group_id] = lucid_engine::g_math->animate(animation_type::lerp, m_scroll_lerp[group_id], m_scroll[group_id], 8);

	// pop clip from earlier. prevents elements from clipping outside of group box bounds.
	lucid_engine::g_renderer->pop_clip();

	// dereference our group box.
	delete this;
}

bool containers::group_box::element_visible(vec2_t pos, vec2_t size) {
	auto style = lucid_engine::g_ui->get_style();

	// Is every point (top left, top right, bottom right, bottom left) within the other rectangle. (true/false).
	return lucid_engine::g_input->rect_clipping_rect(pos, size, m_pos + vec2_t(0, style->m_group_header_size), m_size - vec2_t(0, style->m_group_header_size));
}