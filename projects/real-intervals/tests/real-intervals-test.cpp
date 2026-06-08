#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cmath>

#include <lra/numeric/metrics.hpp>
#include <lra/numeric/real_intervals.hpp>

namespace {

template <std::floating_point Real>
[[nodiscard]] lra::numeric::real_interval<Real> approximate_sqrt_two(int steps) {
    lra::numeric::real_interval<Real> interval{Real{1}, Real{2}};

    for (int step = 0; step < steps; ++step) {
        const Real middle = lra::numeric::midpoint(interval);
        if (middle * middle < Real{2}) {
            interval.lower = middle;
        } else {
            interval.upper = middle;
        }
    }

    return interval;
}

template <std::floating_point Real>
[[nodiscard]] lra::numeric::real_interval<Real> approximate_sqrt_two(
    lra::numeric::real_interval<Real> tolerance
) {
    lra::numeric::real_interval<Real> interval{Real{1}, Real{2}};

    while ((interval.upper - interval.lower) > tolerance.upper) {
        const Real old_lower = interval.lower;
        const Real old_upper = interval.upper;
        const Real middle = lra::numeric::midpoint(interval);

        if (middle == old_lower || middle == old_upper) {
            break;
        }

        if (middle * middle < Real{2}) {
            interval.lower = middle;
        } else {
            interval.upper = middle;
        }
    }

    return interval;
}

template <std::floating_point Real, std::size_t Size>
void expect_real_metric_axioms(const std::array<Real, Size>& points) {
    constexpr lra::numeric::real_metric metric{};

    for (const Real x : points) {
        EXPECT_EQ(metric(x, x), Real{0});

        for (const Real y : points) {
            EXPECT_GE(metric(x, y), Real{0});
            EXPECT_EQ(metric(x, y), metric(y, x));

            if (x == y) {
                EXPECT_EQ(metric(x, y), Real{0});
            } else {
                EXPECT_GT(metric(x, y), Real{0});
            }

            for (const Real z : points) {
                EXPECT_LE(metric(x, z), metric(x, y) + metric(y, z));
            }
        }
    }
}

template <std::floating_point Real, std::size_t Size>
void expect_interval_metric_axioms(
    const std::array<lra::numeric::real_interval<Real>, Size>& intervals
) {
    constexpr lra::numeric::interval_hausdorff_metric metric{};

    for (const auto x : intervals) {
        EXPECT_EQ(metric(x, x), Real{0});

        for (const auto y : intervals) {
            EXPECT_GE(metric(x, y), Real{0});
            EXPECT_EQ(metric(x, y), metric(y, x));

            if (x.lower == y.lower && x.upper == y.upper) {
                EXPECT_EQ(metric(x, y), Real{0});
            } else {
                EXPECT_GT(metric(x, y), Real{0});
            }

            for (const auto z : intervals) {
                EXPECT_LE(metric(x, z), metric(x, y) + metric(y, z));
            }
        }
    }
}

} // namespace

TEST(RealIntervals, TruthIsStable) {
    lra::numeric::real32_interval single_precision{0.0F, 1.0F};
    lra::numeric::real64_interval double_precision{0.0, 1.0};

    EXPECT_TRUE(single_precision.lower <= single_precision.upper);
    EXPECT_TRUE(double_precision.lower <= double_precision.upper);
}

TEST(RealIntervals, AddsReal32Intervals) {
    const lra::numeric::real32_interval left{1.0F, 2.0F};
    const lra::numeric::real32_interval right{3.0F, 5.0F};

    const auto sum = left + right;

    EXPECT_FLOAT_EQ(sum.lower, 4.0F);
    EXPECT_FLOAT_EQ(sum.upper, 7.0F);
}

TEST(RealIntervals, AddsReal64Intervals) {
    const lra::numeric::real64_interval left{1.0, 2.0};
    const lra::numeric::real64_interval right{3.0, 5.0};

    const auto sum = left + right;

    EXPECT_DOUBLE_EQ(sum.lower, 4.0);
    EXPECT_DOUBLE_EQ(sum.upper, 7.0);
}

