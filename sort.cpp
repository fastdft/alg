#include "sort.h"
#include "string.h"
#include "assert.h"
#include "stdlib.h"
#include "time.h"
#include <stdio.h>

using namespace std;

void count_sort(int data[], int data_size, int max_data, int out[])
{
	int buff[5000] = {0};
	for (int i=0; i<data_size; i++)
		buff[data[i]]++;

	for (int i=1; i<max_data; i++)
	{
		buff[i] += buff[i-1];
	}

	//start from the end of array, for this is a stable sort
	for (int i=data_size-1; i>=0; i--)
	{
		out[buff[data[i]]-1] = data[i];
		buff[data[i]]--;
	}
}

void swap(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
}
void quik_sort(int data[], int l, int u)
{
    if (u<=l)
        return;

    int num = l;
    for (int i=l+1; i<=u; i++)
    {
        if (data[l] > data[i])
        {
            num++;
            swap(&data[num], &data[i]);
        }
    }
    swap(&data[l], &data[num]);

    quik_sort(data, l, num-1);
    quik_sort(data, num+1, u);
}

void quiker_sort(int data[], int l, int u)
{
    //in this program, i,j means the next legal pos,
    if (u<=l)
        return;
    int i=l;
    int j=u+1;
    swap(&data[(l+u)/2], &data[l]);
    while(true)
    {
        //data[l+1..i]<data[l] data[j..r]>data[l]
        do i++; while(i<=u && data[i] < data[l]);
        do j--; while(data[j] > data[l]);
        if (i>j)//i>=j also ok
            break;
        swap(&data[i], &data[j]);
    }

    swap(&data[l], &data[j]);
    quiker_sort(data, l, j-1);
    quiker_sort(data, j+1, u);
}

void hoare_sort(int data[], int l, int r)
{
    if (r<=l)
        return;
    if(r<8)
    {
        int xx = 0;
    }
    int i=l-1;
    int j=r+1;
    int x = data[(l+r)/2];
    swap(&data[(l+r)/2], &data[l]);
    while(true)
    {
        do j--; while(data[j] > x);
        do i++; while(data[i] < x);
        
        if (i<j)
            swap(&data[i], &data[j]);
        else
            break;
    }

    hoare_sort(data, l, j);
    hoare_sort(data, j+1, r);
}

//every recursive must have at least 3 elements
void three_seg_qsort(int data[], int l, int r)
{
	if (l >= r)
		return;

    swap(&data[l+1], &data[(l+r)/2]);
    if (data[l]>data[l+1])
        swap(&data[l], &data[l+1]);
    if (data[l]>data[r])
        swap(&data[l], &data[r]);
    if (data[l+1] > data[r])
        swap(&data[l+1], &data[r]);

    if (r-l<=2)
        return;

    int i=l+1, j=r;
    int x = data[l+1];
    while(true)
    {
        //data[l+1..i]<data[l+1], data[j..r]>data[l+1]
        while(data[++i] < x);
        while(data[--j] > x);
        if (i>=j)
            break;
        swap(&data[i], &data[j]);
    }

    swap(&data[l+1], &data[j]);
    three_seg_qsort(data, l, j-1);
    three_seg_qsort(data, j+1, r);
}

void three_seg_qsort_with_insert_sort(int data[], int l, int r)
{
    if (r-l<=9)
        return;

    swap(&data[l+1], &data[(l+r)/2]);
    if (data[l]>data[l+1])
        swap(&data[l], &data[l+1]);
    if (data[l]>data[r])
        swap(&data[l], &data[r]);
    if (data[l+1] > data[r])
        swap(&data[l+1], &data[r]);

    if (r-l<=2)
        return;

    int i=l+1, j=r;
    int x = data[l+1];
    while(true)
    {
        //data[l+1..i]<data[l+1], data[j..r]>data[l+1]
        while(data[++i] < x);
        while(data[--j] > x);
        if (i>=j)
            break;
        swap(&data[i], &data[j]);
    }

    swap(&data[l+1], &data[j]);
    three_seg_qsort(data, l, j-1);
    three_seg_qsort(data, j+1, r);
}

