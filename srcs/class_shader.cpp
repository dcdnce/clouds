#include "class_shader.h"
#include "class_logger.h"
#include <iostream>
#include <fstream>

static int	createShader(GLuint *shaderRef, GLenum type, const char *path);

Shader::Shader() {}


Shader::~Shader()
{
    glDeleteProgram(program);
}


int	Shader::loadShaders(char * const vertexShaderPath, char * const fragmentShaderPath)
{
	int			ret_value = 1;
	char		infoLog[512];
	GLint		status;
	GLuint		vertexShaderRef;
	GLuint		fragmentShaderRef;

	if (!createShader(&vertexShaderRef, GL_VERTEX_SHADER, vertexShaderPath))
		return (0);
	if (!createShader(&fragmentShaderRef, GL_FRAGMENT_SHADER, fragmentShaderPath))
		return (0);
	program = glCreateProgram();
	glAttachShader(program, vertexShaderRef);
	glAttachShader(program, fragmentShaderRef);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		Logger::error(true) << "Shader class linking error" << std::endl;
		Logger::error(false) << infoLog << std::endl;
		ret_value = 0;
	}
	glDeleteShader(vertexShaderRef);
	glDeleteShader(fragmentShaderRef);
	return (ret_value);
}

static int	createShader(GLuint *shaderRef, GLenum type, const char *path)
{
	GLint			status;
	char			infoLog[512];
	std::string		src = "";
	std::string		tmp = "";
	std::ifstream	file;

	// Vertex Shader
	file.open(path);
	if(file.is_open())
		while (std::getline(file, tmp))
			src += tmp + "\n";
	else
	{
		Logger::error(true) << "Shader class couldn't open shader file" << std::endl;
		return (0);	
	}
	*shaderRef = glCreateShader(type);	
	const GLchar	*source = src.c_str();
	glShaderSource(*shaderRef, 1, &source, NULL); 
	glCompileShader(*shaderRef);
	glGetShaderiv(*shaderRef, GL_COMPILE_STATUS, &status);
	file.close();
	if (!status)
	{
		glGetShaderInfoLog(*shaderRef, 512, NULL, infoLog);
		Logger::error(true) << "Shader class compile error" << std::endl;
		Logger::error(false) << infoLog << std::endl;
		return (0);
	}
	return (1);
}

void  Shader::setProjMat(pfm::mat4 const& newProjMat)
{
	_projMat = newProjMat;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "uProj"), 1, GL_FALSE, &_projMat);
	glUseProgram(0);
}

void  Shader::setViewMat(pfm::mat4 const& newViewMat)
{
	_viewMat = newViewMat;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "uView"), 1, GL_FALSE, &_viewMat);
	glUseProgram(0);
}

void  Shader::setModelMat(pfm::mat4 const& newModelMat)
{
	_modelMat = newModelMat;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "uModel"), 1, GL_FALSE, &_modelMat);
	glUseProgram(0);

}

void	Shader::setVec3(char * const uniformName, pfm::vec3 & v)	
{
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, uniformName), 1, GL_FALSE, &v);
	glUseProgram(0);
}

pfm::mat4	Shader::getProjMat() const
{
	return _projMat;
}

pfm::mat4	Shader::getViewMat() const
{
	return _viewMat;
}

pfm::mat4	Shader::getModelMat() const
{
	return _modelMat;
}