TEST(RealIntervals, SubtractsReal32Intervals) {
    const lra::numeric::real32_interval left{1.0F, 4.0F};
    const lra::numeric::real32_interval right{2.0F, 8.0F};

    const auto difference = left - right;

    EXPECT_FLOAT_EQ(difference.lower, -7.0F);
    EXPECT_FLOAT_EQ(difference.upper, 2.0F);
}

TEST(RealIntervals, SubtractsReal64Intervals) {
    const lra::numeric::real64_interval left{1.0, 4.0};
    const lra::numeric::real64_interval right{2.0, 8.0};

    const auto difference = left - right;

    EXPECT_DOUBLE_EQ(difference.lower, -7.0);
    EXPECT_DOUBLE_EQ(difference.upper, 2.0);
}

TEST(RealIntervals, MultipliesReal32Intervals) {
    const lra::numeric::real32_interval left{-2.0F, 3.0F};
    const lra::numeric::real32_interval right{4.0F, 5.0F};

    const auto product = left * right;

    EXPECT_FLOAT_EQ(product.lower, -10.0F);
    EXPECT_FLOAT_EQ(product.upper, 15.0F);
}

TEST(RealIntervals, MultipliesReal64Intervals) {
    const lra::numeric::real64_interval left{-2.0, 3.0};
    const lra::numeric::real64_interval right{4.0, 5.0};

    const auto product = left * right;

    EXPECT_DOUBLE_EQ(product.lower, -10.0);
    EXPECT_DOUBLE_EQ(product.upper, 15.0);
}

TEST(RealIntervals, FindsReal32Midpoint) {
    const lra::numeric::real32_interval interval{1.0F, 2.0F};

    EXPECT_FLOAT_EQ(lra::numeric::midpoint(interval), 1.5F);
}

TEST(RealIntervals, FindsReal64Midpoint) {
    const lra::numeric::real64_interval interval{1.0, 2.0};

    EXPECT_DOUBLE_EQ(lra::numeric::midpoint(interval), 1.5);
}

TEST(RealIntervals, FindsAdjacentReal32Values) {
    const auto value = lra::numeric::real32{1.0F};

    EXPECT_LT(lra::numeric::next_down(value), value);
    EXPECT_GT(lra::numeric::next_up(value), value);
}

TEST(RealIntervals, FindsAdjacentReal64Values) {
    const auto value = lra::numeric::real64{1.0};

    EXPECT_LT(lra::numeric::next_down(value), value);
    EXPECT_GT(lra::numeric::next_up(value), value);
}

TEST(RealIntervals, ContainsReal32Values) {
    const lra::numeric::real32_interval interval{1.0F, 2.0F};

    EXPECT_TRUE(lra::numeric::contains(interval, 1.0F));
    EXPECT_TRUE(lra::numeric::contains(interval, 1.5F));
    EXPECT_TRUE(lra::numeric::contains(interval, 2.0F));
    EXPECT_FALSE(lra::numeric::contains(interval, 0.999F));
    EXPECT_FALSE(lra::numeric::contains(interval, 2.001F));
}

TEST(RealIntervals, ContainsReal64Values) {
    const lra::numeric::real64_interval interval{1.0, 2.0};

    EXPECT_TRUE(lra::numeric::contains(interval, 1.0));
    EXPECT_TRUE(lra::numeric::contains(interval, 1.5));
    EXPECT_TRUE(lra::numeric::contains(interval, 2.0));
    EXPECT_FALSE(lra::numeric::contains(interval, 0.999));
    EXPECT_FALSE(lra::numeric::contains(interval, 2.001));
}

TEST(RealIntervals, ContainsReal32ValuesWithinScalarTolerance) {
    const lra::numeric::real32_interval interval{1.0F, 2.0F};

    EXPECT_TRUE(lra::numeric::contains_within(interval, 0.9995F, 0.001F));
    EXPECT_TRUE(lra::numeric::contains_within(interval, 2.0005F, 0.001F));
    EXPECT_FALSE(lra::numeric::contains_within(interval, 0.998F, 0.001F));
    EXPECT_FALSE(lra::numeric::contains_within(interval, 2.002F, 0.001F));
}

