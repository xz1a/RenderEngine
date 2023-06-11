#include "DX12Buffer.h"

DX12Buffer::DX12Buffer(const BufferDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12Resource> p_buffer) : 
	Buffer(p_description), 
	buffer(p_buffer)
{

}

DX12Buffer::~DX12Buffer() {
	buffer.Reset();
}

RESULT DX12Buffer::Map() {
	if (!mapped) {
		HRESULT result = buffer->Map(0, NULL, reinterpret_cast<void**>(&mapped_data));
		mapped = true;

		if (FAILED(result)) {
			return RESULT::E_RESOURCE_MAP_FAILED;
		}

		return RESULT::SUCCESS;
	}
	return RESULT::E_API_FUNCTION;
}

RESULT DX12Buffer::CopyData(unsigned int p_index, unsigned int p_size, const void* data) { 
	return RESULT::E_API_FUNCTION;
}

RESULT DX12Buffer::UnMap() {
	if (mapped) {
		buffer->Unmap(0, NULL);
		mapped = false;
		return RESULT::SUCCESS;
	}
	return RESULT::E_API_FUNCTION;
}