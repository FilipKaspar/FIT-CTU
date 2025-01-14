#include <basetsd.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <wingdi.h>
#include "malloc_debug.h"

using namespace std;

DWORD_PTR original_malloc;
bool orig_malloc_set = false;
DWORD_PTR original_calloc;
bool orig_calloc_set = false;
DWORD_PTR original_realloc;
bool orig_realloc_set = false;
DWORD_PTR original_free;
bool orig_free_set = false;

class Record{
    public:
        void* address = nullptr;
        int size = 0;
};

Record records[1000];
int records_head = 0;

bool init_mark = false;


HMODULE hPEFile = GetModuleHandle(NULL);  // Get handle to the current process
PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hPEFile;

PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)((BYTE*)pDosHeader + pDosHeader->e_lfanew); // Get the NT header


void replaceFunctionPointer(PIMAGE_THUNK_DATA pThunkIAT, DWORD_PTR fuinctionPointer){
    DWORD old_protect;

    if(!VirtualProtect((LPVOID)pThunkIAT, sizeof(DWORD_PTR), PAGE_READWRITE, &old_protect)){
        cout << "Failed to change memory protection." << endl;
    }

    // Try it anyways, in case we got the necessary permissions by default

    try {
        pThunkIAT->u1.Function = fuinctionPointer;
    } catch (...) {
        cout << "Nope we couldn't write" << endl;
        exit(1);
    }


    if(!VirtualProtect((LPVOID)pThunkIAT, sizeof(DWORD_PTR), old_protect, &old_protect)){
        cout << "Failed to change memory protection back to previous state." << endl;
    }
}

void addRecord(void* ptr, size_t size){
    if(records_head >= sizeof(records) / sizeof(records[0])){
        cout << "No more records will be tracked, including current one!" << endl;
        return;
    }
    else {
        records[records_head].address = ptr;
        records[records_head].size = size;
    }
    records_head++;
}

void* MallocDebug_malloc(size_t  size){
    cout << "Called DebugMalloc!!" << endl;
    
    
    void* ptr = reinterpret_cast<void* (*)(size_t)>(original_malloc)(size);
    if(ptr) addRecord(ptr, size);

    return ptr;
}

void* MallocDebug_calloc(size_t num, size_t  size){
    cout << "Called DebugCalloc!!" << endl;

    void* ptr = reinterpret_cast<void* (*)(size_t, size_t)>(original_calloc)(num, size);
    if(ptr) addRecord(ptr, num * size);

    return ptr;
}

void MallocDebug_free(void * array){
    cout << "Called DebugFree!!" << endl;

    if(array == nullptr){
        cout << "Attempting to free nullptr!" << endl;
    } else {
        bool found = false;
        for(int i = 0; i < records_head; i++){
            if(records[i].address == array){
                if(records[i].size == -1){
                    cout << "Double Free!" << endl;
                }
                records[i].size = -1;
                found = true;
                break;
            }
        }

        if(!found) cout << "Pointer hasn't been found in records!" << endl;
    }

    reinterpret_cast<void* (*)(void*)>(original_free)(array);
}

void* MallocDebug_realloc(void* array, size_t  size){
    cout << "Called DebugRealloc!!" << endl;
    Record* record;
    bool found = false;

    if(array == nullptr) {
        void* ptr = reinterpret_cast<void* (*)(void*, size_t)>(original_realloc)(nullptr, size);
        if (!ptr) {
            cout << "Allocation of new pointer inside realloc failed!" << endl;
            return ptr;
        }
        addRecord(ptr, size);
        return ptr;
    }

    for(int i = 0; i < records_head; i++){
        if(records[i].address == array){
            record = &records[i];
            found = true;
            break;
        }
    }

    if(size == 0){
        if(!found) cout << "Pointer hasn't been found in records. Freeing an unknown pointer!" << endl;
        void* ptr = reinterpret_cast<void* (*)(void*, size_t)>(original_realloc)(array, size);
        if(found){
            if(record->size == -1) cout << "Double Free in realloc!" << endl;
            record->size = -1;
        }
        return ptr;
    }

    if(!found) cout << "Pointer hasn't been found in records!" << endl;

    void* ptr = reinterpret_cast<void* (*)(void*,size_t)>(original_realloc)(array, size);

    if (!ptr) {
        cout << "Reallocation of unkown pointer failed!" << endl;
        return ptr;
    }

    if(found) {
        record->address = ptr;
        record->size = size;
    } else {
        addRecord(ptr, size);
    }

    return ptr;
}

