#include <iostream>
#include <GL/glew.h>
#include "shader_handler.hpp"
#include "shaderIO.h"

Shader::Shader() : ID(0) { }

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLchar* shaderSrc;
	shaderSrc = GetShaderSource(vertexPath);

	glShaderSource(vertexShader, 1, &shaderSrc, NULL);
	glCompileShader(vertexShader);

	shaderSrc = GetShaderSource(fragmentPath);

	glShaderSource(fragmentShader, 1, &shaderSrc, NULL);
	glCompileShader(fragmentShader);

	// CHECK SHADER SUCCESS
	{
		int vecSuccess;
		int fragSuccess;

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vecSuccess);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);

		if (!vecSuccess)
		{
			std::cout << "Vertex Shader failed to compile" << std::endl;
			char why[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, why);
			std::cout << why << std::endl;
		}

		if (!fragSuccess)
		{
			std::cout << "Fragment Shader failed to compile" << std::endl;
			char why[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, why);
			std::cout << why << std::endl;
		}

	}

	ClearShaderSourceBuffer();

	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	ID = shaderProgram;
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}