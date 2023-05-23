#include "DX12RenderDevice.h"

Microsoft::WRL::ComPtr<IDXGIFactory4> DX12RenderDevice::factory = nullptr;

DX12RenderDevice::DX12RenderDevice(const RenderDeviceDescription& p_description) {
	description = p_description;

	//Initialize Factory if not already.
	if (factory == nullptr) {
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));
	}

	if (description.debug) {
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
	}

	//Create DirectX 12 Device.
	HRESULT result = D3D12CreateDevice((IUnknown*)description.adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));
	ThrowIfFailed(result);

	//Cache the Descriptor Sizes.
	RTV_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	DSV_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	CBV_SRV_UAV_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	SAMPLER_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	//Initialize Queues.
	D3D12_COMMAND_QUEUE_DESC graphics_queue_desc = {};
	graphics_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	graphics_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	ThrowIfFailed(device->CreateCommandQueue(&graphics_queue_desc, IID_PPV_ARGS(&command_queues[COMMAND_TYPE::GRAPHICS])));
	
	D3D12_COMMAND_QUEUE_DESC compute_queue_desc = {};
	compute_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	compute_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	
	ThrowIfFailed(device->CreateCommandQueue(&compute_queue_desc, IID_PPV_ARGS(&command_queues[COMMAND_TYPE::COMPUTE])));
	
	D3D12_COMMAND_QUEUE_DESC copy_queue_desc = {};
	copy_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
	copy_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	
	ThrowIfFailed(device->CreateCommandQueue(&copy_queue_desc, IID_PPV_ARGS(&command_queues[COMMAND_TYPE::COPY])));

	//Initialize Fences.
	ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fences[COMMAND_TYPE::GRAPHICS])));
	ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fences[COMMAND_TYPE::COMPUTE])));
	ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fences[COMMAND_TYPE::COPY])));
}

DX12RenderDevice::~DX12RenderDevice() {
	for (int i = 0; i < COMMAND_TYPE::COUNT; ++i) {
		command_queues[i].Reset();
		fences[i].Reset();
	}
	device.Reset();
}

