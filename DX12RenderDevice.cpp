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

	D3D12_FEATURE_DATA_D3D12_OPTIONS options;
	device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options));

	//Cache the Descriptor Sizes.
	rtv_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	dsv_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	cbv_srv_uav_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	sampler_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

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

RESULT DX12RenderDevice::CreateSwapChain(const SwapChainDescription& p_description, DescriptorHeap* p_descriptor_heap, SwapChain*& p_swapchain) const {

	//Create DirectX 12 SwapChain Creation Description.
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = p_description.width;
	sd.BufferDesc.Height = p_description.height;
	sd.BufferDesc.RefreshRate.Numerator = p_description.refresh_rate;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format =	DX12Converter::GetDX12Type(p_description.format);
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

	//Resize SwapChain
	temp_swapchain->ResizeBuffers(p_description.buffer_count, p_description.width, p_description.height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	std::vector<RenderTargetView*> temp_render_target_views = std::vector<RenderTargetView*>(p_description.buffer_count);
	std::vector<Texture*> temp_back_buffers = std::vector<Texture*>(p_description.buffer_count);
	CD3DX12_CPU_DESCRIPTOR_HANDLE temp_cpu_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE((dynamic_cast<DX12DescriptorHeap*>(p_descriptor_heap))->GetFirstDescriptor());
	//TODO RTV and DSV Initialization as well.
	for (int i = 0; i < p_description.buffer_count; ++i) {
		Microsoft::WRL::ComPtr<ID3D12Resource> temp_texture;
		HRESULT result = temp_swapchain->GetBuffer(i, IID_PPV_ARGS(&temp_texture));
		
		device->CreateRenderTargetView(temp_texture.Get(), nullptr, temp_cpu_handle);
		temp_render_target_views[i] = new DX12RenderTargetView(temp_cpu_handle);

		TextureDescription texture_description;
		texture_description.width = p_description.width;
		texture_description.height = p_description.height;
		texture_description.depth = 1;
		texture_description.mip_levels = 1;
		texture_description.format = FORMAT_TYPE::R8_UINT; //TODO CHANGE TO CORRECT FORMAT
		temp_back_buffers[i] = new DX12Texture(texture_description, temp_texture);

		temp_cpu_handle.Offset(1, rtv_descriptor_size);
	}

	p_swapchain = new DX12SwapChain(p_description, temp_back_buffers, temp_render_target_views, temp_swapchain);
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::CreateBuffer(const BufferDescription& p_description, UINT64 offset, const ResourceHeap* p_resource_heap, Buffer*& p_buffer) const {
	
	//Default Buffer Resource Parameters.
	D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;

	if (p_description.type == BUFFER_TYPE::UPLOAD) {
		state = D3D12_RESOURCE_STATE_GENERIC_READ;
	}
	else if (p_description.type == BUFFER_TYPE::READBACK) {
		state = D3D12_RESOURCE_STATE_COPY_DEST;
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> temp_buffer;
		
	HRESULT result = device->CreatePlacedResource((dynamic_cast<const DX12ResourceHeap*>(p_resource_heap))->GetNative(), offset, &CD3DX12_RESOURCE_DESC::Buffer(p_description.size), state, nullptr, IID_PPV_ARGS(temp_buffer.GetAddressOf()));
	if (FAILED(result)) {
		return RESULT::E_API_CREATE_BUFFER;
	}

	p_buffer = new DX12Buffer(p_description, temp_buffer);
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::CreateTexture(const TextureDescription& p_description, UINT64 offset, const ResourceHeap* p_resource_heap, Texture*& p_texture) const {

	D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;

	CD3DX12_RESOURCE_DESC resource_desc;
		switch (p_description.depth) {
	case 1:
		resource_desc = CD3DX12_RESOURCE_DESC::Tex1D(DX12Converter::GetDX12Type(p_description.format), p_description.width, p_description.height, p_description.mip_levels);
		break;
	case 2:
		resource_desc = CD3DX12_RESOURCE_DESC::Tex2D(DX12Converter::GetDX12Type(p_description.format), p_description.width, p_description.height, p_description.mip_levels);
		break;
	case 3:
		resource_desc = CD3DX12_RESOURCE_DESC::Tex3D(DX12Converter::GetDX12Type(p_description.format), p_description.width, p_description.height, p_description.mip_levels);
		break;
	default:
		return RESULT::E_API_CREATE_TEXTURE;
		break;
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> temp_texture;

	HRESULT result = device->CreatePlacedResource((dynamic_cast<const DX12ResourceHeap*>(p_resource_heap))->GetNative(), offset, &resource_desc, state, nullptr, IID_PPV_ARGS(temp_texture.GetAddressOf()));
	if (FAILED(result)) {
		return RESULT::E_API_CREATE_BUFFER;
	}

	p_texture = new DX12Texture(p_description, temp_texture);
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::CreateFrameBuffer(const FrameBufferDescription& p_description, FrameBuffer*& p_framebuffer) const {
	p_framebuffer = new DX12FrameBuffer(p_description);
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::CreateShader(const ShaderDescription& p_description, Shader*& p_shader) const {

	
		/*const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target)*/
	const D3D_SHADER_MACRO* defines = nullptr;
	UINT compile_flags = 0;
	#if defined(DEBUG) || defined(_DEBUG)  
	compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3DBlob> byte_code = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errors;
	std::string entry_point = "VS";
	std::string target = "vs_5_0";
	switch (p_description.type) {
	case SHADER_TYPE::PIXEL:
		entry_point = "PS";
		target = "ps_5_0";
		break;
	}
	result = D3DCompileFromFile(std::wstring(p_description.source_file_path.begin(), p_description.source_file_path.end()).c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point.c_str(), target.c_str(), compile_flags, 0, &byte_code, &errors);

	if (errors != nullptr) {
		OutputDebugStringA((char*)errors->GetBufferPointer());
	}

	if (FAILED(result)) {
		return RESULT::E_API_CREATE_SHADER;
	}

	p_shader = new DX12Shader(p_description, byte_code);
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::CreatePipelineState(const PipelineStateDescription& p_description, PipelineState*& p_pipline_state) const {
	
	std::vector<D3D12_INPUT_ELEMENT_DESC> elements = std::vector<D3D12_INPUT_ELEMENT_DESC>(p_description.input_layout->element_count);
	for (unsigned int i = 0; i < p_description.input_layout->element_count; ++i) {
		InputElementDescription input_description = p_description.input_layout->elements[i];
		elements[i] = { input_description.name, 0, DX12Converter::GetDX12Type(input_description.format), 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }; // TODO Change classification.
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC description;
	description.InputLayout = {
		elements.data(),
		p_description.input_layout->element_count
	};
	description.pRootSignature = nullptr; //TODO
	if (p_description.vertex_shader != nullptr) {
		description.VS = dynamic_cast<DX12Shader*>(p_description.vertex_shader)->GetByteCode();
	}
	if (p_description.pixel_shader != nullptr) {
		description.PS = dynamic_cast<DX12Shader*>(p_description.pixel_shader)->GetByteCode();
	}
	description.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	description.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	description.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	description.SampleMask = UINT_MAX;
	description.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	description.NumRenderTargets = p_description.render_pass->GetAttachmentCount();
	description.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	description.SampleDesc.Count = 1;// m4xMsaaState ? 4 : 1;
	description.SampleDesc.Quality = 0;// m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	description.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	return RESULT::E_API_CREATE_PIPELINE_STATE;
}

RESULT DX12RenderDevice::CreateRenderPass(const RenderPassDescription& p_description, RenderPass*& p_render_pass) const {
	return RESULT::E_API_CREATE_RENDER_PASS;
}

RESULT DX12RenderDevice::CreateDescriptorHeap(const DescriptorHeapDescription& p_description, DescriptorHeap*& p_descriptor_heap) const {
	D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_description;
	descriptor_heap_description.NumDescriptors = p_description.descriptor_count;
	descriptor_heap_description.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptor_heap_description.NodeMask = 0;
	descriptor_heap_description.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	switch (p_description.type) {
	case DESCRIPTOR_HEAP_TYPE::SAMPLER:
		descriptor_heap_description.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		break;
	case DESCRIPTOR_HEAP_TYPE::RTV:
		descriptor_heap_description.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		break;
	case DESCRIPTOR_HEAP_TYPE::DSV:
		descriptor_heap_description.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		break;
	}

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> temp_descriptor_heap;
	
	HRESULT result = device->CreateDescriptorHeap(&descriptor_heap_description, IID_PPV_ARGS(temp_descriptor_heap.GetAddressOf()));
	if (FAILED(result)) {
		return RESULT::E_API_CREATE_DESCRIPTOR_HEAP;
	}

	p_descriptor_heap = new DX12DescriptorHeap(p_description, temp_descriptor_heap);
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::CreateResourceHeap(const ResourceHeapDescription& p_description, ResourceHeap*& p_resource_heap) const {

	D3D12_HEAP_PROPERTIES heap_properties;
	heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_properties.CreationNodeMask = 0;
	heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heap_properties.VisibleNodeMask = 0;

	switch (p_description.type) {
	case RESOURCE_HEAP_TYPE::UPLOAD:
		heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
		break;
	case RESOURCE_HEAP_TYPE::READBACK:
		heap_properties.Type = D3D12_HEAP_TYPE_READBACK;
		break;
	}

	D3D12_HEAP_DESC heap_description;
	heap_description.Alignment = 0;
	heap_description.Flags = D3D12_HEAP_FLAG_NONE;
	heap_description.Properties = heap_properties;
	heap_description.SizeInBytes = p_description.size;

	Microsoft::WRL::ComPtr<ID3D12Heap> temp_resource_heap;
	
	HRESULT result = device->CreateHeap(&heap_description, IID_PPV_ARGS(temp_resource_heap.GetAddressOf()));
	if (FAILED(result)) {
		return RESULT::E_API_CREATE_RESOURCE_HEAP;
	}

	p_resource_heap = new DX12ResourceHeap(p_description, temp_resource_heap);
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::CreateCommandAllocator(const CommandAllocatorDescription& p_description, CommandAllocator*& p_command_allocator) const {

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

RESULT DX12RenderDevice::CreateCommandList(const CommandListDescription& p_description, const CommandAllocator* p_command_allocator, CommandList*& p_command_list) const {
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

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> temp_command_list;
	
	HRESULT result = device->CreateCommandList(0, type, ((DX12CommandAllocator*)p_command_allocator)->GetNative(), pso, IID_PPV_ARGS(temp_command_list.GetAddressOf()));
	if (FAILED(result)) {
		return RESULT::E_API_CREATE_COMMAND_LIST;
	}

	p_command_list = new DX12CommandList(p_description, temp_command_list);
	return RESULT::SUCCESS;
}

RESULT DX12RenderDevice::SubmitCommandList(COMMAND_TYPE p_queue_type, const std::vector<CommandList*>& p_command_lists) const {
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

AllocationInfo DX12RenderDevice::GetAllocationInfo(const TextureDescription& p_description) {
	std::cout << "Texture Alignment: " << std::endl;
	D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;

	CD3DX12_RESOURCE_DESC resource_desc;
	switch (p_description.depth) {
	case 1:
		resource_desc = CD3DX12_RESOURCE_DESC::Tex1D(DX12Converter::GetDX12Type(p_description.format), p_description.width, p_description.height, p_description.mip_levels);
		break;
	case 2:
		resource_desc = CD3DX12_RESOURCE_DESC::Tex2D(DX12Converter::GetDX12Type(p_description.format), p_description.width, p_description.height, p_description.mip_levels);
		break;
	case 3:
		resource_desc = CD3DX12_RESOURCE_DESC::Tex3D(DX12Converter::GetDX12Type(p_description.format), p_description.width, p_description.height, p_description.mip_levels);
		break;
	default:
		break;
	}

	D3D12_RESOURCE_ALLOCATION_INFO info = device->GetResourceAllocationInfo(0, 1, &resource_desc);
	std::cout << info.Alignment << std::endl;
	std::cout << info.SizeInBytes << std::endl;

	std::cout << "------------------------------------------" << std::endl;
	return {};
}

AllocationInfo DX12RenderDevice::GetAllocationInfo(const BufferDescription& p_description) {
	std::cout << "Buffer Alignment: " << std::endl;

	D3D12_RESOURCE_ALLOCATION_INFO info = device->GetResourceAllocationInfo(0, 1, &CD3DX12_RESOURCE_DESC::Buffer(p_description.size));
	std::cout << info.Alignment << std::endl;
	std::cout << info.SizeInBytes << std::endl;

	std::cout << "------------------------------------------" << std::endl;
	return {};
}