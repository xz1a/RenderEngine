#ifndef _SHADER_H_
#define _SHADER_H_
#include <string>

enum class SHADER_TYPE {
	VERTEX,
	PIXEL,
	COMPUTE
};

typedef struct ShaderDescription {
	SHADER_TYPE type;
	std::string source_file_path;
} ShaderDescription;

class Shader {
public:
	virtual ~Shader() = default;
protected:
	Shader(const ShaderDescription& p_description) : description(p_description) {

	}
	ShaderDescription description;
};
#endif