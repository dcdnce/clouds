namespace pfm {
    // Implicit basic constructors
    template<typename T>
    PFM_INLINE  vec<4, T>::vec(vec<4, T> const& v)
        : x(v.x), y(v.y), z(v.z), w(v.w) 
    {}

    // Explicit basic constructors
    template<typename T>
    PFM_INLINE   vec<4,T>::vec(T scalar)
        : x(scalar), y(scalar), z(scalar), w(scalar)
    {}
    template<typename T>
    PFM_INLINE  vec<4, T>::vec(T x, T y, T z, T w)
        : x(x), y(y), z(z), w(w)
    {}

    // Accesses
    template<typename T>
    PFM_INLINE typename vec<4, T>::value_type& vec<4, T>::operator[](length_t i) noexcept
    {
        assert(i >= 0 && i < 4);
        switch(i)
		{
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            case 3:
                return w;
            default:
                return x;
		}
    }

    template<typename T>
    PFM_INLINE  typename vec<4, T>::value_type const& vec<4, T>::operator[](length_t i) const noexcept
    {
        assert(i >= 0 && i < 4);
        switch(i)
		{
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            case 3:
                return w;
            default:
                return x;
		}
    }

    template<typename T>
    PFM_INLINE  typename vec<4, T>::value_type* vec<4, T>::operator&()
    {
        return &x;
    }

    // Unary arithmetic operators
    template<typename T> //struct template
    template<typename U> //extern template
    PFM_INLINE  vec<4, T>& vec<4, T>::operator=(vec<4, U> const& v)
    {
        this->x = static_cast<T>(v.x);
        this->y = static_cast<T>(v.y);
        this->z = static_cast<T>(v.z);
        this->w = static_cast<T>(v.w);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<4, T>& vec<4, T>::operator+=(vec<4, U> const& v)
    {
        this->x += static_cast<T>(v.x);
        this->y += static_cast<T>(v.y);
        this->z += static_cast<T>(v.z);
        this->w += static_cast<T>(v.w);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<4, T>& vec<4, T>::operator+=(U scalar)
    {
        this->x += static_cast<T>(scalar);
        this->y += static_cast<T>(scalar);
        this->z += static_cast<T>(scalar);
        this->w += static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<4, T>& vec<4, T>::operator-=(vec<4, U> const& v)
    {
        this->x -= static_cast<T>(v.x);
        this->y -= static_cast<T>(v.y);
        this->z -= static_cast<T>(v.z);
        this->w -= static_cast<T>(v.w);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<4, T>& vec<4, T>::operator-=(U scalar)
    {
        this->x -= static_cast<T>(scalar);
        this->y -= static_cast<T>(scalar);
        this->z -= static_cast<T>(scalar);
        this->w -= static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<4, T>& vec<4, T>::operator*=(vec<4, U> const& v)
    {
        this->x *= static_cast<T>(v.x);
        this->y *= static_cast<T>(v.y);
        this->z *= static_cast<T>(v.z);
        this->w *= static_cast<T>(v.w);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<4, T>& vec<4, T>::operator*=(U scalar)
    {
        this->x *= static_cast<T>(scalar);
        this->y *= static_cast<T>(scalar);
        this->z *= static_cast<T>(scalar);
        this->w *= static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<4, T>& vec<4, T>::operator/=(vec<4, U> const& v)
    {
        this->x /= static_cast<T>(v.x);
        this->y /= static_cast<T>(v.y);
        this->z /= static_cast<T>(v.z);
        this->w /= static_cast<T>(v.w);
        return *this;
    }

    template<typename T>
    template<typename U>
    PFM_INLINE  vec<4, T>& vec<4, T>::operator/=(U scalar)
    {
        this->x /= static_cast<T>(scalar);
        this->y /= static_cast<T>(scalar);
        this->z /= static_cast<T>(scalar);
        this->w /= static_cast<T>(scalar);
        return *this;
    }

    // Binary arithmetic operators
    template<typename T>
    PFM_INLINE  vec<4, T> operator+(vec<4, T> const& v1, vec<4, T> const& v2)
    {
        return vec<4, T>
            (
                v1.x + v2.x,
                v1.y + v2.y,
                v1.z + v2.z,
                v1.w + v2.w
            );
    }

    template<typename T>
    PFM_INLINE  vec<4, T> operator+(T scalar, vec<4, T> const& v)
    {
        return vec<4, T>
            (
                v.x + scalar,
                v.y + scalar,
                v.z + scalar,
                v.w + scalar
            );
    }

    template<typename T>
    PFM_INLINE  vec<4, T> operator+(vec<4, T> const& v, T scalar)
    {
        return (scalar + v);
    }


    template<typename T>
    PFM_INLINE  vec<4, T> operator-(vec<4, T> const& v1, vec<4, T> const& v2)
    {
        return vec<4, T>
            (
                v1.x - v2.x,
                v1.y - v2.y,
                v1.z - v2.z,
                v1.w - v2.w
            );
    }


    template<typename T>
    PFM_INLINE  vec<4, T> operator-(T scalar, vec<4, T> const& v)
    {
        return vec<4, T>
            (
                v.x - scalar,
                v.y - scalar,
                v.z - scalar,
                v.w - scalar
            );
    }

    template<typename T>
    PFM_INLINE  vec<4, T> operator-(vec<4, T> const& v, T scalar)
    {
        return (scalar - v);
    }


    template<typename T>
    PFM_INLINE  vec<4, T> operator*(vec<4, T> const& v1, vec<4, T> const& v2)
    {
        return vec<4, T>
            (
                v1.x * v2.x,
                v1.y * v2.y,
                v1.z * v2.z,
                v1.w * v2.w
            );
    }

    template<typename T>
    PFM_INLINE  vec<4, T> operator*(T scalar, vec<4, T> const& v)
    {
        return vec<4, T>
            (
                v.x * scalar,
                v.y * scalar,
                v.z * scalar,
                v.w * scalar
            );
    }

    template<typename T>
    PFM_INLINE  vec<4, T> operator*(vec<4, T> const& v, T scalar)
    {
        return (scalar * v);
    }


    template<typename T>
    PFM_INLINE  vec<4, T> operator/(vec<4, T> const& v1, vec<4, T> const& v2)
    {
        return vec<4, T>
        (
            v1.x / v2.x,
            v1.y / v2.y,
            v1.z / v2.z,
            v1.w / v2.w
        );
    }

    template<typename T>
    PFM_INLINE  vec<4, T> operator/(T scalar, vec<4, T> const& v)
    {
        return vec<4, T>
            (
                v.x / scalar,
                v.y / scalar,
                v.z / scalar,
                v.w / scalar
            );
    }

    template<typename T>
    PFM_INLINE  vec<4, T> operator/(vec<4, T> const& v, T scalar)
    {
        return (scalar / v);
    }


    // Boolean operators
    template<typename T>
    PFM_INLINE  bool operator==(vec<4, T> const& v1, vec<4, T> const& v2)
    {
        return
            (
                v1.x == v2.x &&
                v1.y == v2.y &&
                v1.z == v2.z &&
                v1.w == v2.w
            );
    }
    template<typename T>
    PFM_INLINE  bool operator!=(vec<4, T> const& v1, vec<4, T> const& v2)
    {
        return !(v1 == v2);
    }

    // IO operators
    template<typename T>
    PFM_INLINE std::ostream& operator<<(std::ostream & o, vec<4, T> const& v)
    {
        o << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";
        return (o);
    }


}//namespace pfm