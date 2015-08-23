# Find TOLUA library
# Once done this will define
# TOLUA_FOUND
# TOLUA_LIBRARIES
# TOLUA_INCLUDE_DIR
# TOLUA_BINARY
# Glaucous 2011 <glakke1 at gmail dot com>

FIND_PATH(
TOLUA_INCLUDE_DIR

NAMES
tolua++.h
toluapp.h
tolua.h

DOC
"Directory where tolua++.h resides")

FIND_LIBRARY(
TOLUA_LIBRARIES

NAMES
tolua++
toluapp
tolua

DOC
"ToLua shared library location")

FIND_PROGRAM(
TOLUA_BINARY

NAMES
tolua++
toluapp
tolua

DOC
"ToLua binary")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TOLUA DEFAULT_MSG TOLUA_LIBRARIES TOLUA_INCLUDE_DIR TOLUA_BINARY)

MACRO(
TOLUA_PROCESS

headers
sources
pkg_files)

	FOREACH(pkg ${pkg_files})
		GET_FILENAME_COMPONENT(pkg_name_we ${pkg} NAME_WE)
		GET_FILENAME_COMPONENT(pkg_path ${pkg} PATH)
		SET(pkg_full_path "${CMAKE_BINARY_DIR}/lua${pkg_path}")
		FILE(MAKE_DIRECTORY "${pkg_full_path}")
		SET(pkg_path "${CMAKE_BINARY_DIR}/lua${pkg_path}/tolua_${pkg_name_we}")
		EXEC_PROGRAM(${TOLUA_BINARY} ARGS "-o ${pkg_path}.cpp -H ${pkg_path}.hpp -n ${pkg_name_we} ${pkg}")

		LIST(APPEND ${headers} ${pkg_full_path})
		LIST(APPEND ${sources} "${pkg_path}.cpp")
	ENDFOREACH()

ENDMACRO()

#where pkgname represents the name of the package being bound. If we are using C++, we can opt for automatic initialization:
#tolua -a -n pkgname -o myfile.c myfile.pkg