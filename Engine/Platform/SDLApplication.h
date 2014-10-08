#pragma once

#include "IApplication.h"
#include <SDL.h>

class SDLApplication : public IApplication
{
public:
	SDLApplication(Configuration *config);
	~SDLApplication();
	void SwapBuffers() override;
	void SetConfiguration(Configuration* config) override;
private:
	SDL_Window *window  = nullptr;
	SDL_GLContext context = nullptr;
};