void quick_sort_array(int data[], int data_l, int data_r)
{
    int stack_l[256], stack_r[256];
    int stack_size;

    memset(stack_l, 0, sizeof(stack_l));
    memset(stack_r, 0, sizeof(stack_r));

    stack_size = 0;
    stack_l[stack_size] = data_l;
    stack_r[stack_size] = data_r;
    stack_size++;
    while(stack_size > 0)
    {
        int l = stack_l[stack_size - 1];
        int r = stack_r[stack_size - 1];
        stack_size--;
        if (l == r)
            continue;

        swap(&data[l+1], &data[(l + r)/2]);
        if (data[l]>data[l+1])
            swap(&data[l], &data[l+1]);
        if (data[l]>data[r])
            swap(&data[l], &data[r]);
        if (data[l+1] > data[r])
            swap(&data[l+1], &data[r]);
        if (r - l <= 2)
            continue;

        int i = l + 1;
        int j = r;
        int x = data[l+1];
        while(true)
        {
            while(data[++i] < x);
            while(data[--j] > x);
            if (i >= j)
                break;
            swap(&data[i], &data[j]);
        }
        swap(&data[l+1], &data[j]);

        //TODO: optimization: consider the sequence of stack push, shorter one should push last
        if (j - 1 != l)
        {
            stack_l[stack_size] = l;
            stack_r[stack_size] = j - 1;
            stack_size++;
        }
        if (r != j + 1)
        {
            stack_l[stack_size] = j + 1;
            stack_r[stack_size] = r;
            stack_size++;
        }

        assert(stack_size < 256);
    }
}

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
//notes:
//1.sorted result in buff must be copyed to data;
//2.merge step must be careful
void MergeSort(int data[], int len, int buf[])
{
    //set stop condition
    if (len == 1)
        return;
    if (len == 2)
    {
        if (data[0] > data[1])
            swap(data[0], data[1]);
    }

    //recursive
    MergeSort(data, len/2, buf);
    MergeSort(&data[len/2], len - len/2, buf);

    //merge result
    int i=0, j=len/2, k=0;
    while(i<len/2 || j<len)
    {
        if (i >= len/2)
        {
            buf[k++] = data[j++];
            continue;
        }
        if (j >= len)
        {
            buf[k++] = data[i++];
            continue;
        }

        if (data[i] <= data[j])
        {
            buf[k++] = data[i++];
        }
        else
        {
            buf[k++] = data[j++];
        }
    }

    for (int i=0; i<len; i++)
        data[i] = buf[i];
}

void InsertSort(int data[], int l, int r)
{
    for (int i=l; i<=r; i++)
    {
        if (data[i]<data[l])
            swap(&data[i], &data[l]);
    }

    for (int i=l+2; i<=r; i++)
    {
        int j=i;
        int tmp = data[i];
        while(data[j-1] > tmp)
        {
            data[j] = data[j-1];
            j--;
        }
        data[j] = tmp;
    }
}

void Swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
void ShellSort(int data[], int l, int r)
{
#define FIRST_STEP 4
    //loop until step = 1
    for (int step=FIRST_STEP; step>0; step/=2)
    {
        //for every slot
        for (int index = l; index < l+step; index++)
        {
            //make least number first
            for (int i=index; i<=r; i+=step)
            {
                if (data[i] < data[index])
                    Swap(data[i], data[index]);
            }
            
            for (int i=index+step*2; i<=r; i+=step)
            {
                int temp = data[i];
                int j = i;
                while(temp < data[j-step])
                {
                    data[j] = data[j-step];
                    j-=step;
                }
                data[j] = temp;
            }
        }
    }
}

int heap_left_index(int array_index)
{
	return 2*(array_index + 1) - 1;
}

int heap_right_index(int array_index)
{
	return heap_left_index(array_index) + 1;
}

int heap_parent_index(int array_index)
{
	return (array_index + 1)/2 - 1;
}

void heap_shift_up(int data[], int l, int r)
{
	int parent_index = heap_parent_index(r);
	int now_index = r;
	while (now_index > l && data[parent_index] < data[now_index])
	{
		swap(&data[parent_index], &data[now_index]);
		now_index = parent_index;
		parent_index = heap_parent_index(parent_index);
	}
}
void heap_shift_down(int data[], int l, int r)
{

	int now_index = l;
	while(1)
	{
		int left = heap_left_index(now_index);
		int right = heap_right_index(now_index);
		int large_child_index = 0;

		if (left > r)
			return;
		else if (right > r)
			large_child_index = left;
		else
			large_child_index = (data[left] > data[right] ? left :right);

		if (data[large_child_index] > data[now_index])
		{
			swap(&data[large_child_index], &data[now_index]);
			now_index = large_child_index;
		}
		else
			return;
	}
}
void heap_sort(int data[], int l, int r)
{
	for (int i=l+1; i<=r; i++)
	{
		heap_shift_up(data, l, i);
	}

	for (int i=r; i>l; i--)
	{
		swap(&data[i],&data[l]);
		heap_shift_down(data, l, i-1);
	}
}

