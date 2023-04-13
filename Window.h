#ifndef _WINDOW_H_
#define _WINDOW_H_

typedef struct WindowParameter {
	const char* title;
	unsigned int width;
	unsigned int height;
};

class Window {
public:
	Window(const WindowParameter& p_parameter);
	virtual void Initialize() = 0;
protected:
	const char* title;
	unsigned int width;
	unsigned int height;
};
#endif