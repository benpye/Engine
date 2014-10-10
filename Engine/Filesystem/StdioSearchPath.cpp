#include "StdioSearchPath.h"

#include <string>
#include <cstdio>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

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

unsigned int StdioSearchPath::Size(IntFileHandle handle)
{
	FILE *f = static_cast<FILE *>(handle);

	int prev = ftell(f);
	fseek(f, 0L, SEEK_END);
	int len = ftell(f);
	fseek(f, prev, SEEK_SET);
	return len;
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

unsigned int StdioSearchPath::Read(IntFileHandle handle, void *buf, unsigned int size)
{
	if (handle != nullptr)
		return fread(buf, 1, size, static_cast<FILE *>(handle));

	return 0;
}

unsigned int StdioSearchPath::Write(IntFileHandle handle, const void *buf, unsigned int size)
{
	if (handle != nullptr)
		return fwrite(buf, 1, size, static_cast<FILE *>(handle));

	return 0;
}

vector<string> StdioSearchPath::ListDirectory(string path)
{
	vector<string> ls;
#ifdef WIN32
	string filter = ConstructPath(path) + "/*.*";
	WIN32_FIND_DATA findData;
	HANDLE findHandle = nullptr;

	if ((findHandle = FindFirstFile(filter.c_str(), &findData)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
			{
				ls.push_back(findData.cFileName);
			}
		} while (FindNextFile(findHandle, &findData));

		FindClose(findHandle);
	}
#else
#error "UNSUPPORTED ON THIS PLATFORM"
#endif
	return ls;
}

string StdioSearchPath::ConstructPath(string name)
{
	return base + "/" + name;
}