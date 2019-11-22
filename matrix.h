#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include "node.h"
using namespace std;

template <typename T>
class Matrix {
public:
    Node<T>* x;
    Node<T>* y;
    int columns, rows;
    Matrix(int rows, int columns)
    {
        this->rows = rows;
        this->columns = columns;
        auto actual = new HeaderNode<T>;
        this->x = actual;
        for(int i = 0; i < this->rows; i++)
        {
            auto nuevo = new HeaderNode<T>;
            actual->down = nuevo;
            actual = nuevo;
        }

        auto actual2 = new HeaderNode<T>;
        this->y = actual2;
        for(int i = 0; i < this->columns;i++)
        {
            auto nuevo2 = new HeaderNode<T>;
            actual2->next = nuevo2;
            actual2 = nuevo2;
        }
    }

    void set(int a, int b, T d);
    T operator()(int a, int b);
    Matrix<T> operator*(T scalar);
    Matrix<T> operator*(Matrix<T> other);
    Matrix<T> operator+(Matrix<T> other);
    Matrix<T> operator-(Matrix<T> other) ;
    Matrix<T> transpose() ;
    ElementNode<T>* Search_Node(int a,int b);
    vector<Node<T>*>  Search_Roots(int a, int b);
    void print() ;

    ~Matrix()
    {
//        for(int i = 0; i < this->rows; i++)
//        {
//            for(int j = 0; j < this->columns; j++)
//            {
//                set(i,j,0);
//            }
//        }
    }
};
template<typename T1>
void Matrix<T1>::set(int a,int b, T1 d)
{
    auto temp = Search_Node(a,b);
    if(temp==nullptr) {
        if (d != 0) {
            auto aux = new ElementNode<T1>(d, a, b);
            auto _x = this->x;
            for (int i = 0; i < a; i++) {
                _x = _x->down;
            }
            while (_x->next != nullptr) {
                if (dynamic_cast<ElementNode<T1> *>(_x->next)->posy > b)
                    break;
                _x = _x->next;
            }
            aux->next = _x->next;
            _x->next = aux;

            auto _y = this->y;
            for (int i = 0; i < b; i++) {
                _y = _y->next;
            }
            while(_y->down != nullptr)
            {
                if(dynamic_cast<ElementNode<T1>*>(_y->down)->posx > a)
                    break;
                _y = _y->down;
            }
            aux->down = _y->down;
            _y->down = aux;
        }
    }
    else
    {
        if(d!=0)
        {
            temp->data = d;
        }
        else
        {
            vector<Node<T1>*> aux = Search_Roots(temp->posx,temp->posy);
            aux[0]->next = temp->next;
            aux[1]->down = temp->down;
        }
    }
}



template <typename T1>
ElementNode<T1>* Matrix<T1>::Search_Node(int a,int b)
{
    if(a<=b) {
        auto _x = this->x;
        for (int i = 0; i < a; i++) {
            _x = _x->down;
        }
        while (_x->next != nullptr) {
            _x = _x->next;
            if (dynamic_cast<ElementNode<T1> *>(_x)->posy == b) {
                return dynamic_cast<ElementNode<T1> *>(_x);
            }
        }
        return nullptr;
    } else
        {
        auto _y = this->y;
        for(int i = 0; i < b; i++)
        {
            _y = _y->next;
        }
            while (_y->down != nullptr) {
                _y = _y->down;
                dynamic_cast<ElementNode<T1> *>(_y); // Esto para que lo usas?
                if (dynamic_cast<ElementNode<T1> *>(_y)->posx == a) {
                    return dynamic_cast<ElementNode<T1> *>(_y);
                }
            }
    }
    return nullptr;
}

template <typename T1>
vector<Node<T1>*> Matrix<T1>::Search_Roots(int a, int b)
{
    vector<Node<T1>*> roots;
    auto _y = this->y;
    auto _x = this->x;
    for(int i = 0; i < a; i++)
    {
        _x = _x->down;
    }
    while(_x->next!=nullptr)
    {
        if(dynamic_cast<ElementNode<T1>*>(_x->next)->posy==b)
        {
            roots.push_back(_x);
        }
        _x = _x->next;
    }
    for(int j = 0; j < b; j++)
    {
        _y = _y->next;
    }
    while(_y->down!=nullptr)
    {
        if(dynamic_cast<ElementNode<T1>*>(_y->down)->posx==a)
        {
            roots.push_back(_y);
        }
        _y = _y->down;
    }
    return roots;
}

template<typename T1>
T1 Matrix<T1>::operator()(int a,int b)
{
    if(a>=rows || b>=columns) {
        throw exception();
    }
    auto temp = this->Search_Node(a,b);
    if(temp!=nullptr)
    {
        return temp->data;
    }
    return 0;
}

template<typename T1>
Matrix<T1> Matrix<T1>::operator*(T1 scalar)
{
    auto rpta = new Matrix<T1>(this->rows,this->columns);
    auto _x1 = this->x;
    while(_x1!=nullptr)
    {
        auto aux1 = _x1->next;
        while(aux1!=nullptr)
        {
            rpta->set(dynamic_cast<ElementNode<T1>*>(aux1)->posx,dynamic_cast<ElementNode<T1>*>(aux1)->posy,dynamic_cast<ElementNode<T1>*>(aux1)->data*scalar);
            aux1 = aux1->next;
        }
        _x1 = _x1->down;
    }
    return *rpta;
}

