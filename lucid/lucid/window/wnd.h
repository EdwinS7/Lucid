#pragma once

#include "../lucid.h"

namespace lucid_engine {
	class window {
	private:
		HWND	   m_hwnd{ };
		WNDCLASSEX m_window_class{ };

	public:
		bool create_window(const char* title, int x, int y, int w, int h);
		bool dispatch_messages();

		void set_window_title(const char* title);

		vec2_t get_window_size();
		vec2_t get_window_pos();

		HWND get_hwnd();
	};
	inline const auto g_window = std::make_unique< window >();
}