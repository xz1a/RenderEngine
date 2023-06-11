#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_
#include <vector>
#include "RenderTargetView.h"

typedef struct FrameBufferDescription {
	unsigned int width;
	unsigned int height;
	unsigned int attachment_count;
	std::vector<RenderTargetView*> attachments;
} FrameBufferDescription;

class FrameBuffer {
public:
	virtual ~FrameBuffer() = default;
	inline unsigned int GetAttachmentCount() { return description.attachment_count; }
protected:
	FrameBuffer(const FrameBufferDescription& p_description) : description(p_description) {
	
	}
	FrameBufferDescription description;
};
#endif