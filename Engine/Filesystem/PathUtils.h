#pragma once

#include <vector>
#include <string>

namespace PathUtils
{
	std::vector<std::string> SplitPath(const std::string& path);
	std::string GetParentDirectory(const std::string& path);
}