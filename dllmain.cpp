#include "pch.h"

typedef int (WINAPI* PMESSAGEBOXW)(
    HWND hWnd,
    LPCTSTR lpText,
    LPCTSTR lpCaption, 
    UINT uType
);

PMESSAGEBOXW OriginalMessageBoxW = NULL;

__declspec(dllexport) NTSTATUS NTAPI 
DetouredMessageBoxW(
    HWND hWnd,
    LPCTSTR lpText, 
    LPCTSTR lpCaption, 
    UINT uType
)
{
    // your code here

    NTSTATUS result = OriginalMessageBoxW(hWnd, lpText, lpCaption, uType);
    return result;
}

// Entry point of the DLL
BOOL APIENTRY 
DllMain(
    HMODULE hModule, 
    DWORD reason, 
    LPVOID lpReserved
)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            OriginalMessageBoxW = (PMESSAGEBOXW)DetourFindFunction("user32.dll", "MessageBoxW");
            if (!OriginalMessageBoxW) return FALSE;
            if (DetourTransactionBegin() != NO_ERROR || DetourUpdateThread(GetCurrentThread()) != NO_ERROR || DetourAttach(&(PVOID&)OriginalMessageBoxW, DetouredMessageBoxW) != NO_ERROR || DetourTransactionCommit() != NO_ERROR) return FALSE;
            break;
        
        case DLL_PROCESS_DETACH:
            if (DetourTransactionBegin() != NO_ERROR || DetourUpdateThread(GetCurrentThread()) != NO_ERROR || DetourDetach(&(PVOID&)OriginalMessageBoxW, DetouredMessageBoxW) != NO_ERROR || DetourTransactionCommit() != NO_ERROR) return FALSE;
            break;
        
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}