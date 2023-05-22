#ifndef _COMMAND_ALLOCATOR_H_
#define _COMMAND_ALLOCATOR_H_
#include "CommandType.h"

typedef struct CommandAllocatorDescription {
	COMMAND_TYPE type;
} CommandAllocatorDescription;

class CommandAllocator {
public:
	virtual ~CommandAllocator() = default;
	virtual void Reset() = 0;
protected:
	CommandAllocator(const CommandAllocatorDescription& p_description) : description(p_description) {

	}
	CommandAllocatorDescription description;
};
#endif