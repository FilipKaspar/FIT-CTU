#include <windows.h>
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

    // Get the address of LoadLibraryA in kernel32.dll
    LPVOID pLoadLibraryA = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    if (!pLoadLibraryA) {
        printf("Failed to get the address of LoadLibraryA.\n");
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Create a remote thread in the target process to load the DLL
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryA, pRemoteMemory, 0, NULL);
    if (!hThread) {
        printf("Failed to create remote thread.\n");
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Wait for the remote thread to complete
    WaitForSingleObject(hThread, INFINITE);

    // Clean up allocated memory and close handles
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    printf("DLL successfully loaded into the process.\n");
    return 0;
}

