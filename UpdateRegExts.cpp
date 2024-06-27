#include <Windows.h>
#include<../ATH/functions.h>
#include <vector>
#include <iostream>

void UpdateRegExts(const char* extention) {
    HKEY hkey = NULL;
    RegOpenKeyExA(HKEY_CLASSES_ROOT, extention, NULL, KEY_ALL_ACCESS, &hkey);
    RegSetKeyValueA(hkey, NULL, "(Default)", REG_BINARY, "gme\0", 4);
    RegCloseKey(hkey);
};