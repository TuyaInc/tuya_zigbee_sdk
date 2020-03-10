#include "stdio.h"
#include "stdlib.h"


extern int __HeapLimit;
extern int __HeapBase;

caddr_t _sbrk(int incr)
{
    static char * heap_end = NULL;
    char *pHeapBase = ((char *)&__HeapBase);
    char *pHeapLimit = ((char *)&__HeapLimit);
    char *prev_heap_end;

    if (heap_end == NULL) {
        heap_end = pHeapBase;
    }
    prev_heap_end = heap_end;

    if(heap_end + incr > pHeapLimit) {
        return (caddr_t)(-1);
    }

    heap_end += incr;

    return (caddr_t)prev_heap_end;
}   