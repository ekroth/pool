# Find Box2D library
# Once done this will define
# BOX2D_FOUND
# Box2D_LIBRARY
# BOX2D_INCLUDE_DIR
# Glaucous 2011 <glakke1 at gmail dot com>

FIND_PATH(
BOX2D_INCLUDE_DIR

NAMES
Box2D.h
box2d.h
Box2D/Box2D.h
box2d/box2d.h

DOC
"Directory where Box2D.h resides")

FIND_LIBRARY(
BOX2D_LIBRARIES

NAMES
Box2D
box2d
box2D
Box2d

DOC
"Box2D shared library location")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BOX2D DEFAULT_MSG BOX2D_LIBRARIES BOX2D_INCLUDE_DIR)