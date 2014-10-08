#pragma once

#include <string>

using namespace std;

struct Configuration
{
	int Width;
	int Height;

	bool Fullscreen;
	bool Vsync;

	string WindowTitle;
};

class IApplication
{
public:
	virtual void SwapBuffers() = 0;
	virtual void SetConfiguration(Configuration * config) = 0;
};