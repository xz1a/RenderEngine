#include "DX12CommandList.h"

DX12CommandList::DX12CommandList(const CommandListDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> p_command_list) : CommandList(p_description), command_list(p_command_list) {

}

DX12CommandList::~DX12CommandList() {
	command_list.Reset();
}

ID3D12GraphicsCommandList* DX12CommandList::GetNative() {
	return command_list.Get();
}