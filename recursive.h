void hanoi(int , int);

struct KnapItem
{
    int value;
    int size;
};

int knap(struct KnapItem items[], int items_size, int cap);
int dynamic_knap(struct KnapItem items[], int items_size, int cap);
