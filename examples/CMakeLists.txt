cmake_minimum_required(VERSION 3.8)

foreach(example hello enum options)
    add_executable(example_${example} example_${example}.cc)
    target_include_directories(example_${example} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    target_link_libraries(example_${example} PRIVATE cpppoet cpppoet_warnings)
endforeach()
