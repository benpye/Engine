#pragma once

#include "IFilesystem.h"
#include "ISearchPath.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdio>

using namespace std;

struct FSHandle
{
	IntFileHandle fileHandle;
	ISearchPath *searchPath;
};

class Filesystem : public IFilesystem
{
public:
	Filesystem();

	virtual FileHandle Open(const string &name, FileOpen options) override;

	virtual void Close(FileHandle file) override;
	virtual unsigned int Size(FileHandle file) override;
	virtual void Seek(FileHandle file, int pos, FileSeek origin) override;
	virtual unsigned int Tell(FileHandle file) override;
	virtual unsigned int Read(FileHandle file, void *buf, unsigned int size) override;
	virtual unsigned int Write(FileHandle file, const void *buf, unsigned int size) override;

	virtual bool Exists(const string &name) override;

	virtual vector<string> FileFind(const string &wildcard) override;

	virtual void SetWritePath(const string &path) override;
	virtual vector<string> GetSearchPath() override;
	virtual void RemoveSearchPath(const string &path) override;
	virtual void AddSearchPath(const string &path) override;

	static string GetApplicationDirectory();

private:
	// Where search path can be in a pack file, we only support writes directly
	// to the filesystem
	vector<string> searchPathOrder;
	unordered_map<string, ISearchPath *> searchPath;
	ISearchPath *writePath = nullptr;
};