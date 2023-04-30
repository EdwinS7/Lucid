#include "lucid.h"

void lucid_engine::io::create() {
	g_input.get()->m_cursor_style = LoadCursor(NULL, IDC_ARROW);
}

void lucid_engine::io::update() {
	g_input.get()->m_cursor_style = LoadCursor(NULL, IDC_ARROW);

	std::chrono::steady_clock::time_point high_resolution_clock = std::chrono::high_resolution_clock::now();
	m_real_time = static_cast<float>(std::chrono::duration_cast<std::chrono::duration<double>>(high_resolution_clock.time_since_epoch()).count());

	static std::chrono::steady_clock::time_point old_frame_time = std::chrono::high_resolution_clock::now();
	m_delta_time = static_cast<float>(std::chrono::duration_cast<std::chrono::duration<double>>(high_resolution_clock - old_frame_time).count());
	old_frame_time = high_resolution_clock;

	m_frame_rate = static_cast<int>(1.f / m_delta_time);
	
	static int frame_rate_avg = 60;
	static float frame_rate_update = m_real_time + 1.f;
	if (m_real_time > frame_rate_update) {
		m_frame_rate_average = frame_rate_avg;
		frame_rate_update = m_real_time + 1;
		frame_rate_avg = 0.f;
	}
	else
		frame_rate_avg++;
}

void lucid_engine::io::reset() {
	g_input.get()->m_mouse_wheel_delta = 0.f;
	g_input.get()->m_key_info = {};
	g_ui.get()->reset();
}

void lucid_engine::ui::reset() {
	m_window_id = -1;
	m_group_id = -1;
	m_slider_id = -1;
	m_combo_id = -1;
	m_multi_combo_id = -1;
}

void lucid_engine::ui::spacing(vec2_t spacing) {
	return m_elements_pos += spacing;
}

style_t* lucid_engine::ui::get_style() {
	return m_style;
}

bool lucid_engine::ui::is_hovering_element() {
	return m_hovering_element;
}

bool lucid_engine::ui::is_hovering_popup() {
	return m_hovering_popup;
}

void lucid_engine::io::demo_window(bool open) {
	if (!open)
		return;

	g_ui.get()->create_window("edwngui demo window", { 50, 50 }, { 600, 550 }, {600, 550 });
	{
		vec2_t window_pos = g_ui.get()->get_window_pos();
		vec2_t window_size = g_ui.get()->get_window_size();
		vec2_t group_size = { (window_size.x - 45) * 0.5, window_size.y - 100 };

		g_ui.get()->add_tab("A", "aimbot");
		g_ui.get()->add_tab("B", "antiaim");
		g_ui.get()->add_tab("C", "visuals");
		g_ui.get()->add_tab("D", "misc");
		g_ui.get()->add_tab("E", "skins");
		g_ui.get()->handle_tabs();

		g_ui.get()->create_group("group 0", { 15, 40 }, group_size, group_size);
		{
			g_ui.get()->label("label example", g_ui.get()->get_style()->m_accent);

			static bool checkbox_example{ false };
			g_ui.get()->check_box("check box example", &checkbox_example);

			if (g_ui.get()->button("button example")) {
				printf("button pressed\n");
			}

			static int slider_int_example{ 0 };
			g_ui.get()->slider_int("slider int example", 0, 100, &slider_int_example);

			static float slider_float_example{ 0 };
			g_ui.get()->slider_float("slider float example", 0.f, 100.f, &slider_float_example);

			static int combo_example{ 0 };
			g_ui.get()->combo_box("combo example", &combo_example, { "option 1", "option 2", "option 3" });

			static std::vector<bool> multi_combo_example{ };
			g_ui.get()->multi_combo_box("multi combo example", &multi_combo_example, { "option 1", "option 2", "option 3" });
		}
		g_ui.get()->end_group();

		g_ui.get()->create_group("group 1", { group_size.x + 30, 40 }, group_size, group_size);
		{

		}
		g_ui.get()->end_group();
	}
	g_ui.get()->end_window();
}