add_executable(convex_hull_tests
    convex-hull-test.cpp
)

target_link_libraries(convex_hull_tests PRIVATE
    lra::numeric
    GTest::gtest_main
)

lra_configure_target(convex_hull_tests)
lra_enable_sanitizers(convex_hull_tests)

include(GoogleTest)
gtest_discover_tests(convex_hull_tests)
