#include "StdioSearchPath.h"
#include "PathUtils.h"

#include <string>
#include <cstdio>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// We use cstdio as we provide a more C like api to the filesystem
// This maps easier to the cstdio functions than to iostreams
// It is also supported more widely (emscripten)

StdioSearchPath::StdioSearchPath(const std::string& path)
{
#ifdef WIN32
	char full[MAX_PATH];
	_fullpath(full, path.c_str(), MAX_PATH);
	base = std::string(full);
#else
#error "UNSUPPORTED ON THIS PLATFORM"
#endif
}

std::string StdioSearchPath::RelativeToFullPath(const std::string& name)
{
	return base + "/" + name;
}

bool StdioSearchPath::CreateDirectoryHierarchy(std::string const& name)
{
#ifdef WIN32
	auto splitPath = PathUtils::SplitPath(RelativeToFullPath(name));
	std::string fullDir = "";
	BOOL r;
	DWORD dAttribs;

	for (auto dir : splitPath)
	{
		fullDir += dir;
		fullDir += '/';
		dAttribs = GetFileAttributes(fullDir.c_str());
		if (!(dAttribs != INVALID_FILE_ATTRIBUTES && (dAttribs & FILE_ATTRIBUTE_DIRECTORY)))
		{
			r = CreateDirectory(fullDir.c_str(), nullptr);
			if (r == 0)
				return false;
		}
	}

	return true;
#else
#error "UNSUPPORTED ON THIS PLATFORM"
#endif
}

bool StdioSearchPath::Remove(const std::string& name)
{
	return (remove(RelativeToFullPath(name).c_str()) == 0);
}

bool StdioSearchPath::Exists(const std::string& name)
{
	FILE* f = nullptr;

#ifdef _MSC_VER
	fopen_s(&f, RelativeToFullPath(name).c_str(), "rb");
#else
	f = fopen(RelativeToFullPath(name).c_str(), "rb");
#endif

	if (f)
	{
		fclose(f);
		return true;
	}

	return false;
}

IntFileHandle StdioSearchPath::Open(const std::string& name, FileOpen options)
{
	FILE* f = nullptr;

#ifdef _MSC_VER
	fopen_s(&f, RelativeToFullPath(name).c_str(), flags(options & FileOpen::Write) ? "wb" : "rb");
#else
	f = fopen(RelativeToFullPath(name).c_str(), flags(options & FileOpen::Write) ? "wb" : "rb");
#endif

	return f;
}

void StdioSearchPath::Close(IntFileHandle file)
{
	if (file != nullptr)
		fclose(static_cast<FILE *>(file));
}

unsigned int StdioSearchPath::Size(IntFileHandle file)
{
	FILE* f = static_cast<FILE *>(file);

	int prev = ftell(f);
	fseek(f, 0L, SEEK_END);
	int len = ftell(f);
	fseek(f, prev, SEEK_SET);
	return len;
}

void StdioSearchPath::Seek(IntFileHandle file, int pos, FileSeek origin)
{
	if (file != nullptr)
		fseek(static_cast<FILE *>(file), pos, static_cast<int>(origin));
}

unsigned int StdioSearchPath::Tell(IntFileHandle file)
{
	if (file != nullptr)
		return ftell(static_cast<FILE *>(file));

	return 0;
}

unsigned int StdioSearchPath::Read(IntFileHandle file, void* buf, unsigned int size)
{
	if (file != nullptr)
		return fread(buf, 1, size, static_cast<FILE *>(file));

	return 0;
}

unsigned int StdioSearchPath::Write(IntFileHandle file, const void* buf, unsigned int size)
{
	if (file != nullptr)
		return fwrite(buf, 1, size, static_cast<FILE *>(file));

	return 0;
}

std::vector<std::string> StdioSearchPath::ListDirectory(const std::string& path)
{
	std::vector<std::string> ls;
#ifdef WIN32
	std::string filter = RelativeToFullPath(path) + "/*";
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
		}
		while (FindNextFile(findHandle, &findData));

		FindClose(findHandle);
	}
#else
#error "UNSUPPORTED ON THIS PLATFORM"
#endif
	return ls;
}