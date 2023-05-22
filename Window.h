#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <string>

typedef struct WindowDescription {
	std::string title;
	unsigned int width;
	unsigned int height;
};

class Window {
public:
	Window(const WindowDescription& p_parameter);
	virtual void Initialize() = 0;
	virtual int Run() = 0;
	inline unsigned int GetWidth() const { return width; };
	inline unsigned int GetHeight() const { return height; };
protected:
	std::string title;
	unsigned int width;
	unsigned int height;
};
#endif