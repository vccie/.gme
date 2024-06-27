#include <Windows.h>
#include<../ATH/functions.h>
#include <vector>
#include <iostream>


void ListFiles(const std::string& directory, std::vector<std::string>& files) {
    std::string search_path = directory + "\\*.*";
    WIN32_FIND_DATAA found_data;
    HANDLE file_handle = FindFirstFileA(search_path.c_str(), &found_data);

    if (file_handle != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(found_data.cFileName, ".") != 0 && strcmp(found_data.cFileName, "..") != 0) {
                std::string file_path = directory + "\\" + found_data.cFileName;
                if (found_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    ListFiles(file_path, files);
                }
                else {
                    files.push_back(file_path);
                }
            }
        } while (FindNextFileA(file_handle, &found_data));
        FindClose(file_handle);
    }
}