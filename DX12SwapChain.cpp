#include "DX12SwapChain.h"

DX12SwapChain::DX12SwapChain(const SwapChainDescription& p_description, const Microsoft::WRL::ComPtr<IDXGISwapChain> p_swapchain) : SwapChain(p_description), swapchain(p_swapchain) {

};

DX12SwapChain::~DX12SwapChain() {
	swapchain.Reset();
}

void DX12SwapChain::Reset() {
	swapchain.Reset();
}

void DX12SwapChain::Present() {
	swapchain->Present(0, 0);
}