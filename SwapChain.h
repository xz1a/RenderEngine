#ifndef _SWAPCHAIN_H_
#define _SWAPCHAIN_H_
#include "Window.h"
#include "RenderTargetView.h"
#include "Texture.h"
#include "FormatType.h"
#include <vector>

typedef struct SwapChainDescription {
	unsigned int width;
	unsigned int height;
	unsigned int refresh_rate;
	unsigned int buffer_count;
	FORMAT_TYPE format;
	Window* window;
} SwapChainDescription;

class SwapChain {
public:
	virtual ~SwapChain() = default;
	virtual void Reset() = 0;
	virtual void Present() = 0;
	
	inline RenderTargetView** GetRenderTargetViews() { return render_target_views.data(); };
	inline Texture** GetBackBuffers() { return back_buffers.data(); };
	inline Texture* GetCurrentBackBuffer() { return back_buffers[current_buffer_index]; };
	inline unsigned int GetCurrentBufferIndex() { return current_buffer_index; };
protected:
	SwapChain(const SwapChainDescription& p_description, const std::vector<Texture*> p_back_buffers, const std::vector<RenderTargetView*> p_render_target_views) : 
		description(p_description),
		back_buffers(p_back_buffers),
		render_target_views(p_render_target_views), 
		current_buffer_index(0) {
	
	}
	unsigned int current_buffer_index;
	SwapChainDescription description;
	std::vector<RenderTargetView*> render_target_views;
	std::vector<Texture*> back_buffers;
};
#endif