#ifndef BTREE_INCLUDED
#define BTREE_INCLUDED

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


// Binary Tree
template <class elemtype>
class Btree;

template <class elemtype>
struct treenode {
    friend class Btree<elemtype>;
    elemtype data;
    treenode *left, *right;

    treenode(const elemtype& d, treenode *l = nullptr, treenode *r = nullptr): data(d), left(l), right(r) {};
};

template <class elemtype>
class Btree {
    private:
        treenode<elemtype> *root;

        void delTree(treenode<elemtype> *t);
        int size(treenode<elemtype> *t);
        int height(treenode<elemtype> *t);

        /*void postOrder(treenode<elemtype> *t);
        void inOrder(treenode<elemtype> *t);
        void preOrder(treenode<elemtype> *t);*/

        int priOver(const char& ch1, const char& ch2);


    public:
        Btree() {root = nullptr};
        void createTree(const elemtype& stopflag);
        void delTree() {delTree(root); root = nullptr};

        int size() {return size(root)};
        int height() {return height(root)};
        bool isEmpty() const {return root == nullptr};

        void levelOrder() const;
        void postOrder() const;
        void inOrder() const;
        void preOrder() const;

        // preorder & minorder -> binary tree
        treenode<elemtype> *pre_mid_tree(const elemtype *&pre, const elemtype *&mid, int pl, int ml, int pr, int mr);
        // expression tree
        void expr_tree(const char* expr);
};

template<class elemtype>
void Btree<elemtype>::createTree(const elemtype& stopflag)
{
    myqueue<treenode<elemtype> *> que;
    elemtype e, el, er;
    elemtype *p, *pl, *pr;
    cin >> e;
    if (e == stopflag) return;
    p = new treenode<elemtype>(e);
    que.enqueue(p);
    root = p;

    while (!que.isEmpty()) {
        p = que.seeTop();
        que.dequeue();
        cin >> el >> er;
        if (el != stopflag) {
            pl = new treenode<elemtype>(el);
            que.enqueue(pl);
            p->left = pl;
        }
        if (er != stopflag) {
            pr = new treenode<elemtype>(er);
            que.enqueue(pr);
            p->right = pr;
        }
    }
}

template<class elemtype>
void Btree<elemtype>::delTree(treenode<elemtype> *t)
{
    if (!t) return;
    delTree(t->left);
    delTree(t->right);
    delete t;
}

template<class elemtype>
int Btree<elemtype>::size(treenode<elemtype> *t)
{
    if (!t) return 0;
    return 1 + size(t->left) + size(t->right);
}

template<class elemtype>
int Btree<elemtype>::height(treenode<elemtype> *t)
{
    if (!t) return 0;
    int hl, hr;
    hl = height(t->left); 
    hr = height(t->right);
    return max(hl, hr) + 1;
}

template<class elemtype>
void Btree<elemtype>::levelOrder() const
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

template<class elemtype>
void Btree<elemtype>::preOrder() const
{
    if (!root) return;
    mystack<treenode<elemtype> *> s;
    treenode<elemtype> *p;
    s.push(root);

    while (!s.isEmpty()) {
        p = s.topsee();
        s.pop();
        cout << p->data;
        if(p->right) s.push(p->right);
        if(p->left) s.push(p->left);
    }
    cout << endl;
}

template<class elemtype>
void Btree<elemtype>::inOrder() const
{
    if (!root) return;

    const int zero = 0, one = 1;
    mystack<treenode<elemtype> *> s;
    mystack<int> s_s;
    treenode<elemtype> *p;
    int p_sta;
    s.push(root); s_s.push(zero);

    while(!s.isEmpty()) {
        p = s.topsee(); p_sta = s_s.topsee();
        if (p_sta == one) {
            cout << p->data;
            s.pop(); s_s.pop();
            if (p->right) {
                s.push(p->right); s_s.push(zero);
            }
        }
        else {
            s_s.pop(); s_s.push(one);
            if (p->left) {
                s.push(p->left); s_s.push(zero);
            }
        }
    }
    cout << endl;
}

template<class elemtype>
void Btree<elemtype>::postOrder() const
{
    if (!root) return;

    const int zero = 0, one = 1, two = 2;
    mystack<treenode<elemtype> *> s;
    mystack<int> s_s;
    treenode<elemtype> *p;
    int p_sta;
    s.push(root); s_s.push(zero);

    while(!s.isEmpty()) {
        p = s.topsee(); p_sta = s_s.topsee();
        if (p_sta == two) {
            cout << p->data;
            s.pop(); s_s.pop();
        }
        else if (p_sta == one) {
            s_s.pop(); s_s.push(two);
            if (p->right) {
                s.push(p->right); s_s.push(zero);
            }
        }
        else {
            s_s.pop(); s_s.push(one);
            if (p->left) {
                s.push(p->left); s_s.push(zero);
            }
        }
    }
    cout << endl;
}

