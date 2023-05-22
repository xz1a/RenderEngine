#include "GLWindow.h"

GLWindow::GLWindow(const WindowDescription& p_parameter) : Window(p_parameter) {
	window_handle = nullptr;
}

void GLWindow::Initialize() {
	glfwInit();
	window_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}