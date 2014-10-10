#pragma once

#include "IFilesystem.h"
#include "ISearchPath.h"

#include <string>

using namespace std;

class StdioSearchPath : public ISearchPath
{
public:
	StdioSearchPath(string path);
	virtual bool Exists(string name) override;
	virtual IntFileHandle Open(string name, FileOpen options) override;
	virtual void Close(IntFileHandle handle) override;
	virtual unsigned int Size(IntFileHandle handle) override;
	virtual void Seek(IntFileHandle handle, int pos, FileSeek origin) override;
	virtual unsigned int Tell(IntFileHandle file) override;
private:
	string ConstructPath(string name);
	string base;
};