#include <string>
#include <vector>

#include "PathUtils.h"

std::vector<std::string> PathUtils::SplitPath(const std::string& path)
{
	std::vector<std::string> split;
	std::string t = "";
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

std::string PathUtils::GetParentDirectory(const std::string& path)
{
	size_t pos = path.find_last_of("\\/");
	return (std::string::npos == pos)
		? "" : path.substr(0, pos);
}