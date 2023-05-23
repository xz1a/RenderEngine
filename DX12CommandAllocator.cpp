#include "DX12CommandAllocator.h"

DX12CommandAllocator::DX12CommandAllocator(const CommandAllocatorDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12CommandAllocator> p_command_allocator) : CommandAllocator(p_description), command_allocator(p_command_allocator) {

}

DX12CommandAllocator::~DX12CommandAllocator() {
	command_allocator.Reset();
}

ID3D12CommandAllocator* DX12CommandAllocator::GetNative() const {
	return command_allocator.Get();
}

void DX12CommandAllocator::Reset() {
	command_allocator->Reset();
}