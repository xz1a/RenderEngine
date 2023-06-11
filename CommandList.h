#ifndef _COMMAND_LIST_H_
#define _COMMAND_LIST_H_
#include "dx12.h" //remove after
#include "CommandType.h"
#include "FrameBuffer.h"
#include "RenderPass.h"
#include "Buffer.h"
#include "Texture.h"
#include "FormatType.h"

class CommandAllocator;
class PipelineState;
class Buffer;
class Texture;


struct Viewport {
	float top;
	float left;
	float width;
	float height;
	float min_depth;
	float max_depth;
};

typedef struct CommandListDescription {
	COMMAND_TYPE type;
};

class CommandList {
public:
	virtual ~CommandList() = default;

	virtual void Reset(const CommandAllocator* p_allocator, const PipelineState* p_pipeline_state) = 0;
	virtual void Close() = 0;

	virtual void CopyBuffer(const Buffer* p_src_buffer, unsigned int p_src_offset, unsigned int p_src_size, const Buffer* p_dest_buffer, unsigned int p_dest_offset, unsigned int p_dest_size) = 0;
	virtual void CopyTexture(const Texture* p_src_texture, const Texture* p_dest_texture) = 0;
	virtual void SetBufferBarrier(const BufferBarrier& p_barrier) = 0;
	virtual void SetTextureBarrier(const TextureBarrier& p_barrier) = 0;

	virtual void BeginRenderPass(const BeginRenderPassDescription& p_begin_render_pass_description, float p_clear_value[4]) = 0;
	virtual void EndRenderRPass() = 0;

	virtual void SetPipelineState(const PipelineState* p_pipeline_state) = 0;
	virtual void SetViewport(const Viewport& viewport) = 0;
	virtual void SetVertexBuffer(const Buffer* p_buffer) = 0;
	virtual void SetIndexBuffer(const Buffer* p_buffer, FORMAT_TYPE p_type) = 0;

	virtual void ClearRenderTarget() = 0;
	virtual void ClearDepthAndStencil() = 0;
	
	virtual void DrawInstanced() = 0;
	virtual void DrawIndexInstanced() = 0;
	
protected:
	CommandList(const CommandListDescription& p_description) : description(p_description), closed(false) {
	
	};
	CommandListDescription description;
	bool closed;
};
#endif