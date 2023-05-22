#include "DX12Buffer.h"

DX12Buffer::DX12Buffer(const BufferDescription& p_description, Microsoft::WRL::ComPtr<ID3D12Resource> p_buffer) : Buffer(p_description), buffer(p_buffer) {

}

DX12Buffer::~DX12Buffer() {
	buffer.Reset();
}

RESULT DX12Buffer::Map() {
	return RESULT::E_API_FUNCTION;
}

RESULT DX12Buffer::CopyData(unsigned int p_index, unsigned int p_size, const void* data) { 
	return RESULT::E_API_FUNCTION;
}

RESULT DX12Buffer::UnMap() {
	return RESULT::E_API_FUNCTION;
}