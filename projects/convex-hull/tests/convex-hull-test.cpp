#include <gtest/gtest.h>

#include <cstddef>
#include <span>
#include <vector>

#include <lra/numeric/convex-hull.hpp>

namespace {

using point = lra::numeric::point2<double>;

void expect_points_eq(std::span<const point> actual, std::span<const point> expected) {
    ASSERT_EQ(actual.size(), expected.size());

    for (std::size_t index = 0; index < expected.size(); ++index) {
        EXPECT_DOUBLE_EQ(actual[index].x, expected[index].x);
        EXPECT_DOUBLE_EQ(actual[index].y, expected[index].y);
    }
}

} // namespace

TEST(ConvexHull, ReturnsEmptyHullForEmptyInput) {
    const std::vector<point> points;

    EXPECT_TRUE(lra::numeric::convex_hull(std::span<const point>{points}).empty());
}

TEST(ConvexHull, ReturnsSingleUniquePoint) {
    const std::vector<point> points{
        {1.0, 2.0},
        {1.0, 2.0},
    };

    const std::vector<point> expected{
        {1.0, 2.0},
    };

    expect_points_eq(lra::numeric::convex_hull(std::span<const point>{points}), expected);
}

TEST(ConvexHull, ReturnsSegmentEndpointsForCollinearInput) {
    const std::vector<point> points{
        {-1.0, -1.0},
        {0.0, 0.0},
        {1.0, 1.0},
        {2.0, 2.0},
    };

    const std::vector<point> expected{
        {-1.0, -1.0},
        {2.0, 2.0},
    };

    expect_points_eq(lra::numeric::convex_hull(std::span<const point>{points}), expected);
}

TEST(ConvexHull, FindsSquareAroundInteriorPoints) {
    const std::vector<point> points{
        {0.5, 0.5},
        {1.0, 0.0},
        {0.0, 1.0},
        {1.0, 1.0},
        {0.0, 0.0},
        {0.25, 0.75},
    };

    const std::vector<point> expected{
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0},
    };

    expect_points_eq(lra::numeric::convex_hull(std::span<const point>{points}), expected);
}

TEST(ConvexHull, DropsBoundaryPointsOnHullEdges) {
    const std::vector<point> points{
        {0.0, 0.0},
        {0.5, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0},
        {0.0, 0.5},
    };

    const std::vector<point> expected{
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0},
    };

    expect_points_eq(lra::numeric::convex_hull(std::span<const point>{points}), expected);
}

TEST(ConvexHull, ComputesOrientationWithCrossProduct) {
    EXPECT_GT(
        lra::numeric::cross(point{0.0, 0.0}, point{1.0, 0.0}, point{1.0, 1.0}),
        0.0
    );
    EXPECT_LT(
        lra::numeric::cross(point{0.0, 0.0}, point{1.0, 1.0}, point{1.0, 0.0}),
        0.0
    );
    EXPECT_DOUBLE_EQ(
        lra::numeric::cross(point{0.0, 0.0}, point{1.0, 1.0}, point{2.0, 2.0}),
        0.0
    );
}
