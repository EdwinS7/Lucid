#include "wnd.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	float mouse_wheel_delta = 0.f;
	static std::map<int, key_data> key_state;

	switch (msg) {
	case WM_SIZE:
		if (lucid_engine::g_graphics.get()->m_direct_3d_device != NULL && wParam != SIZE_MINIMIZED) {
			lucid_engine::g_graphics.get()->m_direct_3d_paramaters.BackBufferWidth = LOWORD(lParam);
			lucid_engine::g_graphics.get()->m_direct_3d_paramaters.BackBufferHeight = HIWORD(lParam);
			lucid_engine::g_graphics.get()->reset_device();
		}

		return 0;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		key_state[1].m_on = (msg == WM_LBUTTONDOWN) ? true : false;

		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		key_state[2].m_on = (msg == WM_LBUTTONDOWN) ? true : false;

		break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		key_state[4].m_on = (msg == WM_LBUTTONDOWN) ? true : false;

		break;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		key_state[5].m_on = (msg == WM_LBUTTONDOWN) ? true : false;

		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		if (wParam <= 256) {
			bool held = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);

			key_state[static_cast<int>(wParam)].m_style = (held ? key_style::key_hold : key_style::key_press);
			key_state[static_cast<int>(wParam)].m_on = true;
		}

		break;
	case WM_MOUSEMOVE:
		const POINTS p = MAKEPOINTS(lParam);
		lucid_engine::g_input.get()->m_mouse_pos = vec2_t(p.x, p.y);

		break;
	case WM_MOUSEWHEEL:
		mouse_wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam) > 0.f ? 8.f : -8.f;
		lucid_engine::g_input.get()->m_mouse_wheel_delta = mouse_wheel_delta;

		break;
	case WM_SETCURSOR:
		SetCursor(lucid_engine::g_input.get()->m_cursor_style);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;
	}

	lucid_engine::g_input.get()->m_key_info = key_state;

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
} 

bool lucid_engine::window::create_window(const char* title, int x, int y, int w, int h) {
	m_window_class = {
		sizeof(m_window_class),
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

	RegisterClassEx(&m_window_class);

	m_hwnd = CreateWindowA(m_window_class.lpszClassName, title,
		WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, m_window_class.hInstance, NULL);

	::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(m_hwnd);

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
	SetWindowTextA(m_hwnd, title);
}

HWND lucid_engine::window::get_hwnd() {
	return m_hwnd;
}

vec2_t lucid_engine::window::get_window_size() {
	RECT rect{ };

	if (GetClientRect(m_hwnd, &rect))
		return vec2_t(rect.right - rect.left, rect.bottom - rect.top);

	throw std::runtime_error{ "GetClientRect error" };
	return vec2_t(0, 0);
}

vec2_t lucid_engine::window::get_window_pos() {
	RECT rect{ };

	if (GetWindowRect(m_hwnd, &rect))
		return vec2_t(rect.left, rect.top);

	throw std::runtime_error{ "GetWindowRect error" };
	return vec2_t(0, 0);
}