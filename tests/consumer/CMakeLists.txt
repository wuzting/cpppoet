# Copyright (c) 2026 wuzting
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.8)

project(cpppoet_consumer LANGUAGES CXX)

enable_testing()

find_package(cpppoet REQUIRED)

add_executable(consumer main.cc)
target_link_libraries(consumer PRIVATE cpppoet::cpppoet)

# Registered as a test so CI can run it portably (handles .exe and the
# per-configuration output directories of multi-config generators).
add_test(NAME consumer COMMAND consumer)
