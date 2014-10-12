#pragma once

#include "IApplication.h"
#include <SDL.h>

class SDLApplication : public IApplication
{
public:
	SDLApplication(const Configuration& config);
	~SDLApplication();

	// Disallow copy
	SDLApplication(const SDLApplication& other) = delete;
	SDLApplication& operator=(const SDLApplication& other) = delete;

	virtual void SwapBuffers() override;
	virtual void SetConfiguration(const Configuration& config) override;
private:
	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;
};