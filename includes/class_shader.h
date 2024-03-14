#pragma once

#include <glad/glad.h>
#include "pfm/pfm.hpp"

class Shader {
	private:
		pfm::mat4 _proj_mat;
		pfm::mat4 _view_mat;
		pfm::mat4 _model_mat;

	public:
		GLuint		program;

		Shader();
		~Shader();

		int	LoadShaders(char* const vertex_shader_path, char* const fragment_shader_path);
		int	LoadShaders(char* const vertex_shader_path, char* const geometry_shader_path, char* const fragment_shader_path);

		inline void  SetProjMat(pfm::mat4 const& new_proj_mat)
		{
			_proj_mat = new_proj_mat;
			glUseProgram(program);
			glUniformMatrix4fv(glGetUniformLocation(program, "uProj"), 1, GL_FALSE, &_proj_mat);
			glUseProgram(0);
		}

		inline void  SetViewMat(pfm::mat4 const& new_view_mat)
		{
			_view_mat = new_view_mat;
			glUseProgram(program);
			glUniformMatrix4fv(glGetUniformLocation(program, "uView"), 1, GL_FALSE, &_view_mat);
			glUseProgram(0);
		}

		inline void  SetModelMat(pfm::mat4 const& new_model_mat)
		{
			_model_mat = new_model_mat;
			glUseProgram(program);
			glUniformMatrix4fv(glGetUniformLocation(program, "uModel"), 1, GL_FALSE, &_model_mat);
			glUseProgram(0);

		}

		inline void	SetVec3(char* const uniform_name, pfm::vec3& v)
		{
			glUseProgram(program);
			glUniformMatrix4fv(glGetUniformLocation(program, uniform_name), 1, GL_FALSE, &v);
			glUseProgram(0);
		}

		inline pfm::mat4	GetProjMat() const noexcept
		{
			return _proj_mat;
		}

		inline pfm::mat4	GetViewMat() const noexcept
		{
			return _view_mat;
		}

		inline pfm::mat4	GetModelMat() const noexcept
		{
			return _model_mat;
		}
};