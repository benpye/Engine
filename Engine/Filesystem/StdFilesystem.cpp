#include "StdFilesystem.h"

#include <algorithm>
#include <cstdio>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// We use cstdio as we provide a more C like api to the filesystem
// This maps easier to the cstdio functions than to iostreams

StdFilesystem::StdFilesystem()
{
	// By default mount the directory of our main executable
	searchPath.push_back(GetApplicationDirectory());
}

string StdFilesystem::GetApplicationDirectory()
{
#ifdef WIN32
	TCHAR buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	return GetParentDirectory(string(buffer));
#else
#error "UNSUPPORTED ON THIS PLATFORM"
#endif
}

FileHandle StdFilesystem::Open(string name, FileOptions options)
{
	StdFilesystemFileHandle *handle = new StdFilesystemFileHandle();

	// Filenames need sanitizing, currently we can ../ up to root if we want

	string filename;

	if ((options & Write) != 0)
	{
		filename = writePath + '/' + name;
		handle->handle = fopen(filename.c_str(), "r+b");
	}
	else
	{
		for (auto path : searchPath)
		{
			filename = path + '/' + name;

			handle->handle = fopen(filename.c_str(), "rb");
			if (handle->handle != nullptr)
				break;
		}
	}

	if (handle->handle != nullptr)
		handle->isInitialised = true;

	return handle;
}

void StdFilesystem::Close(FileHandle opaque)
{
	StdFilesystemFileHandle *handle = static_cast<StdFilesystemFileHandle *>(opaque);

	if (handle->isInitialised == true)
		fclose(handle->handle);

	handle->isInitialised = false;

	delete handle;
}

bool StdFilesystem::IsValid(FileHandle opaque)
{
	StdFilesystemFileHandle *handle = static_cast<StdFilesystemFileHandle *>(opaque);
	return handle != nullptr && handle->isInitialised;
}

void StdFilesystem::SetWritePath(string path)
{
	writePath = path;
}

void StdFilesystem::AddSearchPath(string path)
{
	searchPath.push_back(path);
}

void StdFilesystem::RemoveSearchPath(string path)
{
	searchPath.erase(remove(searchPath.begin(), searchPath.end(), path), searchPath.end());
}

vector<string> StdFilesystem::GetSearchPath()
{
	return searchPath;
}