option(BUILD_GLAKKENGINE_SHARED "Set ON to build GlakkEngine as shared library" ON)

## >> External Libraries
option(BUILD_SHARED_EXT_LIBS "Set to OFF to build static libraries" ON)
option(BUILD_ALL_EXT_LIBS "Set to ON to build all possible libraries" OFF)

list(APPEND GlakkeLibs_INCLUDE lib)

if (BUILD_SHARED_EXT_LIBS)
	set(LIB_SRC_TYPE "SHARED")
else()
	set(LIB_SRC_TYPE "STATIC")
endif()

# -- TinyXml
find_package(TINYXML)
if(TINYXML_FOUND AND NOT BUILD_ALL_EXT_LIBS)
	list(APPEND GlakkeLibs_LIBRARIES ${TINYXML_LIBRARIES})
	list(APPEND GlakkeLibs_INCLUDE ${TINYXML_INCLUDE_DIR})
else()
	message(STATUS "Configuring internal tinyxml..")
	add_subdirectory(lib/tinyxml)
	list(APPEND GlakkeLibs_INCLUDE lib/tinyxml)
	list(APPEND GlakkeLibs_LIBRARIES tinyxml)
	message(STATUS ".. done configuring tinyxml.")
endif()

# -- ALURE
find_package(ALURE)
if(ALURE_FOUND AND NOT BUILD_ALL_EXT_LIBS)
	list(APPEND GlakkeLibs_LIBRARIES ${ALURE_LIBRARY_STATIC})
	list(APPEND GlakkeLibs_INCLUDE ${ALURE_INCLUDE_DIR})
else()
	message(STATUS "Configuring ALURE..")
	add_subdirectory(alure)
	if (BUILD_SHARED_EXT_LIBS)
		list(APPEND GlakkeLibs_LIBRARIES alure)
	else()
		list(APPEND GlakkeLibs_LIBRARIES alure-static)
	endif()
	list(APPEND GlakkeLibs_INCLUDE lib/alure/include/AL)
	message(STATUS ".. done configuring ALURE.")
endif()

# -- ConvertUTF (resides in simpleini)
message(STATUS "Configuring internal ConvertUTF..")
add_subdirectory(lib/simpleini)
list(APPEND GlakkeLibs_INCLUDE lib/simpleini)
list(APPEND GlakkeLibs_LIBRARIES ConvertUTF)
message(STATUS ".. done configuring ConvertUTF.")

# # -- ToLua
# find_package(TOLUA)
# if(TOLUA_FOUND AND NOT BUILD_ALL_EXT_LIBS)
# 	set_property(GLOBAL APPEND PROPERTY GLAKKENGINE_LIBS ${TOLUA_LIBRARIES})
# 	set_property(GLOBAL APPEND PROPERTY GLAKKENGINE_INCLUDE ${TOLUA_INCLUDE_DIR})
# else()
# 	message(FATAL_ERROR "Build ToLua!")
# #	message(STATUS "Configuring ALURE..")
# # 	add_subdirectory(alure)
# # 	if (BUILD_SHARED_EXT_LIBS)
# # 		set_property(GLOBAL APPEND PROPERTY GLAKKENGINE_LIBS alure)
# # 	else()
# # 		set_property(GLOBAL APPEND PROPERTY GLAKKENGINE_LIBS alure-static)
# # 	endif()
# # 	set_property(GLOBAL APPEND PROPERTY GLAKKENGINE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/alure/include/AL)
# # 	message(STATUS ".. done configuring ALURE.")
# endif()

# -- pugixml
if (PUGIXML_FOUND AND NOT BUILD_ALL_EXT_LIBS)

