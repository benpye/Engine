#pragma once

#include <string>
#include <vector>

#include <enum_flags.h>

typedef void* FileHandle;

ENUM_FLAGS(FileOpen)
enum class FileOpen : int
{
	None = 0,
	Write = 1
};

enum class FileSeek : int
{
	Head = SEEK_SET,
	Current = SEEK_CUR,
	Tail = SEEK_END
};

enum class FileExists : int
{
	None,
	File,
	Directory
};

class IFilesystem
{
public:
	virtual ~IFilesystem() {}

	virtual FileHandle Open(const std::string& name, FileOpen options = FileOpen::None) = 0;
	virtual void Close(FileHandle file) = 0;
	virtual unsigned int Size(FileHandle file) = 0;
	virtual void Seek(FileHandle file, int pos, FileSeek origin) = 0;
	virtual unsigned int Tell(FileHandle file) = 0;
	virtual unsigned int Read(FileHandle file, void* buf, unsigned int size) = 0;
	virtual unsigned int Write(FileHandle file, const void* buf, unsigned int size) = 0;

	virtual std::string RelativeToFullPath(const std::string &name) = 0;
	virtual bool CreateDirectoryHierarchy(const std::string &name) = 0;
	virtual bool Remove(const std::string& name) = 0;
	virtual FileExists Exists(const std::string& name) = 0;

	virtual std::vector<std::string> FileFind(const std::string& wildcard) = 0;

	virtual void SetWritePath(const std::string& path) = 0;
	virtual std::vector<std::string> GetSearchPath() = 0;
	virtual void RemoveSearchPath(const std::string& path) = 0;
	virtual void AddSearchPath(const std::string& path) = 0;

	static std::string GetParentDirectory(const std::string& name)
	{
		size_t pos = name.find_last_of("\\/");
		return (std::string::npos == pos)
			? "" : name.substr(0, pos);
	}
};