# Find TINYXML library
# Once done this will define
# TINYXML_FOUND
# TINYXML_LIBRARY
# TINYXML_INCLUDE_DIR
# Glaucous 2011 <glakke1 at gmail dot com>

FIND_PATH(
TINYXML_INCLUDE_DIR

NAMES
tinyxml.h
tinyxml/tinyxml.h

DOC
"Directory where tinyxml.h resides"
)

FIND_LIBRARY(
TINYXML_LIBRARIES

NAMES
tinyxml

DOC
"TinyXml shared library location")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TINYXML DEFAULT_MSG TINYXML_LIBRARIES TINYXML_INCLUDE_DIR)