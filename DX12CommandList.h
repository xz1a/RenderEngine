#ifndef _DX12_COMMAND_LIST_H_
#define _DX12_COMMAND_LIST_H_
#include "dx12.h"
#include "CommandList.h"
#include "DX12SwapChain.h"
#include "DX12CommandAllocator.h"
#include "DX12PipelineState.h"
#include "DX12RenderTargetView.h"
#include "DX12FrameBuffer.h"
#include "DX12Buffer.h"
#include "DX12Texture.h"
#include "DX12Converter.h"

class DX12CommandList : public CommandList {
public:
	DX12CommandList(const CommandListDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> p_command_list);
	~DX12CommandList() override;
	ID3D12GraphicsCommandList* GetNative();

	void Reset(const CommandAllocator* p_allocator, const PipelineState* p_pipeline_state) override;
	void Close() override;

	void CopyBuffer(const Buffer* p_src_buffer, unsigned int p_src_offset, unsigned int p_src_size, const Buffer* p_dest_buffer, unsigned int p_dest_offset, unsigned int p_dest_size) override;
	void CopyTexture(const Texture* p_src_texture, const Texture* p_dest_texture) override;
	void SetBufferBarrier(const BufferBarrier& p_barrier) override;
	void SetTextureBarrier(const TextureBarrier& p_barrier) override;

	void BeginRenderPass(const BeginRenderPassDescription& p_begin_render_pass_description, float p_clear_value[4]) override;
	void EndRenderRPass() override;

	void SetPipelineState(const PipelineState* p_pipeline_state) override;
	void SetViewport(const Viewport& p_viewport) override;
	void SetVertexBuffer(const Buffer* p_buffer) override;
	void SetIndexBuffer(const Buffer* p_buffer, FORMAT_TYPE p_type) override;

	void ClearRenderTarget() override;
	void ClearDepthAndStencil() override;

	void DrawInstanced() override;
	void DrawIndexInstanced() override;

	void TestClearRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE* handle) {
		command_list->ClearRenderTargetView(*handle, DirectX::Colors::LightSteelBlue, 0, nullptr);
	}
	void SetRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE* handle) {
		command_list->OMSetRenderTargets(1, handle, true, nullptr);
	}
protected:
private:
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> command_list;
};
#endif