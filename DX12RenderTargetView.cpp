#include "DX12RenderTargetView.h"

D3D12_CPU_DESCRIPTOR_HANDLE DX12RenderTargetView::GetNative() {
	return view;
}