TEST(RealIntervals, ContainsReal64ValuesWithinScalarTolerance) {
    const lra::numeric::real64_interval interval{1.0, 2.0};

    EXPECT_TRUE(lra::numeric::contains_within(interval, 0.9995, 0.001));
    EXPECT_TRUE(lra::numeric::contains_within(interval, 2.0005, 0.001));
    EXPECT_FALSE(lra::numeric::contains_within(interval, 0.998, 0.001));
    EXPECT_FALSE(lra::numeric::contains_within(interval, 2.002, 0.001));
}

TEST(RealIntervals, ContainsReal32ValuesWithinIntervalTolerance) {
    const lra::numeric::real32_interval interval{1.0F, 2.0F};
    const lra::numeric::real32_interval tolerance{0.0F, 0.001F};

    EXPECT_TRUE(lra::numeric::contains_within(interval, 0.9995F, tolerance));
    EXPECT_FALSE(lra::numeric::contains_within(interval, 0.998F, tolerance));
}

TEST(RealIntervals, ContainsReal64ValuesWithinIntervalTolerance) {
    const lra::numeric::real64_interval interval{1.0, 2.0};
    const lra::numeric::real64_interval tolerance{0.0, 0.001};

    EXPECT_TRUE(lra::numeric::contains_within(interval, 2.0005, tolerance));
    EXPECT_FALSE(lra::numeric::contains_within(interval, 2.002, tolerance));
}

TEST(RealIntervals, ApproximatesSqrtTwoWithReal32Interval) {
    const auto interval = approximate_sqrt_two<lra::numeric::real32>(24);
    const auto root_two = static_cast<lra::numeric::real32>(std::sqrt(2.0));

    EXPECT_LE(interval.lower, root_two);
    EXPECT_GE(interval.upper, root_two);
    EXPECT_LT(interval.upper - interval.lower, 0.000001F);
}

TEST(RealIntervals, ApproximatesSqrtTwoWithReal64Interval) {
    const auto interval = approximate_sqrt_two<lra::numeric::real64>(48);
    const auto root_two = std::sqrt(2.0);

    EXPECT_LE(interval.lower, root_two);
    EXPECT_GE(interval.upper, root_two);
    EXPECT_LT(interval.upper - interval.lower, 0.00000000000001);
}

TEST(RealIntervals, EpsilonSqrtTwoMatchesEquivalentReal32StepCount) {
    const lra::numeric::real32_interval tolerance{0.0F, 0.001F};
    const auto fixed_steps = approximate_sqrt_two<lra::numeric::real32>(10);
    const auto by_epsilon = approximate_sqrt_two(tolerance);

    EXPECT_FLOAT_EQ(by_epsilon.lower, fixed_steps.lower);
    EXPECT_FLOAT_EQ(by_epsilon.upper, fixed_steps.upper);
    EXPECT_LE(by_epsilon.upper - by_epsilon.lower, tolerance.upper);
}

TEST(RealMetric, ComputesReal32Distance) {
    constexpr lra::numeric::real_metric metric{};

    EXPECT_FLOAT_EQ(metric(1.0F, 4.0F), 3.0F);
    EXPECT_FLOAT_EQ(metric(4.0F, 1.0F), 3.0F);
}

TEST(RealMetric, ComputesReal64Distance) {
    constexpr lra::numeric::real_metric metric{};

    EXPECT_DOUBLE_EQ(metric(1.0, 4.0), 3.0);
    EXPECT_DOUBLE_EQ(metric(4.0, 1.0), 3.0);
}

TEST(RealMetric, SatisfiesMetricAxiomsForReal32Samples) {
    expect_real_metric_axioms(std::array<lra::numeric::real32, 5>{-2.0F, -0.5F, 0.0F, 1.0F, 3.0F});
}

TEST(RealMetric, SatisfiesMetricAxiomsForReal64Samples) {
    expect_real_metric_axioms(std::array<lra::numeric::real64, 5>{-2.0, -0.5, 0.0, 1.0, 3.0});
}

