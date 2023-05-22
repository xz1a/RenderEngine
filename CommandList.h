#ifndef _COMMAND_LIST_H_
#define _COMMAND_LIST_H_
#include "CommandType.h"

class CommandAllocator;
class PipelineState;

typedef struct CommandListDescription {
	COMMAND_TYPE type;
	CommandAllocator* allocator;
	PipelineState* pipeline_state;
};

class CommandList {
public:
	virtual ~CommandList() = default;
protected:
	CommandList(const CommandListDescription& p_description) : description(p_description){
	
	};
	CommandListDescription description;
};
#endif