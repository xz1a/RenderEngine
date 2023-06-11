#ifndef _RESOURCE_HEAP_H_
#define _RESOURCE_HEAP_H_
#define _256_MB_ 268435456


enum class RESOURCE_HEAP_TYPE {
	DEFAULT,
	UPLOAD,
	READBACK,
};

typedef struct ResourceHeapDescription {
	RESOURCE_HEAP_TYPE type;
	unsigned long long size;
} ResourceHeapDescription;

class ResourceHeap {
public:
	virtual ~ResourceHeap() = default;
protected:
	ResourceHeap(const ResourceHeapDescription& p_description) : description(p_description) {

	}
	ResourceHeapDescription description;
};
#endif