cmake_minimum_required(VERSION 2.6)
project(glakkengine)
set(GLAKKENGINE glakkengine CACHE STRING "GlakkEngine output file name")

# message(STATUS "Using gcc-4.7")
# set(CMAKE_C_COMPILER "/usr/bin/c++-4.7")
# set(CMAKE_CXX_COMPILER "/usr/bin/c++-4.7")
#message(STATUS "Using ccache gcc-4.7")
#set(CMAKE_C_COMPILER "/usr/bin/ccache")
#set(CMAKE_CXX_COMPILER "/usr/bin/ccache")
#set(CMAKE_C_COMPILER_ARG1 "/usr/bin/g++-4.7")
#set(CMAKE_CXX_COMPILER_ARG1 "/usr/bin/g++-4.7")

option(BUILD_PROFILE "Set to ON to build profiling" OFF)
option(BUILD_GLAKKEGAME "Set to ON to build GlakkeGame" ON)
option(BUILD_GLAKKEDITOR "Set to ON to build GlakkEditor" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/lib")
list(APPEND CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/lib")

# Arch
if (NOT DEFINED KKE_ARCH)
	if (CMAKE_SIZEOF_VOID_P MATCHES "8")
		set(KKE_ARCH "x86_64" CACHE STRING "ARCH")
	elseif (CMAKE_SIZEOF_VOID_P MATCHES "4")
		set(KKE_ARCH "x86" CACHE STRING "ARCH")
	else()
		set(KKE_ARCH "x86_64" CACHE STRING "ARCH")
		message(WARNING "Could not define ARCH, using ${KKE_ARCH}.")
	endif()
endif()

if (KKE_ARCH MATCHES "x86_64")
	add_definitions("-m64")
elseif (KKE_ARCH MATCHES "x86")
	add_definitions("-m32")
else()
	message(FATAL_ERROR "Wrong architecture, ${KKE_ARCH}! (x86_64 or x86)")
endif()

message(STATUS "Using ARCH: ${KKE_ARCH}")

# Version
set(KKE_VERSION 0.1 CACHE STRING "VERSION")

# OS
if (UNIX)
	add_definitions("-DUNIX")
elseif (WIN32)
	add_definitions("-DWINDOWS")
else (WIN32)
	message(FATAL_ERROR "Operating System ${CMAKE_SYSTEM_NAME} not (officially) supported.")
endif()

# For now removed: -Wfloat-equal 
add_definitions("-std=c++11 -Wno-unused-function -Wno-unused-parameter -Wextra -Wall -Winline -DVERSION='\"${KKE_VERSION}\"' -DROOT_FOLDER='\"${CMAKE_CURRENT_SOURCE_DIR}/src\"'")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fno-omit-frame-pointer -DSWIGRUNTIME_DEBUG" CACHE STRING "Engine debug flags")
set(CMAKE_CXX_FLAGS_RELEASE "-O3" CACHE STRING "Engine release flags")

if (BUILD_PROFILE)
	add_definitions("-pg")
endif()

if(UNIX)
#	message(STATUS "Creating symbolic links..")
#	execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/content ${CMAKE_BINARY_DIR}/content)
#	execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/src/Shaders ${CMAKE_BINARY_DIR}/content/Shader)
#	execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/trace.sh ${CMAKE_BINARY_DIR}/trace.sh)
#	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Cache/Font)
	execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/src/Lua/Scripts ${CMAKE_BINARY_DIR}/enginescripts)
endif()


set(LIBRARY_OUTPUT_PATH
${PROJECT_BINARY_DIR})

set(EXECUTABLE_OUTPUT_PATH
${PROJECT_BINARY_DIR})

add_subdirectory(src)

if (BUILD_GLAKKEGAME)
	add_subdirectory(src/glakkegame)
endif()

if (BUILD_GLAKKEDITOR)
	add_subdirectory(src/glakkeditor)
endif()

get_property(LOCAL_CPPCHECK_INCLUDE GLOBAL PROPERTY CPPCHECK_INCLUDE)
message(STATUS "Generating cppcheck script..")
foreach(type "style" "performance" "portability" "information" "unusedFunction" "missingInclude" "all")
	set(CPPCHECK_INC_FILE "${PROJECT_BINARY_DIR}/cppcheck/cppcheck-${type}.sh")
	file(WRITE ${CPPCHECK_INC_FILE} "#!/bin/bash\n")
	file(APPEND ${CPPCHECK_INC_FILE} "cppcheck --std=c++11 --enable=${type} -j 5 --report-progress -D__cplusplus --force -v --xml -i ${CMAKE_CURRENT_SOURCE_DIR}/src/lib -i ${CMAKE_CURRENT_SOURCE_DIR}/src/glakkeditor")
	foreach(dir ${LOCAL_CPPCHECK_INCLUDE})
		file(APPEND ${CPPCHECK_INC_FILE} " -I ${dir}")
	endforeach()
	file(APPEND ${CPPCHECK_INC_FILE}  " ${CMAKE_CURRENT_SOURCE_DIR}/src 2> ${PROJECT_BINARY_DIR}/cppcheck-${type}.xml")
endforeach()
