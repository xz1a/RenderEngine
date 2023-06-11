#ifndef _INPUT_LAYOUT_H_
#define _INPUT_LAYOUT_H_
#include "FormatType.h"

typedef struct InputElementDescription{
	char* name;
	FORMAT_TYPE format;
} InputElementDescription;

typedef struct InputLayout {
	const InputElementDescription* elements;
	unsigned int element_count;
} InputLayout;
#endif