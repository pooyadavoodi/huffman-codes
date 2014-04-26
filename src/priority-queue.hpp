#ifndef _PRIORITYQUEUE_HPP_
#define _PRIORITYQUEUE_HPP_

#include <queue>
#include <vector>
#include "binary-tree.hpp"
#include <iostream>

template <class T> class pqueue;

template <class T>
class pq_node
{
    private:
        int freq;
        binary_tree_node<T> * node;

    public:
        pq_node(int, binary_tree_node<T> *);
        int getFreq() const;
        binary_tree_node<T> * getNode() const;

    friend class pqueue<T>;
};

template <class T>
class maximum
{
    public:
        bool operator()(const pq_node<T> & a, const pq_node<T> & b);
};

template <class T>
class minimum
{
    public:
        bool operator()(const pq_node<T> & a, const pq_node<T> & b);
};

template <class T>
class pqueue
{
    private:
		std::priority_queue<pq_node<T>,std::vector<pq_node<T> >,minimum<T> > pq;

    public:
        pqueue();
        void push(int, binary_tree_node<T> *);
        pq_node<T> toppop();
        bool isEmpty() const;
        size_t size() const;
};

#endif
