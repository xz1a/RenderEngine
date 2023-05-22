#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#include "Result.h"
#include "DX12Window.h"

#include <iostream>

RESULT CreatePlatformWindow(const WindowDescription& p_description, Window*& p_window) {
#if defined(_WIN32)
	p_window = new DX12Window(p_description);
	return RESULT::SUCCESS;
#elif defined(_WIN64)
	std::cout << "WIN64 Platform Detected." << std::endl;
	p_window = new DX12Window(p_description);
	return RESULT::SUCCESS;
#else
	std::cout << "No Platform Detected." << std::endl;
	return RESULT::E_API_CREATE;
#endif
}
#endif