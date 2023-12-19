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

		void  SetVec3(char* const uniform_name, pfm::vec3& v);
		void  SetProjMat(pfm::mat4 const& new_proj_mat);
		void  SetViewMat(pfm::mat4 const& new_view_mat);
		void  SetModelMat(pfm::mat4 const& new_model_mat);
		pfm::mat4 GetProjMat() const noexcept;
		pfm::mat4 GetViewMat() const noexcept;
		pfm::mat4 GetModelMat() const noexcept;
};