#include <Windows.h>
#include<../ATH/functions.h>
#include <vector>
#include <iostream>

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