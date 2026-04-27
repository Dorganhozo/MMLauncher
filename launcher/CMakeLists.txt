project(LAUNCHER C)
add_executable(launcher
	src/main.c
	src/launcherapp.c
	src/launcherappwin.c
	${CMAKE_CURRENT_BINARY_DIR}/gresource.c
)

add_custom_command(
	OUTPUT gresource.c
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND ${GLIB_COMPILE_RESOURCES}
	ARGS
		--target=${CMAKE_CURRENT_BINARY_DIR}/gresource.c
		--sourcedir=${RESOURCES}
		--generate-source
		launcher.gresource.xml
	MAIN_DEPENDENCY launcher.gresource.xml
	DEPENDS
		${RESOURCES}/layout/launcher.ui
)
target_link_libraries(launcher PRIVATE PkgConfig::LIBS)

