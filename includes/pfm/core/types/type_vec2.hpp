#pragma once

#include "../../setup/setup.hpp"

#include <iostream>

namespace pfm {
    template<typename T>
    struct vec<2, T>
    {
        // Typedefs
        typedef T value_type;
        typedef vec<2, T> type;

        // Data
        union { T x, r;};
        union { T y, g;};

        // Accesses
        PFM_FUNC_DECL value_type& operator[](length_t i) noexcept;
        PFM_FUNC_DECL value_type const& operator[](length_t i) const noexcept;
        PFM_FUNC_DECL  value_type* operator&();

        // Implicit basic constructors
        PFM_FUNC_DECL vec() PFM_DEFAULT;
        PFM_FUNC_DECL vec(vec<2, T> const& v);

        // Explicit basic constructors
        PFM_FUNC_DECL PFM_EXPLICIT vec(T scalar);
        PFM_FUNC_DECL vec(T x, T y);

        // Unary arithmetic operators
        PFM_FUNC_DECL vec<2, T>& operator=(vec<2, T> const& v) PFM_DEFAULT;
        template<typename U>
        PFM_FUNC_DECL vec<2, T>& operator=(vec<2, U> const& v);
        template<typename U>
        PFM_FUNC_DECL vec<2, T>& operator+=(vec<2, U> const& v);
        template<typename U>
        PFM_FUNC_DECL vec<2, T>& operator+=(U scalar);
        template<typename U>
        PFM_FUNC_DECL vec<2, T>& operator-=(vec<2, U> const& v);
        template<typename U>
        PFM_FUNC_DECL vec<2, T>& operator-=(U scalar);
        template<typename U>
        PFM_FUNC_DECL vec<2, T>& operator*=(vec<2, U> const& v);
        template<typename U>
        PFM_FUNC_DECL vec<2, T>& operator*=(U scalar);
        template<typename U>
        PFM_FUNC_DECL vec<2, T>& operator/=(vec<2, U> const& v);
        template<typename U>
        PFM_FUNC_DECL vec<2, T>& operator/=(U scalar);


    };// struct vec<3, T>

    // Binary arithmetic operators
    template<typename T>
    PFM_FUNC_DECL  vec<2, T> operator+(vec<2, T> const& v1, vec<2, T> const& v2);
    template<typename T>
    PFM_FUNC_DECL  vec<2, T> operator+(T scalar, vec<2, T> const& v);
    template<typename T>
    PFM_FUNC_DECL  vec<2, T> operator+(vec<2, T> const& v, T scalar);
    template<typename T>
    PFM_FUNC_DECL  vec<2, T> operator-(vec<2, T> const& v1, vec<2, T> const& v2);
    template<typename T>
    PFM_FUNC_DECL  vec<2, T> operator-(T scalar, vec<2, T> const& v);
    template<typename T>
    PFM_FUNC_DECL  vec<2, T> operator-(vec<2, T> const& v, T scalar);
    template<typename T>
    PFM_FUNC_DECL  vec<2, T> operator*(vec<2, T> const& v1, vec<2, T> const& v2);
    template<typename T>
    PFM_FUNC_DECL  vec<2, T> operator*(T scalar, vec<2, T> const& v);
    template<typename T>
    PFM_FUNC_DECL  vec<2, T> operator*(vec<2, T> const& v, T scalar);
    template<typename T>
    PFM_FUNC_DECL  vec<2, T> operator/(vec<2, T> const& v1, vec<2, T> const& v2);
    template<typename T>
    PFM_FUNC_DECL  vec<2, T> operator/(T scalar, vec<2, T> const& v);
    template<typename T>
    PFM_FUNC_DECL  vec<2, T> operator/(vec<2, T> const& v, T scalar);

    // Boolean operators
    template<typename T>
    PFM_FUNC_DECL  bool operator==(vec<2, T> const& v1, vec<2, T> const& v2);
    template<typename T>
    PFM_FUNC_DECL  bool operator!=(vec<2, T> const& v1, vec<2, T> const& v2);

    // IO operators
    template<typename T>
    PFM_FUNC_DECL std::ostream& operator<<(std::ostream & o, vec<2, T> const& v);


}//namespace pfm

#include "type_vec2.inl"