#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <cmath>
using namespace std;
#endif /* __PROGTEST__ */

class CHeap {
public:
    CHeap * m_Next;
    int m_Size;
    bool m_Free;
};

CHeap *head = nullptr;
int m_BlocksCount;

void HeapInit(void * memPool, int memSize) {
    head = (CHeap*)memPool;
    head->m_Next = nullptr;
    head->m_Size = memSize - sizeof(CHeap);
    head->m_Free = true;
    m_BlocksCount = 0;
}

void * HeapAlloc(int size) {
    CHeap *curr = head;

    while (curr && (!curr->m_Free || curr->m_Size < size)) {
        curr = curr->m_Next;
    }

    if (!curr) return nullptr;

    if (curr->m_Size > size + (int)sizeof(CHeap)) {
        CHeap *new_block = (CHeap*)((uint8_t*)curr + sizeof(CHeap) + size);
        new_block->m_Size = curr->m_Size - size - sizeof(CHeap);
        new_block->m_Free = true;
        new_block->m_Next = curr->m_Next;

        curr->m_Size = size;
        curr->m_Next = new_block;
    }

    curr->m_Free = false;
    m_BlocksCount++;

    return (void*)((uint8_t*)curr + sizeof(CHeap));
}

bool HeapFree(void * blk) {
    if (!blk) return false;

    CHeap * curr = head;
    CHeap * prev = nullptr;

    while(curr && (uint8_t*)curr + sizeof(CHeap) != ((uint8_t*)blk)){
        prev = curr;
        curr = curr->m_Next;
    }

    if (!curr || curr->m_Free) return false;

    curr->m_Free = true;

    if(prev && prev->m_Free){
        prev->m_Size += sizeof(CHeap) + curr->m_Size;
        prev->m_Next = curr->m_Next;

        curr = prev;
    }

    if (curr->m_Next && curr->m_Next->m_Free) {
        curr->m_Size += sizeof(CHeap) + curr->m_Next->m_Size;
        curr->m_Next = curr->m_Next->m_Next;
    }

    m_BlocksCount--;
    return true;
}

void HeapDone(int * pendingBlk) {
    *pendingBlk = m_BlocksCount;
}
#ifndef __PROGTEST__
/*int main ( void )
{
    uint8_t       * p0, *p1, *p2, *p3, *p4;
    int             pendingBlk;
    static uint8_t  memPool[3 * 1048576];

    HeapInit ( memPool, 2097152 );
    assert ( ( p0 = (uint8_t*) HeapAlloc ( 512000 ) ) != NULL );
    memset ( p0, 0, 512000 );
    assert ( ( p1 = (uint8_t*) HeapAlloc ( 511000 ) ) != NULL );
    memset ( p1, 0, 511000 );
    assert ( ( p2 = (uint8_t*) HeapAlloc ( 26000 ) ) != NULL );
    memset ( p2, 0, 26000 );
    HeapDone ( &pendingBlk );
    assert ( pendingBlk == 3 );


    HeapInit ( memPool, 2097152 );
    assert ( ( p0 = (uint8_t*) HeapAlloc ( 1000000 ) ) != NULL );
    memset ( p0, 0, 1000000 );
    assert ( ( p1 = (uint8_t*) HeapAlloc ( 250000 ) ) != NULL );
    memset ( p1, 0, 250000 );
    assert ( ( p2 = (uint8_t*) HeapAlloc ( 250000 ) ) != NULL );
    memset ( p2, 0, 250000 );
    assert ( ( p3 = (uint8_t*) HeapAlloc ( 250000 ) ) != NULL );
    memset ( p3, 0, 250000 );
    assert ( ( p4 = (uint8_t*) HeapAlloc ( 50000 ) ) != NULL );
    memset ( p4, 0, 50000 );
    assert ( HeapFree ( p2 ) );
    assert ( HeapFree ( p4 ) );
    assert ( HeapFree ( p3 ) );
    assert ( HeapFree ( p1 ) );
    assert ( ( p1 = (uint8_t*) HeapAlloc ( 500000 ) ) != NULL );
    memset ( p1, 0, 500000 );
    assert ( HeapFree ( p0 ) );
    assert ( HeapFree ( p1 ) );
    HeapDone ( &pendingBlk );
    assert ( pendingBlk == 0 );


    HeapInit ( memPool, 2359296 );
    assert ( ( p0 = (uint8_t*) HeapAlloc ( 1000000 ) ) != NULL );
    memset ( p0, 0, 1000000 );
    assert ( ( p1 = (uint8_t*) HeapAlloc ( 500000 ) ) != NULL );
    memset ( p1, 0, 500000 );
    assert ( ( p2 = (uint8_t*) HeapAlloc ( 500000 ) ) != NULL );
    memset ( p2, 0, 500000 );
    assert ( ( p3 = (uint8_t*) HeapAlloc ( 500000 ) ) == NULL );
    assert ( HeapFree ( p2 ) );
    assert ( ( p2 = (uint8_t*) HeapAlloc ( 300000 ) ) != NULL );
    memset ( p2, 0, 300000 );
    assert ( HeapFree ( p0 ) );
    assert ( HeapFree ( p1 ) );
    HeapDone ( &pendingBlk );
    assert ( pendingBlk == 1 );


    HeapInit ( memPool, 2359296 );
    assert ( ( p0 = (uint8_t*) HeapAlloc ( 1000000 ) ) != NULL );
    memset ( p0, 0, 1000000 );
    assert ( ! HeapFree ( p0 + 1000 ) );
    HeapDone ( &pendingBlk );
    assert ( pendingBlk == 1 );


    return 0;
}*/
#endif /* __PROGTEST__ */

