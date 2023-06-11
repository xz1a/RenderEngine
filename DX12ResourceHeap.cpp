#include "DX12ResourceHeap.h"

DX12ResourceHeap::DX12ResourceHeap(const ResourceHeapDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12Heap> p_resource_heap) : 
	ResourceHeap(p_description),
	resource_heap(p_resource_heap)
{

}

DX12ResourceHeap::~DX12ResourceHeap() {
	resource_heap.Reset();
}

ID3D12Heap* DX12ResourceHeap::GetNative() const {
	return resource_heap.Get();
}