#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

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

//given a squence containing 0 and 1, output a longest subsquence wich contains equal number of 0 and 1
void longest_equal_number(int seq[], int size)
{
    int i;
    int *this_seq = (int *)malloc(sizeof(int) * size);
    int *this_sum = (int *)malloc(sizeof(int) * size);
    int min_sum = size;
    int map_size = size + 1;

    memcpy(this_seq, seq, sizeof(int) * size);
    memset(this_sum, 0, sizeof(int) * size);

    for (i = 0; i < size; i++)
    {
        if (this_seq[i] == 0) this_seq[i] = -1;
        this_sum[i] = this_seq[i] + (i != 0 ? this_sum[i-1] : 0);
        if (this_sum[i] < min_sum ) min_sum = this_sum[i];
    }
    if (min_sum < 0)
    {
        for (i = 0; i < size; i++) this_sum[i] += (-min_sum);
        map_size += (-min_sum);
    }
    int *left_pos = (int *)malloc(sizeof(int) * map_size);
    int *right_pos = (int *)malloc(sizeof(int) * map_size);
    memset(left_pos, 0xff, sizeof(int) * map_size);
    memset(right_pos, 0xff, sizeof(int) * map_size);

    for (i = 0; i < size; i++)
    {
        int pos = this_sum[i];
        assert(pos >= 0);
        if (left_pos[pos] < 0) left_pos[pos] = i;
        right_pos[pos] = i;
    }
    int max = 0;
    int max_p = 0;
    for (i = 0; i < map_size; i++)
    {
        if (right_pos[i] - left_pos[i] >= max)
        {
            max = right_pos[i] - left_pos[i];
            max_p = i;
        }
    }

    printf("%d %d\n", left_pos[max_p]+1, right_pos[max_p]);

    free(this_seq);
    free(this_sum);
    free(left_pos);
    free(right_pos);
}

//given a sequence, output two consecutive sub-sequence, which will disjoint and the absolute value of the sum of these two sub-sequence
void largest_sum_value(int seq[], int size)
{
    int *llu = (int *)malloc(size * sizeof(int)); //left largest until i
    int *llw = (int *)malloc(size * sizeof(int)); //left largest with i
    int *lsu = (int *)malloc(size * sizeof(int)); //left smallest until i
    int *lsw = (int *)malloc(size * sizeof(int)); //left smallest with i
    int *rlu = (int *)malloc(size * sizeof(int)); //right largest until i
    int *rlw = (int *)malloc(size * sizeof(int)); //right largest with i
    int *rsu = (int *)malloc(size * sizeof(int)); //right smallest until i
    int *rsw = (int *)malloc(size * sizeof(int)); //right smallest with i

    llu[0] = seq[0];
    llw[0] = seq[0];
    lsu[0] = seq[0];
    lsw[0] = seq[0];
    rlu[size-1] = seq[size - 1];
    rlw[size-1] = seq[size - 1];
    rsu[size-1] = seq[size - 1];
    rsw[size-1] = seq[size - 1];

    int i;
    for (i = 1; i < size; i++)
    {
        int x =  seq[i] + llw[i-1];
        llw[i] = x > seq[i] ? x : seq[i];
        llu[i] = llu[i-1] > llw[i] ? llu[i-1] : llw[i];
        x = seq[i] + lsw[i-1];
        lsw[i] = x > seq[i] ? seq[i] : x;
        lsu[i] = lsu[i-1] > lsw[i] ? lsw[i] : lsu[i-1];
    }

    for (i = size-2; i >= 0; i--)
    {
        int x = seq[i] + rlw[i+1];
        rlw[i] = x > seq[i] ? x : seq[i];
        rlu[i] = rlu[i+1] > rlw[i] ? rlu[i+1] : rlw[i];
        x = seq[i] + rsw[i+1];
        rsw[i] = x > seq[i] ? seq[i] : x;
        rsu[i] = rsu[i+1] > rsw[i] ? rsw[i] : rsu[i+1];
    }

    int largest = 0;
    int pos = 0;
    for (i = 1; i < size-1; i++)
    {
        int x = llu[i] - rsu[i];
        int y = rlu[i+1] - lsu[i+1];
        if (x < 0) x = -x;
        if (y < 0) y = -y;
        int l = x > y ? x : y;

        if (l >= largest)
        {
            largest = l;
            pos = i;
        }
    }

    printf("largest: %d, pos: %d\n", largest, pos);


    free(llu);
    free(llw);
    free(lsu);
    free(lsw);
    free(rlu);
    free(rlw);
    free(rsu);
    free(rsw);
}

void number_trick_test()
{
    unsigned int bit_count_num = 879287493;
    printf("bit_count_native: %d\n", bit_count_naive(bit_count_num));
    printf("bit_count2: %d\n", bit_count2(bit_count_num));
    printf("bit_count3: %d\n", bit_count3(bit_count_num));

    int num_seq[] = {0,1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0,1,1};
    longest_equal_number(num_seq, sizeof(num_seq)/sizeof(int));

    int largest_seq[] = {3, 5, -2, 1, 5, 2, -4, -2, -1, 10};
    largest_sum_value(largest_seq, sizeof(largest_seq)/sizeof(int));

}
