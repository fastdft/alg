#ifndef INTERVALTREE
#define INTERVALTREE

#include <stdio.h>

typedef struct
{
    int seg_l;
    int seg_r;
    int value;
} InterTreeNode;

#define MAX_INTERVAL_TREE_NODE 5000

int ConstructIntervalTree(const char *path);
int IntervalTreeQuery(int left, int right);
void IntervalTreeAdd(int index, int num);
void IntervalTreeSub(int index, int num);

int IntervalTreeTest();

#endif

