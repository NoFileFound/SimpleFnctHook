# SimpleFnctHook
Simple function hooking using detours.

For example I hooked MessageBoxW.

# Injection
Code:
[code]
    WCHAR* TARGET_PATH = L"target.exe";
    CHAR* DLL_PATH = "dll.dll";
    STARTUPINFOW startupInfo = { sizeof(startupInfo) };
    PROCESS_INFORMATION processInfo;

    // Create the process with Detour injection
    BOOL success = DetourCreateProcessWithDllW(TARGET_PATH, NULL, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInfo,
        DLL_PATH, NULL);

    if (success)
    {
        // Resume the suspended process
        ResumeThread(processInfo.hThread);

        CloseHandle(processInfo.hThread);
        CloseHandle(processInfo.hProcess);
    }
[/code]
