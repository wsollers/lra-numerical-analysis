add_executable(hello_one
    src/main.cpp
)

target_link_libraries(hello_one PRIVATE lra::numeric)
lra_configure_target(hello_one)
lra_enable_sanitizers(hello_one)

