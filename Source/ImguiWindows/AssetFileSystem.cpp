#include "ImguiWindows/AssetFileSystem.h"

namespace File{

void SetRootPath(fs::path path) {
	if (DirectoryCheck(path))
		fs::current_path(path);
	else {
		std::cerr << "Error " << path << " is an invalid path!" << std::endl;
		abort();
	}
}

fs::path GetRootPath() {

	return fs::current_path();
}

bool DirectoryCheck(fs::path directory_name) {

	return fs::exists(directory_name) && fs::is_directory(directory_name);
}

std::vector<fs::directory_entry> RecursiveDirectoryList(fs::path directory) {

	std::vector<fs::directory_entry> directory_list;

	for (auto& ref : fs::recursive_directory_iterator(directory)) {
		if (fs::is_directory(ref))
			directory_list.push_back(ref);
	}

	return directory_list;
}

std::vector<fs::directory_entry> GetDirectoryList(fs::path directory)
{
	std::vector<fs::directory_entry> directoryList;

	for (auto& dir : fs::directory_iterator(directory)) {
		if (fs::is_directory(dir))
			directoryList.push_back(dir);
	}
	return directoryList;
}

bool CreateFileDirectory(fs::path directory_name) {

	return fs::create_directories(directory_name);
}

bool FileCheck(fs::path file_name) {
	
	return fs::exists(file_name) && fs::is_regular_file(file_name);
}

std::vector<fs::path> ListOfFiles(fs::path directory) {

	std::vector<fs::path> fileList;

	for (auto& file : fs::directory_iterator(directory)) {
		if (fs::is_regular_file(file))
			fileList.push_back(file.path());
	}

	return fileList;
}

}