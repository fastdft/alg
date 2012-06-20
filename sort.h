#ifndef SORT_H
#define SORT_H


#define L_SORT_TEST_SIZE  100000

void count_sort(int data[], int data_size, int max_data, int out[]);
void quik_sort(int data[], int l, int u);
void quiker_sort(int data[], int l, int u);

void InsertSort(int data[], int l, int r);

void ShellSort(int data[], int l, int r);

void MergeSort(int data[], int len, int buf[]);

void SortTest();

void heap_sort(int data[], int l, int r);
#endif
