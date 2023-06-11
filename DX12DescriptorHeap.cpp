#include "DX12DescriptorHeap.h"

DX12DescriptorHeap::DX12DescriptorHeap(const DescriptorHeapDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_descriptor_heap) : 
	DescriptorHeap(p_description),
	descriptor_heap(p_descriptor_heap)
{

}

DX12DescriptorHeap::~DX12DescriptorHeap() {
	descriptor_heap.Reset();
}