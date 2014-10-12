#pragma once

#include "IFilesystem.h"

typedef void* IntFileHandle;

class ISearchPath
{
public:
	virtual ~ISearchPath() {}

	virtual bool Exists(const std::string &name) = 0;
	virtual IntFileHandle Open(const std::string &name, FileOpen options) = 0;

	virtual void Close(IntFileHandle handle) = 0;
	virtual unsigned int Size(IntFileHandle handle) = 0;
	virtual void Seek(IntFileHandle handle, int pos, FileSeek origin) = 0;
	virtual unsigned int Tell(IntFileHandle file) = 0;
	virtual unsigned int Read(IntFileHandle file, void *buf, unsigned int size) = 0;
	virtual unsigned int Write(IntFileHandle file, const void *buf, unsigned int size) = 0;

	virtual std::vector<std::string> ListDirectory(const std::string &path) = 0;
};