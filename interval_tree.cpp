#include "interval_tree.h"
#include <iostream>
#include <fstream>
#include "assert.h"
#include <string.h>

using namespace std;

InterTreeNode interval_tree[MAX_INTERVAL_TREE_NODE] = {0};

void BuildTree(int values[], int l, int r, int index)
{
    interval_tree[index].seg_l = l;
    interval_tree[index].seg_r = r;
    if (r != l)
    {
        for (int i=l; i<=r; i++)
        {
            interval_tree[index].value += values[i];
        }

        int mid = (l+r)/2;
        BuildTree(values, l, mid, index*2);
        BuildTree(values, mid+1, r, index*2+1);
    }
    else
    {
        interval_tree[index].value = values[l];
    }
}
void ConstructIntervalTree(int values[], int num)
{
    memset(interval_tree, 0, sizeof(interval_tree));

    int node_num = 1;
    if ((num & (num-1) == 0))
        node_num = num;
    else
        while(num)
        {
            node_num *= 2;
            num /= 2;
        }

    //node_num = node_num *2 -1;
    assert(node_num < 5000);

    BuildTree(values, 0, node_num-1, 1);
}

int SearchNode(int index, int left, int right)
{
    InterTreeNode *node = &interval_tree[index];
    int mid = (node->seg_l + node->seg_r)/2;

    if (node->seg_l == left && node->seg_r == right)
    {
        return node->value;
    }
    else if (right <= mid)
    {
        return SearchNode(2*index, left, right);
    }
    else if (left > mid)
    {
        return SearchNode(2*index + 1, left, right);
    }
    else
    {
        return SearchNode(2*index, left, mid) + SearchNode(2*index + 1, mid+1, right);
    }
}

int IntervalTreeQuery(int left, int right)
{
    return SearchNode(1, left-1, right-1);
}

void IntervalTreeAdd(int index, int num)
{
    int pos = 1;
    index--;
    InterTreeNode *node = &interval_tree[pos];
    while(node->seg_l != node->seg_r)
    {
        int mid = (node->seg_l + node->seg_r)/2;
        if (index <= mid)
            pos = pos * 2;
        else
            pos = pos * 2 + 1;

        //should update the node in the path to leaf
        node->value += num;
        node = &interval_tree[pos];
    }

    node->value += num;
}

void IntervalTreeSub(int index, int num)
{
    IntervalTreeAdd(index, -num);
}

int IntervalTreeTest()
{
    const char *path = "interval_tree_enemy.txt";

    ifstream infile(path);
    if (infile.fail())
        return -1;
    int num = 0;
    infile>>num;

    int values[5000] = {0};
    for (int i=0; i<num; i++)
    {
        infile>>values[i];
    }
    ConstructIntervalTree(values, num);

    char command[100];
    int left, right;
    while(1)
    {
        memset(command, 0, sizeof(command));
        infile>>command;
        infile>>left;
        infile>>right;

        if (command[0] == 0)
            break;

        if (command[0] == 'Q')
        {
            int ret = IntervalTreeQuery(left, right);
            cout<<"Query "<<ret<<endl;
        }
        else if (command[0] == 'A')
            IntervalTreeAdd(left, right);
        else if (command[0] == 'S')
            IntervalTreeSub(left, right);
    }
    return 0;
}
