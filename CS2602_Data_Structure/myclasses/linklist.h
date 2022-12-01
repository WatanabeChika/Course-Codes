#ifndef LINKLIST_INCLUDED
#define LINKLIST_INCLUDED

# include <iostream>

class outOfBound{};

template <class elemType>
class linkList;

template <class elemType>
class node {
    friend class linkList<elemType>;
    private:
        node* next;
        elemType data;
    public:
        node(): next(NULL){};
        node(const elemType& e, node* n=NULL): data(e), next(n){};
};

template <class elemType>
class linkList {
private:
    node<elemType>* head;
public:
    linkList() {head = new node<elemType>();}
    bool isEmpty() const {return (head->next == NULL);}
    bool isFull() const {return false;}

    int length() const;
    elemType get(int i) const;
    int find(const elemType& e) const;
    void insert(int i, const elemType& e);
    void remove(int i, elemType& e);
    void reverse() const;
    elemType kth_last(int k) const;
    void clear();

    ~linkList (clear();)
};


template <class elemType>
int linkList<elemType>::length() const
{
    int len = 0;
    node<elemType>* p = head->next;
    while(p) {
        p = p->next;
        ++len;
    }
    return len;
}

template <class elemType>
elemType linkList<elemType>::get(int i) const
{
    if (i<1) throw outOfBound();
    node<elemType>* p = head->next;
    while (p && i>1) {
        p = p->next;
        --i;
    }
    if (!p) throw outOfBound();
    return p->data;
}

template <class elemType>
int linkList<elemType>::find(const elemType& e) const
{
    int i=1;
    node<elemType>* p = head->next;
    while (p) {
        if (p->data == e) return i;
        p = p->next;
        ++i;
    }
    return 0;
}

template <class elemType>
void linkList<elemType>::insert(int i, const elemType& e)
{
    if (i<1) throw outOfBound();
    node<elemType>* p = head; 
    while (p && i>1) {
        p = p->next;
        --i;
    }
    if (!p) throw outOfBound();
    p->next = new node<elemType>(e,p->next);
}

template <class elemType>
void linkList<elemType>::remove(int i, elemType& e)
{
    if (i<1) throw outOfBound();
    node<elemType>* p = head;
    while (p && i>1) {
        p = p->next;
        --i;
    }
    if (!p || p->next == NULL) throw outOfBound();
    node<elemType>* q = p->next;
    e = q->data;
    p->next = q->next;
    delete q;
}

template <class elemType>
void linkList<elemType>::reverse() const
{
    node<elemType>* p = head->next;
    node<elemType>* q;
    head->next = NULL;
    while (p) {
        q = p->next;
        p->next = head->next;
        head->next = p;
        p=q;
    }
}

template <class elemType>
elemType linkList<elemType>::kth_last(int k) const
{
    if (k<=0) throw outOfBound();
    node<elemType>* p = head->next;
    node<elemType>* q;
    while (p && k>0) {
        p = p->next;
        --k;
    }
    if (!p) throw outOfBound();
    q = head->next;
    while (p) {
        p = p->next;
        q = q->next;
    }
    return q->data;
}

template <class elemType>
void linkList<elemType>::clear()
{
    node<elemType>* p = head->next;
    node<elemType>* q;
    head->next = NULL;
    while (p) {
        q = p->next;
        delete p;
        p=q;
    }
}

#endif