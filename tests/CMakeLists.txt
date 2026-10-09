cmake_minimum_required(VERSION 3.8)

add_executable(cpppoet_tests
    test_main.cc
    type_name_test.cc
    declarations_test.cc
    member_functions_test.cc
    classes_test.cc
    code_block_test.cc
    file_writer_test.cc
    version_test.cc
    roundtrip_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/code_buffer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/render/include_collector.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/render/renderer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/util.cc
)

target_include_directories(cpppoet_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(cpppoet_tests PRIVATE cpppoet cpppoet_warnings)

target_compile_definitions(cpppoet_tests PRIVATE
    CPPPOET_TEST_CXX_COMPILER="${CMAKE_CXX_COMPILER}"
    CPPPOET_TEST_INCLUDE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../include")

add_test(NAME cpppoet_tests COMMAND cpppoet_tests)
