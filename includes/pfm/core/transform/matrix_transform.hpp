#pragma once

#include "../types/type_vec3.hpp"
#include "../types/type_mat4x4.hpp"
#include "vector_transform.hpp"

namespace pfm {
    // https://github.com/g-truc/glm/
    // https://github.com/raysan5/raylib
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> translate(mat<4, 4, T> const& m, vec<3, T> const& v);

    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> lookAt(vec<3, T> const& eye, vec<3, T> const& target, vec<3, T> const& up);

    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> lookRotation(vec<3, T> const& forward, vec<3, T> const& up = vec3(0.f, 1.f, 0.f));

    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> perspective(T const& fovy, T const& ar,  T const& znear, T const& zfar);

    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> rotate(mat<4, 4, T> const& m, T const& a, vec<3, T> const& v);
}

#include "matrix_transform.inl"