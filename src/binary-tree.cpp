#include "binary-tree.hpp"

namespace huffman_comp
{

template <class T>
binary_tree_node<T>::binary_tree_node()
{
    left = right = nullptr;
}

template <class T>
binary_tree_node<T>::binary_tree_node(T x)
{
    this->left = this->right = nullptr;
    this->data = x;
}

template <class T>
binary_tree_node<T>::binary_tree_node(binary_tree_node<T> * leftChild, binary_tree_node<T> * rightChild, T x)
{
    this->left = leftChild;
    this->right = rightChild;
    this->data = x;
}

template <class T>
binary_tree_node<T> * binary_tree_node<T>::getLeft() const
{
    return this->left;
}

template <class T>
void binary_tree_node<T>::setLeft(binary_tree_node<T> * leftlink)
{
    this->left = leftlink;
}

template <class T>
binary_tree_node<T> * binary_tree_node<T>::getRight() const
{
    return this->right;
}

template <class T>
void binary_tree_node<T>::setRight(binary_tree_node<T> * rightlink)
{
    this->right = rightlink;
}

template <class T>
T binary_tree_node<T>::getData() const
{
    return this->data;
}

template <class T>
void binary_tree_node<T>::setData(const T & x)
{
    this->data = x;
}

template <class T>
binary_tree<T>::binary_tree() {}

template <class T>
void binary_tree<T>::rec_destructor(binary_tree_node<T> * p)
{
    if(p)
    {
        rec_destructor(p->getLeft());
        rec_destructor(p->getRight());
        delete p;
    }
}

//delete all the nodes
template <class T>
binary_tree<T>::~binary_tree()
{
    rec_destructor(this->getRoot());

}

template <class T>
binary_tree_node<T> * binary_tree<T>::getRoot() const
{
    return this->root;
}

template <class T>
void binary_tree<T>::setRoot(binary_tree_node<T> * r)
{
    this->root = r;
}

template <class T>
bool binary_tree<T>::isEmpty() const
{
    return (root == nullptr);
}


template class binary_tree_node<char>;
template class binary_tree<char>;

} // namespace huffman_comp
