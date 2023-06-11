#include "DX12FrameBuffer.h"

DX12FrameBuffer::DX12FrameBuffer(const FrameBufferDescription& p_description) : FrameBuffer(p_description) {
	render_target_views = std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>(p_description.attachment_count);
	for (unsigned int i = 0; i < p_description.attachment_count; ++i) {
		render_target_views[i] = dynamic_cast<DX12RenderTargetView*>(p_description.attachments[i])->GetNative();
	}
}

DX12FrameBuffer::~DX12FrameBuffer() {

}

D3D12_CPU_DESCRIPTOR_HANDLE* DX12FrameBuffer::GetRenderTargets() {
	return render_target_views.data();
}