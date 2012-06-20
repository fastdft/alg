#ifndef SBTREE_H
#define SBTREE_H

class SBValue
{
public:
    SBValue():key(0){}
    int key;
    SBValue operator + (SBValue a)
    {
        SBValue ret;
        ret.key = a.key + key;
        return ret;
    }

    SBValue operator - (SBValue a)
    {
        SBValue ret;
        ret.key = a.key - key;
        return ret;
    }

    bool operator > (SBValue a)
    {
        return key > a.key;
    }

    bool operator < (SBValue a)
    {
        return key < a.key;
    }

    bool operator == (SBValue a)
    {
        return key == a.key;
    }
};

class SBNode
{
public:
    SBNode():left(0), right(0), tick(0){}
    SBNode *left;
    SBNode *right;
    SBValue val;
    int tick;
};

void SBInsert(SBNode **root, SBValue &value);
void SBRemove(SBNode **root, SBValue &value);
SBNode **SBSearch(SBNode **root, SBValue &value);
SBValue SBRank(SBNode *root, int rank);

#endif