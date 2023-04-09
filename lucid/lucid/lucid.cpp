#include "lucid.h"

void lucid_engine::io::create() {
	
}

void lucid_engine::io::update() {
	auto high_resolution_clock = std::chrono::high_resolution_clock::now();
	real_time = static_cast<float>(std::chrono::duration_cast<std::chrono::duration<double>>(high_resolution_clock.time_since_epoch()).count());

	static auto old_frame_time = std::chrono::high_resolution_clock::now();
	delta_time = static_cast<float>(std::chrono::duration_cast<std::chrono::duration<double>>(high_resolution_clock - old_frame_time).count());
	old_frame_time = high_resolution_clock;

	frame_rate = static_cast<int>(1.f / delta_time);
	
	static auto frame_rate_avg = 60.f;
	static auto frame_rate_update = real_time + 1.f;
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
}