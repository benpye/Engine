#include <IApplication.h>
#include <IFilesystem.h>
#include <ITexture.h>
#include <IRenderer.h>

#include "Platform/SDLApplication.h"
#include "Filesystem/Filesystem.h"

#include "Rendering/GL/GLRenderer.h"
#include "Rendering/GL/GLTexture.h"

#include <GLBase.h>

#include <stb_image.h>

int main(int argc, char** argv)
{
	Configuration config;
	config.Fullscreen = false;
	config.Vsync = true;
	config.Width = 800;
	config.Height = 600;
	config.WindowTitle = "Hello World!";

	IApplication* app = new SDLApplication(config);

	IFilesystem* fs = new Filesystem();

	fs->SetWritePath(Filesystem::GetApplicationDirectory());
	fs->AddSearchPath(fs->RelativeToFullPath("Test.zip"));

	FileHandle f = fs->Open("Test.png");
	int flen = fs->Size(f);
	unsigned char *buf = new unsigned char[flen];
	fs->Read(f, buf, flen);
	fs->Close(f);

	int w, h, comp;
	unsigned char *image = stbi_load_from_memory(buf, flen, &w, &h, &comp, STBI_rgb_alpha);

	GLRenderer *r = new GLRenderer();
	auto t = r->CreateTexture();
	t->Init(w, h, TextureFormat::RGBA8, image);

	glewExperimental = true;
	glewInit();

	float x = 0.0f;

	while (true)
	{
		glClearColor(abs(sinf(x)), abs(sinf(x + 1.57079632679f)), abs(sinf(x + 0.78539816339f)), 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		app->SwapBuffers();

		x += 0.05f;
	}

	return 0;
}