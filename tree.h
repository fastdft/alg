#include <string>
#include "stack.h"

using namespace std;

struct Node
{
    int value;
    Node *left;
    Node *right;
};

void test_traverse();