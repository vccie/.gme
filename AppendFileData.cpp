#include <Windows.h>
#include<../ATH/functions.h>
#include <vector>
#include <iostream>

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