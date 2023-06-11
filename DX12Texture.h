#ifndef _DX12_TEXTURE_H_
#define _DX12_TEXTURE_H_
#include "dx12.h"
#include "Texture.h"

class DX12Texture : public Texture {
public:
	DX12Texture(const TextureDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12Resource> p_texture);
	~DX12Texture() override;
	ID3D12Resource* GetNative() const;

	RESULT Map() override;
	RESULT UnMap() override;
	RESULT CopyData(unsigned int p_index, unsigned int p_size, const void* data) override;
protected:
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> texture;
};
#endif