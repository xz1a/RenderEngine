#include "DX12Converter.h"


D3D12_RESOURCE_STATES DX12Converter::GetDX12State(RESOURCE_STATE p_state)  {
	switch (p_state) {
	case RESOURCE_STATE::COMMON:
		return D3D12_RESOURCE_STATE_COMMON;
		break;
	case RESOURCE_STATE::VERTEX_BUFFER:
		return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		break;
	case RESOURCE_STATE::INDEX_BUFFER:
		return D3D12_RESOURCE_STATE_INDEX_BUFFER;
		break;
	case RESOURCE_STATE::CONSTANT_BUFFER:
		return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		break;
	case RESOURCE_STATE::RENDER_TARGET:
		return D3D12_RESOURCE_STATE_RENDER_TARGET;
		break;
	case RESOURCE_STATE::PRESENT:
		return D3D12_RESOURCE_STATE_PRESENT;
		break;
	case RESOURCE_STATE::COPY_SRC:
		return D3D12_RESOURCE_STATE_COPY_SOURCE;
		break;
	case RESOURCE_STATE::COPY_DEST:
		return D3D12_RESOURCE_STATE_COPY_DEST;
		break;
	}
}

DXGI_FORMAT DX12Converter::GetDX12Type(FORMAT_TYPE p_type) {
	switch (p_type) {
	case FORMAT_TYPE::R8_UINT:
		return DXGI_FORMAT_R8_UINT;
		break;
	case FORMAT_TYPE::R8G8B8A8_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case FORMAT_TYPE::R16_FLOAT:
		return DXGI_FORMAT_R16_FLOAT;
		break;
	case FORMAT_TYPE::R16G16_FLOAT:
		return DXGI_FORMAT_R16G16_FLOAT;
		break;
	case FORMAT_TYPE::R16G16B16A16_FLOAT:
		return DXGI_FORMAT_R16G16B16A16_FLOAT;
		break;
	case FORMAT_TYPE::R16_UINT:
		return DXGI_FORMAT_R16_UINT;
		break;
	case FORMAT_TYPE::R16G16_UINT:
		return DXGI_FORMAT_R16G16_UINT;
		break;
	case FORMAT_TYPE::R16G16B16A16_UINT:
		return DXGI_FORMAT_R16G16B16A16_UINT;
		break;
	case FORMAT_TYPE::R16_SINT:
		return DXGI_FORMAT_R16_SINT;
		break;
	case FORMAT_TYPE::R16G16_SINT:
		return DXGI_FORMAT_R16G16_SINT;
		break;
	case FORMAT_TYPE::R16G16B16A16_SINT:
		return DXGI_FORMAT_R16G16B16A16_SINT;
		break;
	case FORMAT_TYPE::R32_FLOAT:
		return DXGI_FORMAT_R32_FLOAT;
		break;
	case FORMAT_TYPE::R32G32_FLOAT:
		return DXGI_FORMAT_R32G32_FLOAT;
		break;
	case FORMAT_TYPE::R32G32B32_FLOAT:
		return DXGI_FORMAT_R32G32B32_FLOAT;
		break;
	case FORMAT_TYPE::R32G32B32A32_FLOAT:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
	case FORMAT_TYPE::R32_UINT:
		return DXGI_FORMAT_R32_UINT;
		break;
	case FORMAT_TYPE::R32G32_UINT:
		return DXGI_FORMAT_R32G32_UINT;
		break;
	case FORMAT_TYPE::R32G32B32_UINT:
		return DXGI_FORMAT_R32G32B32_UINT;
		break;
	case FORMAT_TYPE::R32G32B32A32_UINT:
		return DXGI_FORMAT_R32G32B32A32_UINT;
		break;
	case FORMAT_TYPE::R32_SINT:
		return DXGI_FORMAT_R32_SINT;
		break;
	case FORMAT_TYPE::R32G32_SINT:
		return DXGI_FORMAT_R32G32_SINT;
		break;
	case FORMAT_TYPE::R32G32B32_SINT:
		return DXGI_FORMAT_R32G32B32_SINT;
		break;
	case FORMAT_TYPE::R32G32B32A32_SINT:
		return DXGI_FORMAT_R32G32B32A32_SINT;
		break;
	}
};