RESULT DX12RenderDevice::CreateSwapChain(const SwapChainDescription& p_description, SwapChain*& p_swapchain) const {

	//Create DirectX 12 SwapChain Creation Description.
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = p_description.width;
	sd.BufferDesc.Height = p_description.height;
	sd.BufferDesc.RefreshRate.Numerator = p_description.refresh_rate;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;// m4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = 0;// m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = p_description.buffer_count;
	sd.OutputWindow = ((DX12Window*)p_description.window)->GetWindowInstance();
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	
	//Create Temporary SwapChain Pointer.
	Microsoft::WRL::ComPtr<IDXGISwapChain> temp_swapchain;
	HRESULT result = factory->CreateSwapChain(command_queues[COMMAND_TYPE::GRAPHICS].Get(), &sd, temp_swapchain.GetAddressOf());
	
	if (FAILED(result)) {
		return RESULT::E_API_CREATE_SWAP_CHAIN;
	}

	p_swapchain = new DX12SwapChain(p_description, temp_swapchain);
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::CreateBuffer(const BufferDescription& p_description, Buffer*& p_buffer) const {
	
	//Default Buffer Resource Parameters.
	D3D12_HEAP_TYPE heap_type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;

	if (p_description.type == BUFFER_TYPE::UPLOAD) {
		heap_type = D3D12_HEAP_TYPE_UPLOAD;
		state = D3D12_RESOURCE_STATE_GENERIC_READ;
	}
	else if (p_description.type == BUFFER_TYPE::READBACK) {
		heap_type = D3D12_HEAP_TYPE_READBACK;
		state = D3D12_RESOURCE_STATE_COPY_DEST;
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> temp_buffer;
	HRESULT result =  device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(heap_type), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(p_description.size), state, nullptr, IID_PPV_ARGS(temp_buffer.GetAddressOf()));

	if (FAILED(result)) {
		return RESULT::E_API_CREATE_BUFFER;
	}

	p_buffer = new DX12Buffer(p_description, temp_buffer);
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::CreateTexture(const TextureDescription& p_description, Texture*& p_texture) const {
	return RESULT::E_API_CREATE_TEXTURE;
}

RESULT DX12RenderDevice::CreateFrameBuffer(const FrameBufferDescription& p_description, FrameBuffer*& p_framebuffer) const {
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::CreateShader(const ShaderDescription& p_description, Shader*& p_shader) const {
	return RESULT::E_API_CREATE_SHADER;
}

RESULT DX12RenderDevice::CreatePipelineState(const PipelineStateDescription& p_description, PipelineState*& p_pipline_state) const {
	return RESULT::E_API_CREATE_PIPELINE_STATE;
}

RESULT DX12RenderDevice::CreateRenderPass(const RenderPassDescription& p_description, RenderPass*& p_render_pass) const {
	return RESULT::E_API_CREATE_RENDER_PASS;
}

RESULT DX12RenderDevice::CreateCommandAllocator(const CommandAllocatorDescription& p_description, CommandAllocator*& p_command_allocator) {

	D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	switch (p_description.type) {
	case COMMAND_TYPE::COMPUTE:
		type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
		break;
	case COMMAND_TYPE::COPY:
		type = D3D12_COMMAND_LIST_TYPE_COPY;
		break;
	default:
		break;
	}

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> temp_command_allocator;
	HRESULT result = device->CreateCommandAllocator(type, IID_PPV_ARGS(temp_command_allocator.GetAddressOf()));
	if (FAILED(result)) {
		return RESULT::E_API_CREATE_COMMAND_ALLOCATOR;
	}

	p_command_allocator = new DX12CommandAllocator(p_description, temp_command_allocator);
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::CreateCommandList(const CommandListDescription& p_description, CommandList*& p_command_list) {
	D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	ID3D12PipelineState* pso = nullptr;

	switch (p_description.type) {
	case COMMAND_TYPE::COMPUTE:
		type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
		break;
	case COMMAND_TYPE::COPY:
		type = D3D12_COMMAND_LIST_TYPE_COPY;
		break;
	default:
		break;
	}

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> temp_command_list;
	HRESULT result = device->CreateCommandList(0, type, ((DX12CommandAllocator*)p_description.allocator)->GetNative(), pso, IID_PPV_ARGS(temp_command_list.GetAddressOf()));
	
	if (FAILED(result)) {
		return RESULT::E_API_CREATE_COMMAND_LIST;
	}

	p_command_list = new DX12CommandList(p_description, temp_command_list);
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::SubmitCommandList(COMMAND_TYPE p_queue_type, const std::vector<CommandList*>& p_command_lists) {
	std::vector<ID3D12CommandList*> submit_list = std::vector<ID3D12CommandList*>(p_command_lists.size());
	for (unsigned int i = 0; i < p_command_lists.size(); ++i) {
		submit_list[i] = ((DX12CommandList*)p_command_lists[i])->GetNative();
	}
	command_queues[p_queue_type]->ExecuteCommandLists(p_command_lists.size(), submit_list.data());
	return RESULT::E_COMMAND_LIST_SUBMISSION;
}

RESULT DX12RenderDevice::SignalQueue(COMMAND_TYPE p_queue_type, UINT64 p_fence_value) const {
	HRESULT result = command_queues[p_queue_type]->Signal(fences[p_queue_type].Get(), p_fence_value);
	if (FAILED(result)) {
		return RESULT::E_QUEUE_SIGNAL;
	}
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::WaitForQueueCPU(COMMAND_TYPE p_queue_type, COMMAND_TYPE p_wait_for_queue_type, UINT64 p_fence_value) {
	if (fences[p_wait_for_queue_type]->GetCompletedValue() < p_fence_value) {
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
  
		ThrowIfFailed(fences[p_wait_for_queue_type]->SetEventOnCompletion(p_fence_value, eventHandle));

		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
	return RESULT::SUCCESS;
}