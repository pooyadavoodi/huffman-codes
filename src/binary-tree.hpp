#ifndef _BINARYTREE_HPP_
#define _BINARYTREE_HPP_

namespace huffman_comp
{

template <class T> class binary_tree;

template <class T>
class binary_tree_node
{
	private:
		binary_tree_node<T> * left;
		binary_tree_node<T> * right;
		T data;

    public:
		binary_tree_node();
		binary_tree_node(T);
		binary_tree_node(binary_tree_node<T> *, binary_tree_node<T> *, T x = 0);
		binary_tree_node<T> * getLeft() const;
		void setLeft(binary_tree_node<T> *);
		binary_tree_node<T> * getRight() const;
		void setRight(binary_tree_node<T> *);
		T getData() const;
		void setData(const T &);

    friend class binary_tree<T>;
};

template <class T>
class binary_tree
{
	private:
		binary_tree_node<T> * root;
        void rec_destructor(binary_tree_node<T> *);
    public:
        binary_tree();
        ~binary_tree();
        binary_tree_node<T> * getRoot() const;
        void setRoot(binary_tree_node<T> *);
        bool isEmpty() const;

//    friend class encoder;
//    friend class decoder;
};

} // namespace huffman_comp

#endif
