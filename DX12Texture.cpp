#include "DX12Texture.h"

DX12Texture::DX12Texture(const TextureDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12Resource> p_texture) : 
	Texture(p_description), 
	texture(p_texture) 
{

}

DX12Texture::~DX12Texture() {

}

RESULT DX12Texture::Map() {
	return RESULT::SUCCESS;
}

RESULT DX12Texture::UnMap() {
	return RESULT::SUCCESS;
}

RESULT DX12Texture::CopyData(unsigned int p_index, unsigned int p_size, const void* data) {
	return RESULT::SUCCESS;
}

ID3D12Resource* DX12Texture::GetNative() const {
	return texture.Get(); 
}