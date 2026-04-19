#ifndef minHeap_H
#define minHeap_H
#include <vector>

using namespace std;
template <typename T>
class minHeap
{
public:
    minHeap(int arraysize=8); // constructor
    minHeap(const minHeap& t);
    ~minHeap(); // destructor
    bool empty() const;
    void peek(); // access functions
    minHeap& left();
    minHeap& right();
    minHeap& parent(T x);
// … update …
    void printheap();
    void enqueue(const T x); // compose x into a minHeap
    T dequeue(); // decompose x from a minHeap
    vector<T> arr;
    int heapsize;
private:
    int arraysize;
};

#endif // minHeap_H
