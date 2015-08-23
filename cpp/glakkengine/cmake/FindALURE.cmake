# Find ALURE library
# Once done this will define
# ALURE_FOUND
# ALURE_LIBRARY
# ALURE_LIBRARY_STATIC
# ALURE_INCLUDE_DIR
# Glaucous 2011 <glakke1 at gmail dot com>

FIND_PATH(
ALURE_INCLUDE_DIR

NAMES
alure.h
al/alure.h
AL/alure.h

DOC
"Directory where alure.h resides"
)

FIND_LIBRARY(
ALURE_LIBRARY

NAMES
alure

DOC
"ALURE shared library location")

FIND_LIBRARY(
ALURE_LIBRARY_STATIC

NAMES
alure-static

DOC
"ALURE static library location")

SET(ALURE_REQ_LIB "ALURE_LIBRARY")
IF (NOT ALURE_LIBRARY)
	SET(ALURE_REQ_LIB "ALURE_STATIC_LIBRARY")
ENDIF()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ALURE DEFAULT_MSG ${ALURE_REQ_LIB} ALURE_INCLUDE_DIR)