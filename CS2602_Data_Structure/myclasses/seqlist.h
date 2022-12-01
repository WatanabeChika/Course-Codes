#ifndef SEQLIST_INCLUDED
#define SEQLIST_INCLUDED

#include <iostream>

class outOfBound{};
class illegalSize{};

template <class elemType>
class seqList {
public:
    seqList(int size=100);
    bool isEmpty() const {return len == 0;}
    bool isFull() const {return len == maxSize;}
    int length() const {return len;}
    elemType get(int i) const;
    void insert(int i, const elemType& e);
    void remove(int i, elemType& e);
    int find(const elemType& e) const;
    void clear() {len = 0;}
    ~seqList() {delete []elem};
    
private:
    int len;
    int maxSize;
    elemType* elem;
    void doublespace();
};

template <class elemType>
seqList<elemType>::seqList(int size)
{
    elem = new elemType[size];
    if(!elem) throw illegalSize();
    len = 0;
    maxSize = size-1;
}

template <class elemType>
void seqList<elemType>::doublespace()
{
    elemType* tmp = new elemType[2*maxSize];
    if(!tmp) throw illegalSize();
    for (int i=1; i<=len; ++i)
    {
        tmp[i] = elem[i];
    }
    delete []elem;
    elem = tmp;
    maxSize = 2*maxSize-1;
}

template <class elemType>
int seqList<elemType>::find(const elemType& e) const
{
    elem[0] = e;
    for (int i=len; i>=0; --i)
    {
        if (elem[i] == e)
            return i;
    }
}

template <class elemType>
void seqList<elemType>::insert(int i, const elemType& e)
{
    if (i<1||i>len+1) return;
    if (len==maxSize) doublespace();

    for (int j=len; j>=i; --j)
    {
        elem[j+1]=elem[j];
    }
    elem[i]=e;
    ++len;
}

template <class elemType>
void seqList<elemType>::remove(int i, elemType& e)
{
    if (i<1||i>len) return;

    e=elem[i];
    for (int k=i; k<len; ++k)
    {
        elem[k]=elem[k+1];
    }
    --len;
}

template <class elemType>
elemType seqList<elemType>::get(int i) const
{   
    if (i<1||i>len) throw outOfBound();
    return elem[i];
}

#endif