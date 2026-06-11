#pragma once

#include <algorithm>
#include <compare>
#include <concepts>
#include <span>
#include <vector>

namespace lra::numeric {

template <std::floating_point Real>
struct point2 {
    Real x{};
    Real y{};

    [[nodiscard]] friend constexpr auto operator<=>(
        const point2&,
        const point2&
    ) = default;
};

template <std::floating_point Real>
[[nodiscard]] constexpr Real cross(
    point2<Real> origin,
    point2<Real> left,
    point2<Real> right
) noexcept {
    return (left.x - origin.x) * (right.y - origin.y)
        - (left.y - origin.y) * (right.x - origin.x);
}

template <std::floating_point Real>
[[nodiscard]] std::vector<point2<Real>> convex_hull(std::span<const point2<Real>> points) {
    std::vector<point2<Real>> sorted{points.begin(), points.end()};
    std::ranges::sort(sorted);
    const auto unique_end = std::ranges::unique(sorted).begin();
    sorted.erase(unique_end, sorted.end());

    if (sorted.size() <= 1U) {
        return sorted;
    }

    std::vector<point2<Real>> lower;
    lower.reserve(sorted.size());
    for (const auto point : sorted) {
        while (lower.size() >= 2U
            && cross(lower[lower.size() - 2U], lower.back(), point) <= Real{0}) {
            lower.pop_back();
        }
        lower.push_back(point);
    }

    std::vector<point2<Real>> upper;
    upper.reserve(sorted.size());
    for (auto point = sorted.rbegin(); point != sorted.rend(); ++point) {
        while (upper.size() >= 2U
            && cross(upper[upper.size() - 2U], upper.back(), *point) <= Real{0}) {
            upper.pop_back();
        }
        upper.push_back(*point);
    }

    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

} // namespace lra::numeric
