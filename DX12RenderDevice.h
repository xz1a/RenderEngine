#ifndef _DX12_RENDER_DEVICE_H_
#define _DX12_RENDER_DEVICE_H_
#include "dx12.h"
#include "RenderDevice.h"
#include "DX12SwapChain.h"
#include "DX12Buffer.h"
#include "DX12CommandAllocator.h"
#include "DX12CommandList.h"

class DX12RenderDevice : public RenderDevice {
public:
	DX12RenderDevice(const RenderDeviceDescription& p_description);
	~DX12RenderDevice() override;
	RESULT CreateSwapChain(const SwapChainDescription& p_description, SwapChain*& p_swapchain) const override;
	RESULT CreateBuffer(const BufferDescription& p_description, Buffer*& p_buffer) const override;
	RESULT CreateTexture(const TextureDescription& p_description, Texture*& p_texture) const override;
	RESULT CreateFrameBuffer(const FrameBufferDescription& p_description, FrameBuffer*& p_framebuffer) const override;
	RESULT CreateShader(const ShaderDescription& p_description, Shader*& p_shader) const override;
	RESULT CreatePipelineState(const PipelineStateDescription& p_description, PipelineState*& p_pipline_state) const override;
	RESULT CreateRenderPass(const RenderPassDescription& p_description, RenderPass*& p_render_pass) const override;
	RESULT CreateCommandAllocator(const CommandAllocatorDescription& p_description, CommandAllocator*& p_command_allocator) override;
	RESULT CreateCommandList(const CommandListDescription& p_description, CommandList*& p_command_list) override;
	RESULT SubmitCommandList(COMMAND_TYPE p_queue_type, const std::vector<CommandList*>& p_command_lists) override;
	RESULT SignalQueue(COMMAND_TYPE p_queue_type, UINT64 p_fence_value) const override;
	RESULT WaitForQueueCPU(COMMAND_TYPE p_queue_type, COMMAND_TYPE p_wait_for_queue_type, UINT64 p_fence_value) override;
private:
protected:
	static Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> command_queues[COMMAND_TYPE::COUNT];
	Microsoft::WRL::ComPtr<ID3D12Fence> fences[COMMAND_TYPE::COUNT];
	UINT RTV_descriptor_size;
	UINT DSV_descriptor_size;
	UINT CBV_SRV_UAV_descriptor_size;
	UINT SAMPLER_descriptor_size;
};
#endif