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

//InTraverse(node->left)
//InTraverse(node)
//InTraverse(node->right)
void push_left_node(Node *node)
{
    while(node != NULL)
    {
        Push(node);
        node = node->left;
    }
}

void InTraverse1(Node *root, void (*visit)(Node *))
{
    Node *node = NULL;

    push_left_node(root);
    while((node = Pop(root)) != NULL)
    {
        (*visit)(node);
        push_left_node(node->right);
    }
}

void InTraverse2(Node *root, void (*visit)(Node *))
{
    Node *node = root;
    do
    {
        if (node != NULL)
        {
            Push(node);
            node = node->left;
        }
        else
        {
            node = Pop(root);
            (*visit)(node);
            node = node->right;
        }
    }while( node!=NULL || GetTop(root) != NULL);
}

void PostTraverse(Node *root, void (*visit)(Node *))
{}

//this is a morris_in_traverse
void MorrisTraverse(Node *root, void(*visit)(Node *))
{
    Node *node = root;
    while(node != NULL)
    {
        if (node->left == NULL)
        {
            (*visit)(node);
            node = node->right;
        }
        else
        {
            Node *tmp = node->left;
            while(tmp->right != NULL && tmp->right != node)
            {
                tmp = tmp->right;
            }
            if (tmp->right == node)
            {
                tmp->right = NULL;
                visit(node);
                node = node->right;
            }
            else
            {
                tmp->right = node;
                node = node->left;
            }
        }
    }
}

void visit(Node *node)
{
    printf("node %d,\t", node->value);
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
    printf("pre: \n");
    PreTraverse(root, visit);
    printf("\n");
    printf("in1: \n");
    InTraverse1(root, visit);
    printf("\n");
    printf("in2: \n");
    InTraverse2(root, visit);
    printf("\n");
    printf("in: \n");
    MorrisTraverse(root, visit);
    printf("\n");
    UninitializeStack(root);
}
