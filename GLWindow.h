#ifndef _GL_WINDOW_H_
#define _GL_WINDOW_H_
#include "Window.h"
#include <GLFW/glfw3.h>

class GLWindow : public Window {
public:
	GLWindow(const WindowDescription& p_parameter);
	virtual void Initialize() override;
	inline GLFWwindow* GetWindowInstance() const { return window_handle; };
protected:
private:
	GLFWwindow* window_handle;
};
#endif