#pragma once

#include <IFilesystem.h>
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
	ZipNode* parent = nullptr;
};

struct ZipFileHandle
{
	void* buf = nullptr;
	unsigned int pos = 0;
	unsigned int size;
	unsigned int zipId;
};

class ZipSearchPath : public ISearchPath
{
public:
	ZipSearchPath() = default;
	~ZipSearchPath();

	// Disallow copy
	ZipSearchPath(const ZipSearchPath& other) = delete;
	ZipSearchPath& operator=(const ZipSearchPath& other) = delete;

	virtual bool Init(const std::string& name) override;
	virtual std::string RelativeToFullPath(const std::string& name) override;
	virtual bool CreateDirectoryHierarchy(const std::string& name) override { return false; };
	virtual bool Remove(const std::string& name) override { return false; }
	virtual FileExists Exists(const std::string& name) override;
	virtual IntFileHandle Open(const std::string& name, FileOpen options) override;

	virtual void Close(IntFileHandle file) override;
	virtual unsigned int Size(IntFileHandle file) override;
	virtual void Seek(IntFileHandle file, int pos, FileSeek origin) override;
	virtual unsigned int Tell(IntFileHandle file) override;
	virtual unsigned int Read(IntFileHandle file, void* buf, unsigned int size) override;
	virtual unsigned int Write(IntFileHandle file, const void* buf, unsigned int size) override { return 0; }

	virtual std::vector<std::string> ListDirectory(const std::string& path) override;

private:
	static void DeleteNode(ZipNode* n);
	ZipNode* GetZipNode(const std::string& path);
	mz_zip_archive zipArchive;
	ZipNode* directoryTree;
	std::string zipName;
};