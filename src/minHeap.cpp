#include "minHeap.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <Node.h>
#include "raylib.h"
using namespace std;

template <typename T>
minHeap<T>::minHeap(int arrsize) : arr(arrsize),arraysize(arrsize),heapsize(0)
{

}

template <typename T>
T minHeap<T>::dequeue()
{
    if(!empty())
    {
    T firstelement = arr[1];
    T lastelement = arr[heapsize--];
    int parent = 1;
    int child=1;
    while((parent*2)<=heapsize)
    {
        child = parent*2;
        if((child)+1<=heapsize&& arr[child]>arr[(child+1)] )
            child++;
        if(arr[child]>lastelement)
            break;
        arr[parent]=arr[child];
        parent=child;

    }
    arr[parent]=lastelement;

    return firstelement;
    }
    else
        throw underflow_error("Cannot dequqe from an empty queue");
}


template <typename T>
void minHeap<T>::enqueue(const T x)
{
    if (heapsize == arr.size()-1)
    {
        // resize() keeps existing data and adds new empty slots
        arr.resize(arr.size() * 2);
    }
    T hole = arr[++heapsize];
    int holeindex = heapsize;

    while(x<arr[holeindex/2] && holeindex>1)
    {

        arr[holeindex] = arr[holeindex/2];
        hole = arr[holeindex/2];
        holeindex=holeindex/2;
    }
    arr[holeindex] = x;
}

template <typename T>
bool minHeap<T>::empty() const
{
    if(heapsize==0)
        return true;
    return false;
}



template <typename T>
void minHeap<T>::peek()
{

}

template <typename T>
void minHeap<T>::printheap()
{

}

template <typename T>
minHeap<T>::~minHeap()
{
    //dtor
}


template class minHeap<int>;
template class minHeap<double>;
template class minHeap<Node>;

