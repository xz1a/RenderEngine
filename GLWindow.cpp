#include "GLWindow.h"

GLWindow::GLWindow(const WindowParameter& p_parameter) : Window(p_parameter) {
	handle = nullptr;
}

void GLWindow::Initialize() {
	glfwInit();
	handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
}