add_executable(real_intervals_tests
    real-intervals-test.cpp
)

target_link_libraries(real_intervals_tests PRIVATE
    lra::numeric
    GTest::gtest_main
)

lra_configure_target(real_intervals_tests)
lra_enable_sanitizers(real_intervals_tests)

include(GoogleTest)
gtest_discover_tests(real_intervals_tests)
