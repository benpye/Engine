#include "ZipSearchPath.h"

#include <string>
#include <cstdio>
#include <algorithm>
#include "PathUtils.h"

bool ZipSearchPath::Init(const std::string& zipFile)
{
	zipName = zipFile;

	memset(&zipArchive, 0, sizeof(mz_zip_archive));
	mz_bool status = mz_zip_reader_init_file(&zipArchive, zipFile.c_str(), 0);

	if (!status)
		return false;

	std::vector<std::string> splitPath;

	for (unsigned int i = 0; i < mz_zip_reader_get_num_files(&zipArchive); i++)
	{
		mz_zip_archive_file_stat stat;
		if (mz_zip_reader_file_stat(&zipArchive, i, &stat))
		{
			splitPath = PathUtils::SplitPath(stat.m_filename);

			ZipNode* n = &directoryTree;

			for (auto p : splitPath)
			{
				auto i = n->children.find(p);
				if (i == n->children.end())
				{
					n->children[p] = std::unique_ptr<ZipNode>(new ZipNode());
					n->children[p]->parent = n;
					n = n->children[p].get();
				}
				else
					n = i->second.get();
			}

			n->name = stat.m_filename;
			n->zipId = i;
			n->isDirectory = mz_zip_reader_is_file_a_directory(&zipArchive, i) == MZ_TRUE;
		}
	}

	return true;
}

ZipSearchPath::~ZipSearchPath()
{
	mz_zip_reader_end(&zipArchive);
}

std::string ZipSearchPath::RelativeToFullPath(const std::string& name)
{
	return zipName + "/" + name;
}

FileExists ZipSearchPath::Exists(const std::string& name)
{
	ZipNode* n = GetZipNode(name);
	if (n == nullptr)
		return FileExists::None;

	return n->isDirectory ? FileExists::Directory : FileExists::File;
}

IntFileHandle ZipSearchPath::Open(const std::string& name, FileOpen options)
{
	// Zip files don't support writing
	if(flags(options & FileOpen::Write))
		return nullptr;

	ZipNode* n = GetZipNode(name);
	if (n == nullptr)
		return nullptr;

	ZipFileHandle* h = new ZipFileHandle();
	h->zipId = n->zipId;

	mz_zip_archive_file_stat stat;
	if (mz_zip_reader_file_stat(&zipArchive, h->zipId, &stat))
		h->size = static_cast<unsigned int>(stat.m_uncomp_size);

	return h;
}

void ZipSearchPath::Close(IntFileHandle file)
{
	ZipFileHandle* h = static_cast<ZipFileHandle*>(file);
	if (h != nullptr)
	{
		if (h->buf != nullptr)
			delete[] h->buf;

		delete h;
	}
}

unsigned int ZipSearchPath::Size(IntFileHandle file)
{
	ZipFileHandle* h = static_cast<ZipFileHandle*>(file);
	if (h != nullptr)
	{
		return h->size;
	}

	return 0;
}

void ZipSearchPath::Seek(IntFileHandle file, int pos, FileSeek origin)
{
	ZipFileHandle* h = static_cast<ZipFileHandle*>(file);
	if (h != nullptr)
	{
		switch (origin)
		{
		case FileSeek::Current:
			h->pos += pos;
			break;
		case FileSeek::Head:
			h->pos = pos;
			break;
		case FileSeek::Tail:
			h->pos = h->size + pos;
			break;
		}
	}
}

unsigned int ZipSearchPath::Tell(IntFileHandle file)
{
	ZipFileHandle* h = static_cast<ZipFileHandle*>(file);
	if (h != nullptr)
	{
		return h->pos;
	}

	return 0;
}

unsigned int ZipSearchPath::Read(IntFileHandle file, void* buf, unsigned int size)
{
	ZipFileHandle* h = static_cast<ZipFileHandle*>(file);
	if (h != nullptr)
	{
		// Only allocate and read the file the first time we need the data
		if (h->buf == nullptr)
		{
			h->buf = new char[h->size];
			mz_zip_reader_extract_to_mem(&zipArchive, h->zipId, h->buf, h->size, 0);
		}

		unsigned int rPos = std::min(h->pos, h->size);
		unsigned int rSize = std::min(h->size - rPos, size);
		std::memcpy(buf, &static_cast<char *>(h->buf)[rPos], rSize);
		return rSize;
	}

	return 0;
}

std::vector<std::string> ZipSearchPath::ListDirectory(const std::string& path)
{
	ZipNode* n = GetZipNode(path);
	if (n == nullptr || n->children.empty())
		return{};

	std::vector<std::string> ret;

	std::string name;

	for (auto const& c : n->children)
	{
		name = c.first;
		if (c.second->isDirectory)
			name += '/';

		ret.push_back(name);
	}

	return ret;
}

ZipNode* ZipSearchPath::GetZipNode(const std::string& path)
{
	std::vector<std::string> split = PathUtils::SplitPath(path);
	ZipNode* n = &directoryTree;

	for (auto s : split)
	{
		if (s == "..")
		{
			// Can't break out of zip file
			if (n->parent != nullptr)
				n = n->parent;
			else
				return nullptr;
		}
		else if (s == ".")
			continue; // Skip
		else
		{
			auto i = n->children.find(s);
			// Not found
			if (i == n->children.end())
				return nullptr;

			n = i->second.get();
		}
	}

	return n;
}