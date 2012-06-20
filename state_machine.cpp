#include <stdio.h>
#include <list>
#include <vector>
using namespace std;

struct SimState
{
    bool man_cross;
    bool wolf_cross;
    bool sheep_cross;
    bool veg_cross;
};

bool operator == (const SimState& a, const SimState&b)
{
    if (a.man_cross == b.man_cross && a.sheep_cross == b.sheep_cross && a.veg_cross && b.veg_cross && a.wolf_cross == b.wolf_cross)
        return true;
    else
        return false;
}

enum SimOperation
{
    MAN_CROSS,
    MAN_WITH_WOLF,
    MAN_WITH_SHEEP,
    MAN_WITH_VEG,
    MAN_WITH_WOLF_BACK,
    MAN_WITH_SHEEP_BACK,
    MAN_WITH_VEG_BACK,
    MAN_BACK
};

bool ConfirmState(SimState &st, SimOperation op)
{
    bool confirm = true;
    switch(op)
    {
    case MAN_CROSS:
        if (st.man_cross == true || (st.sheep_cross == false && st.veg_cross == false) || (st.wolf_cross == false && st.sheep_cross == false))
            confirm = false;
        break;
    case MAN_WITH_WOLF:
        if (st.man_cross == true || st.wolf_cross == true || (st.sheep_cross == false && st.veg_cross == false))
            confirm = false;
        break;
    case MAN_WITH_SHEEP:
        if (st.man_cross == true || st.sheep_cross == true)
            confirm = false;
        break;
    case MAN_WITH_VEG:
        if (st.man_cross == true || st.veg_cross == true || (st.wolf_cross == false && st.sheep_cross == false))
            confirm = false;
        break;
    case MAN_WITH_WOLF_BACK:
        if (st.man_cross == false || st.wolf_cross == false || (st.veg_cross == true && st.sheep_cross == true))
            confirm = false;
        break;
    case MAN_WITH_SHEEP_BACK:
        if (st.man_cross == false || st.sheep_cross == false)
            confirm = false;
        break;
    case MAN_WITH_VEG_BACK:
        if (st.man_cross == false || st.veg_cross == false || (st.sheep_cross == true && st.wolf_cross == true))
            confirm = false;
        break;
    case MAN_BACK:
        if (st.man_cross == false || (st.sheep_cross == true && st.veg_cross == true) || (st.wolf_cross == true && st.sheep_cross == true))
            confirm = false;
        break;
    }

    return confirm;
}

void StateTrans(SimState &st, SimOperation op)
{
    switch(op)
    {
    case MAN_CROSS:
        st.man_cross = true;
        break;
    case MAN_WITH_WOLF:
        st.man_cross = true;
        st.wolf_cross = true;
        break;
    case MAN_WITH_SHEEP:
        st.man_cross = true;
        st.sheep_cross = true;
        break;
    case MAN_WITH_VEG:
        st.man_cross = true;
        st.veg_cross = true;
        break;
    case MAN_WITH_WOLF_BACK:
        st.man_cross = false;
        st.wolf_cross = false;
        break;
    case MAN_WITH_SHEEP_BACK:
        st.man_cross = false;
        st.sheep_cross = false;
        break;
    case MAN_WITH_VEG_BACK:
        st.man_cross = false;
        st.veg_cross = false;
        break;
    case MAN_BACK:
        st.man_cross = false;
        break;
    }
}

bool FindState(list<SimState>& state_list, const SimState& st)
{
    for (list<SimState>::const_iterator iter = state_list.begin(); iter != state_list.end(); iter++)
    {
        if ( *iter == st)
            return true;
    }
    return false;
}

void SimpleStateMachine()
{
    SimState ori_st = {false, false, false, false};
    SimState final_st = {true, true, true, true};
    list<SimState> st_list;
    st_list.clear();
    vector<SimState> st_vec;
    st_vec.clear();
    st_vec.push_back(ori_st);
    st_list.push_back(ori_st);

    bool suc = false;
    unsigned int max_vec_size = 0;

    while(st_vec.size() > 0)
    {
        SimState st = st_vec.front();
        st_vec.erase(st_vec.begin());
        for (int i = MAN_CROSS; i <= MAN_BACK; i++)
        {
            if (ConfirmState(st, (SimOperation)i))
            {
                SimState after = st;
                StateTrans(after, (SimOperation)i);
                if (!FindState(st_list, after))
                {
                    st_list.push_back(after);
                    st_vec.push_back(after);
                    max_vec_size = (max_vec_size > st_vec.size() ? max_vec_size : st_vec.size());
                    if (after == final_st)
                    {
                        suc = true;
                        break;
                    }
                }
            }
        }
        if (suc)
            break;
    }

    if (suc)
    {
        printf("can find a path, can max vector size %d\n", max_vec_size);
    }
    else
    {
        printf("oho\n");
    }
}
void StateMachine()
{
    SimpleStateMachine();
}