void MallocDebug_Init(){
    if(init_mark){
        cout << "Init has been already called! Skipping..." << endl;
        return;
    }

    DWORD importDirVA = pNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress; // Second position in DataDirectory array
    DWORD importDirSize = pNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

    PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(((BYTE*)pDosHeader) + importDirVA);  // Get the import directory
    PIMAGE_IMPORT_DESCRIPTOR pImportDescriptorEnd = (PIMAGE_IMPORT_DESCRIPTOR)(((BYTE*)pImportDescriptor) + importDirSize);  // Get the import directory

    // Loop through the Import Descriptors (DLLs)
    for (; pImportDescriptor < pImportDescriptorEnd && pImportDescriptor->Characteristics != NULL; ++pImportDescriptor) {
        char* pszDLLName = (char*)((BYTE*)pDosHeader + pImportDescriptor->Name);
        printf("DLL: %s\n", pszDLLName );
        if(strcmp((char*)((BYTE*)pDosHeader + pImportDescriptor->Name), "api-ms-win-crt-heap-l1-1-0.dll") != 0){
            continue;
        }

        
        if(pImportDescriptor->OriginalFirstThunk != NULL){
            PIMAGE_THUNK_DATA pThunkOrigData = (PIMAGE_THUNK_DATA)((BYTE*)pDosHeader + pImportDescriptor->OriginalFirstThunk); // Get thunks
            PIMAGE_THUNK_DATA pThunkIAT = (PIMAGE_THUNK_DATA)((BYTE*)pDosHeader + pImportDescriptor->FirstThunk);

            for (; pThunkOrigData->u1.AddressOfData != NULL; ++pThunkOrigData, ++pThunkIAT) {
                PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)((BYTE*)pDosHeader + pThunkOrigData->u1.AddressOfData);

                if (strcmp(pImportByName->Name, "malloc") == 0) { // check if name of the function matches. If so change the function pointer
                    if(orig_malloc_set && original_malloc != pThunkIAT->u1.Function){
                        cout << "Function for original malloc is different than previously loaded one! Exiting..." << endl;
                        exit(1);
                    }
                    original_malloc = pThunkIAT->u1.Function;
                    replaceFunctionPointer(pThunkIAT, (DWORD_PTR)MallocDebug_malloc);
                    orig_malloc_set = true;
                    cout << "Malloc hooked!" << endl;
                }
                else if (strcmp(pImportByName->Name, "calloc") == 0) {
                    if(orig_calloc_set && original_calloc != pThunkIAT->u1.Function){
                        cout << "Function for original calloc is different than previously loaded one! Exiting..." << endl;
                        exit(1);
                    }
                    original_calloc = pThunkIAT->u1.Function;
                    replaceFunctionPointer(pThunkIAT, (DWORD_PTR)MallocDebug_calloc);
                    orig_calloc_set = true;
                    cout << "Calloc hooked!" << endl;
                }
                else if (strcmp(pImportByName->Name, "realloc") == 0) {
                    if(orig_realloc_set && original_realloc != pThunkIAT->u1.Function){
                        cout << "Function for original realloc is different than previously loaded one! Exiting..." << endl;
                        exit(1);
                    }
                    original_realloc = pThunkIAT->u1.Function;
                    replaceFunctionPointer(pThunkIAT, (DWORD_PTR)MallocDebug_realloc);
                    orig_realloc_set = true;
                    cout << "Realloc hooked!" << endl;
                }
                else if (strcmp(pImportByName->Name, "free") == 0) {
                    if(orig_free_set && original_free != pThunkIAT->u1.Function){
                        cout << "Function for original free is different than previously loaded one! Exiting..." << endl;
                        exit(1);
                    }
                    original_free = pThunkIAT->u1.Function;
                    replaceFunctionPointer(pThunkIAT, (DWORD_PTR)MallocDebug_free);
                    orig_free_set = true;
                    cout << "Free hooked!" << endl;
                }
            }
        }
    }
    cout << "Loading hook functions done!\n" << endl;

    init_mark = true;
}

