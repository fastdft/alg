#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void dpTest()
{
    //RabitTor();
    const char *p = "abccbauvwwvu";
    pali_cut(p);
}
