cmake_minimum_required(VERSION 2.8)

find_package(OpenGL REQUIRED)

add_executable(Engine
	Private/Main.cpp
	Private/Platform/SDLApplication.cpp
	Private/Platform/SDLApplication.h
	Private/Filesystem/Filesystem.cpp
	Private/Filesystem/Filesystem.h
	Private/Filesystem/StdioSearchPath.cpp
	Private/Filesystem/StdioSearchPath.h
	Private/Filesystem/ZipSearchPath.cpp
	Private/Filesystem/ZipSearchPath.h
	Private/Filesystem/ISearchPath.h
	Private/Filesystem/PathUtils.cpp
	Private/Filesystem/PathUtils.h
	Private/Rendering/GL/GLRenderer.cpp
	Private/Rendering/GL/GLRenderer.h
	Private/Rendering/GL/GLTexture.cpp
	Private/Rendering/GL/GLTexture.h
	Private/Rendering/GL/GLShader.cpp
	Private/Rendering/GL/GLShader.h
	Public/IApplication.h
	Public/IFilesystem.h
	Public/IRenderer.h
	Public/IShader.h
	Public/ITexture.h
	${PUBLIC_INCLUDE}/miniz.c
	${PUBLIC_INCLUDE}/miniz.h
)

source_group(Public REGULAR_EXPRESSION Public/.*)
source_group(Private\\Platform REGULAR_EXPRESSION Platform/.*)
source_group(Private\\Filesystem REGULAR_EXPRESSION Filesystem/.*)
source_group(Private\\Rendering\\GL REGULAR_EXPRESSION Rendering/GL/.*)
source_group(Private\\Rendering REGULAR_EXPRESSION Rendering/.*)
source_group(Private REGULAR_EXPRESSION Main.cpp)

target_include_directories(Engine PUBLIC
	${EXT_LIBS}/glew/glew/include
	${EXT_LIBS}/SDL2/include
	${PUBLIC_INCLUDE}
	${OPENGL_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/Public
)

target_link_libraries(Engine glew SDL2 SDL2main ${OPENGL_LIBRARIES})

set_property(TARGET Engine APPEND PROPERTY COMPILE_DEFINITIONS GLEW_STATIC)

include_directories()
