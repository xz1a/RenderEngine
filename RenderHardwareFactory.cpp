#include "RenderHardwareFactory.h"

bool RenderHardwareFactory::CreateRenderDevice(Platform p_platform, const RenderDeviceDescription& p_description, RenderDevice* p_device) {
	switch (p_platform) {
	case Platform::DIRECTX12:
		p_device = new DX12RenderDevice(p_description);
		std::cout << "DX12 Platform Detected." << std::endl;
		return true;
	default:
		return false;
	}
}