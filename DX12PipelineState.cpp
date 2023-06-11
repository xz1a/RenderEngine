#include "DX12PipelineState.h"

DX12PipelineState::DX12PipelineState(const PipelineStateDescription& p_description, Microsoft::WRL::ComPtr<ID3D12PipelineState> p_pipeline_state) : PipelineState(p_description), pipeline_state(p_pipeline_state) {

}

ID3D12PipelineState* DX12PipelineState::GetNative() {
	return pipeline_state.Get();
}