#include "StdioSearchPath.h"

#include <string>
#include <cstdio>

StdioSearchPath::StdioSearchPath(string path)
{
	base = path;
}

bool StdioSearchPath::Exists(string name)
{
	FILE *f = nullptr;
	f = fopen(ConstructPath(name).c_str(), "rb");
	if (f)
	{
		fclose(f);
		return true;
	}
	return false;
}

IntFileHandle StdioSearchPath::Open(string name, FileOpen options)
{
	FILE *f = nullptr;
	f = fopen(ConstructPath(name).c_str(), (options & FileOpenWrite) ? "wb" : "rb");
	return f;
}

void StdioSearchPath::Close(IntFileHandle handle)
{
	if (handle != nullptr)
		fclose(static_cast<FILE *>(handle));
}

void StdioSearchPath::Seek(IntFileHandle handle, int pos, FileSeek origin)
{
	if (handle != nullptr)
		fseek(static_cast<FILE *>(handle), pos, origin);
}

unsigned int StdioSearchPath::Tell(IntFileHandle handle)
{
	if (handle != nullptr)
		return ftell(static_cast<FILE *>(handle));

	return 0;
}

unsigned int StdioSearchPath::Size(IntFileHandle handle)
{
	FILE *f = static_cast<FILE *>(handle);

	int prev = ftell(f);
	fseek(f, 0L, SEEK_END);
	int len = ftell(f);
	fseek(f, prev, SEEK_SET);
	return len;
}

string StdioSearchPath::ConstructPath(string name)
{
	return base + "/" + name;
}