TEST(RealMetric, ChecksClosedScalarTolerance) {
    constexpr lra::numeric::real_metric metric{};

    EXPECT_TRUE(lra::numeric::within_tolerance(metric, 1.0, 1.5, 0.5));
    EXPECT_TRUE(lra::numeric::within_tolerance(
        metric,
        1.0,
        1.5,
        0.5,
        lra::numeric::closed_tolerance
    ));
    EXPECT_FALSE(lra::numeric::within_tolerance(metric, 1.0, 1.75, 0.5));
}

TEST(RealMetric, ChecksOpenScalarTolerance) {
    constexpr lra::numeric::real_metric metric{};

    EXPECT_TRUE(lra::numeric::within_tolerance(
        metric,
        1.0,
        1.25,
        0.5,
        lra::numeric::open_tolerance
    ));
    EXPECT_FALSE(lra::numeric::within_tolerance(
        metric,
        1.0,
        1.5,
        0.5,
        lra::numeric::open_tolerance
    ));
}

TEST(RealMetric, ChecksIntervalTolerance) {
    constexpr lra::numeric::real_metric metric{};
    const lra::numeric::real64_interval tolerance{0.0, 0.5};

    EXPECT_TRUE(lra::numeric::within_tolerance(metric, 1.0, 1.5, tolerance));
    EXPECT_FALSE(lra::numeric::within_tolerance(
        metric,
        1.0,
        1.5,
        tolerance,
        lra::numeric::open_tolerance
    ));
}

TEST(IntervalHausdorffMetric, ComputesReal32Distance) {
    constexpr lra::numeric::interval_hausdorff_metric metric{};
    const lra::numeric::real32_interval left{1.0F, 3.0F};
    const lra::numeric::real32_interval right{2.0F, 6.0F};

    EXPECT_FLOAT_EQ(metric(left, right), 3.0F);
    EXPECT_FLOAT_EQ(metric(right, left), 3.0F);
}

TEST(IntervalHausdorffMetric, ComputesReal64Distance) {
    constexpr lra::numeric::interval_hausdorff_metric metric{};
    const lra::numeric::real64_interval left{1.0, 3.0};
    const lra::numeric::real64_interval right{2.0, 6.0};

    EXPECT_DOUBLE_EQ(metric(left, right), 3.0);
    EXPECT_DOUBLE_EQ(metric(right, left), 3.0);
}

TEST(IntervalHausdorffMetric, SatisfiesMetricAxiomsForReal32Samples) {
    expect_interval_metric_axioms(std::array<lra::numeric::real32_interval, 5>{
        lra::numeric::real32_interval{-2.0F, -1.0F},
        lra::numeric::real32_interval{-1.0F, 1.0F},
        lra::numeric::real32_interval{0.0F, 0.0F},
        lra::numeric::real32_interval{1.0F, 2.0F},
        lra::numeric::real32_interval{1.5F, 3.0F},
    });
}

TEST(IntervalHausdorffMetric, SatisfiesMetricAxiomsForReal64Samples) {
    expect_interval_metric_axioms(std::array<lra::numeric::real64_interval, 5>{
        lra::numeric::real64_interval{-2.0, -1.0},
        lra::numeric::real64_interval{-1.0, 1.0},
        lra::numeric::real64_interval{0.0, 0.0},
        lra::numeric::real64_interval{1.0, 2.0},
        lra::numeric::real64_interval{1.5, 3.0},
    });
}

TEST(IntervalHausdorffMetric, ChecksClosedTolerance) {
    constexpr lra::numeric::interval_hausdorff_metric metric{};
    const lra::numeric::real64_interval left{1.0, 3.0};
    const lra::numeric::real64_interval right{1.25, 3.5};

    EXPECT_TRUE(lra::numeric::within_tolerance(metric, left, right, 0.5));
    EXPECT_FALSE(lra::numeric::within_tolerance(
        metric,
        left,
        right,
        0.5,
        lra::numeric::open_tolerance
    ));
}
