#pragma once

#include <string>
#include <vector>

using namespace std;

typedef void* FileHandle;

enum FileOpen
{
	FileOpenNone = 0,
	FileOpenWrite = 1
};

enum FileSeek
{
	FileSeekHead = SEEK_SET,
	FileSeekCurrent = SEEK_CUR,
	FileSeekTail = SEEK_END
};

class IFilesystem
{
public:
	virtual ~IFilesystem() {}

	virtual FileHandle Open(const string &name, FileOpen options = FileOpenNone) = 0;
	virtual void Close(FileHandle file) = 0;
	virtual unsigned int Size(FileHandle file) = 0;
	virtual void Seek(FileHandle file, int pos, FileSeek origin) = 0;
	virtual unsigned int Tell(FileHandle file) = 0;
	virtual unsigned int Read(FileHandle file, void *buf, unsigned int size) = 0;
	virtual unsigned int Write(FileHandle file, const void *buf, unsigned int size) = 0;

	virtual bool Exists(const string &name) = 0;

	virtual vector<string> FileFind(const string &wildcard) = 0;
	
	virtual void SetWritePath(const string &path) = 0;
	virtual vector<string> GetSearchPath() = 0;
	virtual void RemoveSearchPath(const string &path) = 0;
	virtual void AddSearchPath(const string &path) = 0;

	static string GetParentDirectory(const string &name)
	{
		size_t pos = name.find_last_of("\\/");
		return (std::string::npos == pos)
			? ""
			: name.substr(0, pos);
	}
};