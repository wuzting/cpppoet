cmake_minimum_required(VERSION 3.8)

project(cpppoet VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_CPPPOET_MODULE "Build the cpppoet code-generation library" ON)
option(ENABLE_CPPPOET_EXAMPLE "Build the cpppoet examples" OFF)
option(ENABLE_CPPPOET_TEST "Build the cpppoet tests" OFF)
option(CPPPOET_WERROR "Treat compiler warnings as errors" OFF)
option(ENABLE_CPPPOET_DOCS "Generate API documentation with Doxygen" OFF)

# Build a shared library by default. Public symbols are marked with
# CPPPOET_EXPORT (see include/cpppoet/export.h) and everything else is hidden,
# so a Windows DLL exports exactly the intended API.
option(BUILD_SHARED_LIBS "Build cpppoet as a shared library" ON)

add_library(cpppoet_warnings INTERFACE)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(cpppoet_warnings INTERFACE -Wall -Wextra)
    if (CPPPOET_WERROR)
        target_compile_options(cpppoet_warnings INTERFACE -Werror)
    endif()
endif()

if (ENABLE_CPPPOET_MODULE)
    add_subdirectory(src)
endif()

if (ENABLE_CPPPOET_MODULE AND ENABLE_CPPPOET_EXAMPLE)
    add_subdirectory(examples)
endif()

if (ENABLE_CPPPOET_MODULE AND ENABLE_CPPPOET_TEST)
    enable_testing()
    add_subdirectory(tests)
endif()

if (ENABLE_CPPPOET_DOCS)
    find_program(DOXYGEN_EXECUTABLE doxygen)
    if (DOXYGEN_EXECUTABLE)
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
    else ()
        message(WARNING "Doxygen not found; the 'docs' target is unavailable")
    endif ()
endif()
