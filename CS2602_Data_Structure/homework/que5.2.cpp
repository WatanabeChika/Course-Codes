#include <iostream>

using namespace std;

long long sum;
class linkList;

class node {
    friend class linkList;
    private:
        node* next;
        int data;
    public:
        node(): next(NULL){};
        node(const int& e, node* n=NULL): data(e), next(n){};
};

class linkList {
private:
    node* head;
public:
    linkList() {head = new node();}

    int find(const int& e) const;
    void insert(int i, const int& e);
    void remove(int i);

    ~linkList();


};

int linkList::find(const int& e) const
{
    int i=1;
    node* p = head->next;
    while (p) {
        ++sum;
        if (p->data == e) return i;
        p = p->next;
        ++i;
    }
    return 0;
}

void linkList::insert(int i, const int& e)
{
    node* p = head; 
    while (p && i>1) {
        p = p->next;
        --i;
    }
    p->next = new node(e,p->next);
}

void linkList::remove(int i)
{
    node* p = head;
    while (p && i>1) {
        p = p->next;
        --i;
    }
    node* q = p->next;
    p->next = q->next;
    delete q;
}

linkList::~linkList()
{
    node* p = head->next;
    node* q;
    head->next = NULL;
    while (p) {
        q = p->next;
        delete p;
        p=q;
    }
}

int main() 
{
    int n, m;
    int pos, data;
    cin >> n;
    int arr[n];
    linkList lis;

    for (int i = 0; i < n; ++i) {
        cin >> data;
        arr[i] = data;
    }

    for (int i = 0; i < n; ++i) {
        lis.insert(1, arr[n-1-i]);
    }

    cin >> m;
    for (int j = 0; j < m; ++j) {
        cin >> data;
        pos = lis.find(data);
        if (pos != 0) {
            lis.remove(pos);
            lis.insert(1, data);
        }
    }

    cout << sum << endl;
    return 0;
}
