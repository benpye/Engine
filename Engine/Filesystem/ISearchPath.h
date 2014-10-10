#pragma once

#include "IFilesystem.h"

using namespace std;

typedef void* IntFileHandle;

class ISearchPath
{
public:
	virtual bool Exists(string name) = 0;
	virtual IntFileHandle Open(string name, FileOpen options) = 0;
	virtual void Close(IntFileHandle handle) = 0;
	virtual unsigned int Size(IntFileHandle handle) = 0;
	virtual void Seek(IntFileHandle handle, int pos, FileSeek origin) = 0;
	virtual unsigned int Tell(IntFileHandle file) = 0;
};