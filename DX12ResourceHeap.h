#ifndef _DX12_RESOURCE_HEAP_H_
#define _DX12_RESOURCE_HEAP_H_
#include "dx12.h"
#include "ResourceHeap.h"

class DX12ResourceHeap : public ResourceHeap {
public:
	DX12ResourceHeap(const ResourceHeapDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12Heap> p_resource_heap);
	~DX12ResourceHeap() override;
	ID3D12Heap* GetNative() const;
protected:
private:
	Microsoft::WRL::ComPtr<ID3D12Heap> resource_heap;
};
#endif