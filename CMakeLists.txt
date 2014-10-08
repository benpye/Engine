cmake_minimum_required(VERSION 2.8)

project(Engine)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
	${PROJECT_BINARY_DIR}/Bin
	CACHE PATH
	"Single Directory for all Libraries"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
	${PROJECT_BINARY_DIR}/Bin
	CACHE PATH
	"Single Directory for all Executables."
)

set(EXT_LIBS PATH ${CMAKE_SOURCE_DIR}/ThirdParty)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(SDL_STATIC  "Compile SDL statically" OFF)

add_subdirectory(ThirdParty)
add_subdirectory(Engine)