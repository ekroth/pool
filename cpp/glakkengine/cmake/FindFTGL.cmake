# Find ALURE library
# Once done this will define
# FTGL_FOUND
# FTGL_LIBRARY
# FTGL_INCLUDE_DIR
# Glaucous 2011 <glakke1 at gmail dot com>

FIND_PATH(
FTGL_INCLUDE_DIR

NAMES
ftgl.h
ftgl/ftgl.h
FTGL/ftgl.h

DOC
"Directory where ftgl.h resides"
)

FIND_LIBRARY(
FTGL_LIBRARY

NAMES
ftgl

DOC
"FTGL library location")

INCLUDE(Modules/FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FTGL DEFAULT_MSG FTGL_LIBRARY FTGL_INCLUDE_DIR)