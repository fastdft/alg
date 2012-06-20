#include "tree.h"
#include <stdio.h>

Node* ConstructTree()
{
    Node *root = new Node();
    root->value = 0;
    Node *left = new Node();
    left->value = 1;
    Node *right = new Node();
    right->value = 2;
    root->left = left;
    root->right = right;

    left = new Node();
    left->value = 3;
    left->left = NULL;
    left->right = NULL;
    right = new Node();
    right->value = 4;
    right->left = NULL;
    right->right = NULL;
    root->left->left = left;
    root->left->right = right;

    left = new Node();
    left->value = 5;
    left->left = NULL;
    left->right = NULL;
    right = new Node();
    right->value = 6;
    right->left = NULL;
    right->right = NULL;
    root->right->left = left;
    root->right->right = right;

    return root;
}
void PreTraverse(Node *root, void (*visit)(Node *))
{
    Push(root);
    Node *node = NULL;
    while((node = Pop(node)) != NULL)
    {
        //this is preoder
        (*visit)(node);
        if (node->right)
            Push(node->right);
        if (node->left)
            Push(node->left);

        //this is inorder
        //this is postorder
    }
}

void InTraverse(Node *root, void (*visit)(Node *))
{
    Push(root);
    Node *node = NULL;
    while(node = GetTop(root))
    {
        while(node->left)
        {
            Push(node);
            node = node->left;
        }

        (*visit)(node);

        node = Pop(node);

        if (node->right)
        {
            Push(node->right);
        }
    }
}

void PostTraverse(Node *root, void (*visit)(Node *))
{}

void visit(Node *node)
{
    printf("node %d\t", node->value);
   // delete node;
}

void FreeNode(Node *node)
{
    delete node;
}
void test_traverse()
{
    Node *root=NULL;
    InitializeStack(root);
    root = ConstructTree();
    PreTraverse(root, visit);
    InTraverse(root, visit);
    UninitializeStack(root);
}
