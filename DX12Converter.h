#ifndef _CONVERTER_H_
#define _CONVERTER_H_
#include "dx12.h"
#include "ResourceState.h"
#include "FormatType.h"

namespace DX12Converter {
	D3D12_RESOURCE_STATES GetDX12State(RESOURCE_STATE p_state);

	DXGI_FORMAT GetDX12Type(FORMAT_TYPE p_type);
};
#endif