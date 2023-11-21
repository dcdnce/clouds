#pragma once

#include "../../setup/setup.hpp"

#include <iostream>

namespace pfm {
    template<typename T>
    struct mat<4, 4, T>
    {
        typedef vec<4, T> col_type;
        typedef vec<4, T> row_type;
        typedef T value_type;
        typedef mat<4, 4, T> type;

        // Data
        col_type value[4];

        // Accesses
        PFM_FUNC_DECL col_type& operator[](length_t i) noexcept;
        PFM_FUNC_DECL col_type const& operator[](length_t i) const noexcept;
        PFM_FUNC_DECL typename col_type::value_type* operator&();

        // Implicit basic constructors
        PFM_FUNC_DECL mat() PFM_DEFAULT;
        PFM_FUNC_DECL mat(mat<4, 4, T> const& m);

        // Explicit basic constructors
        PFM_FUNC_DECL PFM_EXPLICIT mat(T scalar);
		PFM_FUNC_DECL mat(
			T const& x0, T const& y0, T const& z0, T const& w0,
			T const& x1, T const& y1, T const& z1, T const& w1,
			T const& x2, T const& y2, T const& z2, T const& w2,
			T const& x3, T const& y3, T const& z3, T const& w3);
        PFM_FUNC_DECL  mat(
            col_type const& v0,
            col_type const& v1,
            col_type const& v2,
            col_type const& v3);

        // Unary arithmetic operations
		template<typename U>
		PFM_FUNC_DECL mat<4, 4, T> & operator=(mat<4, 4, U> const& m);
		template<typename U>
		PFM_FUNC_DECL mat<4, 4, T> & operator+=(U scalar);
		template<typename U>
		PFM_FUNC_DECL mat<4, 4, T> & operator+=(mat<4, 4, U> const& m);
		template<typename U>
		PFM_FUNC_DECL mat<4, 4, T> & operator-=(U scalar);
		template<typename U>
		PFM_FUNC_DECL mat<4, 4, T> & operator-=(mat<4, 4, U> const& m);
		template<typename U>
		PFM_FUNC_DECL mat<4, 4, T> & operator*=(U scalar);
		template<typename U>
		PFM_FUNC_DECL mat<4, 4, T> & operator*=(mat<4, 4, U> const& m);
		template<typename U>
		PFM_FUNC_DECL mat<4, 4, T> & operator/=(U scalar);
		template<typename U>
		PFM_FUNC_DECL mat<4, 4, T> & operator/=(mat<4, 4, U> const& m);
    };// struct mat<4, 4, T>

    // Binary arithmetic operators
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> operator+(mat<4, 4, T> const& m1, mat<4, 4, T> const& m2);
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> operator+(mat<4, 4, T> const& m, T scalar);
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> operator+(T scalar, mat<4, 4, T> const& m);
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> operator-(mat<4, 4, T> const& m1, mat<4, 4, T> const& m2);
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> operator-(mat<4, 4, T> const& m, T scalar);
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> operator-(T scalar, mat<4, 4, T> const& m);
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> operator*(mat<4, 4, T> const& m1, mat<4, 4, T> const& m2);
    template<typename T>
    PFM_FUNC_DECL typename mat<4, 4, T>::col_type operator*(mat<4, 4, T> const & m, typename mat<4, 4, T>::row_type const& v);
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> operator*(mat<4, 4, T> const& m, T scalar);
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> operator*(T scalar, mat<4, 4, T> const& m);
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> operator/(mat<4, 4, T> const& m1, mat<4, 4, T> const& m2);
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> operator/(mat<4, 4, T> const& m, T scalar);
    template<typename T>
    PFM_FUNC_DECL mat<4, 4, T> operator/(T scalar, mat<4, 4, T> const& m);


    // Boolean operators
	template<typename T>
	PFM_FUNC_DECL bool operator==(mat<4, 4, T> const& m1, mat<4, 4, T> const& m2);
	template<typename T>
	PFM_FUNC_DECL bool operator!=(mat<4, 4, T> const& m1, mat<4, 4, T> const& m2);

    // IO operators
    template<typename T>
    PFM_FUNC_DECL std::ostream& operator<<(std::ostream & o, mat<4, 4, T> const& m);
}

#include "type_mat4x4.inl"