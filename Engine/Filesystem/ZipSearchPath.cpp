#include "ZipSearchPath.h"

#include <string>
#include <cstdio>
#include <iostream>

ZipSearchPath::ZipSearchPath(const string &zipFile)
{
	memset(&zipArchive, 0, sizeof(mz_zip_archive));
	mz_bool status = mz_zip_reader_init_file(&zipArchive, zipFile.c_str(), 0);

	if (!status)
		return;

	vector<string> splitPath;

	directoryTree = new ZipNode();

	for (unsigned int i = 0; i < mz_zip_reader_get_num_files(&zipArchive); i++)
	{
		mz_zip_archive_file_stat stat;
		if(mz_zip_reader_file_stat(&zipArchive, i, &stat))
		{
			splitPath = SplitPath(stat.m_filename);

			ZipNode *n = directoryTree;

			for (auto p : splitPath)
			{
				auto i = n->children.find(p);
				if (i == n->children.end())
				{
					n->children[p] = new ZipNode();
					n->children[p]->parent = n;
					n = n->children[p];
				}
				else
					n = i->second;
			}

			n->name = stat.m_filename;
			n->size = static_cast<unsigned int>(stat.m_uncomp_size);
			n->zipId = i;
			n->isDirectory = mz_zip_reader_is_file_a_directory(&zipArchive, i) == MZ_TRUE;
		}
	}
}

ZipSearchPath::~ZipSearchPath()
{
	mz_zip_reader_end(&zipArchive);
	DeleteNode(directoryTree);
}

void ZipSearchPath::DeleteNode(ZipNode *n)
{
	if (n->children.empty())
		delete n;
	else
	{
		for (auto c : n->children)
		{
			DeleteNode(c.second);
		}
	}
}

bool ZipSearchPath::Exists(const string &name)
{
	return GetZipNode(name) != nullptr;
}

IntFileHandle ZipSearchPath::Open(const string &name, FileOpen options)
{
	return nullptr;
}

void ZipSearchPath::Close(IntFileHandle handle)
{
}

unsigned int ZipSearchPath::Size(IntFileHandle handle)
{
	return 0;
}

void ZipSearchPath::Seek(IntFileHandle handle, int pos, FileSeek origin)
{
}

unsigned int ZipSearchPath::Tell(IntFileHandle file)
{
	return 0;
}

unsigned int ZipSearchPath::Read(IntFileHandle file, void* buf, unsigned size)
{
	return 0;
}

unsigned int ZipSearchPath::Write(IntFileHandle file, void const* buf, unsigned size)
{
	return 0;
}

vector<string> ZipSearchPath::FileFind(const string &wildcard)
{
	return{};
}

vector<string> ZipSearchPath::SplitPath(const string &path)
{
	vector<string> split;
	string t = "";
	for (auto c : path)
	{
		if (c == '/')
		{
			if (t.length() > 0)
				split.push_back(t);
			t = "";
		}
		else
			t += c;
	}

	if (t.length() > 0)
		split.push_back(t);

	return split;
}

ZipNode *ZipSearchPath::GetZipNode(const string &path)
{
	vector<string> split = SplitPath(path);
	ZipNode *n = directoryTree;

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

			n = i->second;
		}
	}

	return n;
}