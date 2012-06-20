#include <stdio.h>
#include "llrb.h"

LlrbNode *LlrbRotRight(LlrbNode *root);
LlrbNode *LlrbRotLeft(LlrbNode *root);

LlrbNode *LlrbRotRight(LlrbNode *root)
{
    LlrbNode *left = root->l;
    root->l = left->r;
    left->r = root;

    left->bRed = root->bRed;
    root->bRed = true;

    return left;
}

LlrbNode *LlrbRotLeft(LlrbNode *root)
{
    LlrbNode *right = root->r;
    root->r = right->l;
    right->l = root;

    right->bRed = root->bRed;
    root->bRed = true;

    return right;
}

void FlipColor(LlrbNode *root)
{
    root->bRed = !root->bRed;
    root->l->bRed = !root->l->bRed;
    root->r->bRed = !root->r->bRed;
}

LlrbNode *LlrbFixUp(LlrbNode *root)
{
	//the sequence of "if" is important
    if (root->r && root->r->bRed)
        root = LlrbRotLeft(root);
    if (root->l && root->l->l && root->l->bRed && root->l->l->bRed)
        root = LlrbRotRight(root);
    if (root->l && root->r && root->l->bRed && root->r->bRed)
        FlipColor(root);

    return root;
}
LlrbNode *LlrbInsert(LlrbNode *root, int key, int value)
{
    if (root == NULL)
        return new LlrbNode(key, value, true);

    if (root->key == key)
    {
        root->value = value;
    }
    else if (key > root->key)
    {
        root->r = LlrbInsert(root->r, key, value);
    }
    else
    {
        root->l = LlrbInsert(root->l, key, value);
    }

    root = LlrbFixUp(root);

    return root;
}

LlrbNode *LlrbSearch(LlrbNode *root, int key)
{
    if (root == NULL || root->key == key)
        return root;

    if (root->key > key)
        return LlrbSearch(root->l, key);
    else
        return LlrbSearch(root->r, key);
}

//the root must be red node
LlrbNode *LlrbMoveRedLeft(LlrbNode *root)
{
    FlipColor(root);
	//take care of the node which is not on the way up
    if (root->r && root->r->l && root->r->l->bRed)
    {
        root->r = LlrbRotRight(root->r);
        root = LlrbRotLeft(root);
        FlipColor(root);
    }
    return root;
}

LlrbNode *LlrbMoveRedRight(LlrbNode *root)
{
    FlipColor(root);
	//still don't know why this step
    if (root->l && root->l->l && root->l->l->bRed)
    {
        root = LlrbRotRight(root);
        FlipColor(root);
    }
    return root;
}

LlrbNode *LlrbRemoveMin(LlrbNode *root)
{
    if (root->l == NULL)
    {
        delete root;
        return NULL;
    }

    if (!(root->l && root->l->bRed) && !(root->l && root->l->l && root->l->l->bRed))
        root = LlrbMoveRedLeft(root);

    root->l = LlrbRemoveMin(root->l);

    return LlrbFixUp(root);
}

LlrbNode *LlrbGetMin(LlrbNode *root)
{
    if (root->l)
        return LlrbGetMin(root->l);
    else
        return root;
}

//Notice: the key must be in the tree
//invairent: the root or its child is red
LlrbNode *LlrbRemove(LlrbNode *root, int key)
{
    if (key < root->key)
    {
        if (!(root->l && root->l->bRed) && !(root->l && root->l->l && root->l->l->bRed))
            root = LlrbMoveRedLeft(root);
        root->l = LlrbRemove(root->l, key);
    }
    else
    {
        //because it will go right, so we have to make its right child red, after this operation, the root will be smaller, still go right
        if (root->l && root->l->bRed)
            root = LlrbRotRight(root);

		//in this step, root->left can't be red, if root have left child, it must have right child, so verse
        //if find the node in the leaf
        if (root->key == key && root->r == NULL)
        {
            delete root;
            return NULL;
        }

        //will go to right
        if (!(root->r && root->r->bRed) && !(root->r && root->r->l && root->r->l->bRed))
            root = LlrbMoveRedRight(root);
        if (root->key == key)
        {
            LlrbNode *node = LlrbGetMin(root->r);
            root->key = node->key;
            root->value = node->value;
            root->r = LlrbRemoveMin(root->r);
        }
        else
            root->r = LlrbRemove(root->r, key);
    }

    return LlrbFixUp(root);
}

#include "ReadData.h"
#define SBTREE_TEST_DATA_SIZE  1000

#include <time.h>
#include "time_statistics.h"
#include <iostream>
#include "stdio.h"

void PreTraverse(LlrbNode *root, int deep)
{
    if (root->l)
        PreTraverse(root->l, deep+1);

    printf("%d\t%d\t%d\t%d\n", root->key, root->value, root->bRed, deep);

    if (root->r)
        PreTraverse(root->r, deep+1);
}

void LlrbTest()
{
    //construct the tree
    int data[SBTREE_TEST_DATA_SIZE] = {0};
    ReadData((void *)data, sizeof(int), SBTREE_TEST_DATA_SIZE, RAWDATA_TYPE_BYTE, "raw_data.img");

    LlrbNode *root = NULL;
    for (int i=0; i<SBTREE_TEST_DATA_SIZE; i++)
    {
        root = LlrbInsert(root, data[i], data[i]);
    }

    //PreTraverse(root, 0);

	time_start();
    for (int j=0; j<1000; j++)
    for (int i=0; i<499; i++)
    {
        LlrbSearch(root, data[i*2]);
        root = LlrbRemove(root, data[i*2]);
        root = LlrbInsert(root, data[i*2], data[i*2]);
    }
    printf("LlrbTest time %d\n", get_time_used());
}
