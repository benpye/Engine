#pragma once

#include <string>

struct Configuration
{
	int Width;
	int Height;

	bool Fullscreen;
	bool Vsync;

	std::string WindowTitle;
};

class IApplication
{
public:
	virtual ~IApplication() {}

	virtual void SwapBuffers() = 0;
	virtual void SetConfiguration(const Configuration &config) = 0;
};