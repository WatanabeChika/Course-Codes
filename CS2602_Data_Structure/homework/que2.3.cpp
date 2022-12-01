#include <iostream>
#include <cstdio>
using namespace std;

namespace LIST
{

    struct NODE {
        int data;
        NODE *next;
        NODE(NODE *n) : next(n) {};
        NODE(int d, NODE *n = NULL) : data(d), next(n) {}
    };

    NODE *head = nullptr;
    int len = 0;

    void init() {
        head = new NODE(head);
        head->next = head;
    }

    NODE* move(int i) {
        if (i<0 || i>len-1) return head;
        NODE *tmp = head->next;
        while (i>0)
        {
            tmp = tmp->next;
            --i;
        }
        return tmp;
    }

    void insert(int i, int x) {
        if (i<0 || i>len) return;
        NODE *tmp = head;
        while (i>0)
        {
            tmp = tmp->next;
            --i;
        }
        tmp->next = new NODE(x, tmp->next);
        ++len;
    }

    void remove(int i) {
        if (i<0 || i>len-1) return;
        NODE *tmp = head;
        while (i>0)
        {
            tmp = tmp->next;
            --i;
        }
        NODE *p = tmp->next;
        tmp->next = p->next;
        delete p;
        --len;
    }

    void remove_insert(int i) {
        if (i<0 || i>len-1) return;
        NODE *tmp = head;
        while (i>0)
        {
            tmp = tmp->next;
            --i;
        }
        NODE *p = tmp->next;
        tmp->next = p->next;
        while(tmp->next != head)
        {
            tmp = tmp->next;
        }
        tmp->next = p;
        p->next = head;
    }

    void get_length() {
        cout << len << endl;
    }

    void query(int i) {
        if (i<0) return;
        if (i>len-1) {
            cout << -1 << endl;
            return;
        }
        NODE *tmp = head->next;
        while (i>0)
        {
            tmp = tmp->next;
            --i;
        }
        cout << tmp->data << endl;
    }

    void get_max() {
        if (len == 0) {
            cout << -1 << endl;
            return;
        }
        NODE *tmp = head->next;
        int maxdata = tmp->data;
        while (tmp != head)
        {
            if (tmp->data > maxdata) {
                maxdata = tmp->data;
            }
            tmp = tmp->next;
        }
        cout << maxdata << endl;
    }

    void clear() {
        if (len == 0) return;
        NODE *p = head->next;
        NODE *q = p->next;
        while(q != head)
        {
            delete p;
            p = q;
            q = p->next;
        }
        delete p;
        head->next = nullptr;
        delete head;
    }
}
int n;
int main()
{
    cin >> n;
    int op, x, p;
    LIST::init();
    for (int _ = 0; _ < n; ++_)
    {
        cin >> op;
        switch(op) {
            case 0:
                LIST::get_length();
                break;
            case 1:
                cin >> p >> x;
                LIST::insert(p,x);
                break;
            case 2:
                cin >> p;
                LIST::query(p);
                break;
            case 3:
                cin >> p;
                LIST::remove(p);
                break;
            case 4:
                cin >> p;
                LIST::remove_insert(p);
                break;
            case 5:
                LIST::get_max();
                break;
        }
    }
    LIST::clear();
    return 0;
}