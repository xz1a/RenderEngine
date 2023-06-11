#ifndef _DX12_PIPELINE_STATE_H_
#define _DX12_PIPELINE_STATE_H_
#include "dx12.h"
#include "PipelineState.h"

class DX12PipelineState : public PipelineState {
public:
	DX12PipelineState(const PipelineStateDescription& p_description, Microsoft::WRL::ComPtr<ID3D12PipelineState> p_pipeline_state);
	ID3D12PipelineState* GetNative();
protected:
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipeline_state;
};
#endif