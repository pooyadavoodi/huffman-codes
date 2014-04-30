#include "priority-queue.hpp"

namespace huffman_comp
{

template <class T>
pq_node<T>::pq_node(int x, binary_tree_node<T> * p)
{
    this->freq = x;
    this->node = p;
}

template <class T>
int pq_node<T>::getFreq() const
{
    return this->freq;
}

template <class T>
binary_tree_node<T> * pq_node<T>::getNode() const
{
    return this->node;
}

template <class T>
bool maximum<T>::operator()(const pq_node<T> & a, const pq_node<T> & b)
{
    if(a.getFreq() <= b.getFreq())
        return true;
    return false;
}

template <class T>
bool minimum<T>::operator()(const pq_node<T> & a, const pq_node<T> & b)
{
    if(a.getFreq() >= b.getFreq())
        return true;
    return false;
}

template <class T>
pqueue<T>::pqueue() {}

template <class T>
void pqueue<T>::push(int f, binary_tree_node<T> * x)
{
    pq_node<T> p(f,x);
    this->pq.push(p);
}

template <class T>
pq_node<T> pqueue<T>::toppop()
{
    pq_node<T> p = this->pq.top();
    this->pq.pop();
    return p;
}


template <class T>
bool pqueue<T>::isEmpty() const
{
    return this->pq.empty();
}

template <class T>
size_t pqueue<T>::size() const
{
    return this->pq.size();
}

template class pqueue<char>;
template class pq_node<char>;
template class minimum<char>;
template class maximum<char>;

} // namespace huffman_comp


