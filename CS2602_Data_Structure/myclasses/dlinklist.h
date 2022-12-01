#ifndef DLINKLIST_INCLUDED
#define DLINKLIST_INCLUDED

# include <iostream>

class outOfBound{};

template<class elemType>
class dlinkList;

template<class elemType>
class dnode {
    friend class dlinkList<elemType>;
    private:
        elemType data;
        dnode* prior;
        dnode* next;
    public:
        dnode(): prior(NULL), next(NULL) {};
        dnode(const elemType& e, dnode* p = NULL, dnode* n = NULL): data(e), prior(p), next(n) {};
};

template<class elemType>
class dlinkList {
    private:
        dnode<elemType>* head;
        dnode<elemType>* tail;
    public:
        dlinkList();
        bool isEmpty() const {return (head->next == tail && tail->prior == head);}
        bool isFull() const {return false;}

        int length() const;
        elemType get(int i) const;
        int find(const elemType& e) const;
        void insert(int i, const elemType& e);
        void remove(int i, elemType& e);
        elemType kth_last(int k) const;
        void clear();

        ~dlinkList (clear();)
};

template <class elemType>
dlinkList<elemType>::dlinkList()
{
    head = new dnode<elemType> ();
    tail = new dnode<elemType> ();
    head -> next = tail;
    tail -> prior = head;
}

template <class elemType>
int dlinkList<elemType>::length() const
{
    dnode<elemType>* p = head->next;
    int i = 0;
    while (p) {
        p = p->next;
        ++i;
    }
    return i-1;
}

template <class elemType>
elemType dlinkList<elemType>::get(int i) const
{
    if (i<1) throw outOfBound();
    dnode<elemType>* p = head->next;
    while (p && i>1) {
        p = p->next;
        --i;
    }
    if (!p || p->next == NULL) throw outOfBound();
    return p->data;
}

template <class elemType>
int dlinkList<elemType>::find(const elemType& e) const
{
    int i=1;
    dnode<elemType>* p = head->next;
    while (p) {
        if (p->next != NULL && p->data == e) return i;
        p = p->next;
        ++i;
    }
    return 0;
}

template <class elemType>
void dlinkList<elemType>::insert(int i, const elemType& e)
{
    if (i<1) throw outOfBound();
    dnode<elemType>* p = head; 
    while (p && i>1) {
        p = p->next;
        --i;
    }
    if (!p || p->next == NULL) throw outOfBound();
    dnode<elemType>* tmp = new dnode<elemType>(e,p,p->next);
    p->next->prior = tmp;
    p->next = tmp;
}

template <class elemType>
void dlinkList<elemType>::remove(int i, elemType& e)
{
    if (i<1) throw outOfBound();
    dnode<elemType>* p = head;
    while (p && i>1) {
        p = p->next;
        --i;
    }
    if (!p || p->next->next == NULL) throw outOfBound();
    dnode<elemType>* q = p->next;
    e = q->data;
    p->next = q->next;
    q->next->prior = p;
    delete q;
}

template <class elemType>
void dlinkList<elemType>::clear()
{
    dnode<elemType>* p = head->next;
    dnode<elemType>* q;
    head->next = tail;
    while (p && p->next) {
        q = p->next;
        delete p;
        p=q;
    }
    tail->prior = head;
}

#endif