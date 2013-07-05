#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ReadData.h"

#define DP_TEST_SIZE 10000

void RabitTor()
{
#define MAX_GAS_STATION 100

    int l, n, c, t, vr, vt1, vt2;
    int dis[MAX_GAS_STATION+2] = {0};
    freopen("rabit.txt", "r", stdin);
    while(scanf("%d", &l) != EOF)
    {
        scanf("%d %d %d", &n, &c, &t);
        scanf("%d %d %d", &vr, &vt1, &vt2);
        for (int i=0; i<n; i++)
            scanf("%d", &dis[i]);
    }
}

bool is_pali(const char *str, int n)
{
    int i, j;
    for (i=0, j=n-1; i<j; i++, j--)
    {
        if (str[i] != str[j])
            return false;
    }

    return true;
}

void print_cut_index(int *matrix, int *i_matrix, int width, int i, int j)
{
    int index = i_matrix[i*width + j];
    //printf("i: %d, j: %d, matrix: %d\n",i, j, matrix[i*width + j]);
    if (matrix[i*width + j] == 0)
        return;

    printf("%d\n", index);
    print_cut_index(matrix, i_matrix, width, i, index);
    print_cut_index(matrix, i_matrix, width, index+1, j);
}

void pali_cut(const char *str)
{
    int str_len = strlen(str);
    int *matrix = (int *)malloc(sizeof(int) * str_len * str_len);
    int *i_matrix = (int *)malloc(sizeof(int) * str_len * str_len);
    memset(matrix, 0, sizeof(int) * str_len * str_len);
    memset(i_matrix, 0, sizeof(int) * str_len * str_len);

    //[i, j]
    for (int i=str_len-1; i>=0; i--)
    {
        //for (int j=str_len-1; j<=i; j--)
        for (int j=i; j<str_len; j++)
        {
            if (is_pali(str+i, j-i+1))
                matrix[i*str_len + j] = 0;
            else
            {
                matrix[i*str_len + j] = str_len;
                for (int k=i; k<j; k++)
                {
                    int now = matrix[i*str_len+k] + matrix[(k+1)*str_len + j] + 1;
                    if (now < matrix[i*str_len + j])
                    {
                        matrix[i*str_len + j] = now;
                        i_matrix[i*str_len + j] = k;
                    }
                }
            }
        }
    }

    printf("min cut of str is %d\n", matrix[str_len - 1]);
    print_cut_index(matrix, i_matrix, str_len, 0, str_len-1);
}

void lis_dp(int array[], int size)
{
    int *dp = (int *)malloc(sizeof(int) * size);
    int *bt = (int *)malloc(sizeof(int) * size);
    int max_pos, max;
    int i, j;

    max = 0;
    for (i = 0; i < size; i++)
    {
        dp[i] = 1;
        bt[i] = -1;
        for (j = 0; j < i; j++)
        {
            if (array[j] <= array[i] && dp[j] + 1 > dp[i])
            {
                dp[i] = dp[j] + 1;
                bt[i] = j;
            }
        }
        if (dp[i] > max)
        {
            max = dp[i];
            max_pos = i;
        }
    }

    int cnt = 0;
    for (i = max_pos; i >= 0; i = bt[i])
    {
        printf("%d\n", array[i]);
        cnt += 1;
    }
    printf("n*n -- len: %d\n", cnt);

    free(bt);
    free(dp);
}

static inline int divide_find_smaller(int array[], int size, int des)
{
    int s = 0, e = size - 1;
    while(s <= e)
    {
        int m = ((s + e)>>1);
        if (array[m] > des)
        {
            e = m - 1;
        }
        else
        {
            s = m + 1;
        }
    }
    return s; //s must be larger than des
}
/*
void lis_nlogn(int array[], int size)
{
    int *dp = (int *)malloc(sizeof(int) * size);
    int len, i;

    for (i = len = 0; i < size; i++)
    {
        int pos = divide_find_smaller(dp, len, array[i]);

        if (pos == len)
        {
            len += 1;
            dp[len - 1] = array[i];
            last_pos = i;
        }
        else
        {
            dp[pos] = array[i];
        }
    }

    printf("nlogn -- len: %d\n", len);

    free(dp);
}*/

static inline int divide_find_smaller2(int array[], int pos[], int size, int des)
{
    int s = 0, e = size - 1;
    while(s <= e)
    {
        int m = ((s + e)>>1);
        if (array[pos[m]] > des)
        {
            e = m - 1;
        }
        else
        {
            s = m + 1;
        }
    }
    return s; //s must be larger than des
}

void lis_nlogn(int array[], int size)
{
    int *m = (int *)malloc(sizeof(int) * size);
    int *p = (int *)malloc(sizeof(int) * size);
    int len, i;

    m[0] = 0;
    p[0] = -1;
    for (i = len = 1; i < size; i++)
    {
        int pos = divide_find_smaller2(array, m, len, array[i]);
        if (pos > 0)
            p[i] = m[pos - 1];
        else
            p[i] = -1;
        m[pos] = i;

        if (pos == len)
        {
            len += 1;
        }
    }

    i = m[len-1];
    do
    {
        printf("%d\n", array[i]);
        i = p[i];
    }while(i >= 0);
    printf("nlogn -- len: %d\n", len);

    free(m);
    free(p);
}



void dpTest()
{
    //RabitTor();
    const char *p = "abccbauvwwvu";
    int *test_data = (int *)malloc(sizeof(int) * DP_TEST_SIZE);
    ReadData(test_data, sizeof(int), DP_TEST_SIZE, RAWDATA_TYPE_BYTE, "raw_data.img");

    pali_cut(p);

    int sample[] = {1,2,3,4,5,3,4,5,7,8,9, 3,3,3,3,3,3,3,3,3};
    lis_dp(test_data, DP_TEST_SIZE);
    lis_nlogn(test_data, DP_TEST_SIZE);
    lis_dp(sample, sizeof(sample)/sizeof(int));
    lis_nlogn(sample, sizeof(sample)/sizeof(int));
}
