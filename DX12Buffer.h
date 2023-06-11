#ifndef _DX12_BUFFER_H_
#define _DX12_BUFFER_H_
#include "dx12.h"
#include "Buffer.h"
#include <iostream>

class DX12Buffer : public Buffer {
public:
	DX12Buffer(const BufferDescription& p_description, const Microsoft::WRL::ComPtr<ID3D12Resource> p_buffer);
	~DX12Buffer() override;
	ID3D12Resource* GetNative() const { return buffer.Get(); };

	RESULT Map() override;
	RESULT UnMap() override;
	RESULT CopyData(unsigned int p_index, unsigned int p_size, const void* data) override;
protected:
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
};
#endif