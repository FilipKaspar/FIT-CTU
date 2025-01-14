#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <path_to_DLL> <target_process_PID>\n", argv[0]);
        return 1;
    }

    const char *dllPath = argv[1];
    DWORD pid = (DWORD)atoi(argv[2]);

    // Open the target process with the desired access rights
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        printf("Failed to open process (PID: %d).\n", pid);
        return 1;
    }

    // Allocate memory in the target process for the DLL path
    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (!pRemoteMemory) {
        printf("Failed to allocate memory in the target process.\n");
        CloseHandle(hProcess);
        return 1;
    }

    // Write the DLL path into the allocated memory in the target process
    if (!WriteProcessMemory(hProcess, pRemoteMemory, dllPath, strlen(dllPath) + 1, NULL)) {
        printf("Failed to write the DLL path to memory in the target process.\n");
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Find the base address of the injected DLL
    HMODULE hInjectedDLL = NULL;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 moduleEntry = {0};
        moduleEntry.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(hSnapshot, &moduleEntry)) {
            do {
                if (_stricmp(moduleEntry.szModule, strrchr(dllPath, '\\') + 1) == 0) {
                    hInjectedDLL = (HMODULE)moduleEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnapshot, &moduleEntry));
        }
        CloseHandle(hSnapshot);
    }
    

    if (!hInjectedDLL) {
        printf("Failed to find the injected DLL in the target process.\n");
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Get the address of FreeLibrary in the target process
    LPVOID pFreeLibrary = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "FreeLibrary");
    if (!pFreeLibrary) {
        printf("Failed to get the address of FreeLibrary.\n");
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Create a remote thread to call FreeLibrary
    HANDLE hUnloadThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFreeLibrary, hInjectedDLL, 0, NULL);
    if (!hUnloadThread) {
        printf("Failed to create thread for unloading DLL.\n");
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Wait for the unload thread to finish
    WaitForSingleObject(hUnloadThread, INFINITE);

    
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hUnloadThread);
    CloseHandle(hProcess);

    printf("DLL successfully unloaded from the process.\n");
    return 0;
}