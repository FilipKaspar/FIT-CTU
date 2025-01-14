#include <windows.h>

// C++ name mangling and multiple inclusions solved
#ifndef MALLOC_DEBUG_H
#define MALLOC_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

void MallocDebug_Init(void);
void MallocDebug_Done(void);

#ifdef __cplusplus
}
#endif

#endif // MALLOC_DEBUG_H