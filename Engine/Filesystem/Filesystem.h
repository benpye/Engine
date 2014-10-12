#pragma once

#include "IFilesystem.h"
#include "ISearchPath.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdio>

struct FSHandle
{
	IntFileHandle fileHandle;
	ISearchPath *searchPath;
};

class Filesystem : public IFilesystem
{
public:
	Filesystem();

	virtual FileHandle Open(const std::string &name, FileOpen options) override;

	virtual void Close(FileHandle file) override;
	virtual unsigned int Size(FileHandle file) override;
	virtual void Seek(FileHandle file, int pos, FileSeek origin) override;
	virtual unsigned int Tell(FileHandle file) override;
	virtual unsigned int Read(FileHandle file, void *buf, unsigned int size) override;
	virtual unsigned int Write(FileHandle file, const void *buf, unsigned int size) override;

	virtual bool Exists(const std::string &name) override;

	virtual std::vector<std::string> FileFind(const std::string &wildcard) override;

	virtual void SetWritePath(const std::string &path) override;
	virtual std::vector<std::string> GetSearchPath() override;
	virtual void RemoveSearchPath(const std::string &path) override;
	virtual void AddSearchPath(const std::string &path) override;

	static std::string GetApplicationDirectory();

private:
	static bool WildcardCompare(const std::string &str, const std::string &wildcard);
	// Where search path can be in a pack file, we only support writes directly
	// to the filesystem
	std::vector<std::string> searchPathOrder;
	std::unordered_map<std::string, ISearchPath*> searchPath;
	ISearchPath *writePath = nullptr;
};