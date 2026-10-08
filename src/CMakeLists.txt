cmake_minimum_required(VERSION 3.8)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

file(GLOB CPPPOET_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/render/*.cc)

add_library(cpppoet ${CPPPOET_SOURCES})

target_include_directories(cpppoet
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# file_writer.cc uses <filesystem> (C++17); PUBLIC propagates the
# requirement to consumers such as the examples and downstream generators.
target_compile_features(cpppoet PUBLIC cxx_std_17)

target_link_libraries(cpppoet PRIVATE $<BUILD_INTERFACE:cpppoet_warnings>)

if (BUILD_SHARED_LIBS)
    target_compile_definitions(cpppoet PRIVATE CPPPOET_BUILDING_LIBRARY)
else ()
    target_compile_definitions(cpppoet PUBLIC CPPPOET_STATIC)
endif ()

set_target_properties(cpppoet PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON)

install(TARGETS cpppoet
    EXPORT cpppoetTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT cpppoetTargets
    FILE cpppoetTargets.cmake
    NAMESPACE cpppoet::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpppoet)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/cpppoetConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cpppoetConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpppoet)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cpppoetConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cpppoetConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cpppoetConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpppoet)
