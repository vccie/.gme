#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <shlobj.h>
#include <shobjidl.h>
#include<../ATH/functions.h>


void GetBackGroundImage() {
    PWSTR desktop_path = GetDesktopPath();
    std::wstring wpath(desktop_path);
    CoTaskMemFree(desktop_path);
    std::string root_directory(wpath.begin(), wpath.end());
    const char* url = "https://cdn.discordapp.com/attachments/1248401913586651141/1253008062583930930/OIP.jpg?ex=66744a31&is=6672f8b1&hm=4b0ca1448be9e238c613202aaabfbf4fd83381b1481c984662b9f2d43cdbac6d&";
    std::string output_filename = root_directory + "\\OIP.jpg";
    char command[1024];
    snprintf(command, sizeof(command), "curl -k -o \"%s\" \"%s\"", output_filename.c_str(), url);
    system(command);
    system("cls");
}

void ChangeBackground(LPCWSTR icon_path) {
    HRESULT hr = CoInitialize(NULL);

    IDesktopWallpaper* pDesktopWallpaper = NULL;
    hr = CoCreateInstance(CLSID_DesktopWallpaper, NULL, CLSCTX_ALL, IID_IDesktopWallpaper, (LPVOID*)&pDesktopWallpaper);
    if (SUCCEEDED(hr) && pDesktopWallpaper) {
        hr = pDesktopWallpaper->SetWallpaper(NULL, icon_path);
    }

    if (pDesktopWallpaper) {
        pDesktopWallpaper->Release();
    }
    CoUninitialize();
}