#ifndef _DX12_WINDOW_H_
#define _DX12_WINDOW_H_
#include "Window.h"
#include <windows.h>

class DX12Window : public Window {
public:
	DX12Window(const WindowDescription& p_parameter);
	~DX12Window();
	virtual void Initialize() override;
	int Run() override;
	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y) { }
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	inline HWND GetWindowInstance() const { return window_handle; };
private:
	HINSTANCE app_instance;
	WNDCLASS description;
	HWND window_handle;
	RECT screen;
};

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static DX12Window* window_application = 0;
#endif