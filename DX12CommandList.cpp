#include "DX12CommandList.h"

DX12CommandList::DX12CommandList(const CommandListDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> p_command_list) : 
	CommandList(p_description), 
	command_list(p_command_list) 
{
	command_list->Close();
}

DX12CommandList::~DX12CommandList() {
	command_list.Reset();
}

void DX12CommandList::Reset(const CommandAllocator* p_allocator, const PipelineState* p_pipeline_state) {
	if (p_pipeline_state == nullptr) {
		command_list->Reset(((DX12CommandAllocator*)p_allocator)->GetNative(), nullptr);
	}
	else {

	}
}

void DX12CommandList::Close() {
	command_list->Close();
}

void DX12CommandList::CopyBuffer(const Buffer* p_src_buffer, unsigned int p_src_offset, unsigned int p_src_size, const Buffer* p_dest_buffer, unsigned int p_dest_offset, unsigned int p_dest_size) {
	//command_list->CopyBufferRegion();
}

void DX12CommandList::CopyTexture(const Texture* p_src_texture, const Texture* p_dest_texture) {
	//command_list->CopyTextureRegion();
}

void DX12CommandList::SetBufferBarrier(const BufferBarrier& p_barrier) {
	command_list->ResourceBarrier(1, (&CD3DX12_RESOURCE_BARRIER::Transition((dynamic_cast<DX12Buffer*>(p_barrier.buffer))->GetNative(), DX12Converter::GetDX12State(p_barrier.start_state), DX12Converter::GetDX12State(p_barrier.end_state))));
}

void DX12CommandList::SetTextureBarrier(const TextureBarrier& p_barrier) {
	command_list->ResourceBarrier(1, (&CD3DX12_RESOURCE_BARRIER::Transition((dynamic_cast<DX12Texture*>(p_barrier.texture))->GetNative(), DX12Converter::GetDX12State(p_barrier.start_state), DX12Converter::GetDX12State(p_barrier.end_state))));
}

void DX12CommandList::BeginRenderPass(const BeginRenderPassDescription& p_begin_render_pass_description, float p_clear_value[4]) {
	DX12FrameBuffer* frame_buffer = dynamic_cast<DX12FrameBuffer*>(p_begin_render_pass_description.frame_buffer);
	command_list->OMSetRenderTargets(frame_buffer->GetAttachmentCount(), frame_buffer->GetRenderTargets(), true, nullptr);
	command_list->ClearRenderTargetView(frame_buffer->GetRenderTargets()[0], DirectX::Colors::LightSteelBlue, 0, nullptr);
}

void DX12CommandList::EndRenderRPass() {
	//command_list->EndRenderPass();
}

void DX12CommandList::SetPipelineState(const PipelineState* p_pipeline_state) {
	//command_list->SetPipelineState();
}

void DX12CommandList::SetViewport(const Viewport& p_viewport) {
	//command_list->RSSetViewports();
}

void DX12CommandList::SetVertexBuffer(const Buffer* p_buffer) {
	D3D12_VERTEX_BUFFER_VIEW view;
	view.BufferLocation = (dynamic_cast<const DX12Buffer*>(p_buffer))->GetNative()->GetGPUVirtualAddress();
	view.SizeInBytes = 0;
	view.StrideInBytes = 0;
	command_list->IASetVertexBuffers(0, 1, &view);
}

void DX12CommandList::SetIndexBuffer(const Buffer* p_buffer, FORMAT_TYPE p_type) {
	D3D12_INDEX_BUFFER_VIEW view;
	view.BufferLocation = (dynamic_cast<const DX12Buffer*>(p_buffer))->GetNative()->GetGPUVirtualAddress();
	view.Format = DX12Converter::GetDX12Type(p_type);
	view.SizeInBytes = 0;
	command_list->IASetIndexBuffer(&view);
}

void DX12CommandList::ClearRenderTarget() {
	//command_list->ClearRenderTargetView();
}

void DX12CommandList::ClearDepthAndStencil() {
	//command_list->ClearDepthStencilView();
}

void DX12CommandList::DrawInstanced() {
	//command_list->DrawInstanced();
}

void DX12CommandList::DrawIndexInstanced() {
	//command_list->DrawIndexedInstanced();
}

ID3D12GraphicsCommandList* DX12CommandList::GetNative() {
	return command_list.Get();
}