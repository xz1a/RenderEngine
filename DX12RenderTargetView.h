#ifndef _DX12_RENDER_TARGET_VIEW_H_
#define _DX12_RENDER_TARGET_VIEW_H_
#include "dx12.h"
#include "RenderTargetView.h"

class DX12RenderTargetView : public RenderTargetView {
public:
	DX12RenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE p_view) : view(p_view) {};
	D3D12_CPU_DESCRIPTOR_HANDLE GetNative();
protected:
private:
	D3D12_CPU_DESCRIPTOR_HANDLE view;
};
#endif