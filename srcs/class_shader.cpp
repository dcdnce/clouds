#include "class_shader.h"
#include "class_logger.h"
#include <iostream>
#include <fstream>

static int	CreateShader(GLuint *shader_ref, GLenum type, const char *path);

Shader::Shader() {}


Shader::~Shader()
{
    glDeleteProgram(program);
}


int	Shader::LoadShaders(char * const vertex_shader_path, char * const fragment_shader_path)
{
	int			ret_value = 1;
	char		infoLog[512];
	GLint		status;
	GLuint		vertex_shader_ref;
	GLuint		fragment_shader_ref;

	if (!CreateShader(&vertex_shader_ref, GL_VERTEX_SHADER, vertex_shader_path))
		return (0);
	if (!CreateShader(&fragment_shader_ref, GL_FRAGMENT_SHADER, fragment_shader_path))
		return (0);
	program = glCreateProgram();
	glAttachShader(program, vertex_shader_ref);
	glAttachShader(program, fragment_shader_ref);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		Logger::error(true) << "Shader class linking error" << std::endl;
		Logger::error(false) << infoLog << std::endl;
		ret_value = 0;
	}
	glDeleteShader(vertex_shader_ref);
	glDeleteShader(fragment_shader_ref);
	return (ret_value);
}

static int	CreateShader(GLuint *shader_ref, GLenum type, const char *path)
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
	*shader_ref = glCreateShader(type);	
	const GLchar	*source = src.c_str();
	glShaderSource(*shader_ref, 1, &source, NULL); 
	glCompileShader(*shader_ref);
	glGetShaderiv(*shader_ref, GL_COMPILE_STATUS, &status);
	file.close();
	if (!status)
	{
		glGetShaderInfoLog(*shader_ref, 512, NULL, infoLog);
		Logger::error(true) << "Shader class compile error" << std::endl;
		Logger::error(false) << infoLog << std::endl;
		return (0);
	}
	return (1);
}

void  Shader::SetProjMat(pfm::mat4 const& new_proj_mat)
{
	_proj_mat = new_proj_mat;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "uProj"), 1, GL_FALSE, &_proj_mat);
	glUseProgram(0);
}

void  Shader::SetViewMat(pfm::mat4 const& new_view_mat)
{
	_view_mat = new_view_mat;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "uView"), 1, GL_FALSE, &_view_mat);
	glUseProgram(0);
}

void  Shader::SetModelMat(pfm::mat4 const& new_model_mat)
{
	_model_mat = new_model_mat;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "uModel"), 1, GL_FALSE, &_model_mat);
	glUseProgram(0);

}

void	Shader::SetVec3(char * const uniform_name, pfm::vec3 & v)	
{
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, uniform_name), 1, GL_FALSE, &v);
	glUseProgram(0);
}

pfm::mat4	Shader::GetProjMat() const
{
	return _proj_mat;
}

pfm::mat4	Shader::GetViewMat() const
{
	return _view_mat;
}

pfm::mat4	Shader::GetModelMat() const
{
	return _model_mat;
}