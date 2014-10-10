#include "Filesystem.h"
#include "StdioSearchPath.h"

#include <algorithm>
#include <cstdio>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// We use cstdio as we provide a more C like api to the filesystem
// This maps easier to the cstdio functions than to iostreams

Filesystem::Filesystem()
{
	AddSearchPath(GetApplicationDirectory());
}

string Filesystem::GetApplicationDirectory()
{
#ifdef WIN32
	TCHAR buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	return GetParentDirectory(string(buffer));
#else
#error "UNSUPPORTED ON THIS PLATFORM"
#endif
}

FileHandle Filesystem::Open(string name, FileOpen options)
{
	FSHandle *handle = new FSHandle();

	if (writePath != nullptr)
	{
		handle->searchPath = writePath;
		handle->fileHandle = writePath->Open(name, options);
		if (handle->fileHandle != nullptr)
			return handle;
	}

	if (options & FileOpenWrite)
		return nullptr;

	for (auto path : searchPath)
	{
		if (path.second->Exists(name))
		{
			handle->searchPath = path.second;
			handle->fileHandle = path.second->Open(name, options);
			if (handle->fileHandle == nullptr)
				return nullptr;
			return handle;
		}
	}

	return nullptr;
}

void Filesystem::Close(FileHandle handle)
{
	FSHandle *f = static_cast<FSHandle *>(handle);
	f->searchPath->Close(f->fileHandle);
}

unsigned int Filesystem::Size(FileHandle handle)
{
	FSHandle *f = static_cast<FSHandle *>(handle);
	return f->searchPath->Size(f->fileHandle);
}

void Filesystem::Seek(FileHandle handle, int pos, FileSeek origin)
{
	FSHandle *f = static_cast<FSHandle *>(handle);
	return f->searchPath->Seek(f->fileHandle, pos, origin);
}

unsigned int Filesystem::Tell(FileHandle handle)
{
	FSHandle *f = static_cast<FSHandle *>(handle);
	return f->searchPath->Tell(f->fileHandle);
}

bool Filesystem::Exists(string name)
{
	for (auto path : searchPath)
	{
		if (path.second->Exists(name))
			return true;
	}

	return false;
}

void Filesystem::SetWritePath(string path)
{
	// Write path is always stdio
	if (writePath != nullptr)
		delete writePath;
	writePath = new StdioSearchPath(path);
}

void Filesystem::AddSearchPath(string path)
{
	searchPathOrder.push_back(path);
	// Assume everything is stdio file system currently
	ISearchPath *internalPath = new StdioSearchPath(path);
	searchPath[path] = internalPath;
}

void Filesystem::RemoveSearchPath(string path)
{
	// Remove the path from both the ordered list and the interface list
	searchPathOrder.erase(remove(begin(searchPathOrder), end(searchPathOrder), path), end(searchPathOrder));
	searchPath.erase(path);
}

vector<string> Filesystem::GetSearchPath()
{
	return searchPathOrder;
}