#include <bitset>

const int kNumberLeonardoNumbers = 46;
const unsigned int kLeonardoNumbers[kNumberLeonardoNumbers] = {
	1u, 1u, 3u, 5u, 9u, 15u, 25u, 41u, 67u, 109u, 177u, 287u, 465u, 753u, 
    1219u, 1973u, 3193u, 5167u, 8361u, 13529u, 21891u, 35421u, 57313u, 92735u,
    150049u, 242785u, 392835u, 635621u, 1028457u, 1664079u, 2692537u, 
    4356617u, 7049155u, 11405773u, 18454929u, 29860703u, 48315633u, 78176337u,
    126491971u, 204668309u, 331160281u, 535828591u, 866988873u, 1402817465u,
    2269806339u, 3672623805u
};

struct LeonardoHeapShape
{
	std::bitset<kNumberLeonardoNumbers> trees;
	int smallest_tree_size;
};

int leonardo_right_child(int index)
{
	return index - 1;
}
int leonardo_left_child(int index, int tree_size)
{
	return index - kLeonardoNumbers[tree_size - 2] - 1;
}
void maintain_single_heap(int data[], int l, int r, int now, const struct LeonardoHeapShape *shape)
{
	int now_index = now, now_size = shape->smallest_tree_size;
	while(now_size > 1)
	{
		int left = leonardo_left_child(now_index, now_size);
		int right = leonardo_right_child(now_index);
		int large_index = 0, large_size = 0;
		if (data[left] > data[right])
		{
			large_index = left;
			large_size = now_size - 1;
		}
		else
		{
			large_index = right;
			large_size = now_size - 2;
		}

		if (data[now_index] < data[large_index])
		{
			swap(&data[now_index], &data[large_index]);
			now_index = large_index;
			now_size = large_size;
		}
		else
			break;
	}
}
void maintain_trees(int data[], int l, int r, int now, const struct LeonardoHeapShape *now_shape)
{
	struct LeonardoHeapShape shape = *now_shape;
	while(1)
	{
		int pre = now - kLeonardoNumbers[shape.smallest_tree_size];
		int compare = now;
		if (pre >= l)
		{
			if (shape.smallest_tree_size > 1)
			{
				int left = leonardo_left_child(now, shape.smallest_tree_size);
				int right = leonardo_right_child(now);
				if (data[compare] < data[left])
					compare = left;
				if (data[compare] < data[right])
					compare = right;
			}
			if (data[pre] > data[compare])
			{
				swap(&data[pre], &data[now]);

				do
				{
					shape.trees >>= 1;
					shape.smallest_tree_size++;
				}while(shape.trees[0] == 0);
				now = pre;
			}
			else
				break;
		}
		else
			break;
	}

	maintain_single_heap(data, l, r, now, &shape);
}

char is_last_heap(int now, int r, const struct LeonardoHeapShape *shape)
{
	if (shape->smallest_tree_size == 0 && now == r)
		return 1;
	else if (shape->smallest_tree_size == 1 && r - now < 2)
		return 1;
	else if (r - now < kLeonardoNumbers[shape->smallest_tree_size - 1] + 1)
		return 1;
	else 
		return 0;
}

#include <assert.h>
void smooth_add(int data[], int l, int r, int now, struct LeonardoHeapShape *shape)
{
	if (shape->trees[0] == 0)
	{
		shape->trees[0] = 1;
		shape->smallest_tree_size = 1;
	}
	else if (shape->trees[0] == 1 && shape->trees[1] == 1)
	{
		shape->trees >>= 2;
		assert(shape->trees[0] == 0);
		shape->trees[0] = 1;
		shape->smallest_tree_size += 2;
	}
	else if (shape->smallest_tree_size == 1)
	{
		shape->trees <<= 1;
		shape->trees[0] = 1;
		shape->smallest_tree_size = 0;
	}
	else
	{
		shape->trees <<= (shape->smallest_tree_size-1);
		shape->trees[0] = 1;
		shape->smallest_tree_size = 1;
	}

	if (is_last_heap(now, r, shape))
	{
		maintain_trees(data, l, r, now, shape);
	}
	else
	{
		maintain_single_heap(data, l, r, now, shape);
	}
}

