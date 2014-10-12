#pragma once

#include "IApplication.h"
#include <SDL.h>

class SDLApplication : public IApplication
{
public:
	SDLApplication(const Configuration &config);
	~SDLApplication();
	virtual void SwapBuffers() override;
	virtual void SetConfiguration(const Configuration &config) override;
private:
	SDL_Window *window  = nullptr;
	SDL_GLContext context = nullptr;
};