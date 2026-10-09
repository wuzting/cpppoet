cmake_minimum_required(VERSION 3.8)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# `version.h` is generated from its `.in` template into the build tree and
# installed alongside the other public headers.
set(CPPPOET_GENERATED_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/cpppoet/version.h.in
    ${CPPPOET_GENERATED_INCLUDE_DIR}/cpppoet/version.h
    @ONLY)

add_library(cpppoet
    class_spec.cc
    code_block.cc
    code_buffer.cc
    data_member_spec.cc
    enum_spec.cc
    file_spec.cc
    file_writer.cc
    forward_decl_spec.cc
    free_function_spec.cc
    member_function_spec.cc
    parameter_spec.cc
    type_alias_spec.cc
    type_name.cc
    util.cc
    variable_spec.cc
    render/include_collector.cc
    render/renderer.cc)

target_include_directories(cpppoet
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<BUILD_INTERFACE:${CPPPOET_GENERATED_INCLUDE_DIR}>
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

# Install the static public headers, but not the version.h.in template.
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PATTERN "*.in" EXCLUDE)

install(FILES ${CPPPOET_GENERATED_INCLUDE_DIR}/cpppoet/version.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cpppoet)

install(EXPORT cpppoetTargets
    FILE cpppoetTargets.cmake
    NAMESPACE cpppoet::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpppoet)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/cpppoetConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cpppoetConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpppoet)

# Pre-1.0, a minor bump may break the API, so require a matching minor version.
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cpppoetConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cpppoetConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cpppoetConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpppoet)
