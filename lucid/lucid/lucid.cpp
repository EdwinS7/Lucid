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
}

void lucid_engine::io::reset() {
	lucid_engine::input::get_instance().key_info = {};
}