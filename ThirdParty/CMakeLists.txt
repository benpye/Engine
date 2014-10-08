cmake_minimum_required(VERSION 2.8)

option(DIRECTX "Use DirectX for Windows audio/video" OFF)

add_subdirectory(SDL2)
set_property(TARGET SDL2 SDL2main PROPERTY FOLDER "ThirdParty")

if(SDL_STATIC)
	set_property(TARGET SDL2-static PROPERTY FOLDER "ThirdParty")
endif(SDL_STATIC)

add_subdirectory(glew)
set_property(TARGET glew PROPERTY FOLDER "ThirdParty")