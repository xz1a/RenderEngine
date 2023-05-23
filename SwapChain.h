#ifndef _SWAPCHAIN_H_
#define _SWAPCHAIN_H_
#include "Window.h"

typedef struct SwapChainDescription {
	unsigned int width;
	unsigned int height;
	unsigned int refresh_rate;
	unsigned int buffer_count;
	Window* window;
} SwapChainDescription;

class SwapChain {
public:
	virtual ~SwapChain() = default;
	virtual void Reset() = 0;
	virtual void Present() = 0;
protected:
	SwapChain(const SwapChainDescription& p_description) : description(p_description), current_buffer(0) {

	}
	unsigned int current_buffer;
	SwapChainDescription description;
};
#endif