#ifndef _DX12_SWAP_CHAIN_H_
#define _DX12_SWAP_CHAIN_H_
#include "dx12.h"
#include "SwapChain.h"
#include "DX12Window.h"

class DX12SwapChain : public SwapChain {
public:
	DX12SwapChain(const SwapChainDescription& p_description, Microsoft::WRL::ComPtr<IDXGISwapChain> p_swapchain) {
		description = p_description;
		swapchain = p_swapchain;
	};

	~DX12SwapChain() {
		swapchain.Reset();
	}

	void Reset() override {
		swapchain.Reset();
	}
	
	void Present() override {
		swapchain->Present(0, 0);
	}
protected:
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D12Resource> buffers;
};
#endif