#ifndef _BUFFER_H_
#define _BUFFER_H_
#include "Resource.h"
#include <iostream>
#include <memory>

enum BUFFER_TYPE {
	DEFAULT,
	UPLOAD,
	READBACK
};

typedef struct BufferDescription {
	void* data;
	unsigned long long size;
	BUFFER_TYPE type;
} BufferDescription;

class Buffer : public Resource {
public:
	virtual ~Buffer() = default;
protected:
	Buffer(const BufferDescription& p_description) : Resource(), description(p_description) {

	}
	BufferDescription description;
};

typedef struct BufferBarrier {
	BufferBarrier(Buffer* p_buffer, RESOURCE_STATE p_start_state, RESOURCE_STATE p_end_state) : buffer(p_buffer), start_state(p_start_state), end_state(p_end_state) {

	}
	RESOURCE_STATE start_state;
	RESOURCE_STATE end_state;
	Buffer* buffer;
} BufferBarrier;

typedef std::shared_ptr<Buffer> BufferPtr;
#endif