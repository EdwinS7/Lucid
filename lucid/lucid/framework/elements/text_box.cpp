#include "../framework.h"

struct key_code_info_t {
	int vk_key;

	char regular;
	char upper;
};

inline static key_code_info_t special_characters[22] = {
			{48,  '0',  ')'},
			{49,  '1',  '!'},
			{50,  '2',  '@'},
			{51,  '3',  '#'},
			{52,  '4',  '$'},
			{53,  '5',  '%'},
			{54,  '6',  '^'},
			{55,  '7',  '&'},
			{56,  '8',  '*'},
			{57,  '9',  '('},
			{32,  ' ',  ' '},
			{192, '`',  '~'},
			{189, '-',  '_'},
			{187, '=',  '+'},
			{219, '[',  '{'},
			{220, '\\', '|'},
			{221, ']',  '}'},
			{186, ';',  ':'},
			{222, '\'', '"'},
			{188, ',',  '<'},
			{190, '.',  '>'},
			{191, '/',  '?'}
};

bool lucid_engine::ui::text_box(const char* title, std::string& data) {
	m_text_box_id++;

	vec2_t text_size = g_renderer->get_text_size(g_renderer->m_defualt_font, title);
	vec2_t text_box_size = vec2_t(m_group->m_size.x - m_style->m_group_padding * 4, text_size.y + 4);

	if (!m_group->element_visible(m_group->m_elements_pos, text_box_size + vec2_t(0, text_size.y))) {
		m_group->m_elements_pos += vec2_t(0, text_box_size.y + m_style->m_group_spacing);
		return false;
	}

	if (g_input->mouse_hovering_rect(m_group->m_elements_pos + vec2_t(0, text_size.y), text_box_size)) {
		if (g_input->is_key_pressed(VK_LBUTTON))
			m_text_box_info[m_text_box_id].in_use = !m_text_box_info[m_text_box_id].in_use;

		m_hovering_element = true;
	}

	if (m_text_box_info[m_text_box_id].in_use) {
		for (int i = 32; i <= 222; i++) {
			if ((i > 32 && i < 48) || (i > 57 && i < 65) || (i > 90 && i < 186))
				continue;

			if (i > 57 && i <= 90) {
				if (g_input->is_key_pressed(i))
					data += (g_input->is_key_held(VK_SHIFT) || GetKeyState(VK_CAPITAL)) ? static_cast<char>(i) : static_cast<char>(i + 32);
			}
			else {
				if (g_input->is_key_pressed(i)) {
					for (int j = 0; j < sizeof(special_characters); j++) {
						if (special_characters[j].vk_key == i)
							data += (g_input->is_key_held(VK_SHIFT) || GetKeyState(VK_CAPITAL)) ? special_characters[j].upper : special_characters[j].regular;
					}
				}
			}
		}

		if (!data.empty() && g_input->is_key_pressed(VK_BACK)) {
			m_text_box_info[m_text_box_id].press_time = g_io->m_elapsed_time;
			data.pop_back();
		}

		if (!data.empty() && g_input->is_key_held(VK_BACK) && g_io->m_elapsed_time - m_text_box_info[m_text_box_id].press_time > 0.5f) {
			m_text_box_info[m_text_box_id].press_time = g_io->m_elapsed_time - 0.46f;
			data.pop_back();
		}

		if (g_input->is_key_pressed(VK_ESCAPE) || g_input->is_key_pressed(VK_RETURN))
			m_text_box_info[m_text_box_id].in_use = !m_text_box_info[m_text_box_id].in_use;
	}

	g_renderer->filled_rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y), text_box_size, m_style->m_element_inactive);
	g_renderer->rectangle(m_group->m_elements_pos + vec2_t(1, text_size.y + 1), text_box_size - vec2_t(2, 2), m_style->m_element_in_outline);
	g_renderer->rectangle(m_group->m_elements_pos + vec2_t(0, text_size.y), text_box_size, m_style->m_element_outline);

	//g_renderer->push_clip(m_group->m_elements_pos, text_box_size + vec2_t(0, text_size.y));
	g_renderer->text(g_renderer->m_defualt_font, title, m_group->m_elements_pos - vec2_t(0, 2), m_text_box_info[m_text_box_id].in_use ? m_style->m_accent : m_style->m_text_active);
	g_renderer->text(g_renderer->m_defualt_font, data.c_str(), m_group->m_elements_pos + vec2_t(6, text_size.y + 3), m_style->m_accent);
	//g_renderer->pop_clip();

	m_group->m_elements_pos += vec2_t(0, text_size.y + text_box_size.y + m_style->m_group_spacing);
}