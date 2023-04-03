#include "wnd.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_SIZE:
		if (lucid_engine::graphics::get_instance().direct_3d_device != NULL && wParam != SIZE_MINIMIZED) {
			lucid_engine::graphics::get_instance().direct_3d_paramaters.BackBufferWidth = LOWORD(lParam);
			lucid_engine::graphics::get_instance().direct_3d_paramaters.BackBufferHeight = HIWORD(lParam);
			lucid_engine::graphics::get_instance().reset_device();
		}

		return 0;

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

HWND lucid_engine::window::get_hwnd() {
	return hwnd;
}