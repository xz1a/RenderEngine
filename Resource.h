#ifndef _RESOURCE_H_
#define _RESOURCE_H_
#include "Result.h"
#include "ResourceState.h"

class Resource {
public:
	virtual RESULT Map() = 0;
	virtual RESULT CopyData(unsigned int p_index, unsigned int p_size, const void* data) = 0;
	virtual RESULT UnMap() = 0;
protected:
	Resource() : state(RESOURCE_STATE::COMMON), mapped(false), mapped_data(nullptr) {

	}
	//template<typename T> T GetNative() const = 0;
	BYTE* mapped_data;
	bool mapped;
	RESOURCE_STATE state;
};
#endif