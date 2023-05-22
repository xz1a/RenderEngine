#ifndef _DX12_COMMAND_LIST_H_
#define _DX12_COMMAND_LIST_H_
#include "dx12.h"
#include "CommandList.h"

class DX12CommandList : public CommandList {
public:
	DX12CommandList(const CommandListDescription& p_description, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> p_command_list);
	~DX12CommandList();

	ID3D12GraphicsCommandList* GetNative();
protected:
private:
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> command_list;
};
#endif