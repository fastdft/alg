#ifndef PRIQUEUE_H
#define PRIQUEUE_H
#include <assert.h>
template<class T>
class PriQueue
{
public:
	PriQueue(int MaxSize);
	PriQueue(T data[], int size);
	~PriQueue(){}
	int insert(T t);
	T extract_min();

	void shift_down();
	void shift_up();
	void swap(int i, int j);

private:
	T *m_data;
	int m_max_size;
	int m_now_size;
};

#include "priqueue.h"

template<class T>
PriQueue<T>::PriQueue(int MaxSize)
: m_max_size(MaxSize)
, m_now_size(0)
{
	m_data = new T[MaxSize + 1];
}

template<class T>
PriQueue<T>::PriQueue(T data[], int size)
: m_max_size(size)
, m_now_size(size)
{
	m_data = data;
}

template<class T>
int PriQueue<T>::insert(T t)
{
	if (m_now_size > m_max_size)
		return -1;
	m_data[++m_now_size] = t;
	shift_up();
	return 0;
}

template<class T>
T PriQueue<T>::extract_min()
{
	if (m_now_size <= 0)
		return T(-1);
	T min_value = m_data[1];
	m_data[1] = m_data[m_now_size--];
	shift_down();

	return min_value;
}

template<class T>
void PriQueue<T>::shift_down()
{
	int shift_down_index = 1;
	while(true)
	{
		int l_child_index = 2 * shift_down_index;
		int child_index = l_child_index;

		if (m_now_size < l_child_index)
			break;
		if (l_child_index + 1 <= m_now_size)
			if (m_data[l_child_index + 1] < m_data[l_child_index])
				child_index = l_child_index + 1;

		if (m_data[shift_down_index] < m_data[child_index])
			break;

		swap(shift_down_index, child_index);
		shift_down_index = child_index;
	}
}

template<class T>
void PriQueue<T>::shift_up()
{
	int shift_up_index = m_now_size;
	while (m_data[shift_up_index] < m_data[shift_up_index/2])
	{
		swap(shift_up_index, shift_up_index/2);
		shift_up_index /= 2;
		if (shift_up_index == 0)
			break;
	}
}

template<class T>
void PriQueue<T>::swap(int i, int j)
{
	assert(i>=1 && i<= m_max_size);
	assert(j>=1 && j<= m_max_size);

	T temp = m_data[i];
	m_data[i] = m_data[j];
	m_data[j] = temp;
}

#endif