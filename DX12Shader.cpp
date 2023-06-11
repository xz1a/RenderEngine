#include "DX12Shader.h"

DX12Shader::DX12Shader(const ShaderDescription& p_description, const Microsoft::WRL::ComPtr<ID3DBlob> p_source_buffer) :
	Shader(p_description),
	source_buffer(p_source_buffer)
{

}

DX12Shader::~DX12Shader() {

}

D3D12_SHADER_BYTECODE DX12Shader::GetByteCode() {
	return { source_buffer->GetBufferPointer(), source_buffer->GetBufferSize() };
}