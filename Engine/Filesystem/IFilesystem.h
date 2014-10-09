#pragma once

#include <string>
#include <vector>

using namespace std;

typedef void* FileHandle;

enum FileOptions
{
	None = 0,
	Write = 1
};

class IFilesystem
{
public:
	virtual FileHandle Open(string name, FileOptions options = None) = 0;
	virtual void Close(FileHandle file) = 0;

	// This is to be used to check if open was successful, not if a file handle
	// has been closed. This may return true even if the handle is invalid if
	// close has been called.
	virtual bool IsValid(FileHandle file) = 0;
	
	virtual void SetWritePath(string path) = 0;
	virtual vector<string> GetSearchPath() = 0;
	virtual void RemoveSearchPath(string path) = 0;
	virtual void AddSearchPath(string path) = 0;

	static string GetParentDirectory(string name)
	{
		size_t pos = name.find_last_of("\\/");
		return (std::string::npos == pos)
			? ""
			: name.substr(0, pos);
	}
};