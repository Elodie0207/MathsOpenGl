//
// Created by ianpo on 29/12/2023.
//

#pragma once


#include "Core.hpp"
#include "Math.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

class SystemHelper {
public:
	inline static std::string ReadFile(const std::filesystem::path &path) {
		std::string result;
		std::ifstream fileStream(path, std::ios::in | std::ios::binary);
		if(fileStream)
		{
			fileStream.seekg(0, std::ios::end);
			result.resize(fileStream.tellg());
			fileStream.seekg(0, std::ios::beg);
			fileStream.read(result.data(), result.size());
			fileStream.close();
		}
		else
		{
			REI_ERROR("Could not open file '{0}'.", path.string());
		}
		return result;
	}
};
