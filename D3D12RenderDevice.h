#ifndef _D3D12_RENDER_DEVICE_H_
#define _D3D12_RENDER_DEVICE_H_

#include "RenderDevice.h"

class D3D12RenderDevice : public RenderDevice {
public:
	virtual void CreateBuffer() override;
	virtual void CreateTexture() override;
	virtual void CreateShader() override;
	virtual void CreatePipelineState() override;
};

#endif