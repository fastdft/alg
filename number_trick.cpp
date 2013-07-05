#include <stdio.h>

//unsigned int is 32 bit long
int bit_count_naive(unsigned int num)
{
    int cnt = 0;
    for (; num; num>>=1)
        cnt += (num & 1);
    return cnt;
}

int bit_count2(unsigned int num)
{
    int i;
    for (i=0; num; i++)
        num &= (num-1);
    return i;
}

//group num by 3bits: abc, value is 4a+2b+c, 4a+2b+c - (2a+b) - (a) = a + b + c, so tmp can grouped by 3bit, in each group store the count of '1'
int bit_count3(unsigned int num)
{
    unsigned int tmp = num - ((num>>1)&033333333333) - ((num>>2)&011111111111);
    printf("%x\n", tmp);
    return ((tmp+(tmp>>3))&030707070707) % 63;
}

void number_trick_test()
{
    unsigned int bit_count_num = 879287493;
    printf("bit_count_native: %d\n", bit_count_naive(bit_count_num));
    printf("bit_count2: %d\n", bit_count2(bit_count_num));
    printf("bit_count3: %d\n", bit_count3(bit_count_num));
}
