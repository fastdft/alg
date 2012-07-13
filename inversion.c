#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define COUNT 100000

unsigned long long count_inversions(int *numbers, int size, int *buffer, int *tmp)
{
    if (size <= 1)
    {
        buffer[0] = numbers[0];
        return 0;
    }
    if (size == 2)
    {
        if (numbers[0] <= numbers[1])
        {
            buffer[0] = numbers[0];
            buffer[1] = numbers[1];
            return 0;
        }
        else
        {
            buffer[0] = numbers[1];
            buffer[1] = numbers[0];
            return 1;
        }
    }

    int half = size / 2;
    unsigned long long n1 = count_inversions(numbers, half, buffer, tmp);
    unsigned long long  n2 = count_inversions(numbers + half, size - half, buffer + half, tmp);
    unsigned long long ret = n1 + n2;
    int i, j;

    //two halfs of buffer is sorted
    for (i=0, j=half; i<half && j<size;)
    {
        if (buffer[j] < buffer[i])
        {
            ret += (half - i);
            j++;
        }
        else
        {
            i++;
        }
    }

    //merge sort two halfs of buffer
    memcpy(tmp, buffer, size * sizeof(int));
    int p;
    for (p=0, i=0, j=half; i<half && j<size;)
    {
        if (tmp[i] < tmp[j])
            buffer[p++] = tmp[i++];
        else
            buffer[p++] = tmp[j++];
    }
    for (; i<half; i++)
        buffer[p++] = tmp[i];
    for (; j<size; j++)
        buffer[p++] = tmp[j];
    assert(p == size);

    return ret;
}
