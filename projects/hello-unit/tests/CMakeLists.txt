add_executable(hello_unit_tests
    test_hello.cpp
)

target_link_libraries(hello_unit_tests PRIVATE
    lra::numeric
    GTest::gtest_main
)

lra_configure_target(hello_unit_tests)
lra_enable_sanitizers(hello_unit_tests)

include(GoogleTest)
gtest_discover_tests(hello_unit_tests)

