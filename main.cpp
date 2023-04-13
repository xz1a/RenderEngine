#include "GLWindow.h"
int main() {
	WindowParameter window_param;
	window_param.title = "Render Engine";
	window_param.width = 800;
	window_param.height = 600;

	GLWindow window = GLWindow(window_param);
	window.Initialize();

	while (1) {

	}

	return 0;
}