#include "stdio.h"
#include "stdlib.h"
#include "math.h"

struct Point
{
    double x, y;
    Point operator + (Point a)
    {
        Point ret;
        ret.x = x + a.x;
        ret.y = y + a.y;
        return ret;
    }
    Point operator - (Point a)
    {
        Point ret;
        ret.x = x - a.x;
        ret.y = y - a.y;
        return ret;
    }
};

struct CraneTreeNode
{
    int left, right;
    int angle;
    Point s, e;
    int mid()
    {
        return (left + right)/2;
    }

    void turn(int angle_now)
    {
        Point diff = e - s;
        double ang_diff = (angle_now - angle + 360)%360 *Pi /180;
        int dx = diff.x*cos(ang_diff) - diff.y*sin(ang_diff);
        int dy = diff.x*sin(ang_diff) + diff.y*cos(ang_diff);
        e.x += dx;
        e.y += dy;
        angle = angle_now;
    }
};

#define N 100
CraneTreeNode nodes[4*N];
int length[N];
int sum[N];

void BuildTree(int l, int r, int idx)
{
    if (l == r)
    {
        nodes[idx].left = nodes[idx].right = l;
        nodes[idx].angle = 180;
        nodes[idx].s.x = 0;
        nodes[idx].s.y = sum[l] - length[l];
        nodes[idx].e.x = 0;
        nodes[idx].e.y = sum[l];
        return;
    }
    int mid = (l+r)/2;
    BuildTree(l, mid, 2*idx);
    BuildTree(mid+1, r, 2*idx + 1);

    nodes[idx].left = l;
    nodes[idx].right = r;
    nodes[idx].angle = 180;
    nodes[idx].s.x = 0;
    nodes[idx].s.y = nodes[idx*2].s.y;
    nodes[idx].e.x = 0;
    nodes[idx].e.y = nodes[idx*2 + 1].e.y;
}

void update(int l, int angle, int idx)
{
    if (nodes[idx].left == l)
    {
        nodes[idx].trun(angle);
        return;
    }

    int mid = (nodes[idx].left + nodes[idx].right)/2;
    if (l <= mid)
    {
        int old_ang = nodes[idx].angle;
        update(l, angle, 2*idx);
        
    }
    else
    {
        update(l, angle, 2*idx+1);
    }
}
