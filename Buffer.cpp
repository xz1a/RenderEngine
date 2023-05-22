#include "Buffer.h"

Buffer::Buffer(const BufferDescription& p_description) : description(p_description), is_mapped(false), mapped_data(nullptr) {

}