#include <Windows.h>
#include<../ATH/functions.h>
#include <vector>
#include <iostream>

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