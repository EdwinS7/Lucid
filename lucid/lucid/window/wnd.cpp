#include "wnd.h"

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	float mouse_wheel_delta = 0.0f;
	key_style state = key_style::key_none;
	key_style keyboard_state = key_style::key_none;
	int keyboard_key = 0;
	int key = 0;

	switch (uMsg) {
	case WM_SIZE:
		if (lucid_engine::g_graphics->m_direct_3d_device && wParam != SIZE_MINIMIZED) {
			lucid_engine::g_graphics->m_direct_3d_paramaters.BackBufferWidth = LOWORD(lParam);
			lucid_engine::g_graphics->m_direct_3d_paramaters.BackBufferHeight = HIWORD(lParam);
			lucid_engine::g_graphics->reset_device();
		}
		return false;
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		state = uMsg == WM_MBUTTONUP ? key_style::key_up : key_style::key_down;
		key = VK_MBUTTON;
		break;
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		state = uMsg == WM_RBUTTONUP ? key_style::key_up : key_style::key_down;
		key = VK_RBUTTON;
		break;
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		state = uMsg == WM_LBUTTONUP ? key_style::key_up : key_style::key_down;
		key = VK_LBUTTON;
		break;
	case WM_XBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		state = uMsg == WM_XBUTTONUP ? key_style::key_up : key_style::key_down;
		key = (HIWORD(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		keyboard_key = wParam;
		keyboard_state = key_style::key_down;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		keyboard_key = wParam;
		keyboard_state = key_style::key_up;
		break;
	case WM_MOUSEMOVE:
		lucid_engine::g_input->m_mouse_pos = vec2_t(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_MOUSEWHEEL:
		mouse_wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam) > 0.0f ? 8.0f : -8.0f;
		lucid_engine::g_input->m_mouse_wheel_delta = mouse_wheel_delta;
		break;

	case WM_SETCURSOR:
		SetCursor(lucid_engine::g_input->m_cursor_style);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	if (state == key_style::key_up && lucid_engine::g_input->m_key_info[key] == key_style::key_down)
		lucid_engine::g_input->m_key_info[key] = key_style::key_press;
	else
		lucid_engine::g_input->m_key_info[key] = state;

	if (keyboard_state == key_style::key_up && lucid_engine::g_input->m_key_info[key] == key_style::key_down)
		lucid_engine::g_input->m_key_info[keyboard_key] = key_style::key_press;
	else
		lucid_engine::g_input->m_key_info[keyboard_key] = keyboard_state;

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
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
	return vec2_t();
}

vec2_t lucid_engine::window::get_window_pos() {
	RECT rect{ };

	if (GetWindowRect(m_hwnd, &rect))
		return vec2_t(rect.left, rect.top);

	throw std::runtime_error{ "GetWindowRect error" };
	return vec2_t();
}