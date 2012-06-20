#include <string.h>
#include "stdlib.h"
#include <stdio.h>
#include "utils.h"
#include "ReadData.h"

void FindLargestSeq(int seq[], int seq_len, int *value, int *pos)
{
    int max = seq[0];
    int max_pos = 0;
    int suffix = seq[0];
    int suffix_pos = 0;

    for (int i=1; i<seq_len; i++)
    {
        if (suffix >= 0)
        {
            suffix = suffix + seq[i];
        }
        else
        {
            suffix = seq[i];
            suffix_pos = i;
        }

        if (suffix > max)
        {
            max = suffix;
            max_pos = suffix_pos;
        }
    }

    *value = max;
    *pos = max_pos;
}

void UtilsTest()
{
    int large_seq_data[16] = {14, 5, 3, -4, 6, -20, 45, 23, -100, 0, 9, 23, 32, 10, -19, -30};
    int *test_data = (int *)malloc(sizeof(int) * 100);
    int value = 0, pos = 0;
    memset(test_data, 0, sizeof(int) * 100);

    ReadData(test_data, sizeof(int), 100, RAWDATA_TYPE_BYTE, "raw_data.img");
    FindLargestSeq(large_seq_data, 16, &value, &pos);
    printf("FindLargestSeq : %d, %d\n", value, pos);
}