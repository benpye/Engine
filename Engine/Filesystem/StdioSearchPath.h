#pragma once

#include "IFilesystem.h"
#include "ISearchPath.h"

#include <string>

using namespace std;

class StdioSearchPath : public ISearchPath
{
public:
	StdioSearchPath(const string &path);

	virtual bool Exists(const string &name) override;
	virtual IntFileHandle Open(const string &name, FileOpen options) override;

	virtual void Close(IntFileHandle handle) override;
	virtual unsigned int Size(IntFileHandle handle) override;
	virtual void Seek(IntFileHandle handle, int pos, FileSeek origin) override;
	virtual unsigned int Tell(IntFileHandle file) override;
	virtual unsigned int Read(IntFileHandle file, void *buf, unsigned int size) override;
	virtual unsigned int Write(IntFileHandle file, const void *buf, unsigned int size) override;

	virtual vector<string> ListDirectory(const string &path) override;

private:
	string ConstructPath(const string &name);
	string base;
};