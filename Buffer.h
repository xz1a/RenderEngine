#ifndef _BUFFER_H_
#define _BUFFER_H_
#include "Result.h"
#include <iostream>

enum BUFFER_TYPE {
	DEFAULT,
	UPLOAD,
	READBACK
};

typedef struct BufferDescription {
	void* data;
	unsigned int size;
	BUFFER_TYPE type;
} BufferDescription;

class Buffer {
public:
	virtual ~Buffer() = default;
	virtual RESULT Map() = 0;
	virtual RESULT CopyData(unsigned int p_index, unsigned int p_size, const void* data) = 0;
	virtual RESULT UnMap() = 0;
protected:
	Buffer(const BufferDescription& p_description);
	bool is_mapped;
	void* mapped_data;
	BufferDescription description;
};
#endif