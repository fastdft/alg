#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <cmath>
#include <assert.h>

using namespace std;

template <class Type>
class MinMaxHeap
{
    public:
        MinMaxHeap(int size);
        void insert(Type element);
        Type extract_min();
        Type extract_max();
        bool has_next();

    private:
        void fix_down(int index);
        void fix_up(int index);

        bool compare(int key, int i, int j);
        void swap(Type &a, Type &b);

        int get_parent(int index);
        int get_grand_father(int index);
        int get_proper_child(int index);
        int get_proper_grand_child(int index);

    private:
        vector<Type> m_array;
        int m_end;
};

template<class Type>
MinMaxHeap<Type>::MinMaxHeap(int size)
:m_array(size)
,m_end(0)
{}


template<class Type>
void MinMaxHeap<Type>::insert(Type element)
{
    m_array[m_end++] = element;

    int p = get_parent(m_end - 1);
    if (p < m_end)
    {
        if (compare(p, p, m_end - 1) == false)
        {
            swap(m_array[p], m_array[m_end - 1]);
            fix_up(p);
        }
        else
        {
            fix_up(m_end - 1);
        }
    }
}

template<class Type>
Type MinMaxHeap<Type>::extract_min()
{
    assert(m_end > 0);
    Type ret = m_array[0];
    m_array[0] = m_array[--m_end];

    //it's same to the min node increase
    fix_down(0);

    return ret;
}

template<class Type>
Type MinMaxHeap<Type>::extract_max()
{
    assert(m_end > 0);

    if (m_end > 2)
    {
        Type ret;
        int index;

        if (m_array[1] > m_array[2])
        {
            ret = m_array[1];
            index = 1;
        }
        else
        {
            ret = m_array[2];
            index = 2;
        }

        m_array[index] = m_array[--m_end];
        //it same to the max node decrease
        fix_down(index);

        return ret;
    }
    else
        return m_array[--m_end];
}

template<class Type>
bool MinMaxHeap<Type>::has_next()
{
    return m_end > 0;
}

template<class Type>
void MinMaxHeap<Type>::fix_up(int index)
{
    int gf = get_grand_father(index);
    if (gf < m_end && compare(index, gf, index) == false)
    {
        swap(m_array[index], m_array[gf]);
        fix_up(gf);
    }
}

template<class Type>
void MinMaxHeap<Type>::fix_down(int index)
{
    //take index in min heap for example
    //three cases:
    //1. m_array[index] decrease, nothing could happend
    //2. m_array[index] increase, and m_array[index] < m_array[left, right], only need to consider the min heap level
    //3. m_array[index] increase, and m_array[index] > marray[left | right]
    int child = get_proper_child(index);
    if (child < m_end && compare(index, index, child) == false)
    {
        //turn to case 2
        swap(m_array[index], m_array[child]);
    }

    int gchild = get_proper_grand_child(index);
    if (gchild < m_end && compare(index, index, gchild) == false)
    {
        swap(m_array[index], m_array[gchild]);
        fix_down(gchild);
    }
}

template<class Type>
void MinMaxHeap<Type>::swap(Type &a, Type &b)
{
    Type t = a;
    a = b;
    b = t;
}

template<class Type>
bool MinMaxHeap<Type>::compare(int key, int i, int j)
{   
    int level = log2(key + 1);
    if (level % 2 == 0)
    {
        //min heap
        if (m_array[i] < m_array[j])
            return true;
        else
            return false;
    }
    else
    {
        //max heap
        if (m_array[i] > m_array[j])
            return true;
        else
            return false;
    }
}

template<class Type>
int MinMaxHeap<Type>::get_parent(int index)
{
    if (index == 0)
        return m_end;
    else
        return (index - 1)/2;
}

template<class Type>
int MinMaxHeap<Type>::get_grand_father(int index)
{
    if (index < 3)
        return m_end;
    else
        return ((index - 1)/2 - 1)/2;
}

template<class Type>
int MinMaxHeap<Type>::get_proper_child(int index)
{
    int l = index * 2 + 1;
    int r = index * 2 + 2;
    if (l >= m_end)
        return m_end;
    else if (r >= m_end)
        return l;
    else
        return (compare(index, l, r) ? l : r);
}

template<class Type>
int MinMaxHeap<Type>::get_proper_grand_child(int index)
{
    int l = index * 2 + 1;
    int r = index * 2 + 2;
    if (l * 2 + 1 >= m_end)
    {
        return m_end;
    }

    int gc[4];
    gc[0] = l * 2 + 1;
    gc[1] = l * 2 + 2;
    gc[2] = r * 2 + 1;
    gc[3] = r * 2 + 2;

    int ret = gc[0];
    for (int i=1; i<4 && gc[i] < m_end; i++)
    {
        if (compare(index, ret, gc[i]) == false)
            ret = gc[i];
    }

    return ret;
}

#endif
