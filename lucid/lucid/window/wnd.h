#pragma once

#include "../lucid.h"

namespace lucid_engine {
	class window : public singleton<window> {
	private:
		HWND hwnd;
		WNDCLASSEX window_class;

	public:
		bool create_window(const char* title, int x, int y, int w, int h);
		bool dispatch_messages();

		void set_window_title(const char* title);
		HWND get_hwnd();

		vec2_t get_window_size();
		vec2_t get_window_pos();
	};
}