template<class elemtype>
treenode<elemtype> *Btree<elemtype>::pre_mid_tree(const elemtype *&pre, const elemtype *&mid, int pl, int ml, int pr, int mr) 
{
    treenode<elemtype> *p, *leftroot, *rightroot;
    int lpl, lpr, rpl, rpr, lml, lmr, rml, rmr;
    int i, num;

    p = new treenode(pre[pl]);
    if (!root) root = p;

    for (i = ml; i < mr; ++i) {
        if (mid[i] == pre[pl]) break;
    }

    num = i - ml;

    lpl = pl + 1; lpr = num + pl;
    rpl = lpr + 1; rpr = pr;

    lml = ml; lmr = i - 1;
    rml = i + 1; rmr = mr;

    leftroot = pre_mid_tree(pre, mid, lpl, lml, lpr, lmr);
    rightroot = pre_mid_tree(pre, mid, rpl, rml, rpr, rmr);

    p->left = leftroot; p->right = rightroot;
    return p;
}

template<class elemtype>
int Btree<elemtype>::priOver(const char& ch1, const char& ch2)
{
    // ch1--top of stack; ch2--outside
    // 1: ch1 is prior to ch2 or they have same priority
    if (ch2 == '(') return 0;
    switch (ch1)
    {
    case '(':
    case '#':
        return 0;
    case '+':
    case '-':
        if (ch2 == '*' || ch2 == '/') return 0;
        else return 1;
    case '*':
    case '/':
        return 1;
    default:
        return -1;
    }
}

template<class elemtype>
void Btree<elemtype>::expr_tree(const char* expr)
{
    mystack<char> sign;
    mystack<treenode<char> *> num;
    
    sign.push('#');
    while (*expr) {
        if (*expr >= '0' && *expr <= '9') {
            num.push(new treenode<char>(*expr));
        }
        else {
            int pri = priOver(sign.topsee(), *expr);
            while (pri == 1) {
                char oper = sign.topsee();
                sign.pop();
                treenode<char> num2 = num.topsee(); num.pop();
                treenode<char> num1 = num.topsee(); num.pop();
                num.push(new treenode<char>(oper, num1, num2));
                pri = priOver(sign.topsee(), *expr);
            }
            if (pri == 0) sign.push(*expr);
        }
        ++expr;
    }
    char ch = sign.topsee();
    while (ch != '#') {
        sign.pop();
        treenode<char> num2 = num.topsee(); num.pop();
        treenode<char> num1 = num.topsee(); num.pop();
        num.push(new treenode<char>(ch, num1, num2));
        ch = sign.topsee();
    }
    root = num.topsee(); num.pop();
} // Binary Tree


// Huffman Tree
template <class elemtype>
struct Huffnode {
    elemtype data;
    double weight;
    int parent, left, right;
};

template <class elemtype>
int minIndex(Huffnode<elemtype> *bt, int k, int m)
{
    int i, min, minweight = 10000000;

    for (i = m-1, i>k, ++i) {
        if (bt[i].parent == 0 && bt[i].weight < minweight) {
            min = i; minweight = bt[i].weight;
        }
    }

    return min;
}

template <class elemtype>
Huffnode<elemtype> *huff_tree(double *w, elemtype *a, int n)
{
    int m = n*2;
    Huffnode<elemtype> BBtree[m];
    int first_min, second_min;
    int i, j;

    for (i = 0, i < n, ++i) {
        BBtree[m-1-i].weight = w[i];
        BBtree[m-1-i].data = a[i];
        BBtree[m-1-i].parent = 0;
        BBtree[m-1-i].left = 0;
        BBtree[m-1-i].right = 0;

    }

    j = n-1;
    while (j > 0) {
        first_min = minIndex(BBtree, j, m);
        BBtree[first_min].parent = j;
        BBtree[j].left = first_min;

        second_min = minIndex(BBtree, j, m);
        BBtree[second_min].parent = j;
        BBtree[j].right = second_min;

        BBtree[j].weight = BBtree[first_min].weight + BBtree[second_min].weight;
        BBtree[j].parent = 0;
        --j;
    }
    return BBtree;
} // Huffman Tree

#endif