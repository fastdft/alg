#include <vector>
#include <stdio.h>

using namespace std;

template <class T>
class MStack
{
public:
    T pop(){ return m_stack.pop_back(); }
    void push(T t) {m_stack.push_back(t);}

private:
    vector<T> m_stack;
};

#define STACK_SIZE 100
extern void *g_stack;
extern int g_stack_end;
template <class T>
void InitializeStack(T t)
{
    //g_stack = (void *)malloc(sizeof(T) * STACK_SIZE);
    g_stack = new T[STACK_SIZE];
    g_stack_end=0;
}
template <class T>
void UninitializeStack(T t){/*free(g_stack); */delete g_stack;g_stack = 0;};
template <class T>
void Push(T t)
{
    if (g_stack_end != STACK_SIZE)
    {
        T *p = (T*)g_stack;
        p[g_stack_end] = t;
        g_stack_end++;
    }
    //TODO: should throw a exception
}
template <class T>
T Pop(T t)
{
    if (g_stack_end != 0)
    {
        g_stack_end--;
        T *p = (T*)g_stack;
        return p[g_stack_end];
    }
    return NULL;
}

template <class T>
T GetTop(T t)
{
    if (g_stack_end != 0)
    {
        T *p = (T*)g_stack;
        return p[g_stack_end - 1];
    }
    return NULL;
}
