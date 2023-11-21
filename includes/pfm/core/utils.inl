#include <limits>

namespace pfm {
    template<typename T>
    PFM_FUNC_DECL float radians(T const& degrees)
    {
        static_assert(std::numeric_limits<T>::is_iec559, "'radians' only accepts floats value");

        return (degrees * 0.01745329251994329576923690768489);
    }
}