template<typename T1>
Matrix<T1> Matrix<T1> ::operator*(Matrix<T1> other)
{
    Matrix<T1> producto(this->rows,other.columns);
    auto mat = *this;
    if(this->columns == other.rows)
    {
        for(int i = 0; i < this->rows;i++)
        {
            for(int j = 0; j < other.columns; j++)
            {
                T1 num = 0;
                for(int k = 0; k < this->rows; k++)
                {
                    num = num + (mat(i,k)*other(k,j));
                }
                producto.set(i,j,num);
            }
        }
        return producto;
    }
    else
    {
        throw exception();
    }
}

template<typename T1>
Matrix<T1> Matrix<T1> ::operator+(Matrix<T1> other)
{
    auto rpta = new Matrix<T1>(this->rows,this->columns);
    auto _x1 = this->x;
    while(_x1!=nullptr)
    {
        auto aux1 = _x1->next;
        while(aux1!=nullptr)
        {
            rpta->set(dynamic_cast<ElementNode<T1>*>(aux1)->posx,dynamic_cast<ElementNode<T1>*>(aux1)->posy,dynamic_cast<ElementNode<T1>*>(aux1)->data);
            aux1 = aux1->next;
        }
        _x1 = _x1->down;
    }
    if(this->columns==other.columns && this->rows==other.rows) {
        auto _x = other.x;
        while (_x != nullptr) {
            auto aux = _x->next;
            while (aux != nullptr) {
                // Esto va a ser muy pesado en las operaciones
                auto nd = rpta->Search_Node(dynamic_cast<ElementNode<T1> *>(aux)->posx,
                                            dynamic_cast<ElementNode<T1> *>(aux)->posy);
                if (nd == nullptr) {
                    rpta->set(dynamic_cast<ElementNode<T1> *>(aux)->posx, dynamic_cast<ElementNode<T1> *>(aux)->posy,
                              dynamic_cast<ElementNode<T1> *>(aux)->data);
                } else {
                    nd->data = nd->data + dynamic_cast<ElementNode<T1> *>(aux)->data;
                }
                aux = aux->next;
            }
            _x = _x->down;
        }
    }
    else
    {
        throw exception();
    }
    return *rpta;
}

template<typename T1>
Matrix<T1> Matrix<T1> ::operator-(Matrix<T1> other)
{
    auto rpta = new Matrix<T1>(this->rows,this->columns);
    auto _x1 = this->x;
    while(_x1!=nullptr)
    {
        auto aux1 = _x1->next;
        while(aux1!=nullptr)
        {
            rpta->set(dynamic_cast<ElementNode<T1>*>(aux1)->posx,dynamic_cast<ElementNode<T1>*>(aux1)->posy,dynamic_cast<ElementNode<T1>*>(aux1)->data);
            aux1 = aux1->next;
        }
        _x1 = _x1->down;
    }
    if(this->columns==other.columns && this->rows==other.rows) {
        auto _x = other.x;
        while (_x != nullptr) {
            auto aux = _x->next;
            while (aux != nullptr) {
                // Esto va a ser muy pesado en las operaciones
                auto nd = rpta->Search_Node(dynamic_cast<ElementNode<T1> *>(aux)->posx,
                                            dynamic_cast<ElementNode<T1> *>(aux)->posy);
                if (nd == nullptr) {
                    rpta->set(dynamic_cast<ElementNode<T1> *>(aux)->posx, dynamic_cast<ElementNode<T1> *>(aux)->posy,
                              -dynamic_cast<ElementNode<T1> *>(aux)->data);
                } else {
                    nd->data = nd->data - dynamic_cast<ElementNode<T1> *>(aux)->data;
                }
                aux = aux->next;
            }
            _x = _x->down;
        }
    }
    else
    {
        throw exception();
    }
    return *rpta;
}

template<typename T1>
Matrix<T1> Matrix<T1> ::transpose()
{
    auto rpta = new Matrix<T1>(this->rows,this->columns);
    auto _x1 = this->x;
    while(_x1!=nullptr)
    {
        auto aux1 = _x1->next;
        while(aux1!=nullptr)
        {
            rpta->set(dynamic_cast<ElementNode<T1>*>(aux1)->posy,dynamic_cast<ElementNode<T1>*>(aux1)->posx,dynamic_cast<ElementNode<T1>*>(aux1)->data);
            aux1 = aux1->next;
        }
        _x1 = _x1->down;
    }
    return *rpta;
}

template<typename T1>
void Matrix<T1>::print()
{
    auto mat = *this;
    for(int i = 0; i < this->rows;i++)
    {
        for(int j = 0; j < this->columns;j++)
        {
            cout<<mat(i,j)<<" ";
        }
        cout<<endl;
    }
}



#endif //SPARSE_MATRIX_MATRIX_H