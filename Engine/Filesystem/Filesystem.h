#pragma once

#include "IFilesystem.h"
#include "ISearchPath.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdio>

using namespace std;

enum FSType
{
	FSStdio,
	FSZip
};

struct FSHandle
{
	IntFileHandle fileHandle;
	ISearchPath *searchPath;
};

class Filesystem : public IFilesystem
{
public:
	Filesystem();

	virtual FileHandle Open(string name, FileOpen options) override;

	virtual void Close(FileHandle file) override;
	virtual unsigned int Size(FileHandle file) override;
	virtual void Seek(FileHandle file, int pos, FileSeek origin) override;
	virtual unsigned int Tell(FileHandle file) override;
	virtual unsigned int Read(FileHandle file, void *buf, unsigned int size) override;
	virtual unsigned int Write(FileHandle file, const void *buf, unsigned int size) override;

	virtual bool Exists(string name) override;

	virtual vector<string> ListDirectory(string path) override;

	virtual void SetWritePath(string path) override;
	virtual vector<string> GetSearchPath() override;
	virtual void RemoveSearchPath(string path) override;
	virtual void AddSearchPath(string path) override;

	static string GetApplicationDirectory();

private:
	// Where search path can be in a pack file, we only support writes directly
	// to the filesystem
	vector<string> searchPathOrder;
	unordered_map<string, ISearchPath *> searchPath;
	ISearchPath *writePath = nullptr;
};