cmake_minimum_required(VERSION 3.8)

foreach(example hello enum options)
    add_executable(example_${example} example_${example}.cc)
    target_include_directories(example_${example} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    target_link_libraries(example_${example} PRIVATE cpppoet cpppoet_warnings)

    # A shared cpppoet must sit next to the example on Windows to be found.
    if (WIN32 AND BUILD_SHARED_LIBS)
        add_custom_command(TARGET example_${example} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:cpppoet> $<TARGET_FILE_DIR:example_${example}>)
    endif ()
endforeach()
