#include "SDLApplication.h"

#include <cstdlib>
#include <iostream>

using namespace std;

SDLApplication::SDLApplication(Configuration *config)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		exit(-1);
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	window = SDL_CreateWindow(
		config->WindowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		config->Width,
		config->Height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN |
		(config->Fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0)
		);

	if (window == nullptr)
	{
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		exit(-1);
	}

	context = SDL_GL_CreateContext(window);

	if (context == nullptr)
	{
		cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << endl;
		exit(-1);
	}

	if (config->Vsync)
	{
		// Prefer late swap tearing for vsync, fall back to standard vsync if unavaliable
		if (SDL_GL_SetSwapInterval(-1) < 0)
		{
			cout << "Warning: Late swap tearing not avaliable (" << SDL_GetError() << ")" << endl;
			if (SDL_GL_SetSwapInterval(1) < 0)
			{
				cout << "Warning: Vsync not avaliable (" << SDL_GetError() << ")" << endl;
			}
		}
	}


	int major, minor;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
}

SDLApplication::~SDLApplication()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void SDLApplication::SwapBuffers()
{
	SDL_GL_SwapWindow(window);
}

void SDLApplication::SetConfiguration(Configuration *config)
{
	// TODO: Add support for runtime changes to window configuration
}