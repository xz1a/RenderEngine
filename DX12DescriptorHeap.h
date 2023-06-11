#ifndef _DX12_DESCRIPTOR_HEAP_H_
#define _DX12_DESCRIPTOR_HEAP_H_
#include "dx12.h"
#include "DescriptorHeap.h"

class DX12DescriptorHeap : public DescriptorHeap {
public:
	DX12DescriptorHeap(const DescriptorHeapDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_descriptor_heap);
	~DX12DescriptorHeap() override;
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetFirstDescriptor() { return descriptor_heap->GetCPUDescriptorHandleForHeapStart(); }
protected:
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap;
};
#endif