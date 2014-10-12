#include "Filesystem.h"
#include "StdioSearchPath.h"
#include "ZipSearchPath.h"

#include <algorithm>
#include <cstdio>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace std;

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

FileHandle Filesystem::Open(const string &name, FileOpen options)
{
	FSHandle *handle = new FSHandle();

	if (writePath != nullptr)
	{
		handle->searchPath = writePath;
		handle->fileHandle = writePath->Open(name, options);
		if (handle->fileHandle != nullptr)
			return handle;
	}

	if (flags(options & FileOpen::Write))
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

unsigned int Filesystem::Read(FileHandle handle, void *buf, unsigned int size)
{
	FSHandle *f = static_cast<FSHandle *>(handle);
	return f->searchPath->Read(f->fileHandle, buf, size);
}

unsigned int Filesystem::Write(FileHandle handle, const void *buf, unsigned int size)
{
	FSHandle *f = static_cast<FSHandle *>(handle);
	return f->searchPath->Write(f->fileHandle, buf, size);
}

bool Filesystem::Exists(const string &name)
{
	if (writePath != nullptr)
		if (writePath->Exists(name))
			return true;

	for (auto path : searchPath)
	{
		if (path.second->Exists(name))
			return true;
	}

	return false;
}

bool Filesystem::WildcardCompare(const string &str, const string &wildcard)
{
	unsigned int i = 0;
	char wc;
	char c;
	for (unsigned int j = 0; j < str.length(); j++)
	{
		if (i >= wildcard.length())
			return false;

		wc = wildcard[i];
		c = str[j];
		// Characters are the same
		if (c == wc)
		{
			i++;
		}
		// Single wildcard matches all also
		else if (wc == '?')
		{
			i++;
		}
		// Non greedy 0-n wildcard *
		else if (wc == '*')
		{
			// If the next wildcard character matches our current char
			if (wildcard[i + 1] == c)
				i+=2;
		}
		else
		{
			return false;
		}
	}

	if (wc == '*')
		i++;

	if (i != wildcard.length())
		return false;

	return true;
}

vector<string> Filesystem::FileFind(const string &wildcard)
{
	vector<string> ls;

	auto found = wildcard.find_last_of('/');
	string path = "";
	if(found != string::npos)
		path = wildcard.substr(0, found);

	string matchString = wildcard.substr(found + 1);

	if (writePath != nullptr)
		ls = writePath->ListDirectory(path);

	for (auto sp : searchPath)
	{
		vector<string> t = sp.second->ListDirectory(path);
		if (t.size() == 0) continue;
		
		// Join the vectors
		vector<string> n;
		n.reserve(t.size() + ls.size());
		n.insert(n.end(), ls.begin(), ls.end());
		n.insert(n.end(), t.begin(), t.end());
		ls = n;
	}

	// Sort then remove any duplicate entries
	sort(ls.begin(), ls.end());
	ls.erase(unique(ls.begin(), ls.end()), ls.end());
	
	// Filter by wildcard here, * means 0-n any character, ? means 1 any character
	for (auto i = ls.begin(); i != ls.end(); )
	{
		if (!WildcardCompare(*i, matchString))
			i = ls.erase(i);
		else
			++i;
	}

	return ls;
}

void Filesystem::SetWritePath(const string &path)
{
	// Write path is always stdio
	if (writePath != nullptr)
		delete writePath;

	writePath = new StdioSearchPath(path);
}

void Filesystem::AddSearchPath(const string &path)
{
	searchPathOrder.push_back(path);
	// Assume everything is stdio file system currently
	ISearchPath *internalPath;

	string extension = path.substr(path.length() - 4);

	if (extension == ".zip")
		internalPath = new ZipSearchPath(path);
	else
		internalPath = new StdioSearchPath(path);

	searchPath[path] = internalPath;
}

void Filesystem::RemoveSearchPath(const string &path)
{
	// Remove the path from both the ordered list and the interface list
	searchPathOrder.erase(remove(begin(searchPathOrder), end(searchPathOrder), path), end(searchPathOrder));
	searchPath.erase(path);
}

vector<string> Filesystem::GetSearchPath()
{
	return searchPathOrder;
}