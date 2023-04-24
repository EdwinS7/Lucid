#include "lucid.h"

void lucid_engine::io::create() {
	lucid_engine::input::get_instance().cursor_style = LoadCursor(NULL, IDC_ARROW);
}

void lucid_engine::io::update() {
	lucid_engine::input::get_instance().cursor_style = LoadCursor(NULL, IDC_ARROW);

	auto high_resolution_clock = std::chrono::high_resolution_clock::now();
	real_time = static_cast<float>(std::chrono::duration_cast<std::chrono::duration<double>>(high_resolution_clock.time_since_epoch()).count());

	static auto old_frame_time = std::chrono::high_resolution_clock::now();
	delta_time = static_cast<float>(std::chrono::duration_cast<std::chrono::duration<double>>(high_resolution_clock - old_frame_time).count());
	old_frame_time = high_resolution_clock;

	frame_rate = static_cast<int>(1.f / delta_time);
	
	static int frame_rate_avg = 60;
	static float frame_rate_update = real_time + 1.f;
	if (real_time > frame_rate_update) {
		frame_rate_average = frame_rate_avg;
		frame_rate_update = real_time + 1;
		frame_rate_avg = 0.f;
	}
	else
		frame_rate_avg++;
}

void lucid_engine::io::reset() {
	lucid_engine::input::get_instance().key_info = {};
	lucid_engine::ui::get_instance().reset();
}

void lucid_engine::ui::reset() {
	window_id = -1;
	group_id = -1;
}

void lucid_engine::io::demo_window(bool open) {
	if (!open)
		return;

	lucid_engine::ui::get_instance().create_window("edwngui demo window", { 50, 50 }, { 600, 550 }, {600, 550 });
	{
		vec2_t window_pos = lucid_engine::ui::get_instance().get_window_pos();
		vec2_t window_size = lucid_engine::ui::get_instance().get_window_size();
		vec2_t group_size = { (window_size.x - 45) / 2, 100 };

		lucid_engine::ui::get_instance().create_group("i hit kids 4000", { 15, 40 }, group_size, group_size);
		{
			lucid_engine::ui::get_instance().add_tab("A", "aimbot");
			lucid_engine::ui::get_instance().add_tab("B", "antiaim");
			lucid_engine::ui::get_instance().add_tab("C", "visuals");
			lucid_engine::ui::get_instance().add_tab("D", "misc");
			lucid_engine::ui::get_instance().add_tab("E", "skins");
			lucid_engine::ui::get_instance().handle_tabs();
		}
		lucid_engine::ui::get_instance().end_group();
	}
	lucid_engine::ui::get_instance().end_window();
}