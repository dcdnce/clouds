#pragma once

#include <cassert>
#include <cmath>

#define PFM_FUNC_DECL
#define PFM_INLINE inline
#define PFM_EXPLICIT explicit
#define PFM_DEFAULT = default

namespace pfm {
    // Typedefs
    typedef int length_t;

    // Templates classes
    template<length_t L, typename T> struct vec;
    template<length_t C, length_t R, typename T> struct mat;
}