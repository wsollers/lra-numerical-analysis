function(lra_enable_sanitizers target_name)
    if(NOT LRA_NUM_ENABLE_SANITIZERS)
        return()
    endif()

    if(MSVC)
        return()
    endif()

    target_compile_options(${target_name} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>
        $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
    )
    target_link_options(${target_name} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>
    )
endfunction()

