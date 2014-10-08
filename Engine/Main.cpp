#include "Platform/IApplication.h"
#include "Platform/SDLApplication.h"

#include <GL/glew.h>

int main(int argc, char **argv)
{
	Configuration config;
	config.Fullscreen  = false;
	config.Vsync       = true;
	config.Width       = 800;
	config.Height      = 600;
	config.WindowTitle = "Hello World!";

	IApplication *app = new SDLApplication(&config);

	glewExperimental = true;
	glewInit();

	float x = 0.0f;

	while (true)
	{
		glClearColor(sinf(x), cosf(x), 1 - sinf(x), 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		app->SwapBuffers();

		x += 0.1f;
	}

	return 0;
}