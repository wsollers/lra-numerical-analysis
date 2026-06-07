#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
#include <limits>

namespace lra::numeric {

using real32 = float;
using real64 = double;

// TODO: Add feature-gated real16/real128 aliases when std::float16_t,
// std::float128_t, and the __STDCPP_FLOAT*_T__ macros are available across the
// supported toolchains.
template <std::floating_point Real>
struct real_interval {
    Real lower{};
    Real upper{};
};

template <std::floating_point Real>
[[nodiscard]] constexpr real_interval<Real> operator+(
    const real_interval<Real>& left,
    const real_interval<Real>& right
) noexcept {
    return {
        .lower = left.lower + right.lower,
        .upper = left.upper + right.upper,
    };
}

template <std::floating_point Real>
[[nodiscard]] constexpr real_interval<Real> operator-(
    const real_interval<Real>& left,
    const real_interval<Real>& right
) noexcept {
    return {
        .lower = left.lower - right.upper,
        .upper = left.upper - right.lower,
    };
}

template <std::floating_point Real>
[[nodiscard]] constexpr real_interval<Real> operator*(
    const real_interval<Real>& left,
    const real_interval<Real>& right
) noexcept {
    const auto lower_lower = left.lower * right.lower;
    const auto lower_upper = left.lower * right.upper;
    const auto upper_lower = left.upper * right.lower;
    const auto upper_upper = left.upper * right.upper;

    return {
        .lower = std::min({lower_lower, lower_upper, upper_lower, upper_upper}),
        .upper = std::max({lower_lower, lower_upper, upper_lower, upper_upper}),
    };
}

template <std::floating_point Real>
[[nodiscard]] constexpr Real midpoint(const real_interval<Real>& interval) noexcept {
    return (interval.lower + interval.upper) / Real{2};
}

using real32_interval = real_interval<real32>;
using real64_interval = real_interval<real64>;

template <std::floating_point Real>
[[nodiscard]] Real next_down(Real value) noexcept {
    return std::nextafter(value, -std::numeric_limits<Real>::infinity());
}

template <std::floating_point Real>
[[nodiscard]] Real next_up(Real value) noexcept {
    return std::nextafter(value, std::numeric_limits<Real>::infinity());
}

template <std::floating_point Real>
[[nodiscard]] constexpr bool contains(
    const real_interval<Real>& interval,
    Real value
) noexcept {
    return interval.lower <= value && value <= interval.upper;
}

template <std::floating_point Real>
[[nodiscard]] constexpr bool contains_within(
    const real_interval<Real>& interval,
    Real value,
    Real tolerance
) noexcept {
    return (interval.lower - tolerance) <= value
        && value <= (interval.upper + tolerance);
}

template <std::floating_point Real>
[[nodiscard]] constexpr bool contains_within(
    const real_interval<Real>& interval,
    Real value,
    real_interval<Real> tolerance
) noexcept {
    return contains_within(interval, value, tolerance.upper);
}

} // namespace lra::numeric
