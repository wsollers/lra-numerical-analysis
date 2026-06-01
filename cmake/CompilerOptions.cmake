function(lra_configure_target target_name)
    target_compile_features(${target_name} PRIVATE cxx_std_23)

    if(MSVC)
        target_compile_options(${target_name} PRIVATE
            /W4
            /permissive-
            /utf-8
            $<$<BOOL:${LRA_NUM_WARNINGS_AS_ERRORS}>:/WX>
        )
    else()
        target_compile_options(${target_name} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            $<$<BOOL:${LRA_NUM_WARNINGS_AS_ERRORS}>:-Werror>
        )
    endif()
endfunction()

