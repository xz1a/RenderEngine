#ifndef _DX12_SWAP_CHAIN_H_
#define _DX12_SWAP_CHAIN_H_
#include "dx12.h"
#include "SwapChain.h"
#include "DX12Window.h"
#include "DX12RenderTargetView.h"
#include <vector>

class DX12SwapChain : public SwapChain {
public:
	DX12SwapChain(const SwapChainDescription& p_description, const std::vector<Texture*>& p_back_buffers, const std::vector<RenderTargetView*>& p_back_buffer_views, const Microsoft::WRL::ComPtr<IDXGISwapChain> p_swapchain);
	~DX12SwapChain() override;

	void Reset() override;
	void Present() override;
protected:
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D12Resource> buffers;
};
#endif