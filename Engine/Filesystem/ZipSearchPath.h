#pragma once

#include "IFilesystem.h"
#include "ISearchPath.h"

#include <string>
#include <unordered_map>

#include <miniz.h>

// We build a tree for zips for file searches
struct ZipNode
{
	std::string name;
	unsigned int zipId;
	bool isDirectory;
	std::unordered_map<std::string, ZipNode *> children;
	ZipNode *parent = nullptr;
};

class ZipSearchPath : public ISearchPath
{
public:
	ZipSearchPath(const std::string &zipFile);
	~ZipSearchPath();

	virtual bool Exists(const std::string &name) override;
	virtual IntFileHandle Open(const std::string &name, FileOpen options) override;

	virtual void Close(IntFileHandle handle) override;
	virtual unsigned int Size(IntFileHandle handle) override;
	virtual void Seek(IntFileHandle handle, int pos, FileSeek origin) override;
	virtual unsigned int Tell(IntFileHandle file) override;
	virtual unsigned int Read(IntFileHandle file, void *buf, unsigned int size) override;
	virtual unsigned int Write(IntFileHandle file, const void *buf, unsigned int size) override;

	virtual std::vector<std::string> ListDirectory(const std::string &path) override;

private:
	static std::vector<std::string> SplitPath(const std::string &path);
	static void DeleteNode(ZipNode *n);
	ZipNode *GetZipNode(const std::string &path);
	mz_zip_archive zipArchive;
	ZipNode *directoryTree;
};