#include "stdio.h"

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


void dpTest()
{
    RabitTor();
}
