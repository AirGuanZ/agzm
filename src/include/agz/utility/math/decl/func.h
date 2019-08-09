#pragma once

#include "./common.h"

namespace agz::math
{
    
template<typename T, typename F>
auto mix(const T &left, const T &right, F factor) noexcept(noexcept((1 - factor) * left + factor * right))
{
    return (1 - factor) * left + factor * right;
}

template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
bool is_finite(T val) noexcept
{
    return !std::isnan(val) && !std::isinf(val);
}

} // namespace agz::math