else()
	set(pugixml_sources "${CMAKE_CURRENT_SOURCE_DIR}/lib/pugixml/pugixml.cpp")
	list(APPEND GlakkeLibs_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/lib/pugixml")
	
	if (BUILD_SHARED_EXT_LIBS)
		add_library(pugixml SHARED ${pugixml_sources})
	else()
		add_library(pugixml STATIC ${pugixml_sources})
	endif()

	list(APPEND GlakkeLibs_LIBRARIES pugixml)
endif()

## << External Libraries

if (BUILD_GLAKKENGINE_SHARED)
	set(GLAKKENGINE_LIB_TYPE "SHARED")
else()
	set(GLAKKENGINE_LIB_TYPE "STATIC")
endif()

#file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Cache/Font)

# find_package(Boost COMPONENTS serialization thread REQUIRED)
# include(FindDevIL REQUIRED)
find_package(DevIL REQUIRED)
#find_package(BOX2D REQUIRED)
# find_package(OpenCL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
#find_package(FTGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(OpenAL REQUIRED)
find_package(Freetype REQUIRED)
find_package(Lua51 REQUIRED)
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

# list(APPEND LUA_FILES 
# "${CMAKE_CURRENT_SOURCE_DIR}/Lua/Api/LuaManager.pkg"
# "${CMAKE_CURRENT_SOURCE_DIR}/Lua/Api/EventData.pkg"
# "${CMAKE_CURRENT_SOURCE_DIR}/Lua/Api/EventManager.pkg"
# "${CMAKE_CURRENT_SOURCE_DIR}/Lua/Api/FNV1aHash.pkg"
# 
# )
#"${CMAKE_CURRENT_SOURCE_DIR}/Lua/Api/EventManager.pkg"

# # Process LUA files
# find_package(TOLUA REQUIRED)
# ToLua_PROCESS(
# 	GLAKKENGINE_LUA_INC
# 	GLAKKENGINE_LUA_SRC
# 
# 	"${LUA_FILES}")
# 
# 	message(STATUS ${GLAKKENGINE_LUA_INC})
# 	message(STATUS ${GLAKKENGINE_LUA_SRC})

set(GLAKKENGINE_INCLUDE
	${CMAKE_CURRENT_SOURCE_DIR}/../include

	${GlakkeLibs_INCLUDE}
	${CMAKE_BINARY_DIR}
	${CMAKE_BINARY_DIR}/Lua
	${LUA_INCLUDE_DIR}
	${IL_INCLUDE_DIR}
	${ALURE_INCLUDE_DIR}
	${GLEW_INCLUDE_PATH}
	${GLFW_INCLUDE_DIR}
	${OPENAL_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIR_freetype2} 
	${FREETYPE_INCLUDE_DIR_ft2build}
)

set(GLAKKENGINE_LIBS 
	${GLAKKENGINE}

	${GlakkeLibs_LIBRARIES}
	pthread # Until gcc supports threads
	${OPENGL_LIBRARIES}
	${LUA_LIBRARY}
	${IL_LIBRARIES}
	${ILU_LIBRARIES}
	${DEVIL_LIBRARIES}
	${ALURE_LIBRARY}
	${GLEW_LIBRARY}
	${GLFW_LIBRARY}
	${OPENAL_LIBRARY}
	${FREETYPE_LIBRARY}
)

include_directories(
	${GLAKKENGINE_INCLUDE}
)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Lua/Api")
#list(APPEND LUA_i_Events
#"Lua/Api/EventData.i")
#list(APPEND LUA_i_Events
#"Lua/Api/FNV1aHash.i")

set(CMAKE_SWIG_FLAGS "-c++")
set(CMAKE_SWIG_OUTDIR "${CMAKE_BINARY_DIR}/Lua")
# set_source_files_properties(${LUA_i_Events} PROPERTIES CPLUSPLUS ON)
# set_source_files_properties(${LUA_i_Events} PROPERTIES CMAKE_SWIG_FLAGS "-includeall")
# swig_add_module(Events lua "${LUA_i_Events}")
# swig_link_libraries(Events "${LOCAL_GLAKKENGINE_LIBS}")
# 
include(UseSWIGH)
# kke_swig_gen_headers(Events "${LUA_i_Events}" "${CMAKE_BINARY_DIR}/Lua")

#kke_swig_create_module_list(Events "LUA_i_Events" "LOCAL_GLAKKENGINE_LIBS" "${CMAKE_BINARY_DIR}/Lua" kke_swigs)

#set_property(GLOBAL APPEND PROPERTY GLAKKENGINE_LIBS "${kke_swigs}")
#set_property(GLOBAL APPEND PROPERTY GLAKKENGINE_INCLUDE "${CMAKE_BINARY_DIR}/Lua")

include(UseFactory)
set(state_classes)

set(state_include)

kke_factory_registration(
StateFactory
State
"StateManager*;"
"${state_classes}"
"${state_include}"
kke)

kke_factory_register_all(
"StateFactory;"
kke)

add_library(
	${GLAKKENGINE}

	${GLAKKENGINE_LIB_TYPE}

	${KKE_FACTORY_SOURCE}

	Events

	GameApp.cpp

	GameTime.cpp
	ResourceType.cpp
	ResourceFactory.cpp

	Events/EventManager.cpp
	Events/EventData.cpp
	Events/ScopedListener.cpp
	Events/PassEventData.cpp

	Graphics/VertexArray.cpp
	Graphics/Buffer.cpp
	Graphics/Font.cpp
	Graphics/FTManager.cpp
	Graphics/GfxCache.cpp
	Graphics/GfxManager.cpp
	Graphics/RenderThread.cpp
	Graphics/GLManager.cpp
	Graphics/GLException.cpp
	Graphics/Glyph.cpp	
	Graphics/IRenderer.cpp
	Graphics/ShaderPart.cpp
	Graphics/SpriteSheet.cpp
	Graphics/Texture2D.cpp
	Graphics/TextureBatch.cpp
	Graphics/FrameBuffer.cpp
	Graphics/RenderBuffer.cpp
	Graphics/Shader.cpp
	Graphics/Camera.cpp

	IO/ILog.cpp
	IO/LogOStream.cpp
	IO/LogManager.cpp
	IO/ArgParser.cpp
	IO/Argument.cpp
	IO/IniConfig.cpp
	IO/XmlConfig.cpp

	Input/InputManager.cpp

	Lua/LuaManager.cpp
	Lua/LuaScript.cpp
	Lua/LuaAddon.cpp

	Math/Randomizer.cpp
	Math/PackNode.cpp

	Sound/SoundManager.cpp
	Sound/SoundBuffer.cpp
	Sound/Sound.cpp

	States/State.cpp
	States/StateManager.cpp
)

target_link_libraries(
	${GLAKKENGINE}

	${GLAKKENGINE_LIBS})

foreach (dir ${GLAKKENGINE_INCLUDE})
	set_property(GLOBAL APPEND PROPERTY CPPCHECK_INCLUDE ${dir})
endforeach()