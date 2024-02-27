#ifndef _SHADER_HANDLER_HPP_
#define _SHADER_HANDLER_HPP_

#include <string>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int ID;

	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);

	void use();
	
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

};

#endif