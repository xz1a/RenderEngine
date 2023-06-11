#ifndef _PIPELINE_STATE_H_
#define _PIPELINE_STATE_H_
#include <vector>
#include "Shader.h"
#include "RenderPass.h"
#include "InputLayout.h"

typedef struct PipelineStateDescription {
	Shader* vertex_shader;
	Shader* pixel_shader;
	RenderPass* render_pass;
	InputLayout* input_layout;
} PipelineStateDescription;

class PipelineState {
public:
	virtual ~PipelineState() = default;
protected:
	PipelineState(const PipelineStateDescription& p_description) : description(p_description) {

	}
	PipelineStateDescription description;
};
#endif