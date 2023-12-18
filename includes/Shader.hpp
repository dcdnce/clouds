#pragma once

#include <glad/glad.h>
#include "pfm/pfm.hpp"

class Shader {
    private:
        pfm::mat4 _projMat;
        pfm::mat4 _viewMat;
        pfm::mat4 _modelMat;

    public:
        GLuint		program;

        Shader();
        ~Shader();

        int	loadShaders(char * const vertexShaderPath, char * const fragmentShaderPath);

        void  setVec3(char * const uniformName, pfm::vec3 & v);
        void  setProjMat(pfm::mat4 const& newProjMat);
        void  setViewMat(pfm::mat4 const& newViewMat);
        void  setModelMat(pfm::mat4 const& newModelMat);
        pfm::mat4 getProjMat() const;
        pfm::mat4 getViewMat() const;
        pfm::mat4 getModelMat() const;
};