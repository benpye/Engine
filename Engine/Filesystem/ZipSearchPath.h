#pragma once

#include "IFilesystem.h"
#include "ISearchPath.h"

#include <string>
#include <unordered_map>

#include <miniz.h>

using namespace std;

// We build a tree for zips for file searches
struct ZipNode
{
	string name;
	unsigned int size;
	unsigned int zipId;
	bool isDirectory;
	unordered_map<string, ZipNode *> children;
	ZipNode *parent = nullptr;
};

class ZipSearchPath : public ISearchPath
{
public:
	ZipSearchPath(const string &zipFile);
	~ZipSearchPath();

	virtual bool Exists(const string &name) override;
	virtual IntFileHandle Open(const string &name, FileOpen options) override;

	virtual void Close(IntFileHandle handle) override;
	virtual unsigned int Size(IntFileHandle handle) override;
	virtual void Seek(IntFileHandle handle, int pos, FileSeek origin) override;
	virtual unsigned int Tell(IntFileHandle file) override;
	virtual unsigned int Read(IntFileHandle file, void *buf, unsigned int size) override;
	virtual unsigned int Write(IntFileHandle file, const void *buf, unsigned int size) override;

	virtual vector<string> FileFind(const string &wildcard) override;

private:
	static vector<string> SplitPath(const string &path);
	static void DeleteNode(ZipNode *n);
	ZipNode *GetZipNode(const string &path);
	mz_zip_archive zipArchive;
	ZipNode *directoryTree;
};