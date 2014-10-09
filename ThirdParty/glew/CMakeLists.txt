cmake_minimum_required(VERSION 2.8)
project(glew C)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/glew/include")

add_library(glew STATIC glew/src/glew.c)
set_property(TARGET glew APPEND PROPERTY COMPILE_DEFINITIONS GLEW_STATIC)
