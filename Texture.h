#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_
#include "Resource.h"
#include "FormatType.h"

typedef struct TextureDescription {
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	unsigned int mip_levels;
	FORMAT_TYPE format;
} TextureDescription;

class Texture : public Resource {
public:
	virtual ~Texture() = default;
protected:
	Texture(const TextureDescription& p_description) : Resource(), description(p_description) {

	}
	TextureDescription description;
};

typedef struct TextureBarrier {
	TextureBarrier(Texture* p_texture, RESOURCE_STATE p_start_state, RESOURCE_STATE p_end_state) : texture(p_texture), start_state(p_start_state), end_state(p_end_state) {

	}
	RESOURCE_STATE start_state;
	RESOURCE_STATE end_state;
	Texture* texture;
} TextureBarrier;
#endif