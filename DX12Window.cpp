#include "DX12Window.h"
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return window_application->MsgProc(hwnd, msg, wParam, lParam);
}
DX12Window::DX12Window(const WindowDescription& p_parameter) : Window(p_parameter) {
	app_instance = nullptr;
	window_handle = NULL;
	
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
	window_application = this;
}

DX12Window::~DX12Window() {
	window_application = 0;
}

void DX12Window::Initialize() {
	AdjustWindowRect(&screen, WS_OVERLAPPEDWINDOW, false);
	window_handle = CreateWindow(L"MainWnd", std::wstring(title.begin(), title.end()).c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, (int)(screen.right - screen.left), (int)(screen.bottom - screen.top), 0, 0, app_instance, 0);
	
	if (!window_handle) {
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return;
	}
	ShowWindow(window_handle, SW_SHOW);
	UpdateWindow(window_handle);
}

int DX12Window::Run() {
	MSG msg = { 0 };

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else {

			/*if (!mAppPaused)
			{
				CalculateFrameStats();
				Update(mTimer);
				Draw(mTimer);
			}
			else
			{
				Sleep(100);
			}*/
}
	}
	return (int)msg.wParam;
}

LRESULT DX12Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		return 0;
	case WM_MOUSEMOVE:
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}