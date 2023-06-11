#ifndef _RENDER_PASS_H_
#define _RENDER_PASS_H_

class FrameBuffer;

typedef struct Attachment {

} Attachment;

typedef struct RenderPassDescription {
	unsigned int attachment_count;
	const Attachment* attachments;
} RenderPassDescription;

class RenderPass {
public:
	virtual ~RenderPass() = default;
	inline unsigned int GetAttachmentCount() const { return description.attachment_count; }
protected:
	RenderPassDescription description;
};

typedef struct BeginRenderPassDesription {
	RenderPass* render_pass;
	FrameBuffer* frame_buffer;
} BeginRenderPassDescription;
#endif