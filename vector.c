#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <search.h>

static void VectorGrow(vector *v)
{
    v -> allocLen = (v -> allocLen * 2);
    v -> elements = realloc(v -> elements, v -> allocLen * v -> elemSize);
}

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
    v -> allocLen = initialAllocation;
    v -> logLen = 0;
    v -> elemSize = elemSize;
    v -> freeFn = freeFn;
    v -> elements = malloc(initialAllocation * sizeof(elemSize));
}

void VectorDispose(vector *v)
{
    if (v -> freeFn)
    {
        char *current = v -> elements;
        for (int i = 0; i < v -> allocLen; i++, current += v -> elemSize)
        {
            v -> freeFn(current);
        }
    }
    free(v -> elements);
}

int VectorLength(const vector *v)
{
    return v -> logLen;
}

void *VectorNth(const vector *v, int position)
{
    assert(position >= 0);
    assert(position < v -> logLen);
    assert(v -> elements);
    assert((char *) (v -> elements) + position * (v -> elemSize));
    void *ret = (char *) (v -> elements) + position * (v -> elemSize);
    assert(ret);
    return ret;
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
    assert(position >= 0);
    assert(position < v -> logLen);
    void *destination = (char *) (v -> elements) + position * v -> elemSize;
    if (v -> freeFn)
        v -> freeFn(destination);
    memcpy((char *) (v -> elements) + position * v -> elemSize, elemAddr, v -> elemSize);
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{
    assert(position >= 0);
    assert(position <= v -> logLen);
    if (v -> logLen == v -> allocLen)
    {
        printf("\nGREW\n");
        VectorGrow(v);
    }
    void *source = ((char *) (v -> elements)) + (position * v -> elemSize);
    if (position < v -> logLen)
    {
        memmove((void *) ((char *) source + v -> elemSize), source, (v -> elemSize) * (v -> logLen - position));
    }
    memcpy(((char *) (v -> elements)) + (position * v -> elemSize), elemAddr, v -> elemSize); // NOTE: source as first argument is incorrect as it may be altered by memmove in the preceding if-statement.

    v -> logLen++;}

void VectorAppend(vector *v, const void *elemAddr)
{
    if (v -> logLen == v -> allocLen)
        VectorGrow(v);
    memcpy((void *) ((char *) (v -> elements) + (v -> logLen) * (v -> elemSize)), elemAddr, v -> elemSize);
    v -> logLen++;
}

void VectorDelete(vector *v, int position)
{
    assert(v -> logLen > 0);
    assert(position >= 0);
    assert(position < v -> logLen);
    void *elemAddr = (char *) v -> elements + position * v -> elemSize;
    if (v -> freeFn)
        v -> freeFn(elemAddr);
    if (position < v -> logLen - 1)
    {
        elemAddr = (char *) v -> elements + position * v -> elemSize;
        memcpy(elemAddr, (char *) elemAddr + v -> elemSize, (v -> elemSize) * (v -> logLen - position - 1)); // NOTE: remember to scale by elemSize
    }
    v -> logLen--;
}

static void swap(void *vp1, void *vp2, int size)
{
    char *buffer = malloc(size);
    memcpy(buffer, vp1, size);
    memcpy(vp1, vp2, size);
    memcpy(vp2, buffer, size);
    free(buffer);
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
    assert(compare);
    qsort(v -> elements, v -> logLen, v -> elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
    assert(mapFn);
    for (int i = 0; i < v -> logLen; i++)
    {
        assert(VectorNth(v,i));
        mapFn(VectorNth(v, i), auxData);
    }
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{
    assert(searchFn);
    assert(key);
    assert(startIndex >= 0 && startIndex < v -> logLen);
    void *found = NULL;
    int length = v -> logLen - startIndex + 1;
    if (isSorted)
    {
        assert(VectorNth(v, startIndex));
        found = bsearch(key, VectorNth(v, startIndex), length, v -> elemSize, searchFn);
    }
    else
    {
        assert(VectorNth(v, startIndex));
        found = lfind(key, VectorNth(v, startIndex), &length, v -> elemSize, searchFn);
    }
    if (found)
        return ((char *) (found) - (char *) (v -> elements)) / v -> elemSize;
    return -1;
}
