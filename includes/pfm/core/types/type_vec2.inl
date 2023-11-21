namespace pfm {
    // Implicit basic constructors
    template<typename T>
    PFM_INLINE  vec<2, T>::vec(vec<2, T> const& v)
        : x(v.x), y(v.y) 
    {}

    // Explicit basic constructors
    template<typename T>
    PFM_INLINE   vec<2,T>::vec(T scalar)
        : x(scalar), y(scalar)
    {}
    template<typename T>
    PFM_INLINE  vec<2, T>::vec(T x, T y)
        : x(x), y(y)
    {}

    // Accesses
    template<typename T>
    PFM_INLINE typename vec<2, T>::value_type& vec<2, T>::operator[](length_t i) noexcept
    {
        assert(i >= 0 && i < 2);
        switch(i)
		{
            case 0:
                return x;
            case 1:
                return y;
            default:
                return x;
		}
    }

    template<typename T>
    PFM_INLINE  typename vec<2, T>::value_type const& vec<2, T>::operator[](length_t i) const noexcept
    {
        assert(i >= 0 && i < 2);
        switch(i)
		{
            case 0:
                return x;
            case 1:
                return y;
            default:
                return x;
		}
    }

    template<typename T>
    PFM_INLINE  typename vec<2, T>::value_type* vec<2, T>::operator&()
    {
        return &x;
    }



    // Unary arithmetic operators
    template<typename T> //struct template
    template<typename U> //extern template
    PFM_INLINE  vec<2, T>& vec<2, T>::operator=(vec<2, U> const& v)
    {
        this->x = static_cast<T>(v.x);
        this->y = static_cast<T>(v.y);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<2, T>& vec<2, T>::operator+=(vec<2, U> const& v)
    {
        this->x += static_cast<T>(v.x);
        this->y += static_cast<T>(v.y);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<2, T>& vec<2, T>::operator+=(U scalar)
    {
        this->x += static_cast<T>(scalar);
        this->y += static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<2, T>& vec<2, T>::operator-=(vec<2, U> const& v)
    {
        this->x -= static_cast<T>(v.x);
        this->y -= static_cast<T>(v.y);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<2, T>& vec<2, T>::operator-=(U scalar)
    {
        this->x -= static_cast<T>(scalar);
        this->y -= static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<2, T>& vec<2, T>::operator*=(vec<2, U> const& v)
    {
        this->x *= static_cast<T>(v.x);
        this->y *= static_cast<T>(v.y);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<2, T>& vec<2, T>::operator*=(U scalar)
    {
        this->x *= static_cast<T>(scalar);
        this->y *= static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<2, T>& vec<2, T>::operator/=(vec<2, U> const& v)
    {
        this->x /= static_cast<T>(v.x);
        this->y /= static_cast<T>(v.y);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<2, T>& vec<2, T>::operator/=(U scalar)
    {
        this->x /= static_cast<T>(scalar);
        this->y /= static_cast<T>(scalar);
        return *this;
    }

    // Binary arithmetic operators
    template<typename T>
    PFM_INLINE  vec<2, T> operator+(vec<2, T> const& v1, vec<2, T> const& v2)
    {
        return vec<2, T>
            (
                v1.x + v2.x,
                v1.y + v2.y
            );
    }

    template<typename T>
    PFM_INLINE  vec<2, T> operator+(T scalar, vec<2, T> const& v)
    {
        return vec<2, T>
            (
                v.x + scalar,
                v.y + scalar
            );
    }

    template<typename T>
    PFM_INLINE  vec<2, T> operator+(vec<2, T> const& v, T scalar)
    {
        return (scalar + v);
    }


    template<typename T>
    PFM_INLINE  vec<2, T> operator-(vec<2, T> const& v1, vec<2, T> const& v2)
    {
        return vec<2, T>
            (
                v1.x - v2.x,
                v1.y - v2.y
            );
    }


    template<typename T>
    PFM_INLINE  vec<2, T> operator-(T scalar, vec<2, T> const& v)
    {
        return vec<2, T>
            (
                v.x - scalar,
                v.y - scalar
            );
    }

    template<typename T>
    PFM_INLINE  vec<2, T> operator-(vec<2, T> const& v, T scalar)
    {
        return (scalar - v);
    }


    template<typename T>
    PFM_INLINE  vec<2, T> operator*(vec<2, T> const& v1, vec<2, T> const& v2)
    {
        return vec<2, T>
            (
                v1.x * v2.x,
                v1.y * v2.y
            );
    }

    template<typename T>
    PFM_INLINE  vec<2, T> operator*(T scalar, vec<2, T> const& v)
    {
        return vec<2, T>
            (
                v.x * scalar,
                v.y * scalar
            );
    }

    template<typename T>
    PFM_INLINE  vec<2, T> operator*(vec<2, T> const& v, T scalar)
    {
        return (scalar * v);
    }


    template<typename T>
    PFM_INLINE  vec<2, T> operator/(vec<2, T> const& v1, vec<2, T> const& v2)
    {
        return vec<2, T>
        (
            v1.x / v2.x,
            v1.y / v2.y
        );
    }

    template<typename T>
    PFM_INLINE  vec<2, T> operator/(T scalar, vec<2, T> const& v)
    {
        return vec<2, T>
            (
                v.x / scalar,
                v.y / scalar
            );
    }

    template<typename T>
    PFM_INLINE  vec<2, T> operator/(vec<2, T> const& v, T scalar)
    {
        return (scalar * v);
    }


    // Boolean operators
    template<typename T>
    PFM_INLINE  bool operator==(vec<2, T> const& v1, vec<2, T> const& v2)
    {
        return
            (
                v1.x == v2.x &&
                v1.y == v2.y
            );
    }
    template<typename T>
    PFM_INLINE  bool operator!=(vec<2, T> const& v1, vec<2, T> const& v2)
    {
        return !(v1 == v2);
    }

    // IO operators
    template<typename T>
    PFM_INLINE std::ostream& operator<<(std::ostream & o, vec<2, T> const& v)
    {
        o << "[" << v.x << ", " << v.y << "]";
        return (o);
    }
}//namespace pfm