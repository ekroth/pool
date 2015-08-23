# Find packages

# ------------------------
# GLEW
# ------------------------

find_package(GLEW REQUIRED)

if(${GLEW_FOUND} MATCHES "YES")
	message(STATUS "GLEW found!")
else(${GLEW_FOUND} MATCHES "YES")
	message(FATAL_ERROR "GLEW not found!")
endif()

# ------------------------
# Qt4
# ------------------------

set(QT_MIN_VERSION "4.3.0")

find_package(Qt4 COMPONENTS QtCore QtGui QtOpengl REQUIRED)

if(${QT4_FOUND} MATCHES "YES")
	message(STATUS "Qt4 found! (${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH})")
else()
	message(FATAL_ERROR "Qt4 not found!")
endif()

if(${QT_QTCORE_FOUND} MATCHES "1")
	message(STATUS "QtCore found!")
else()
	message(FATAL_ERROR "QtCore not found!")
endif()

if(${QT_QTGUI_FOUND} MATCHES "1")
	message(STATUS "QtGui found!")
else()
	message(FATAL_ERROR "QtGui not found!")
endif()

if(${QT_QTOPENGL_FOUND} MATCHES "1")
	message(STATUS "QtOpenGL found!")
else()
	message(FATAL_ERROR "QtOpenGL not found!")
endif()

# ------------------------
# Boost
# ------------------------

find_package(Boost COMPONENTS serialization thread REQUIRED)

if(${Boost_FOUND} MATCHES "TRUE")
	message(STATUS "Boost found! (${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION})")
else()
	message(FATAL_ERROR "Boost not found!")
endif()


set(Glakkeditor_HEADERS

	Windows/EditorWindow.hpp
	Widgets/MainGLWidget.hpp 
)

set(Glakkeditor_SOURCES
	
	main.cpp

	Editor/Commands/LevelItemName.cpp
	Editor/Commands/LevelItemInsert.cpp
	Editor/Commands/LayerItemInsert.cpp
	Editor/Commands/LevelItemErase.cpp
	Editor/Commands/LayerItemErase.cpp
	Editor/Commands/LevelName.cpp
	Editor/Commands/LayerSpeed.cpp
	Editor/Commands/LayerTextureSource.cpp
	Editor/Commands/LayerTexturePosition.cpp
	Editor/Commands/LayerTextureOrigin.cpp
	Editor/Commands/LayerTextureColor.cpp
	Editor/Commands/LayerTextureRotation.cpp
	Editor/Commands/LayerTextureScale.cpp
	Editor/Commands/LayerTextureTexcoords.cpp

	Editor/Selectable.cpp

	Widgets/LevelTreeWidgetItem.cpp
	Widgets/MainGLWidget.cpp 
	Widgets/TreeLevelView.cpp

	Windows/EditorWindow.cpp
)

# Set UI files
set(Glakkeditor_FORMS

	Design/MainWindow.ui
)

# See http://developer.qt.nokia.com/quarterly/view/using_cmake_to_build_qt_projects
qt4_wrap_cpp(Glakkeditor_HEADERS_MOC ${Glakkeditor_HEADERS})
qt4_wrap_ui(Glakkeditor_FORMS_HEADERS ${Glakkeditor_FORMS})
add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})

get_property(LOCAL_GLAKKENGINE_INCLUDE GLOBAL PROPERTY GLAKKENGINE_INCLUDE)
get_property(LOCAL_GLAKKENGINE_LIBS GLOBAL PROPERTY GLAKKENGINE_LIBS)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/src

	${LOCAL_GLAKKENGINE_INCLUDE}
	${Boost_INCLUDE_DIRS}
	${GLEW_INCLUDE_PATH}
)

add_executable(
    ${GLAKKEDITOR} 
    
    ${Glakkeditor_SOURCES} 
    ${Glakkeditor_HEADERS_MOC} 
    ${Glakkeditor_FORMS_HEADERS}
)

target_link_libraries(
    ${GLAKKEDITOR}
    
	${LOCAL_GLAKKENGINE_LIBS}
	${Boost_LIBRARIES}
    ${QT_LIBRARIES}
    ${GLEW_LIBRARIES}
)