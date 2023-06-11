#ifndef _DESCRIPTOR_HEAP_H_
#define _DESCRIPTOR_HEAP_H_

enum class DESCRIPTOR_HEAP_TYPE {
	CBV_SRV_UAV,
	SAMPLER,
	RTV,
	DSV,
};

typedef struct DescriptorHeapDescription {
	unsigned long long descriptor_count;
	DESCRIPTOR_HEAP_TYPE type;
} DescriptorHeapDescription;

class DescriptorHeap {
public:
	virtual ~DescriptorHeap() = default;
protected:
	DescriptorHeap(const DescriptorHeapDescription& p_description) : description(p_description) {

	}
	DescriptorHeapDescription description;
};
#endif