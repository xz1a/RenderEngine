#ifndef _RENDER_DEVICE_H_
#define _RENDER_DEVICE_H_

class RenderDevice {
public:
	virtual void CreateBuffer() = 0;
	virtual void CreateTexture() = 0;
	virtual void CreateShader() = 0;
	virtual void CreatePipelineState() = 0;
};
#endif