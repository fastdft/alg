#include "SbTree.h"
#include "stdio.h"

void SBRotateRight(SBNode **root);
void SBRotateLeft(SBNode **root);
void SBMaintain(SBNode **root);

void SBRotateRight(SBNode **root)
{
    SBNode *node = *root;

    if (node == NULL || node->left == NULL)
        return;

    SBNode *l = node->left;
    node->left = l->right;
    l->right = node;

    l->tick = node->tick;
    node->tick = 1;
    if (node->left)
        node->tick += node->left->tick;
    if (node->right)
        node->tick += node->right->tick;

    *root = l;
}

void SBRotateLeft(SBNode **root)
{
    SBNode *node = *root;

    if (node == NULL || node->right == NULL)
        return;

    SBNode *r = node->right;
    node->right = r->left;
    r->left = node;

    r->tick = node->tick;
    node->tick = 1;
    if (node->right)
        node->tick += node->right->tick;
    if (node->left)
        node->tick += node->left->tick;
    
    *root = r;
}

void SBMaintain(SBNode **root)
{
    SBNode *node = *root;

    if (node == NULL)
        return;
    //this is stupid method
    if (node->left && node->left->left && (node->right == NULL || node->left->left->tick > node->right->tick))
    {
        SBRotateRight(root);
        SBMaintain(&node->right);
        SBMaintain(root);
    }
    if (node->left && node->left->right && (node->right == NULL || node->left->right->tick > node->right->tick))
    {
        SBRotateLeft(&node->left);
        SBRotateRight(root);
        SBMaintain(&(*root)->left);
        SBMaintain(&(*root)->right);
        SBMaintain(root);
    }
    if (node->right && node->right->right && (node->left == NULL || node->right->right->tick > node->left->tick))
    {
        SBRotateLeft(root);
        SBMaintain(&node->left);
        SBMaintain(root);
    }
    if (node->right && node->right->left && (node->left == NULL || node->right->left->tick > node->left->tick))
    {
        SBRotateRight(&node->right);
        SBRotateLeft(root);
        SBMaintain(&(*root)->right);
        SBMaintain(&(*root)->left);
        SBMaintain(root);
    }
}

void SBInsert(SBNode **root, SBValue &value)
{
    if (*root == NULL)
    {
        *root = new SBNode;
        (*root)->val = value;
        (*root)->tick = 1;
        return;
    }
    if ((*root)->val == value)
        return;

    (*root)->tick++;
    if (value < (*root)->val)
    {
        SBInsert(&(*root)->left, value);
    }
    else
    {
        SBInsert(&(*root)->right, value);
    }

    SBMaintain(root);
}

SBNode **SBSearch(SBNode **root, SBValue &value)
{
    SBNode *node = *root;
    if (node == NULL || node->val == value)
    {
        return root;
    }

    if (node->val > value)
    {
        return SBSearch(&node->left, value);
    }
    else
    {
        return SBSearch(&node->right, value);
    }
}

void SBRemove(SBNode **root, SBValue &value)
{
    if (*root == NULL)
        return;
    SBNode *pNode = *root;
    SBNode **ppNode = root;

    pNode->tick--;

    if (pNode->val == value)
    {
        //if the node have no child
        if (pNode->left == NULL && pNode->right == NULL)
        {
            delete pNode;
            (*ppNode) = NULL;
        }
        //if the node have two children
        else if (pNode->left && pNode->right)
        {
            SBNode *p = pNode, *now = pNode->right;
            while(now->left)
            {
                //now has left children
                now->tick--;
                p = now;
                now = now->left;
            }
            if (p->right != now)
                p->left = now->right;
            else
                p->right = now->right;

            (*ppNode) = now;
            now->left = pNode->left;
            now->right = pNode->right;
            now->tick = pNode->tick;

            delete pNode;
        }
        //if the node have one child
        else if (pNode->left || pNode->right)
        {
            (*ppNode) = (pNode->left ? pNode->left : pNode->right);
            delete pNode;
        }
    }
    else if (value > pNode->val)
    {
        SBRemove(&(pNode->right), value);
    }
    else
    {
        SBRemove(&(pNode->left), value);
    }
}


#include "ReadData.h"
#define SBTREE_TEST_DATA_SIZE  1000

#include <time.h>
#include <iostream>
#include "stdio.h"
#include "time_statistics.h"

using namespace std;

void PreTraverse(SBNode *root, int deep)
{
    if (root->left)
        PreTraverse(root->left, deep+1);

    printf("%d\t%d\t%d\n", root->val.key, root->tick, deep);

    if (root->right)
        PreTraverse(root->right, deep+1);
}

void SBTreeTest()
{
    //construct the tree
    int data[SBTREE_TEST_DATA_SIZE] = {0};
    ReadData((void *)data, sizeof(int), SBTREE_TEST_DATA_SIZE, RAWDATA_TYPE_BYTE, "raw_data.img");

    SBNode *root = NULL;
    for (int i=0; i<SBTREE_TEST_DATA_SIZE; i++)
    {
        SBValue value;
        value.key = data[i];
        SBInsert(&root, value);
    }

    SBValue rm_node;
    rm_node.key = 2124572311;
    SBRemove(&root, rm_node);
    
    //PreTraverse(root, 0);
	time_start();
    for (int j=0; j<1000; j++)
    for (int i=0; i<499; i++)
    {
        SBValue value;
        value.key = data[i*2];
        SBSearch(&root, value);
        SBRemove(&root, value);
        SBInsert(&root, value);
    }
    printf("SBTreeTest time %d\n", get_time_used());
}
