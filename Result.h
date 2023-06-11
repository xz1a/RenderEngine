#ifndef _RESULT_H_
#define _RESULT_H_
#include <iostream>

enum class RESULT {
	SUCCESS,
	E_API_CREATE_SWAP_CHAIN,
	E_API_CREATE_BUFFER,
	E_API_CREATE_TEXTURE,
	E_API_CREATE_SHADER,
	E_API_CREATE_PIPELINE_STATE,
	E_API_CREATE_RENDER_PASS,
	E_API_CREATE_DESCRIPTOR_HEAP,
	E_API_CREATE_RESOURCE_HEAP,
	E_API_CREATE_COMMAND_ALLOCATOR,
	E_API_CREATE_COMMAND_LIST,
	E_API_FUNCTION,
	E_COMMAND_LIST_SUBMISSION,
	E_QUEUE_SIGNAL,
	E_RESOURCE_MAP_FAILED,
};

#ifndef DEBUG_IF_FAILED
#define DEBUG_IF_FAILED(x)																\
{																						\
	RESULT result = (x);																\
	if (result != RESULT::SUCCESS) {													\
		switch (result) {																\
			case RESULT::E_API_CREATE_SWAP_CHAIN:										\
				std::cout << "<ERROR> Create Swap Chain Failed." << std::endl;			\
			break;																		\
			case RESULT::E_API_CREATE_BUFFER:											\
				std::cout << "<ERROR> Create Buffer Failed." << std::endl;				\
			break;																		\
			case RESULT::E_API_CREATE_TEXTURE:											\
				std::cout << "<ERROR> Create Texture Failed." << std::endl;				\
			break;																		\
			case RESULT::E_API_CREATE_SHADER:											\
				std::cout << "<ERROR> Create Shader Failed." << std::endl;				\
			break;																		\
			case RESULT::E_API_CREATE_PIPELINE_STATE:									\
				std::cout << "<ERROR> Create Pipeline State Failed." << std::endl;		\
			break;																		\
			case RESULT::E_API_CREATE_RENDER_PASS:										\
				std::cout << "<ERROR> Create Render Pass Failed." << std::endl;			\
			break;																		\
			case RESULT::E_API_CREATE_DESCRIPTOR_HEAP:									\
				std::cout << "<ERROR> Create Descriptor Heap Failed." << std::endl;		\
			break;																		\
			case RESULT::E_API_CREATE_RESOURCE_HEAP:									\
				std::cout << "<ERROR> Create Resource Heap Failed." << std::endl;		\
			break;																		\
			default:																	\
				std::cout << "<ERROR> Debug Not Implemented." << std::endl;				\
			break;																		\
		}																				\
	}																					\
}							
#endif
#endif