#include "lucid.h"

void lucid_engine::io::create() {
	g_input->m_cursor_style = LoadCursor(NULL, IDC_ARROW);
}

void lucid_engine::io::update() {
	reset();

	g_input->m_cursor_style = LoadCursor(NULL, IDC_ARROW);

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
	g_input->m_mouse_wheel_delta = 0.f;
	g_input->m_key_info = {};
	g_ui->reset();
}

void lucid_engine::ui::reset() {
	m_window_id = -1;
	m_group_id = -1;
	m_slider_id = -1;
	m_combo_id = -1;
	m_multi_combo_id = -1;
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

	auto window = new containers::window("LUCID", { 50, 50 }, { 680, 430 }, { 680, 430 });
	{
		g_ui->add_tab("GENERAL");
		g_ui->add_tab("OTHER");
		g_ui->add_tab("COLORS");
		g_ui->add_tab("MISC");
		g_ui->handle_tabs();

		vec2_t group_size = { (window->m_size.x - 195) * 0.5, window->m_size.y - 75 };

		auto general = new containers::group_box("general", window->m_pos + vec2_t(165, 60), group_size);
		{
			g_ui->label("label example", g_ui->get_style()->m_accent);

			static bool checkbox_example{ false };
			g_ui->check_box("check box example", &checkbox_example);

			static int slider_int_example{ 0 };
			g_ui->slider_int("slider int example", 0, 100, &slider_int_example);

			static float slider_float_example{ 0 };
			g_ui->slider_float("slider float example", 0.f, 100.f, &slider_float_example);

			static int combo_example{ 0 };
			g_ui->combo_box("combo example", &combo_example, { "option 1", "option 2", "option 3" });

			static std::vector<bool> multi_combo_example{ };
			g_ui->multi_combo_box("multi combo example", &multi_combo_example, { "option 1", "option 2", "option 3" });

			if (g_ui->button("button example")) {
				printf("button pressed\n");
			}
		}
		general->destroy();

		auto other = new containers::group_box("other", window->m_pos + vec2_t(180 + group_size.x, 60), group_size);
		{
			// empty.
		}
		other->destroy();
	}
	window->destroy();
}