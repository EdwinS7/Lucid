#include "wnd.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	float mouse_wheel_delta = 0.f;

	switch (msg) {
	case WM_SIZE:
		if (lucid_engine::graphics::get_instance().direct_3d_device != NULL && wParam != SIZE_MINIMIZED) {
			lucid_engine::graphics::get_instance().direct_3d_paramaters.BackBufferWidth = LOWORD(lParam);
			lucid_engine::graphics::get_instance().direct_3d_paramaters.BackBufferHeight = HIWORD(lParam);
			lucid_engine::graphics::get_instance().reset_device();
		}

		return 0;
	case WM_KEYDOWN:
	case WM_KEYUP:
		lucid_engine::input::get_instance().key_info[static_cast<int>(wParam)].style = (msg == WM_KEYUP ? key_style::press : key_style::hold);
		lucid_engine::input::get_instance().key_info[static_cast<int>(wParam)].on = true;

		break;
	case WM_MOUSEMOVE:
		const POINTS p = MAKEPOINTS(lParam);
		lucid_engine::input::get_instance().mouse_pos = vec2_t(p.x, p.y);

		break;
	case WM_MOUSEWHEEL:
		mouse_wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam) > 0.f ? 8.f : -8.f;
		lucid_engine::input::get_instance().mouse_wheel_delta = mouse_wheel_delta;

		break;
	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
} 

bool lucid_engine::window::create_window(const char* title, int x, int y, int w, int h) {
	window_class = {
		sizeof(window_class),
		CS_CLASSDC,
		WndProc,
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		title,
		NULL
	};

	RegisterClassEx(&window_class);

	hwnd = CreateWindowA(window_class.lpszClassName, title, WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, window_class.hInstance, NULL);

	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	return true;
}

bool lucid_engine::window::dispatch_messages() {
	MSG msg;
	std::memset(&msg, 0, sizeof(MSG));
	if (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			return false;
	}

	return true;
}

void lucid_engine::window::set_window_title(const char* title) {
	SetWindowTextA(hwnd, title);
}

HWND lucid_engine::window::get_hwnd() {
	return hwnd;
}