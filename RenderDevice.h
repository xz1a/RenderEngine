#ifndef _RENDER_DEVICE_H_
#define _RENDER_DEVICE_H_
#include "SwapChain.h"
#include "Buffer.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "PipelineState.h"
#include "RenderPass.h"
#include "DescriptorHeap.h"
#include "ResourceHeap.h"
#include "CommandAllocator.h"
#include "CommandList.h"
#include <memory>
#include <vector>

typedef struct RenderDeviceDescription {
	bool debug;
	int* adapter;
};

typedef struct AllocationInfo {
	unsigned long long alignment;
	unsigned long long size;
} AllocationInfo;

class RenderDevice {
public:
	enum PRIMITIVE_TYPE {
		UNDEFINED,
		POINTLIST,
		LINELIST,
		LINESTRIP,
		TRIANGLELIST,
		TRIANGLESTRIP,
	};

	virtual ~RenderDevice() = default;
	virtual RESULT CreateSwapChain(const SwapChainDescription& p_description, DescriptorHeap* p_descriptor_heap, SwapChain*& p_swapchain) const = 0;
	virtual RESULT CreateBuffer(const BufferDescription& p_description, UINT64 offset, const ResourceHeap* p_resource_heap, Buffer*& p_buffer) const = 0;
	virtual RESULT CreateTexture(const TextureDescription& p_description, UINT64 offset,const ResourceHeap* p_resource_heap, Texture*& p_texture) const = 0;
	virtual RESULT CreateFrameBuffer(const FrameBufferDescription& p_description, FrameBuffer*& p_framebuffer) const = 0;
	virtual RESULT CreateShader(const ShaderDescription& p_description, Shader*& p_shader) const = 0;
	virtual RESULT CreatePipelineState(const PipelineStateDescription& p_description, PipelineState*& p_pipline_state) const = 0;
	virtual RESULT CreateRenderPass(const RenderPassDescription& p_description, RenderPass*& p_render_pass) const = 0;
	virtual RESULT CreateDescriptorHeap(const DescriptorHeapDescription& p_description, DescriptorHeap*& p_descriptor_heap) const = 0;
	virtual RESULT CreateResourceHeap(const ResourceHeapDescription& p_description, ResourceHeap*& p_resource_heap) const = 0;
	virtual RESULT CreateCommandAllocator(const CommandAllocatorDescription& p_description, CommandAllocator*& p_command_allocator) const = 0;
	virtual RESULT CreateCommandList(const CommandListDescription& p_description, const CommandAllocator* p_command_allocator, CommandList*& p_command_list) const = 0;
	virtual RESULT SubmitCommandList(COMMAND_TYPE p_queue_type, const std::vector<CommandList*>& p_command_lists) const = 0;
	virtual RESULT SignalQueue(COMMAND_TYPE p_queue_type, UINT64 p_fence_value) const = 0;
	virtual RESULT WaitForQueueCPU(COMMAND_TYPE p_queue_type, COMMAND_TYPE p_wait_for_queue_type, UINT64 p_fence_value) = 0;
	virtual AllocationInfo GetAllocationInfo(const TextureDescription& p_description) = 0;
	virtual AllocationInfo GetAllocationInfo(const BufferDescription& p_description) = 0;
protected:
	RenderDeviceDescription description;
};
#endif