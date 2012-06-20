#include <stdio.h>
#include <time.h>
#include "time_statistics.h"

#define ITEMS_SIZE 15
#define MAX_PACKAGE_VOL 1000

void Package01(int pkg_vol, int cost[], int value[], int size)
{
    int val_by_vol[MAX_PACKAGE_VOL] = {0};
    
    for (int i=0; i<size; i++)
    {
        for (int j = pkg_vol; j>cost[i]; j--)
        {
            val_by_vol[j] = val_by_vol[j] < (val_by_vol[j-cost[i]] + value[i]) ? val_by_vol[j-cost[i]] + value[i] : val_by_vol[j];
        }
    }

    printf("Package01 : \n vol : %d, value : %d\n", pkg_vol, val_by_vol[pkg_vol]);
}

void PackageComplete(int pkg_vol, int cost[], int value[], int size)
{
    int val_by_vol[MAX_PACKAGE_VOL] = {0};
    
    for (int i=0; i<size; i++)
    {
        for (int j=cost[i]+1; j<=pkg_vol; j++)
        {
            val_by_vol[j] = val_by_vol[j] < (val_by_vol[j-cost[i]] + value[i]) ? val_by_vol[j-cost[i]] + value[i] : val_by_vol[j];
        }
    }

    //printf("PackageComplete : \n vol : %d, value : %d\n", pkg_vol, val_by_vol[pkg_vol]);
}

int g_pkg_complete_recurve[MAX_PACKAGE_VOL] = {0};
int PackageCompleteRecurve(int pkg_vol, int cost[], int value[], int size)
{
    int max_value = 0;

    if (g_pkg_complete_recurve[pkg_vol])
        return g_pkg_complete_recurve[pkg_vol];
    
    for (int i=0; i<size; i++)
    {
        if (pkg_vol > cost[i])
        {
            int v = PackageCompleteRecurve(pkg_vol - cost[i], cost, value, size) + value[i];
            max_value = max_value < v ? v : max_value;
        }
    }
    g_pkg_complete_recurve[pkg_vol] = max_value;

    return max_value;
}

void PackageMulti(int pkg_vol, int cost[], int value[], int number[], int size)
{}

void PackageTest()
{
    int package_vol = MAX_PACKAGE_VOL - 1;
    int cost[ITEMS_SIZE] = {2,3,4,7,9, 10, 14, 15, 17, 19, 20, 22, 30, 34, 50, };
    int value[ITEMS_SIZE] = {10,16,19,25,30, 40, 43, 56, 54, 32, 57, 76, 87, 90, 89};
    int number[ITEMS_SIZE] = {0};

    Package01(package_vol, cost, value, ITEMS_SIZE);

    time_start();
    for (int i=0; i<1000; i++)
        PackageComplete(package_vol, cost, value, ITEMS_SIZE);
    printf("PackageComplete time %d\n", get_time_used());

	time_start();
    for (int i=0; i<1000; i++)
        PackageCompleteRecurve(package_vol, cost, value, ITEMS_SIZE);
    printf("PackageCompleteRecurve time %d\n", get_time_used());
    //printf("PackageCompleteRecurve : \n vol : %d, value : %d", package_vol, ret);
}
