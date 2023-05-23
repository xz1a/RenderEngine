#ifndef _PIPELINE_STATE_H_
#define _PIPELINE_STATE_H_

typedef struct PipelineStateDescription {

} PipelineStateDescription;

class PipelineState {
public:
	virtual ~PipelineState() = default;
private:
	PipelineStateDescription description;
};
#endif