#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ReadData.h"

#define HEAP_TEST_SIZE 1000
void heap_test()
{
    int *test_data = (int *)malloc(sizeof(int) * HEAP_TEST_SIZE);
    memset(test_data, 0, sizeof(int) * HEAP_TEST_SIZE);
    ReadData(test_data, sizeof(int), HEAP_TEST_SIZE, RAWDATA_TYPE_BYTE, "raw_data.img");

    int sample[10] = {7, 8, 3, 4, 2, 1, 10, 9, 5, 6};

    //Min Max Heap Test
    /*
    MinMaxHeap<int> *mm_heap = new MinMaxHeap<int>(HEAP_TEST_SIZE);
    for (int i=0; i<HEAP_TEST_SIZE; i++)
        mm_heap->insert(test_data[i]);
    while (mm_heap->has_next())
    {
        printf("max: %d, min: %d\n", mm_heap->extract_max(), mm_heap->extract_min());
    }
    delete mm_heap;
    */

    //medial head test
    MedHeap<int> *med_heap = new MedHeap<int>();
    for (int i=0; i<sizeof(sample)/sizeof(int); i++)
    {
        med_heap->insert(sample[i]);
    }
    while(med_heap->size() > 0)
    {
        printf("med: %d\n", med_heap->extract_medial());
    }
    /*
    for (int i=0; i<HEAP_TEST_SIZE; i++)
    {
        med_heap->insert(test_data[i]);
    }
    while(med_heap->size() > 0)
    {
        printf("med: %d\n", med_heap->extract_medial());
    }
    */
    delete med_heap;

    free(test_data);
}
