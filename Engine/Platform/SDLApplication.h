#pragma once

#include "IApplication.h"
#include <SDL.h>

class SDLApplication : public IApplication
{
public:
	SDLApplication(Configuration *config);
	~SDLApplication();
	virtual void SwapBuffers() override;
	virtual void SetConfiguration(Configuration* config) override;
private:
	SDL_Window *window  = nullptr;
	SDL_GLContext context = nullptr;
};