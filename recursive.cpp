#include "recursive.h"
#include "stdio.h"
void shift(int, int);
void hanoi(int n, int d)
{
    if (n == 0)
        return;
    hanoi(n-1, -d);
    shift(n, d);
    hanoi(n-1, -d);
}

void shift(int index, int d)
{
    printf("shift index %d to %s\n", index, d > 0 ? "right" : "left");
}


int knap(struct KnapItem items[], int items_size, int cap)
{
    int max_value = 0;
    for (int i=0; i<items_size; i++)
    {
        if (cap - items[i].size >= 0)
        {
            int remain = knap(items, items_size, cap-items[i].size);
            if (remain + items[i].value > max_value)
                max_value = remain + items[i].value;
        }
    }

    return max_value;
}

int known_value[100] = {0};
int dynamic_knap(struct KnapItem items[], int items_size, int cap)
{
    int max_value = 0;
    if (known_value[cap])
        return known_value[cap];

    for (int i=0; i<items_size; i++)
    {
        if (cap - items[i].size >= 0)
        {
            int remain = dynamic_knap(items, items_size, cap-items[i].size);
            if (remain + items[i].value > max_value)
                max_value = remain + items[i].value;
        }
    }

    known_value[cap] = max_value;

    return max_value;
}