#include <Windows.h>
#include<../ATH/functions.h>
#include <shobjidl.h>
#include <shlobj.h>
#include <vector>
#include <iostream>

PWSTR GetDesktopPath() {
    PWSTR desktopPath = NULL;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &desktopPath);
    return desktopPath;
}