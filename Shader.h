#ifndef _SHADER_H_
#define _SHADER_H_

typedef struct ShaderDescription {

} ShaderDescription;

class Shader {
public:
	virtual ~Shader() = default;
private:
	ShaderDescription description;
};
#endif