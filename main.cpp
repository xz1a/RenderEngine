#include "Platform.h"

#include <iostream>
#include <vector>

#include "DX12RenderDevice.h"

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

	SwapChain* swapchain = nullptr;
	SwapChainDescription swapchain_desc;
	swapchain_desc.refresh_rate = 60;
	swapchain_desc.buffer_count = 2;
	swapchain_desc.width = window_desc.width;
	swapchain_desc.height = window_desc.height;
	swapchain_desc.window = window;
	DEBUG_IF_FAILED(device->CreateSwapChain(swapchain_desc, swapchain));


	Buffer* buffer = nullptr;
	BufferDescription buffer_desc;
	buffer_desc.data = nullptr;
	buffer_desc.size = 1;
	buffer_desc.type = BUFFER_TYPE::DEFAULT;
	DEBUG_IF_FAILED(device->CreateBuffer(buffer_desc, buffer));


	Buffer* upload_buffer = nullptr;
	BufferDescription upload_buffer_desc;
	upload_buffer_desc.data = nullptr;
	upload_buffer_desc.size = 1;
	upload_buffer_desc.type = BUFFER_TYPE::DEFAULT;
	DEBUG_IF_FAILED(device->CreateBuffer(upload_buffer_desc, upload_buffer));



	window->Run();

	delete buffer;
	delete swapchain;
	delete device;
	delete window;
	return 0;
}