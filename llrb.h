#ifndef LLRB_H
#define LLRB_H

struct LlrbNode
{
    LlrbNode(int k, int v, bool color){key = k; value = v; bRed = color; l = NULL; r = NULL;}
    int key;
    int value;
    LlrbNode *l;
    LlrbNode *r;
    bool bRed;
};

LlrbNode *LlrbInsert(LlrbNode *root, int key, int value);
LlrbNode *LlrbRemove(LlrbNode *root, int key);

#endif
