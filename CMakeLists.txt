cmake_minimum_required(VERSION 3.25.1)

project(MMLauncher C)
set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME} src/app.c)

find_package(PkgConfig REQUIRED)
pkg_search_module(GTK REQUIRED gtk+-3.0)
