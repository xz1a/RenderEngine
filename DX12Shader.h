#ifndef _DX12_SHADER_H_
#define _DX12_SHADER_H_
#include "dx12.h"
#include "Shader.h"

class DX12Shader : public Shader {
public:
	DX12Shader(const ShaderDescription& p_description, const Microsoft::WRL::ComPtr<ID3DBlob> p_source_buffer);
	~DX12Shader() override;
	D3D12_SHADER_BYTECODE GetByteCode();
protected:
private:
	Microsoft::WRL::ComPtr<ID3DBlob> source_buffer;
};
#endif