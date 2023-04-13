#ifndef _GL_WINDOW_H_
#define _GL_WINDOW_H_
#include "Window.h"
#include <GLFW/glfw3.h>

class GLWindow : public Window {
public:
	GLWindow(const WindowParameter& p_parameter);
	virtual void Initialize() override;
protected:
private:
	GLFWwindow* handle;
};
#endif