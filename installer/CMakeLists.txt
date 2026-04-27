add_executable(installer 
	src/main.c
	src/installerapp.c
	src/installerappwin.c
	${CMAKE_CURRENT_BINARY_DIR}/launcher_info.h
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
		installer.gresource.xml
	MAIN_DEPENDENCY installer.gresource.xml
	DEPENDS
		${RESOURCES}/layout/installer.ui
)
target_link_libraries(installer PRIVATE PkgConfig::LIBS)

add_custom_command(
	TARGET installer
	COMMAND ${CMAKE_COMMAND} -E cat ../launcher/launcher >> installer
	POST_BUILD
)

add_custom_command(
	OUTPUT launcher_info.h
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND}
	ARGS
		-P ${CMAKE_CURRENT_SOURCE_DIR}/gen_launcher_info.cmake

)

target_include_directories(installer PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
