cmake_minimum_required(VERSION 3.25.1)
project(MMLauncher C)


find_package(PkgConfig REQUIRED)

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

pkg_check_modules(LIBS REQUIRED IMPORTED_TARGET libcurl gtk+-3.0 libzip)

set(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/res)

add_subdirectory(launcher)
add_subdirectory(installer)
add_dependencies(installer launcher)

