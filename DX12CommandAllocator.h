#ifndef _DX12_COMMAND_ALLOCATOR_H_
#define _DX12_COMMAND_ALLOCATOR_H_
#include "dx12.h"
#include "CommandAllocator.h"

class DX12CommandAllocator : public CommandAllocator {
public:
	DX12CommandAllocator(const CommandAllocatorDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12CommandAllocator> p_command_allocator);
	~DX12CommandAllocator() override;
	ID3D12CommandAllocator* GetNative() const;

	void Reset() override;
protected:
	
private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator;
};
#endif