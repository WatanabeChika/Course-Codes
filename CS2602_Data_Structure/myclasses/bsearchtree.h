# ifndef B_SEARCHTREE_INCLUDED
# define B_SEARCHTREE_INCLUDED

#include <iostream>

using namespace std;

template <class elemtype>
class myqueue {
    private:
        elemtype *array;
        elemtype front, rear;
        int maxsize, len;
    public:
        myqueue(int s = 100) {
            array = new char[s];
            front = rear = 0;
            maxsize = s;
            len = 0;
        }
        bool isEmpty() const {
            return (len == 0);
        }
        bool isFull() const {
            return (len == maxsize-1);
        }
        elemtype seeTop() const {
            if (isEmpty()) return;
            return front;
        }
        void enqueue(const elemtype& x) {
            if (isFull()) return;
            array[rear] = x;
            if (rear == maxsize-1) rear = 0;
            else ++rear;
            ++len;
        }
        void dequeue() {
            if (isEmpty()) return;
            if (front == maxsize-1) front = 0;
            else ++front;
            --len;
        }
        ~myqueue() {
            delete []array;
        }
};

template <class elemType>
class mystack;

template <class elemType>
struct node {
    friend class mystack;
    int data;
    node *next;
    node(const int d, node *n = nullptr) : data(d), next(n) {};
};

template <class elemType>
class mystack {
    private:
        node<elemType> *top;
    public:
        mystack() {
            top = nullptr;
        }
        bool isEmpty() const {
            return (!top);
        }
        int topsee() const {
            if (isEmpty()) return -1;
            return top->data;
        }
        void pop() {
            if (isEmpty()) return;
            node<elemType> *j = top;
            top = j->next;
            delete j;
        }
        void push(const int x) {
            node<elemType> *j = new node(x, top);
            top = j;
        }
        ~mystack() {
            while (!isEmpty()) {
                pop();
            }
        }
};


// Binary Search Tree
template<class elemtype>
class BStree;

template<class elemtype>
class BSnode {
    friend class BStree;
    private:
        elemtype data;
        BSnode *left, *right;
        int factor;
    public:
        BSnode(): left(NULL), right(NULL), factor(0) {};
        BSnode(const elemtype& d, BSnode *l = NULL, BSnode *r = NULL, int f = 0)
            : data(d), left(l), right(r), factor(f) {};
};

template<class elemtype>
class BStree {
    private:
        BSnode<elemtype> *root;
        
        /* bool search(const elemtype &data, BSnode<elemtype> *t);
        void insert(const elemtype &data, BSnode<elemtype> *&t);
        void remove(const elemtype &data, BSnode<elemtype> *&t);*/

    public:
        BStree(): root(NULL) {};
        bool search(const elemtype &d) const;
        void insert(const elemtype &d);
        void remove(const elemtype &d);
        void levelOrder() const; 
};

template<class elemtype>
bool BStree<elemtype>::search(const elemtype &d) const
{
    if (!root) 
        return false;
    BSnode<elemtype> *t = root;
    while (t) {
        if (t->data == d) 
            return true;
        else if (t->data < d) 
            t = t->right;
        else 
            t = t->left;
    }
    return false;
}

template<class elemtype>
void BStree<elemtype>::insert(const elemtype &d)
{
    if (!root) {
        root = new BSnode(d);
        return;
    }
    BSnode<elemtype> *p = root;
    while (p) {
        if (p->data == d) 
            return;
        else if (p->data < d) {
            if (p->right) 
                p = p->right;
            else {
                p->right = new BSnode(d);
                return;
            }
        }
        else {
            if (p->left) 
                p = p->left;
            else {
                p->left = new BSnode(d);
                return;
            }
        }
    }
    return;
}

template<class elemtype>
void BStree<elemtype>::remove(const elemtype &d) 
{
    if (!root) return;
    BSnode<elemtype> *p = root;
    BSnode<elemtype> *q = NULL;
    int flag; // 0->left  1->right

    while (p) {
        if (p->data < d) {
            q = p; flag = 1; 
            p = p->right;
        }
        else if (p->data > d) {
            q = p; flag = 0; 
            p = p->left;
        }
        else {
            if (!p->left && !p->right) {
                delete p; 
                if (flag == 0) 
                    q->left = NULL;
                else 
                    q->right = NULL;
                return;
            } // no kid
            else if (!p->left || !p->right) {
                if (!q) 
                    root = p->left ? p->left : p->right;
                else {
                    if (flag == 0) 
                        q->left = p->left ? p->left : p->right;
                    else 
                        q->right = p->left ? p->left : p->right;
                }
            } // one kid
            else {  
                BSnode<elemtype> *sub = p->right;
                flag = 1; q = p;
                while (sub->left) {
                    q = sub;
                    sub = sub->left;
                    flag = 0;
                }
                elemtype tmp = p->data;
                p->data = sub->data;
                sub->data = tmp;
                p = sub;
            } // two kids
        } //if
    } //while
}



template<class elemtype>
void BStree<elemtype>::levelOrder() const
{
    if (!root) return;
    myqueue<treenode<elemtype> *> que;
    treenode<elemtype> *p;
    que.enqueue(root);

    while(!que.isEmpty()) {
        p = que.seeTop();
        cout << p->data;
        que.dequeue();
        if (p->left) que.enqueue(p->left);
        if (p->right) que.enqueue(p->right);
    }
    cout << endl;
}

# endif