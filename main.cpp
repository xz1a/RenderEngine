#include "Platform.h"

#include <iostream>
#include <vector>

#include "DX12RenderDevice.h"

#include <array>

int main() {
	WindowDescription window_desc;
	window_desc.title = "Render Engine";
	window_desc.width = 1280;
	window_desc.height = 720;

	Window* window;

	DEBUG_IF_FAILED(CreatePlatformWindow(window_desc, window));
	window->Initialize();

	RenderDeviceDescription device_desc;
	device_desc.adapter = nullptr;
	RenderDevice* device = new DX12RenderDevice(device_desc);

	//Create Test Cases
	DescriptorHeap* descriptor_heap_rtv;
	DescriptorHeapDescription descriptor_heap_desc_rtv;
	descriptor_heap_desc_rtv.descriptor_count = 2;
	descriptor_heap_desc_rtv.type = DESCRIPTOR_HEAP_TYPE::RTV;
	DEBUG_IF_FAILED(device->CreateDescriptorHeap(descriptor_heap_desc_rtv, descriptor_heap_rtv));

	DescriptorHeap* descriptor_heap_dsv;
	DescriptorHeapDescription descriptor_heap_desc_dsv;
	descriptor_heap_desc_dsv.descriptor_count = 1;
	descriptor_heap_desc_dsv.type = DESCRIPTOR_HEAP_TYPE::DSV;
	DEBUG_IF_FAILED(device->CreateDescriptorHeap(descriptor_heap_desc_dsv, descriptor_heap_dsv));

	SwapChain* swapchain = nullptr;
	SwapChainDescription swapchain_desc;
	swapchain_desc.refresh_rate = 60;
	swapchain_desc.buffer_count = 2;
	swapchain_desc.width = window_desc.width;
	swapchain_desc.height = window_desc.height;
	swapchain_desc.window = window;
	swapchain_desc.format = FORMAT_TYPE::R8G8B8A8_UNORM;
	DEBUG_IF_FAILED(device->CreateSwapChain(swapchain_desc, descriptor_heap_rtv, swapchain));


	CommandAllocator* allocator;
	CommandAllocatorDescription allocator_desc;
	allocator_desc.type = COMMAND_TYPE::GRAPHICS;
	DEBUG_IF_FAILED(device->CreateCommandAllocator(allocator_desc, allocator));

	CommandList* list;
	CommandListDescription list_desc;
	list_desc.type = COMMAND_TYPE::GRAPHICS;
	DEBUG_IF_FAILED(device->CreateCommandList(list_desc, allocator, list));


	std::vector <FrameBuffer*> swapchain_frame_buffers = std::vector<FrameBuffer*>(swapchain_desc.buffer_count);

	for (unsigned int i = 0; i < swapchain_desc.buffer_count; ++i) {
		FrameBufferDescription fb_desc;
		fb_desc.attachment_count = 1;
		fb_desc.width = window_desc.width;
		fb_desc.height = window_desc.height;
		fb_desc.attachments = { swapchain->GetRenderTargetViews()[i] };
		DEBUG_IF_FAILED(device->CreateFrameBuffer(fb_desc, swapchain_frame_buffers[i]));
	}
	

	allocator->Reset();
	list->Reset(allocator, nullptr);
	list->SetTextureBarrier(TextureBarrier(swapchain->GetCurrentBackBuffer(), RESOURCE_STATE::PRESENT, RESOURCE_STATE::RENDER_TARGET)); //set texture
	list->SetViewport({ 0.0f, 0.0f, (float)window->GetWidth(), (float)window->GetHeight(), 0.0f, 1.0f });
	BeginRenderPassDesription rpd;
	rpd.frame_buffer = swapchain_frame_buffers[swapchain->GetCurrentBufferIndex()];
	rpd.render_pass = nullptr;
	list->BeginRenderPass(rpd, nullptr);
	list->EndRenderRPass();
	list->SetTextureBarrier(TextureBarrier(swapchain->GetCurrentBackBuffer(), RESOURCE_STATE::RENDER_TARGET, RESOURCE_STATE::PRESENT)); //set texture
	list->Close();
	device->SubmitCommandList(COMMAND_TYPE::GRAPHICS, { list });
	UINT64 fence = 0;
	device->SignalQueue(COMMAND_TYPE::GRAPHICS, ++fence);
	device->WaitForQueueCPU(COMMAND_TYPE::GRAPHICS, COMMAND_TYPE::GRAPHICS, fence);
	swapchain->Present();

	

	struct Vertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	std::array<Vertex, 8> vertices = {
		Vertex { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::White) },
		Vertex { DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Black) },
		Vertex { DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Red) },
		Vertex { DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Green) },
		Vertex { DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Blue) },
		Vertex { DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Yellow) },
		Vertex { DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Cyan) },
		Vertex { DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Magenta) }
	};

	std::array<std::uint16_t, 36> indices = {
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 6, 5,
		4, 7, 6,
		// left face
		4, 5, 1,
		4, 1, 0,
		// right face
		3, 2, 6,
		3, 6, 7,
		// top face
		1, 5, 6,
		1, 6, 2,
		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	/*D3D12_INPUT_ELEMENT_DESC input_layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};*/

	InputElementDescription element_descriptions[] = {
		{"POSITION", FORMAT_TYPE::R32G32B32_FLOAT},
		{"COLOR", FORMAT_TYPE::R32G32B32A32_FLOAT}
	};

	InputLayout input_layout = {
		element_descriptions,
		2
	};

	Shader* vs_shader;
	ShaderDescription vs_shader_desc;
	vs_shader_desc.source_file_path = "./shaders/basic.vs";
	vs_shader_desc.type = SHADER_TYPE::VERTEX;
	DEBUG_IF_FAILED(device->CreateShader(vs_shader_desc, vs_shader));

	Shader* fs_shader;
	ShaderDescription fs_shader_desc;
	fs_shader_desc.source_file_path = "./shaders/basic.fs";
	fs_shader_desc.type = SHADER_TYPE::PIXEL;
	DEBUG_IF_FAILED(device->CreateShader(fs_shader_desc, fs_shader));

	PipelineStateDescription pso_desc;
	pso_desc.input_layout = &input_layout;
	pso_desc.vertex_shader = vs_shader;
	pso_desc.pixel_shader = fs_shader;
	pso_desc.render_pass = nullptr;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
	
	DescriptorHeapDescription gpu_heap_desc;
	gpu_heap_desc.descriptor_count = 10000;
	gpu_heap_desc.type = DESCRIPTOR_HEAP_TYPE::SAMPLER;
	DescriptorHeap* gpu_heap;
	DEBUG_IF_FAILED(device->CreateDescriptorHeap(gpu_heap_desc, gpu_heap));


	window->Run();
	
	delete swapchain;
	delete descriptor_heap_rtv;
	delete descriptor_heap_dsv;
	delete list;
	delete allocator;
	delete device;
	delete window;
	return 0;
}