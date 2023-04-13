#ifndef _DX12_WINDOW_H_
#define _DX12_WINDOW_H_
#include "Window.h"
#include <windows.h>

class DX12Window : public Window {
public:
	DX12Window(const WindowParameter& p_parameter);
	virtual void Initialize() override;
private:
	HINSTANCE app_instance;
	WNDCLASS description;
	HWND handle;
	RECT screen;
};
#endif