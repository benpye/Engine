cmake_minimum_required(VERSION 2.8)

find_package(OpenGL REQUIRED)

add_executable(Engine
	Main.cpp
	Platform/SDLApplication.cpp
	Platform/SDLApplication.h
	Platform/IApplication.h
)

source_group(Source\\Platform REGULAR_EXPRESSION Platform/.*)
source_group(Source REGULAR_EXPRESSION Main.cpp)

target_include_directories(Engine PUBLIC
	${EXT_LIBS}/glew/include
	${EXT_LIBS}/SDL2/include
	${OPENGL_INCLUDE_DIRS}
)

target_link_libraries(Engine glew SDL2 SDL2main ${OPENGL_LIBRARIES})

set_property(TARGET Engine APPEND PROPERTY COMPILE_DEFINITIONS GLEW_STATIC)

include_directories()