void smooth_remove(int data[], int l, int r, int now, struct LeonardoHeapShape *shape)
{
	assert(shape->trees[0] != 0);

	if (shape->smallest_tree_size <= 1)
	{
		do
		{
			shape->trees >>= 1;
			shape->smallest_tree_size += 1;
		}while(shape->trees[0] == 0 && shape->trees.any());
		return;
	}
	
	int left = leonardo_left_child(now, shape->smallest_tree_size);
	int right = leonardo_right_child(now);

	shape->trees[0] = 0;
	shape->trees <<= 2;
	shape->trees[0] = shape->trees[1] = 1;
	shape->smallest_tree_size -= 2;

	struct LeonardoHeapShape shape_except_last = *shape;
	shape_except_last.trees >>= 1;
	shape_except_last.smallest_tree_size += 1;
	
	maintain_trees(data, l, r, left, &shape_except_last);
	maintain_trees(data, l, r, right, shape);
}

void smooth_sort(int data[], int l, int r)
{
	struct LeonardoHeapShape shape;
	shape.smallest_tree_size = 0;

	for (int i=l; i<=r; i++)
		smooth_add(data, l, r, i, &shape);

	for (int i=r; i>l; i--)
		smooth_remove(data, l, r, i, &shape);
}

int Rank(int data[], int l, int r, int k)
{
    int i=l, j=r+1;
    while(true)
    {
        do i++; while(i<=r && data[i] <= data[l]);
        do j--; while(j>l && data[j] > data[l]);

        if (i>=j)
            break;

        swap(&data[l], &data[j]);
    }

    if (j == k)
        return data[j];
    else if (j<k)
        return Rank(data, j+1, r, k);
    else
        return Rank(data, l, j-1, k); 
}

#include "time_statistics.h"
#include <iostream>
#include <stdio.h>
#include "ReadData.h"

using namespace std;

template<typename DataType>
void DiffData(DataType a[], DataType b[], int size)
{
    int diff_count = 0;
    for (int i=0; i<size; i++)
    {
        if (a[i] != b[i])
        {
            if (diff_count <10)
            {
                cout<<"different : "<<"["<<i<<"]"<<a[i]<<"vs"<<b[i]<<endl;
                diff_count++;
            }
            else
            {
                cout<<"more"<<endl;
                break;
            }
        }
    }
}

#define L_SORT_TEST_SIZE 10000

