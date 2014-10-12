#include "StdioSearchPath.h"

#include <string>
#include <cstdio>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// We use cstdio as we provide a more C like api to the filesystem
// This maps easier to the cstdio functions than to iostreams
// It is also supported more widely (emscripten)

StdioSearchPath::StdioSearchPath(const std::string &path)
{
	base = path;
}

bool StdioSearchPath::Exists(const std::string &name)
{
	FILE *f = nullptr;

#ifdef _MSC_VER
	fopen_s(&f, ConstructPath(name).c_str(), "rb");
#else
	f = fopen(ConstructPath(name).c_str(), "rb");
#endif

	if (f)
	{
		fclose(f);
		return true;
	}

	return false;
}

IntFileHandle StdioSearchPath::Open(const std::string &name, FileOpen options)
{
	FILE *f = nullptr;

#ifdef _MSC_VER
	fopen_s(&f, ConstructPath(name).c_str(), flags(options & FileOpen::Write) ? "wb" : "rb");
#else
	f = fopen(ConstructPath(name).c_str(), flags(options & FileOpen::Write) ? "wb" : "rb");
#endif

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
		fseek(static_cast<FILE *>(handle), pos, static_cast<int>(origin));
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

std::vector<std::string> StdioSearchPath::ListDirectory(const std::string &path)
{
	std::vector<std::string> ls;
#ifdef WIN32
	std::string filter = ConstructPath(path) + "/*";
	WIN32_FIND_DATA findData;
	HANDLE findHandle = nullptr;

	if ((findHandle = FindFirstFile(filter.c_str(), &findData)) != INVALID_HANDLE_VALUE)
	{
		std::string name;

		do
		{
			if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
			{
				name = std::string(findData.cFileName);
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					name += '/';

				ls.push_back(name);
			}
		} while (FindNextFile(findHandle, &findData));

		FindClose(findHandle);
	}
#else
#error "UNSUPPORTED ON THIS PLATFORM"
#endif
	return ls;
}

std::string StdioSearchPath::ConstructPath(const std::string &name)
{
	return base + "/" + name;
}