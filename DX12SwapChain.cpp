#include "DX12SwapChain.h"

DX12SwapChain::DX12SwapChain(const SwapChainDescription& p_description, const std::vector<Texture*>& p_back_buffers, const std::vector<RenderTargetView*>& p_back_buffer_views, const Microsoft::WRL::ComPtr<IDXGISwapChain> p_swapchain) :
	SwapChain(p_description, p_back_buffers, p_back_buffer_views),
	swapchain(p_swapchain)
{

}

DX12SwapChain::~DX12SwapChain() {
	back_buffers.clear();
	render_target_views.clear();
	swapchain.Reset();
}

void DX12SwapChain::Reset() {
	swapchain.Reset();
}

void DX12SwapChain::Present() {
	swapchain->Present(0, 0);
	current_buffer_index = (current_buffer_index + 1) % description.buffer_count;
}