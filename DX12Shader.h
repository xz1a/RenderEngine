#ifndef _DX12_SHADER_H_
#define _DX12_SHADER_H_
#include "dx12.h"
#include "Shader.h"

class DX12Shader : public Shader {
public:
protected:
private:
	Microsoft::WRL::ComPtr<ID3DBlob> shader;
};
#endif