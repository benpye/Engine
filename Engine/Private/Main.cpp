#include <IApplication.h>
#include <IFilesystem.h>

#include "Platform/SDLApplication.h"
#include "Filesystem/Filesystem.h"

#include <GL/glew.h>

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

	auto testlist = fs->FileFind("*");
	FileExists exists = FileExists::None;
	exists = fs->Exists("glew-1.11.0/src/glew.c");
	fs->Remove("Test2.txt");
	FileHandle f = fs->Open("test.txt", FileOpen::Write);
	char* test = "Hello world!\n";
	fs->Write(f, test, strlen(test));
	unsigned int sz = fs->Size(f);
	fs->Close(f);

	auto t = fs->Open("glew-1.11.0/README.txt");
	fs->Seek(t, 4, FileSeek::Head);
	fs->Seek(t, -2, FileSeek::Current);
	char *buf = new char[fs->Size(t)];
	fs->Read(t, buf, fs->Size(t));

	bool r = fs->CreateDirectoryHierarchy("Test/Dir/Structure");

	auto e2 = fs->Exists("TestDirectory");
	auto e3 = fs->Exists("glew-1.11.0");
	auto e4 = fs->Exists("SDL2.dll");

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