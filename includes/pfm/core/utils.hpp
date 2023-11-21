#pragma once

#include "../setup/setup.hpp"

namespace pfm {

    template<typename T>
    PFM_FUNC_DECL float radians(T const& degrees);
}

#include "utils.inl"