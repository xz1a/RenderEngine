#ifndef _DX12_FRAME_BUFFER_H_
#define _DX12_FRAME_BUFFER_H_
#include "dx12.h"
#include "FrameBuffer.h"
#include "DX12RenderTargetView.h"

class DX12FrameBuffer : public FrameBuffer {
public:
	DX12FrameBuffer(const FrameBufferDescription& p_description);
	~DX12FrameBuffer() override;
	D3D12_CPU_DESCRIPTOR_HANDLE* GetRenderTargets();
private:
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> render_target_views;
};
#endif