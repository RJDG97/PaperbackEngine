#pragma once
#ifndef _ASSET_FILE_SYSTEM_H_
#define _ASSET_FILE_SYSTEM_H_

#include <filesystem>
#include <vector>
#include <iostream>

namespace File{
	namespace fs = std::filesystem;
	 // root path
	void SetRootPath(fs::path path);
	fs::path GetRootPath();

	// directory management
	bool DirectoryCheck(fs::path directory_name);
	bool CreateFileDirectory(fs::path directory_name);
	std::vector<fs::directory_entry> RecursiveDirectoryList(fs::path directory = fs::current_path());
	std::vector<fs::directory_entry> GetDirectoryList(fs::path directory);

	// file management
	bool FileCheck(fs::path file_name);
	std::vector<fs::path> ListOfFiles(fs::path directory);


}


#endif