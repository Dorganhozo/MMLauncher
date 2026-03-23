cmake_minimum_required(VERSION 3.25.1)
project(MMLauncher C)
set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME} 
	src/app.c
	src/launcher.c
	src/launcherwin.c
	${CMAKE_CURRENT_BINARY_DIR}/gresource.c
)

find_package(PkgConfig REQUIRED)
pkg_search_module(GTK REQUIRED IMPORTED_TARGET gtk+-3.0)
target_link_libraries(MMLauncher PRIVATE PkgConfig::GTK)

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)
add_custom_command(
	OUTPUT gresource.c
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND ${GLIB_COMPILE_RESOURCES}
	ARGS
		--target=${CMAKE_CURRENT_BINARY_DIR}/gresource.c
		--sourcedir=res
		--generate-source
		mmlauncher.gresource.xml
	MAIN_DEPENDENCY mmlauncher.gresource.xml
	DEPENDS
		res/layout/welcome.ui
		res/layout/launcher.ui
)
# target_link_options(MMLauncher PRIVATE -rdynamic)
