#include <iostream>

using namespace std;

struct node {
    int data;
    node *next;
    node(node *n) : next(n) {};
    node(int d, node *n = nullptr) : data(d), next(n) {};
};

struct loopList {
    int len = 1;
    node *init;

    loopList(int m) {
        int i = 1;
        node *tmp = new node(i);
        node *first = tmp;
        while (m>1) {
            --m;
            ++len; ++i;
            tmp->next = new node(i);
            tmp = tmp->next;
        }
        tmp->next = first;
        init = tmp;
    }

    node* remove(node *n, int k) {
        node *tmp = n;
        k = k%len;
        if (k == 0) k = len;
        while (k>1)
        {
            tmp = tmp->next;
            --k;
        }
        node *p = tmp->next;
        tmp->next = p->next;
        delete p;
        --len;
        return tmp;
    }

    void clear(node *p) {
        delete p;
    }

};



int main() 
{
    int M, k;
    cin >> M;
    loopList monLoop(M);
    node *sign = monLoop.init;
    while (monLoop.len > 1) {
        cin >> k;
        sign = monLoop.remove(sign, k);
    }
    cout << sign->data << endl;
    monLoop.clear(sign);
    return 0;
}