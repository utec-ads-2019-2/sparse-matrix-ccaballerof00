#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
public:
    Node<T> *next, *down;
    Node():next{nullptr},down{nullptr}{}
    virtual ~Node() = default;
    friend class Matrix<T>;
};

template <typename T1>
class HeaderNode: public Node<T1>
{
protected:

public:

};

template <typename T1>
class ElementNode: public Node<T1>
{
public:
    T1 data;
    int posx, posy;
    ElementNode(T1 data,int posx, int posy)
    {
        this->data = data;
        this->posx = posx;
        this->posy = posy;
    }

};


#endif //SPARSE_MATRIX_NODE_H
