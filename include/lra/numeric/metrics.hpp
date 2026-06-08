#pragma once

#include <algorithm>
#include <concepts>

#include <lra/numeric/real_intervals.hpp>

namespace lra::numeric {

struct real_metric {
    template <std::floating_point Real>
    [[nodiscard]] constexpr Real distance(Real left, Real right) const noexcept {
        return left < right ? right - left : left - right;
    }

    template <std::floating_point Real>
    [[nodiscard]] constexpr Real operator()(Real left, Real right) const noexcept {
        return distance(left, right);
    }
};

struct interval_hausdorff_metric {
    template <std::floating_point Real>
    [[nodiscard]] constexpr Real distance(
        real_interval<Real> left,
        real_interval<Real> right
    ) const noexcept {
        constexpr real_metric metric{};
        return std::max(
            metric(left.lower, right.lower),
            metric(left.upper, right.upper)
        );
    }

    template <std::floating_point Real>
    [[nodiscard]] constexpr Real operator()(
        real_interval<Real> left,
        real_interval<Real> right
    ) const noexcept {
        return distance(left, right);
    }
};

struct closed_tolerance_bound {
    template <class Distance, class Tolerance>
    [[nodiscard]] constexpr bool operator()(
        Distance distance,
        Tolerance tolerance
    ) const noexcept(noexcept(distance <= tolerance)) {
        return distance <= tolerance;
    }
};

struct open_tolerance_bound {
    template <class Distance, class Tolerance>
    [[nodiscard]] constexpr bool operator()(
        Distance distance,
        Tolerance tolerance
    ) const noexcept(noexcept(distance < tolerance)) {
        return distance < tolerance;
    }
};

inline constexpr closed_tolerance_bound closed_tolerance{};
inline constexpr open_tolerance_bound open_tolerance{};

template <class Metric, class Point, class Tolerance, class Bound>
[[nodiscard]] constexpr bool within_tolerance(
    Metric metric,
    const Point& point,
    const Point& center,
    Tolerance tolerance,
    Bound bound
) noexcept(noexcept(bound(metric(point, center), tolerance))) {
    return bound(metric(point, center), tolerance);
}

template <class Metric, class Point, class Tolerance>
[[nodiscard]] constexpr bool within_tolerance(
    Metric metric,
    const Point& point,
    const Point& center,
    Tolerance tolerance
) noexcept(noexcept(within_tolerance(metric, point, center, tolerance, closed_tolerance))) {
    return within_tolerance(metric, point, center, tolerance, closed_tolerance);
}

template <class Metric, class Point, std::floating_point Real, class Bound>
[[nodiscard]] constexpr bool within_tolerance(
    Metric metric,
    const Point& point,
    const Point& center,
    real_interval<Real> tolerance,
    Bound bound
) noexcept(noexcept(within_tolerance(metric, point, center, tolerance.upper, bound))) {
    return within_tolerance(metric, point, center, tolerance.upper, bound);
}

template <class Metric, class Point, std::floating_point Real>
[[nodiscard]] constexpr bool within_tolerance(
    Metric metric,
    const Point& point,
    const Point& center,
    real_interval<Real> tolerance
) noexcept(noexcept(within_tolerance(metric, point, center, tolerance, closed_tolerance))) {
    return within_tolerance(metric, point, center, tolerance, closed_tolerance);
}

} // namespace lra::numeric
