set(CMAKE_INCLUDE_PATH 
	${CMAKE_INCLUDE_PATH})

set(CMAKE_LIBRARY_PATH 
	${CMAKE_BINARY_DIR} 
	${CMAKE_LIBRARY_PATH})

#list(APPEND LUA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/LUA/character.pkg")
list(APPEND LUA_FILES 
"${CMAKE_CURRENT_SOURCE_DIR}/LUA/Buttfuck.pkg"
"${CMAKE_CURRENT_SOURCE_DIR}/LUA/Cake.pkg")


# Process LUA files
# find_package(TOLUA REQUIRED)
# ToLua_PROCESS(
# 	GLAKKE_LUA_INC
# 	GLAKKE_LUA_SRC
# 
# 	"${LUA_FILES}")
# 
# 	message(STATUS ${GLAKKE_LUA_INC})
# 	message(STATUS ${GLAKKE_LUA_SRC})

get_property(GLAKKENGINE_INCLUDE DIRECTORY ${CMAKE_SOURCE_DIR}/src PROPERTY INCLUDE_DIRECTORIES)

add_executable(
    ${GLAKKEGAME}
    
	#${GLAKKE_LUA_SRC}
    main.cpp
	GlakkeGame.cpp
 	States/OrthoState.cpp
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}

	${GLAKKE_LUA_INC}
	${GLAKKENGINE_INCLUDE}
)

target_link_libraries(
    ${GLAKKEGAME}
	glakkengine
)

set_property(GLOBAL APPEND PROPERTY CPPCHECK_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})