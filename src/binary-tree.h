#ifndef _BINARYTREE_H_
#define _BINARYTREE_H_

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
		binary_tree_node<T> * getRight() const;
		T getData() const;
		void setData(const T &);

    friend class binary_tree<T>;
};

template <class T>
class binary_tree
{
	private:
		binary_tree_node<T> * root;

    public:
        binary_tree();
        ~binary_tree();
        binary_tree_node<T> * getRoot() const;
        void setRoot(binary_tree_node<T> *);
        bool isEmpty() const;

//    friend class encoder;
//    friend class decoder;
};

#endif
