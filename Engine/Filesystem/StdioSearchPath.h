#pragma once

#include "IFilesystem.h"
#include "ISearchPath.h"

#include <string>

class StdioSearchPath : public ISearchPath
{
public:
	StdioSearchPath(const std::string& path);

	virtual std::string RelativeToFullPath(const std::string& name) override;
	virtual bool CreateDirectoryHierarchy(const std::string& name) override;
	virtual bool Remove(const std::string& name) override;
	virtual bool Exists(const std::string& name) override;
	virtual IntFileHandle Open(const std::string& name, FileOpen options) override;

	virtual void Close(IntFileHandle file) override;
	virtual unsigned int Size(IntFileHandle file) override;
	virtual void Seek(IntFileHandle file, int pos, FileSeek origin) override;
	virtual unsigned int Tell(IntFileHandle file) override;
	virtual unsigned int Read(IntFileHandle file, void* buf, unsigned int size) override;
	virtual unsigned int Write(IntFileHandle file, const void* buf, unsigned int size) override;

	virtual std::vector<std::string> ListDirectory(const std::string& path) override;

private:
	std::string base;
};