void MallocDebug_Done(){
    if(!init_mark){
        cout << "Init hasn't been called! Skipping..." << endl;
        return;
    }

    DWORD importDirVA = pNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress; // Second position in DataDirectory array
    DWORD importDirSize = pNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

    PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(((BYTE*)pDosHeader) + importDirVA);  // Get the import directory
    PIMAGE_IMPORT_DESCRIPTOR pImportDescriptorEnd = (PIMAGE_IMPORT_DESCRIPTOR)(((BYTE*)pImportDescriptor) + importDirSize);  // Get the import directory

    cout << endl;

    // Loop through the Import Descriptors (DLLs)
   for (; pImportDescriptor < pImportDescriptorEnd && pImportDescriptor->Characteristics != NULL; ++pImportDescriptor) {
        if(strcmp((char*)((BYTE*)pDosHeader + pImportDescriptor->Name), "api-ms-win-crt-heap-l1-1-0.dll") != 0){
            continue;
        }

        if(pImportDescriptor->OriginalFirstThunk != NULL){
            PIMAGE_THUNK_DATA pThunkOrigData = (PIMAGE_THUNK_DATA)((BYTE*)pDosHeader + pImportDescriptor->OriginalFirstThunk); // Get thunks
            PIMAGE_THUNK_DATA pThunkIAT = (PIMAGE_THUNK_DATA)((BYTE*)pDosHeader + pImportDescriptor->FirstThunk);

            for (; pThunkOrigData->u1.AddressOfData != NULL; ++pThunkOrigData, ++pThunkIAT) {
                PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)((BYTE*)pDosHeader + pThunkOrigData->u1.AddressOfData);

                if (strcmp(pImportByName->Name, "malloc") == 0) { // check if name of the function matches. If so change the function pointer
                    if(orig_malloc_set && pThunkIAT->u1.Function != (DWORD_PTR)MallocDebug_malloc){
                        cout << "Found function address is different than previously hooked one! Exiting..." << endl;
                        exit(1);
                    }
                    replaceFunctionPointer(pThunkIAT, original_malloc);
                    orig_malloc_set = true;
                    cout << "Malloc hooked back!" << endl;
                }
                else if (strcmp(pImportByName->Name, "calloc") == 0) {
                    if(orig_calloc_set && pThunkIAT->u1.Function != (DWORD_PTR)MallocDebug_calloc){
                        cout << "Found function address is different than previously hooked one! Exiting..." << endl;
                        exit(1);
                    }
                    replaceFunctionPointer(pThunkIAT, original_calloc);
                    orig_calloc_set = true;
                    cout << "Calloc hooked back!" << endl;
                }
                else if (strcmp(pImportByName->Name, "realloc") == 0) {
                    if(orig_realloc_set && pThunkIAT->u1.Function != (DWORD_PTR)MallocDebug_realloc){
                        cout << "Found function address is different than previously hooked one! Exiting..." << endl;
                        exit(1);
                    }
                    replaceFunctionPointer(pThunkIAT, original_realloc);
                    orig_realloc_set = true;
                    cout << "Realloc hooked back!" << endl;
                }
                else if (strcmp(pImportByName->Name, "free") == 0) {
                    if(orig_free_set && pThunkIAT->u1.Function != (DWORD_PTR)MallocDebug_free){
                        cout << "Found function address is different than previously hooked one! Exiting..." << endl;
                        exit(1);
                    }
                    replaceFunctionPointer(pThunkIAT, original_free);
                    orig_free_set = true;
                    cout << "Free hooked back!" << endl;
                }
            }
        }
    }
    cout << "Original funtions hooked back!\n" << endl;

    for(Record record : records){ // Search for any memory leaks
        if(record.address && record.size != -1) cout << "Memory at " << record.address << " hasn't been freed!" << endl;
    }

    init_mark = false;
}