#include "smooth_sort.h"
#include "natural_mergesort.h"
#include "inplace_merge.h"
#include <vector>
struct SmoothSortComp
{
	bool operator()(int a, int b)
	{
		return a < b;
	}
};
void SortTest()
{
    unsigned int time_used = 0;
    int *test_data = (int *)malloc(sizeof(int) * L_SORT_TEST_SIZE);
    int *out_data = (int *)malloc(sizeof(int) * L_SORT_TEST_SIZE);
    int *first_out_data = (int *)malloc(sizeof(int) * L_SORT_TEST_SIZE);
    memset(test_data, 0, sizeof(int) * L_SORT_TEST_SIZE);
    memset(out_data, 0, sizeof(int) * L_SORT_TEST_SIZE);
    memset(first_out_data, 0, sizeof(int) * L_SORT_TEST_SIZE);

    ReadData(test_data, sizeof(int), L_SORT_TEST_SIZE, RAWDATA_TYPE_BYTE, "raw_data.img");

	//for (int i=0; i<L_SORT_TEST_SIZE; i++)
	//	test_data[i] = i;
    /*int merge_sort_buf[L_SORT_TEST_SIZE] = {0};*/
    int *merge_sort_buf = (int *)malloc(sizeof(int) * L_SORT_TEST_SIZE);
    memset(merge_sort_buf, 0, sizeof(int) * L_SORT_TEST_SIZE);
    memcpy(out_data, test_data, sizeof(int)*L_SORT_TEST_SIZE);
    time_start();
    MergeSort(out_data, L_SORT_TEST_SIZE, merge_sort_buf);
    cout<<"MergeSort time: "<<get_time_used()<<endl;
    memcpy(first_out_data, out_data, sizeof(int)*L_SORT_TEST_SIZE);

    memcpy(out_data, test_data, sizeof(int)*L_SORT_TEST_SIZE);
    time_start();
    quik_sort(out_data, 0, L_SORT_TEST_SIZE-1);
    cout<<"quik_sort time: "<<get_time_used()<<endl;
    DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);

    memcpy(out_data, test_data, sizeof(int)*L_SORT_TEST_SIZE);
    time_start();
    quiker_sort(out_data, 0, L_SORT_TEST_SIZE-1);
    cout<<"quiker_sort time: "<<get_time_used()<<endl;
    DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);

    memcpy(out_data, test_data, sizeof(int)*L_SORT_TEST_SIZE);
    time_start();
    hoare_sort(out_data, 0, L_SORT_TEST_SIZE-1);
    cout<<"hoare_sort time: "<<get_time_used()<<endl;
    DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);

    memcpy(out_data, test_data, sizeof(int)*L_SORT_TEST_SIZE);
    time_start();
    three_seg_qsort_with_insert_sort(out_data, 0, L_SORT_TEST_SIZE-1);
    InsertSort(out_data, 0, L_SORT_TEST_SIZE-1);
    cout<<"three_seg_qsort_with_insert_sort time: "<<get_time_used()<<endl;
    DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);

    memcpy(out_data, test_data, sizeof(int)*L_SORT_TEST_SIZE);
    time_start();
    three_seg_qsort(out_data, 0, L_SORT_TEST_SIZE-1);
    cout<<"three_seg_qsort time: "<<get_time_used()<<endl;
    DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);

	memcpy(out_data, test_data, sizeof(int)*L_SORT_TEST_SIZE);
    time_start();
    quick_sort_array(out_data, 0, L_SORT_TEST_SIZE-1);
    cout<<"quick sort array time: "<<get_time_used()<<endl;
    DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);


    memcpy(out_data, test_data, sizeof(int)*L_SORT_TEST_SIZE);
    time_start();
    heap_sort(out_data, 0, L_SORT_TEST_SIZE-1);
    cout<<"heap_sort time: "<<get_time_used()<<endl;
    DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);

	memcpy(out_data, test_data, sizeof(int)*L_SORT_TEST_SIZE);
    time_start();
    smooth_sort(out_data, 0, L_SORT_TEST_SIZE-1);
    cout<<"smooth_sort time: "<<get_time_used()<<endl;
    DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);

    //memcpy(out_data, test_data, sizeof(int)*L_SORT_TEST_SIZE);
    //time_start();
    //ShellSort(out_data, 0, L_SORT_TEST_SIZE-1);
    //cout<<"ShellSort time: "<<get_time_used()<<endl;
    //DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);

    //memcpy(out_data, test_data, sizeof(int)*L_SORT_TEST_SIZE);
    //time_start();
    //InsertSort(out_data, 0, L_SORT_TEST_SIZE-1);
    //cout<<"InsertSort time: "<<get_time_used()<<endl;
    //DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);

	vector<int> data_vec(test_data, test_data + L_SORT_TEST_SIZE);

	vector<int> smooth_data_vec = data_vec;
	SmoothSortComp smooth_sort_comp;
    time_start();
	Smoothsort(smooth_data_vec.begin(), smooth_data_vec.end(), smooth_sort_comp);
    cout<<"Smoothsort time: "<<get_time_used()<<endl;
	for (int i=0; i<L_SORT_TEST_SIZE; i++)
		out_data[i] = smooth_data_vec[i];
    DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);

	vector<int> natural_merge_data_vec = data_vec;
    time_start();
	NaturalMergesort(natural_merge_data_vec.begin(), natural_merge_data_vec.end(), smooth_sort_comp);
    cout<<"natural merge sort time: "<<get_time_used()<<endl;
	for (int i=0; i<L_SORT_TEST_SIZE; i++)
		out_data[i] = natural_merge_data_vec[i];
    DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);

	vector<int> inplace_merge_data_vec = data_vec;
    time_start();
	InplaceMerge(inplace_merge_data_vec.begin(), inplace_merge_data_vec.end(), smooth_sort_comp);
    cout<<"inplace merge sort time: "<<get_time_used()<<endl;
	for (int i=0; i<L_SORT_TEST_SIZE; i++)
		out_data[i] = natural_merge_data_vec[i];
    DiffData(out_data, first_out_data, L_SORT_TEST_SIZE);

#define QUERY_RANK_INDEX 100
    memcpy(out_data, test_data, sizeof(int)*L_SORT_TEST_SIZE);
    time_start();
    for (int i=0; i<10; i++)
    int ret = Rank(out_data, 0, L_SORT_TEST_SIZE-1, QUERY_RANK_INDEX+i);
    cout<<"Rank time: "<<get_time_used()<<endl;
    //if (first_out_data[QUERY_RANK_INDEX] != ret)
    //{
    //    cout<<"Rank failed"<<first_out_data[QUERY_RANK_INDEX]<<"vs"<<ret<<endl;
    //}
	

	/*int debug_data[10] = {9, 4, 3, 2, 0, 1, 8, 6, 7, 5};
	smooth_sort(debug_data, 0, 9);
	for (int i=0; i<10; i++)
		printf("%d\n", debug_data[i]);*/
}
