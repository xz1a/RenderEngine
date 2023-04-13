#include "DX12Window.h"


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	//return D3DApp::GetApp()->MsgProc(hwnd, msg, wParam, lParam);
	return DefWindowProc(hwnd, msg, wParam, lParam);
};

DX12Window::DX12Window(const WindowParameter& p_parameter) : Window(p_parameter) {
	app_instance = nullptr;
	handle = NULL;
	
	description = {};
	description.style = CS_HREDRAW | CS_VREDRAW;
	description.lpfnWndProc = MainWndProc;
	description.cbClsExtra = 0;
	description.cbWndExtra = 0;
	description.hInstance = app_instance;
	description.hIcon = LoadIcon(0, IDI_APPLICATION);
	description.hCursor = LoadCursor(0, IDC_ARROW);
	description.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	description.lpszMenuName = 0;
	description.lpszClassName = L"MainWnd";

	if (!RegisterClass(&description)) {
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
	}

	screen = { 0, 0, (LONG)p_parameter.width, (LONG)p_parameter.height };
}

void DX12Window::Initialize() {
	AdjustWindowRect(&screen, WS_OVERLAPPEDWINDOW, false);
	handle = CreateWindow(L"MainWnd", (const wchar_t*)title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, (int)(screen.right - screen.left), (int)(screen.bottom - screen.top), 0, 0, app_instance, 0);
	
	if (!handle) {
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return;
	}
	ShowWindow(handle, SW_SHOW);
	UpdateWindow(handle);
}