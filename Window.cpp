#include "Window.h"

Window::Window(const WindowDescription& p_parameter) {
	title = p_parameter.title;
	width = p_parameter.width;
	height = p_parameter.height;
}