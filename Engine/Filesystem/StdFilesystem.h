#pragma once

#include "IFilesystem.h"

#include <string>
#include <vector>
#include <cstdio>

using namespace std;

struct StdFilesystemFileHandle
{
	bool isInitialised = false;
	FILE *handle = nullptr;
};

class StdFilesystem : public IFilesystem
{
public:
	StdFilesystem();
	FileHandle Open(string name, FileOptions options);
	void Close(FileHandle file);
	bool IsValid(FileHandle file);

	void SetWritePath(string path);
	vector<string> GetSearchPath();
	void RemoveSearchPath(string path);
	void AddSearchPath(string path);
private:
	static string GetApplicationDirectory();

	vector<string> searchPath;
	string writePath;
};